#ifndef __GRID_H__
#define __GRID_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sigc++/sigc++.h>
using namespace std;

enum Color {
	COLOR_WHITE = 0,
	COLOR_YELLOW,
	COLOR_RED,
	COLOR_ORANGE,
	COLOR_GREEN,
	COLOR_BLUE,

	Wh = 0,
	Ye = 1,
	Re = 2,
	Or = 3,
	Gr = 4,
	Bl = 5,
	NUM_COLORS,
	NO_COLOR = NUM_COLORS,
	COLOR_FIRST = 0,
	COLOR_LAST = NUM_COLORS - 1,
};

enum Step {
	STEP_BACK = -1,
	STEP_NONE = 0,
	STEP_FORW = +1,
};

typedef int Index;
typedef int Size;
constexpr Index NO_INDEX = (Index)-1;

class Grid {
	Size size_y;
	Size size_x;
	vector <vector <Color>> colors;
	ostringstream bug;
	Size passable_threshold;

public:
	Size get_size_y();
	Size get_size_x();
	Color get_color(Index y, Index x);
	const char *get_color_name(Color color);
	string get_color_name(Index y, Index x);
	void set_color(Index y, Index x, Color color);

	sigc::signal<void(Index, Index, Color)> signal_on_set_color;

	void draw_line(Index y1, Index x1, Index y2, Index x2, Color color);

	void draw_rect(Index y1, Index x1, Index y2, Index x2, Color color, bool without_corners = false);
	void draw_filled_rect(Index y1, Index x1, Index y2, Index x2, Color color, bool without_corners = false);
	void draw_filled_rect_2(Index y1, Index x1, Index y2, Index x2, Color color1, Color color2, bool without_corners = false);
	void draw_filled_rect_rainbow(Index y1, Index x1, Index y2, Index x2, Color color0 = Re, bool without_corners = false);

	void draw_circle(Index y0, Index x0, Size radius, Color color);
	void draw_filled_circle(Index y0, Index x0, Size radius, Color color);
	void draw_filled_circle_2(Index y0, Index x0, Size radius, Color color1, Color color2);
	void draw_filled_circle_rainbow(Index y0, Index x0, Size radius, Color color0 = Re);
	Size get_circle_diagonal_delta(Size radius);

	void draw_rhomb(Index y0, Index x0, Size radius, Color color);
	void draw_filled_rhomb(Index y0, Index x0, Size radius, Color color);
	void draw_filled_rhomb_2(Index y0, Index x0, Size radius, Color color1, Color color2);
	void draw_filled_rhomb_rainbow(Index y0, Index x0, Size radius, Color color0 = Re);

	void draw_square(Index y0, Index x0, Size radius, Color color);
	void draw_filled_square(Index y0, Index x0, Size radius, Color color);
	void draw_filled_square_2(Index y0, Index x0, Size radius, Color color1, Color color2);
	void draw_filled_square_rainbow(Index y0, Index x0, Size radius, Color color0 = Re);

	void draw_circle_crest(Index y0, Index x0, Size radius, Color color1, Color color2, int axes_or_diagonal);
	void draw_clock(Index y0, Index x0, Size radius, Color color1, Color color2, Color color3, unsigned int hours = 3, unsigned int minutes = 0);
	void draw_smile(Index y0, Index x0, Size radius, Color color1, Color color2);

	void draw_char(Index y0, Index x0, char ch, Color fg_color, Color bg_color = NO_COLOR);
	void draw_text(Index y0, Index x0, string str, Color fg_color, Color bg_color = NO_COLOR, int y_offset = 0, int x_offset = 0);
	void draw_text_rainbow(Index y0, Index x0, string str, Color fg_color0 = Re, Color bg_color = NO_COLOR, int y_offset = 0, int x_offset = 0);

	bool is_coord_visible(Index y, Index x);
	bool is_coord_passable(Index y, Index x);
	bool is_color_real(Color color);

	Color get_next_color_nowrap(Color color);
	Color get_next_color(Color color);
	Color get_prev_color(Color color);

	bool save(ostream &os);
	bool save(string filename);
	bool load(istream &is);
	bool load(string filename);

	Size get_line_size(Index y1, Index x1, Index y2, Index x2);

	void show();
	void clear();

	Grid(Size size_y0, Size size_x0);//: size_y(size_y0), size_x(size_x0);

protected:
	void exit_with_bug();
	void assert_coord_visible(Index y, Index x);
	void assert_coord_passable(Index y, Index x);
	void assert_color_real(Color color);
};

#endif /* __GRID_H__ */
