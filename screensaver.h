#ifndef __SCREENSAVER_H__
#define __SCREENSAVER_H__

#include "grid.h"

enum ScreensaverType {
	SCREENSAVER_RANDOM,
	SCREENSAVER_CLOCK,
	SCREENSAVER_NUM_TYPES,
};

class Screensaver {
public:
	Screensaver(Grid &grid);
	bool is_active();
	void start(ScreensaverType type = SCREENSAVER_RANDOM);
	void stop();

protected:
	Grid &grid;
	ScreensaverType type;
	bool started;
	sigc::connection timer;
	Index x, y;
	Size clock_len;

	void set_random_y();
	bool draw_clock();
};

#endif /* __SCREENSAVER_H__ */
