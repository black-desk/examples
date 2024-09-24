#!/usr/bin/env bash

# Use /usr/bin/env to find shell interpreter for better portability.
# Reference: https://en.wikipedia.org/wiki/Shebang_%28Unix%29#Portability

# Exit immediately if any commands (even in pipeline)
# exits with a non-zero status.

set -e
set -o pipefail

# WARNING:
# This is not reliable when current script file is sourced by `source` or `.`
CURRENT_SOURCE_FILE_PATH="${BASH_SOURCE[0]:-$0}"
CURRENT_SOURCE_FILE_DIR="$(dirname -- "$CURRENT_SOURCE_FILE_PATH")"

cd "$CURRENT_SOURCE_FILE_DIR"

function download_latest_github_release() {
	REPOSITORY="$1"
	ASSET="$2"
	SAVE_AS="$3"

	URL="https://github.com/$REPOSITORY/releases/latest/download/$ASSET"

	mkdir -p "$(dirname -- "$SAVE_AS")"

	if command -v curl &>/dev/null; then
		curl -L "$URL" -o "$SAVE_AS"
		return
	fi

	if command -v wget &>/dev/null; then
		wget "$URL" -O "$SAVE_AS"
		return
	fi

	exit 1
}

download_latest_github_release \
	"black-desk/PFL.cmake" "PFL.cmake" "../cmake/PFL.cmake"
