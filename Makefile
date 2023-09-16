# Usage: make CPPFLAGS=-std=c++20
CPPFLAGS=
GPP_CMD=g++ `pkg-config --cflags --libs gtkmm-4.0` $(CPPFLAGS)

all: mosaic

mosaic: main.o program.o graphics.o grid.o screensaver.o
	@$(GPP_CMD) -o $@ $^

tests: tests.o testgrid.o program.o grid.o
	@$(GPP_CMD) -o $@ $^

main.o: main.cpp program.h graphics.h grid.h screensaver.h
	@$(GPP_CMD) -c $*.cpp

program.o: program.cpp program.h
	@$(GPP_CMD) -c $*.cpp

graphics.o: graphics.cpp mosaic-css.h program.h graphics.h grid.h screensaver.h
	@$(GPP_CMD) -c $*.cpp

grid.o: grid.cpp grid.h font3x5.h
	@$(GPP_CMD) -c $*.cpp

testgrid.o: testgrid.cpp testgrid.h grid.cpp grid.h
	@$(GPP_CMD) -c $*.cpp

tests.o: tests.cpp testgrid.h grid.h
	@$(GPP_CMD) -c $*.cpp

screensaver.o: screensaver.cpp screensaver.h grid.h
	@$(GPP_CMD) -c $*.cpp

clean:
	@rm -f mosaic tests *.o
