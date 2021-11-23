PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main#                # desired bin name

include ../Makefile.x86_64.gcc       # compilation logic
# aditional project-specific flags
CFLAGS += -Wall -Wconversion -Wextra -O0 -fno-exceptions -std=c++17 -g
CFLAGS += `pkg-config --static --libs glew` `pkg-config --static --libs glfw3` `pkg-config --static --libs opengl`
CFLAGS += -I home/wolf/git/playground2/cpp/opengl_ui/glew-2.2.0/include/ -lglfw #-fsanitize=address
