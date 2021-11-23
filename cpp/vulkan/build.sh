#!/bin/sh

# expected by lower-level scripts
export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# out-of-source build setup
mkdir -p build
cd build

# build
source ../build_shaders.sh &
source ../build_host.sh    &

# sync with jobs
source ../sync_jobs.sh