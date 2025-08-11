#!/bin/bash

BASEDIR=$(dirname "$0")
REQUIREMENTS=$BASEDIR/requirements-fixed.txt

# Detect OS and set tools version file
case "$(uname -s)" in
    Linux*)   TOOLS_VERSIONS="$BASEDIR/tools-versions-linux.yml" ;;
    Darwin*)  TOOLS_VERSIONS="$BASEDIR/tools-versions-darwin.yml" ;;
    *) echo "Unsupported OS"; exit 1 ;;
esac
# Compute SHA256 and take first 10 characters
TOOLCHAIN_VERSION=$(cat "$REQUIREMENTS" "$TOOLS_VERSIONS" | sed 's/\r$//' | shasum -a 256 | cut -c1-10)

echo "${TOOLCHAIN_VERSION}"
