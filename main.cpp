#include "grid.h"
#include "graphics.h"

int main(int argc, char **argv) {
	Grid grid(30, 30);

//	grid.set_color(20, 20, Bl);

//	grid.set_line_color(17, 28, 22, 6, Bl);
//	grid.set_line_color(16, 29, 18, 4, Re);

//	grid.set_rhomb_color(4, 3, 3, Re);
//	grid.set_rhomb_color(15, 18, 10, Ye);

//	grid.set_filled_rhomb_color(4, 3, 3, Re);
//	grid.set_filled_rhomb_color(15, 18, 10, Ye);

//	grid.set_filled_rhomb_2_color(4, 3, 3, Re, Bl);
//	grid.set_filled_rhomb_2_color(15, 18, 10, Ye, Gr);

//	grid.set_filled_rhomb_rainbow_color(4, 3, 3);
//	grid.set_filled_rhomb_rainbow_color(15, 18, 10, Ye);

//	grid.set_circle_color(4, 3, 3, Re);
//	grid.set_circle_color(15, 18, 10, Ye);

//	grid.set_filled_circle_color(4, 3, 3, Re);
//	grid.set_filled_circle_color(15, 18, 10, Ye);

//	grid.set_filled_circle_2_color(4, 3, 3, Re, Or);
//	grid.set_filled_circle_2_color(15, 18, 10, Ye, Gr);

//	grid.set_filled_circle_rainbow_color(4, 3, 3);
//	grid.set_filled_circle_rainbow_color(15, 18, 10, Ye);

//	grid.set_rect_color(25, 6, 2, 20, Re);
//	grid.set_rect_color(16, 28, 19, 8, Ye, true);

//	grid.set_filled_rect_color(16, 28, 19, 8, Ye, false);
//	grid.set_filled_rect_color(24, 4, 1, 1, Gr, true);

//	grid.set_filled_rect_2_color(25, 6, 2, 20, Bl, Re);
//	grid.set_filled_rect_2_color(25, 22, 2, 28, Bl, Re, true);

//	grid.set_filled_rect_rainbow_color(25, 5, 2, 20);
//	grid.set_filled_rect_rainbow_color(25, 22, 2, 28, Bl, true);

//	grid.set_circle_crest_color(10, 17, 3, Re, Gr, 2);
//	grid.set_circle_crest_color(20, 9, 9, Gr, Or, 5);
//	grid.set_circle_crest_color(4, 4, 4, Bl, Bl, 2);
//	grid.set_circle_crest_color(23, 24, 5, Or, Bl, 1);

// 	grid.set_clock_color(4, 4, 3, Re, Or, Gr);
// 	grid.set_clock_color(15, 15, 9, Gr, Re, Or);

//	grid.set_filled_circle_rainbow_color(15, 17, 12, Re);
//	grid.set_char_color(10, 10, 'D', Gr, Ye);

//	grid.set_text_color(14, 10, "Danik", Bl, Gr);
//	grid.set_text_color(9, 1, "Irisha", Re, NO_COLOR, -1, 1);
//	grid.set_text_color(20, 1, "Lion", Gr, Ye, 1, 0);

//	grid.set_text_color(2, 14, "I", Re);
//	grid.set_text_color(7, 10, "love", Re);
//	grid.set_text_color(12, 14, "you", Re, NO_COLOR, 3);

//	Size size = grid.get_line_size(16, 28, 29, 8);
//	cout << "Size between (16, 28) and (29, 8) is " << size << endl;

	grid.set_text_rainbow_color(1, 1, "Welcome to    ", Re, Ye);
	grid.set_text_rainbow_color(grid.get_size_y() - 6, 1, "Mosaic!    ", Bl, Gr, 0, 1);

	grid.save("text-example.sav");
//	if (!grid.load("unexisting-file.txt")) exit(1);
//	if (!grid.load("main.cpp")) exit(1);
	grid.load("text-example.sav");

	auto app = Gtk::Application::create("Migo.Mosaic");

	return app->make_window_and_run<MosaicWindow>(argc, argv, grid);
}

