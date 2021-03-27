dir=new_project

all: new_project
	chmod 777 $(dir)/make.sh

new_project:
	mkdir -p     $(dir)/src
	cp .Makefile $(dir)/Makefile
	cp .make.sh  $(dir)/make.sh
	touch        $(dir)/src/main.cpp

clean:
	rm -rf $(dir)
