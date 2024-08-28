#!/usr/bin/env bash

set -e
set -o pipefail

cd "$(dirname -- "${BASH_SOURCE[0]}")"

mkdir -p ../cmake
wget -O ../cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake
