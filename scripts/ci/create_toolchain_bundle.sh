#!/usr/bin/env bash
# Build an NCS toolchain bundle on the current machine (Linux / Windows / macOS).
#
# Usage:
#   MANIFEST_LABEL=main ./scripts/ci/create_toolchain_bundle.sh
#
# Writes output/*.tar.gz. Sets GITHUB_OUTPUT bundle_id when run in Actions.

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$REPO_ROOT"

MANIFEST_LABEL="${MANIFEST_LABEL:-local}"
OUTPUT_DIR="${OUTPUT_DIR:-output}"
TOOLCHAIN_BUNDLER_VERSION="${TOOLCHAIN_BUNDLER_VERSION:-0.26.0}"
NRFUTIL_BASE="https://files.nordicsemi.com/artifactory/swtools/external/nrfutil/executables"

install_linux_deps() {
	sudo apt-get update
	sudo apt-get install -y --no-install-recommends \
		ca-certificates curl debootstrap fakechroot fakeroot
}

case "$(uname -s)" in
Linux*)
	PLATFORM=linux
	NRFUTIL_URL="${NRFUTIL_BASE}/x86_64-unknown-linux-gnu/nrfutil"
	NRFUTIL="$REPO_ROOT/nrfutil"
	install_linux_deps
	;;
MINGW* | MSYS* | CYGWIN*)
	PLATFORM=windows
	git config --global core.autocrlf false
	NRFUTIL_URL="${NRFUTIL_BASE}/x86_64-pc-windows-msvc/nrfutil.exe"
	NRFUTIL="$REPO_ROOT/nrfutil.exe"
	;;
Darwin*)
	PLATFORM=macos
	export PATH="/opt/homebrew/bin:/opt/homebrew/opt/coreutils/libexec/gnubin:$PATH"
	NRFUTIL_URL="${NRFUTIL_BASE}/universal-apple-darwin/nrfutil"
	NRFUTIL="$REPO_ROOT/nrfutil"
	;;
*)
	echo "Unsupported OS: $(uname -s)" >&2
	exit 1
	;;
esac

BUNDLE_ID=$("$REPO_ROOT/scripts/print_toolchain_checksum.sh")

curl -fsSL "$NRFUTIL_URL" -o "$NRFUTIL"
chmod +x "$NRFUTIL"
"$NRFUTIL" self-upgrade
"$NRFUTIL" install --force "toolchain-bundler=${TOOLCHAIN_BUNDLER_VERSION}"

mkdir -p "$OUTPUT_DIR"
REPO_URI="file://${REPO_ROOT//\\//}"
build_args=(
	"$NRFUTIL" toolchain-bundler build
	--output-dir "$OUTPUT_DIR"
	--ncs-repository "$REPO_URI"
	"$MANIFEST_LABEL"
)
if [[ "$PLATFORM" == windows ]]; then
	build_args+=(--allow-changes-to-host)
fi
"${build_args[@]}"

if [[ -n "${GITHUB_OUTPUT:-}" ]]; then
	echo "bundle_id=$BUNDLE_ID" >> "$GITHUB_OUTPUT"
fi
