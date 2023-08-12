all:
	@g++ -o mosaic main.cpp graphics.cpp grid.cpp `pkg-config --cflags --libs gtkmm-4.0`

