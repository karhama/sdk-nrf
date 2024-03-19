/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "app_task.h"
#include "bolt_lock_manager.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <app/clusters/door-lock-server/door-lock-server.h>
#include <app/data-model/Nullable.h>
#include <lib/core/DataModelTypes.h>
#include <lib/support/CodeUtils.h>

using namespace ::chip;
using namespace ::chip::app::Clusters;
using namespace ::chip::app::Clusters::DoorLock;
using ::chip::app::DataModel::Nullable;

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath &attributePath, uint8_t type,
				       uint16_t size, uint8_t *value)
{
	VerifyOrReturn(attributePath.mClusterId == DoorLock::Id &&
		       attributePath.mAttributeId == DoorLock::Attributes::LockState::Id);

	/* Post events only if current lock state is different than given */
	switch (*value) {
	case to_underlying(DlLockState::kLocked):
		BoltLockMgr().Lock(BoltLockManager::OperationSource::kRemote);
		break;
	case to_underlying(DlLockState::kUnlocked):
		BoltLockMgr().Unlock(BoltLockManager::OperationSource::kRemote);
		break;
	default:
		break;
	}
}

bool emberAfPluginDoorLockGetUser(EndpointId endpointId, uint16_t userIndex, EmberAfPluginDoorLockUserInfo &user)
{
	return BoltLockMgr().GetUser(userIndex, user);
}

bool emberAfPluginDoorLockSetUser(EndpointId endpointId, uint16_t userIndex, FabricIndex creator, FabricIndex modifier,
				  const CharSpan &userName, uint32_t uniqueId, UserStatusEnum userStatus,
				  UserTypeEnum userType, CredentialRuleEnum credentialRule,
				  const CredentialStruct *credentials, size_t totalCredentials)
{
	return BoltLockMgr().SetUser(userIndex, creator, modifier, userName, uniqueId, userStatus, userType,
				     credentialRule, credentials, totalCredentials);
}

bool emberAfPluginDoorLockGetCredential(EndpointId endpointId, uint16_t credentialIndex,
					CredentialTypeEnum credentialType,
					EmberAfPluginDoorLockCredentialInfo &credential)
{
	return BoltLockMgr().GetCredential(credentialIndex, credentialType, credential);
}

bool emberAfPluginDoorLockSetCredential(EndpointId endpointId, uint16_t credentialIndex, FabricIndex creator,
					FabricIndex modifier, DlCredentialStatus credentialStatus,
					CredentialTypeEnum credentialType, const ByteSpan &secret)
{
	return BoltLockMgr().SetCredential(credentialIndex, creator, modifier, credentialStatus, credentialType,
					   secret);
}

bool emberAfPluginDoorLockOnDoorLockCommand(EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx,
                                            const Nullable<chip::NodeId> & nodeId, const Optional<ByteSpan> & pinCode,
                                            OperationErrorEnum & err)
{
	bool result = BoltLockMgr().ValidatePIN(pinCode, err);

	/* Handle changing attribute state on command reception */
	if (result) {
		BoltLockMgr().Lock(BoltLockManager::OperationSource::kRemote);
	}

	return result;
}

bool emberAfPluginDoorLockOnDoorUnlockCommand(EndpointId endpointId, const Nullable<chip::FabricIndex> & fabricIdx,
                                              const Nullable<chip::NodeId> & nodeId, const Optional<ByteSpan> & pinCode,
                                              OperationErrorEnum & err)
{
	bool result = BoltLockMgr().ValidatePIN(pinCode, err);

	/* Handle changing attribute state on command reception */
	if (result) {
		BoltLockMgr().Unlock(BoltLockManager::OperationSource::kRemote);
	}

	return result;
}

void emberAfDoorLockClusterInitCallback(EndpointId endpoint)
{
	DoorLockServer::Instance().InitServer(endpoint);

	const auto logOnFailure = [](EmberAfStatus status, const char *attributeName) {
		if (status != EMBER_ZCL_STATUS_SUCCESS) {
			ChipLogError(Zcl, "Failed to set DoorLock %s: %x", attributeName, status);
		}
	};

	logOnFailure(DoorLock::Attributes::LockType::Set(endpoint, DlLockType::kDeadBolt), "type");
	logOnFailure(DoorLock::Attributes::NumberOfTotalUsersSupported::Set(endpoint, CONFIG_LOCK_NUM_USERS),
		     "number of users");
	logOnFailure(DoorLock::Attributes::NumberOfPINUsersSupported::Set(endpoint, CONFIG_LOCK_NUM_USERS),
		     "number of PIN users");
	logOnFailure(DoorLock::Attributes::NumberOfRFIDUsersSupported::Set(endpoint, 0), "number of RFID users");
	logOnFailure(DoorLock::Attributes::NumberOfCredentialsSupportedPerUser::Set(
			     endpoint, CONFIG_LOCK_NUM_CREDENTIALS_PER_USER),
		     "number of credentials per user");

	AppTask::Instance().UpdateClusterState(BoltLockMgr().GetState(),
					       BoltLockManager::OperationSource::kUnspecified);
}
