#include "program.h"
#include "grid.h"
#include "graphics.h"

int main(int argc, char *argv[], char *envp[]) {
	map <string, int> preferences = parse_options(argc, argv, envp);

	Grid grid(preferences["size_y"], preferences["size_x"]);

	grid.push_undo();
	grid.start_rainbow(RAINBOW_BEST);
	grid.draw_text(1, 1, "Welcome to    ", Re);
	grid.draw_text(grid.get_size_y() - 6, 1, "Mosaic!    ", Bl, Gr, 0, 1);
	grid.stop_rainbow();

	if (initial_sav_filename) {
		grid.push_undo();
		grid.load(initial_sav_filename);
	}

	auto app = Gtk::Application::create("Migo.Mosaic");
	if (preferences["new-instance"])
		app->set_flags(Gtk::Application::Flags::NON_UNIQUE);

	return app->make_window_and_run<MosaicWindow>(argc, argv, grid);
}

