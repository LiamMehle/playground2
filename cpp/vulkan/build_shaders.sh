

#SCRIPT_DIR should be exported from top-level build.sh

glslc $SCRIPT_DIR/src/simple_shader.frag -o ./simple_shader.frag.spv &
glslc $SCRIPT_DIR/src/simple_shader.vert -o ./simple_shader.vert.spv &

# sync with jobs
source  $SCRIPT_DIR/sync_jobs.sh