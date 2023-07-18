all:
	@g++ -o mosaic main.cpp `pkg-config --cflags --libs gtkmm-4.0`

