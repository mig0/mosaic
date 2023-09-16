#include "screensaver.h"
#include <glibmm.h>

Screensaver::Screensaver(Grid &grid_) : grid(grid_), started(false), y() {
	srand(time(nullptr));
}

void Screensaver::start(ScreensaverType type_/* = SCREENSAVER_RANDOM*/) {
	type = type_;

	if (type == SCREENSAVER_RANDOM)
		type = (ScreensaverType)(1 + rand() % (SCREENSAVER_NUM_TYPES - 1));

	started = true;

	switch (type) {
	case SCREENSAVER_CLOCK:
		grid.push_undo(true);
		set_random_y();
		timer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Screensaver::draw_clock), 0);
		break;
	}
}

void Screensaver::stop() {
	started = false;
	timer.disconnect();

	switch (type) {
	case SCREENSAVER_CLOCK:
		grid.undo();
		break;
	}
}

bool Screensaver::is_active() {
	return started;
}

void set_time_hh_mm_ss(string &hh, string &mm, string &ss) {
	time_t now = time(nullptr);
	istringstream ctime_str(ctime(&now));

	string wday, dd, mmm;
	ctime_str >> wday >> dd >> mmm;
	ctime_str.ignore(1, ' ');
	getline(ctime_str, hh, ':');
	getline(ctime_str, mm, ':');
	getline(ctime_str, ss, ' ');
}

void Screensaver::set_random_y() {
	grid.undo();
	grid.push_undo(true);

	Size size_y = grid.get_size_y();
	Size size_x = grid.get_size_x();

	clock_len = size_x > 29 ? 27 : size_x > 19 ? 17 : 7;
	x = (size_x - clock_len) / 2;
	y = 5 + rand() % (size_y / 2);
}

bool Screensaver::draw_clock() {
	if (!is_active())
		return false;

	if (rand() % 20 == 0)
		set_random_y();

	string hh, mm, ss;
	set_time_hh_mm_ss(hh, mm, ss);

	grid.draw_rect(y - 3, x - 4, y + 7, x + clock_len + 3, Wh);
	grid.draw_filled_rect_2(y - 2, x - 3, y + 6, x + clock_len + 2, Ye, Gr);

	grid.draw_text(y, x +  0,  hh, Re);
	if (clock_len >= 17) {
	grid.draw_char(y, x +  7, ':', Or);
	grid.draw_text(y, x + 10,  mm, Re);
	if (clock_len >= 27) {
	grid.draw_char(y, x + 17, ':', Or);
	grid.draw_text(y, x + 20,  ss, Re);
	}
	}

	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Screensaver::draw_clock), 500);

	return false;
}
