#!/bin/bash

set -e
set -o pipefail

cd "$(dirname -- "${BASH_SOURCE[0]}")"

mkdir -p ../cmake
wget -O ../cmake/GitSemver.cmake https://github.com/black-desk/GitSemver.cmake/releases/latest/download/GitSemver.cmake
