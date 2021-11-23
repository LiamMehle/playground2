PWD = $(shell pwd)

.DEFAULT_GOAL = all                # should be unneccessary

binary_name = main.exe             # desired bin name

include ../Makefile.win64       # compilation logic
# aditional project-specific flags
CFLAGS += -O0 -fopenmp
CFLAGS += -I./qt/include
LIBS   += -L./qt/lib -lQt5Widgetsd # -lQt5Core -lQt5Gui -lQt5OpenGL  -lQt5Gui -lQt5Core
#CFLAGS += -L/usr/X11R6/lib64 -L/opt/Qt5.0.1/5.0.1/gcc_64/lib  -lGL -lpthread#-fsanitize=address
#-Wconversion -Werror -O0 -fno-exceptions

#/usr/lib/arm-linux-gnueabihf/libQt5Gui.so
