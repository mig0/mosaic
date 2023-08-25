#ifndef __TESTGRID_H__
#define __TESTGRID_H__

#include "grid.h"

unsigned int sqrt(unsigned int n);

class TestGrid: public Grid {
public:
	TestGrid(Size size_y, Size size_x) : Grid(size_y, size_x) {}
	TestGrid(Size size_y, Size size_x, Color bg_color) : Grid(size_y, size_x, bg_color) {}

	void draw_filled_rect_rainbow(Index y1, Index x1, Index y2, Index x2, Color color0 = Re, bool without_corners = false);
	void draw_filled_circle_rainbow(Index y0, Index x0, Size radius, Color color0 = Re);
	void draw_filled_rhomb_rainbow(Index y0, Index x0, Size radius, Color color0 = Re);
	void draw_filled_square_rainbow(Index y0, Index x0, Size radius, Color color0 = Re);
	void draw_text_rainbow(Index y0, Index x0, string str, Color fg_color0 = Re, Color bg_color = NO_COLOR, int y_offset = 0, int x_offset = 0);

	Color get_next_color(Color color);
	Color get_prev_color(Color color);

	vector <vector <Color>> get_colors() { return colors; }
};

#endif /* __GRID_H__ */
