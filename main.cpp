#include "program.h"
#include "grid.h"
#include "graphics.h"

int main(int argc, char *argv[], char *envp[]) {
	map <string, string> preferences = parse_options(argc, argv, envp);

	Grid grid(30, 30);

//	grid.set_color(20, 20, Bl);

//	grid.draw_line(17, 28, 22, 6, Bl);
//	grid.draw_line(16, 29, 18, 4, Re);

//	grid.draw_rhomb(4, 3, 3, Re);
//	grid.draw_rhomb(15, 18, 10, Ye);

//	grid.draw_filled_rhomb(4, 3, 3, Re);
//	grid.draw_filled_rhomb(15, 18, 10, Ye);

//	grid.draw_filled_rhomb_2(4, 3, 3, Re, Bl);
//	grid.draw_filled_rhomb_2(15, 18, 10, Ye, Gr);

//	grid.draw_filled_rhomb_rainbow(4, 3, 3);
//	grid.draw_filled_rhomb_rainbow(15, 18, 10, Ye);

//	grid.draw_square(12, 12, 10, Gr);
//	grid.draw_filled_square(12, 12, 10, Gr);
//	grid.draw_filled_square_2(12, 12, 10, Gr, Bl);
//	grid.draw_filled_square_rainbow(12, 12, 10);

//	grid.draw_circle(4, 3, 3, Re);
//	grid.draw_circle(15, 18, 10, Ye);

//	grid.draw_filled_circle(4, 3, 3, Re);
//	grid.draw_filled_circle(15, 18, 10, Ye);

//	grid.draw_filled_circle_2(4, 3, 3, Re, Or);
//	grid.draw_filled_circle_2(15, 18, 10, Ye, Gr);

//	grid.draw_filled_circle_rainbow(4, 3, 3);
//	grid.draw_filled_circle_rainbow(15, 18, 10, Ye);

//	grid.draw_rect(25, 6, 2, 20, Re);
//	grid.draw_rect(16, 28, 19, 8, Ye, true);

//	grid.draw_filled_rect(16, 28, 19, 8, Ye, false);
//	grid.draw_filled_rect(24, 4, 1, 1, Gr, true);

//	grid.draw_filled_rect_2(25, 6, 2, 20, Bl, Re);
//	grid.draw_filled_rect_2(25, 22, 2, 28, Bl, Re, true);

//	grid.draw_filled_rect_rainbow(25, 5, 2, 20);
//	grid.draw_filled_rect_rainbow(25, 22, 2, 28, Bl, true);

//	grid.draw_circle_crest(10, 17, 3, Re, Gr, 2);
//	grid.draw_circle_crest(20, 9, 9, Gr, Or, 5);
//	grid.draw_circle_crest(4, 4, 4, Bl, Bl, 2);
//	grid.draw_circle_crest(23, 24, 5, Or, Bl, 1);

// 	grid.draw_clock(4, 4, 3, Re, Or, Gr);
// 	grid.draw_clock(15, 15, 9, Gr, Re, Or);

//	grid.draw_smile(15, 15, 10, Re, Ye);

//	grid.draw_triangle(0, 0, 1, 29, 29, 10, Gr);

//	grid.draw_filled_circle_rainbow(15, 17, 12, Re);
//	grid.draw_char(10, 10, 'D', Gr, Ye);

//	grid.draw_text(14, 10, "Danik", Bl, Gr);
//	grid.draw_text(9, 1, "Irisha", Re, NO_COLOR, -1, 1);
//	grid.draw_text(20, 1, "Lion", Gr, Ye, 1, 0);

//	grid.draw_text(2, 14, "I", Re);
//	grid.draw_text(7, 10, "love", Re);
//	grid.draw_text(12, 14, "you", Re, NO_COLOR, 3);

//	Size size = grid.get_line_size(16, 28, 29, 8);
//	cout << "Size between (16, 28) and (29, 8) is " << size << endl;

//	grid.start_rainbow(RAINBOW_BEST);
//	grid.draw_filled_rect(0, 0, 29, 29, Gr);
//	grid.stop_rainbow();

//	grid.push_undo();
//	grid.draw_smile(15, 15, 10, Re, Ye);
// 	grid.draw_clock(4, 4, 3, Re, Or, Gr);
//	grid.pop_undo();

	grid.push_undo();
	grid.draw_text_rainbow(1, 1, "Welcome to    ", Re);
	grid.draw_text_rainbow(grid.get_size_y() - 6, 1, "Mosaic!    ", Bl, Gr, 0, 1);

//	grid.save("text-example.sav");
//	if (!grid.load("unexisting-file.txt")) exit(1);
//	if (!grid.load("main.cpp")) exit(1);
//	grid.load("text-example.sav");

	auto app = Gtk::Application::create("Migo.Mosaic");
	if (preferences["new-instance"] == "requested")
		app->set_flags(Gtk::Application::Flags::NON_UNIQUE);

	return app->make_window_and_run<MosaicWindow>(argc, argv, grid);
}

