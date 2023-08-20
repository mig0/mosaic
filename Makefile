all:
	@g++ -o mosaic main.cpp program.cpp graphics.cpp grid.cpp `pkg-config --cflags --libs gtkmm-4.0`

