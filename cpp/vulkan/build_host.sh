# SCRIPT_DIR should be exported from top-level build.sh

cmake $SCRIPT_DIR -DCMAKE_BUILD_TYPE=Debug && \
make -j`nproc`