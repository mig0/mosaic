#include "testgrid.h"

void TestGrid::draw_filled_rect_rainbow(Index y1, Index x1, Index y2, Index x2, Color color0/* = Re*/, bool without_corners/* = false*/) {
	Index min_y = min(y1, y2);
	Index max_y = max(y1, y2);
	Index min_x = min(x1, x2);
	Index max_x = max(x1, x2);
	Size size_y = max_y - min_y;
	Size size_x = max_x - min_x;
	Size size_min = min(size_y, size_x);

	rainbow.push_none();
	Color color = color0;
	for (int s = 0; s * 2 <= size_min; s++) {
		draw_filled_rect(min_y + s, min_x + s, max_y - s, max_x - s, color, without_corners);
		color = get_next_color(color);
	}
	rainbow.pop();
}

void TestGrid::draw_filled_circle_rainbow(Index y0, Index x0, Size radius, Color color0/* = Re*/) {
	rainbow.push_none();
	Color color = color0;
	for (int r = radius; r >= 0; r--) {
		draw_filled_circle(y0, x0, r, color);
		color = get_next_color(color);
	}
	rainbow.pop();
}

void TestGrid::draw_filled_rhomb_rainbow(Index y0, Index x0, Size radius, Color color0/* = Re*/) {
	rainbow.push_none();
	Color color = color0;
	for (int r = radius; r >= 0; r--) {
		draw_rhomb(y0, x0, r, color);
		color = get_next_color(color);
	}
	rainbow.pop();
}

void TestGrid::draw_filled_square_rainbow(Index y0, Index x0, Size radius, Color color0/* = Re*/) {
	rainbow.push_none();
	Color color = color0;
	for (int r = radius; r >= 0; r--) {
		draw_square(y0, x0, r, color);
		color = get_next_color(color);
	}
	rainbow.pop();
}

void TestGrid::draw_text_rainbow(Index y0, Index x0, string str, Color fg_color0, Color bg_color/* = NO_COLOR*/, int y_offset/* = 0*/, int x_offset/* = 0*/) {
	Color fg_color = fg_color0;
	assert_coord_visible(y0, x0);

	for (int c = 0; c < str.length(); c++) {
		if (fg_color == bg_color || fg_color == this->bg_color && bg_color == NO_COLOR)
			fg_color = get_next_color(fg_color);
		draw_char(y0 + c * y_offset, x0 + c * (4 + x_offset), str[c], fg_color, bg_color);
		if (str[c] != ' ' || !TEXT_RAINBOW_SKIPS_COLOR_FOR_SPACE)
			fg_color = get_next_color(fg_color);
	}
}

Color TestGrid::get_next_color(Color color) {
	int next_color = ((int)color + 1) % NUM_COLORS;
	return (Color)next_color;
}

Color TestGrid::get_prev_color(Color color) {
	int prev_color = ((int)color - 1) % NUM_COLORS;
	return (Color)prev_color;
}

