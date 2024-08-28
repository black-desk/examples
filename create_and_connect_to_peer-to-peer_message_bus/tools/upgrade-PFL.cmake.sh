#!/usr/bin/env bash

set -e
set -o pipefail

cd "$(dirname -- "${BASH_SOURCE[0]}")"

mkdir -p ../cmake
wget -O ../cmake/PFL.cmake https://github.com/black-desk/PFL.cmake/releases/latest/download/PFL.cmake
