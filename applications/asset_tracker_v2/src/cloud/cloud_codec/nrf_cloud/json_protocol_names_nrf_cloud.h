/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <net/nrf_cloud_defs.h>

#define DATA_GNSS_LONGITUDE	NRF_CLOUD_JSON_GNSS_PVT_KEY_LON
#define DATA_GNSS_LATITUDE	NRF_CLOUD_JSON_GNSS_PVT_KEY_LAT
#define DATA_GNSS_ALTITUDE	NRF_CLOUD_JSON_GNSS_PVT_KEY_ALTITUDE
#define DATA_GNSS_SPEED		NRF_CLOUD_JSON_GNSS_PVT_KEY_SPEED
#define DATA_GNSS_HEADING	NRF_CLOUD_JSON_GNSS_PVT_KEY_HEADING
#define DATA_GNSS_ACCURACY	NRF_CLOUD_JSON_GNSS_PVT_KEY_ACCURACY

#define DATA_MODEM_DYNAMIC	NRF_CLOUD_DEVICE_JSON_KEY_NET_INF
#define DATA_MODEM_STATIC	NRF_CLOUD_DEVICE_JSON_KEY_DEV_INF
#define DATA_CONFIG		NRF_CLOUD_JSON_KEY_CFG
#define DATA_VERSION		"version"
#define DATA_IMPACT		"impact"

#define DATA_GROUP		NRF_CLOUD_JSON_MSG_TYPE_KEY
#define DATA_ID			NRF_CLOUD_JSON_APPID_KEY
#define DATA_TYPE		NRF_CLOUD_JSON_DATA_KEY
#define DATA_TIMESTAMP		NRF_CLOUD_MSG_TIMESTAMP_KEY

#define MESSAGE_TYPE_DATA	NRF_CLOUD_JSON_MSG_TYPE_VAL_DATA

#define APP_ID_BUTTON		NRF_CLOUD_JSON_APPID_VAL_BTN
#define APP_ID_BATTERY		"BATTERY"
#define APP_ID_DEVICE		NRF_CLOUD_JSON_APPID_VAL_DEVICE
#define APP_ID_GNSS		NRF_CLOUD_JSON_APPID_VAL_GNSS
#define APP_ID_HUMIDITY		NRF_CLOUD_JSON_APPID_VAL_HUMID
#define APP_ID_AIR_PRESS	NRF_CLOUD_JSON_APPID_VAL_AIR_PRESS
#define APP_ID_AIR_QUAL		NRF_CLOUD_JSON_APPID_VAL_AIR_QUAL
#define APP_ID_TEMPERATURE	NRF_CLOUD_JSON_APPID_VAL_TEMP
#define APP_ID_RSRP		NRF_CLOUD_JSON_APPID_VAL_RSRP
#define APP_ID_CELL_POS		NRF_CLOUD_JSON_APPID_VAL_LOCATION
#define APP_ID_IMPACT		"IMPACT"

#define MODEM_CURRENT_BAND     "currentBand"
#define MODEM_NETWORK_MODE     "networkMode"
#define MODEM_RSRP	       "rsrp"
#define MODEM_AREA_CODE	       "areaCode"
#define MODEM_MCCMNC	       "mccmnc"
#define MODEM_CELL_ID	       "cellID"
#define MODEM_IP_ADDRESS       "ipAddress"

#define CONFIG_DEVICE_MODE		  "activeMode"
#define CONFIG_ACTIVE_TIMEOUT		  "activeWaitTime"
#define CONFIG_MOVE_TIMEOUT		  "movementTimeout"
#define CONFIG_MOVE_RES			  "movementResolution"
#define CONFIG_LOCATION_TIMEOUT		  "locationTimeout"
#define CONFIG_ACC_ACT_THRESHOLD	  "accThreshAct"
#define CONFIG_ACC_INACT_THRESHOLD	  "accThreshInact"
#define CONFIG_ACC_INACT_TIMEOUT	  "accTimeoutInact"
#define CONFIG_NO_DATA_LIST		  "nod"
#define CONFIG_NO_DATA_LIST_GNSS	  "gnss"
#define CONFIG_NO_DATA_LIST_NEIGHBOR_CELL "ncell"
#define CONFIG_NO_DATA_LIST_WIFI	  "wifi"

#define OBJECT_CONFIG	NRF_CLOUD_JSON_KEY_CFG
#define OBJECT_REPORTED	NRF_CLOUD_JSON_KEY_REP
#define OBJECT_STATE	NRF_CLOUD_JSON_KEY_STATE
