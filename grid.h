#ifndef __GRID_H__
#define __GRID_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sigc++/sigc++.h>
using namespace std;

extern bool IS_DUNGEON;

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

	CELL_BORDER = 0,
	CELL_CLOUD,
	CELL_FLOOR,
	CELL_CHAR,
	CELL_ENEMY,
	CELL_PORTAL,
	CELL_BARREL,
	CELL_PLATE,
	CELL_GATE0,
	CELL_GATE1,
	CELL_CRACK,
	CELL_BONES,
	CELL_ROCKS,
	CELL_STATUS,

/*
	NUM_COLORS,
	NO_COLOR = NUM_COLORS,
	COLOR_FIRST = 0,
	COLOR_LAST = NUM_COLORS - 1,
*/
#define NUM_COLORS (Color)(IS_DUNGEON ? CELL_STATUS + 1 : 6)
#define NO_COLOR NUM_COLORS
#define COLOR_FIRST (Color)0
#define COLOR_LAST (NUM_COLORS - 1)
};

enum RainbowType {
	RAINBOW_NONE,
	RAINBOW_BEST,
	RAINBOW_CONCENTRIC,
	RAINBOW_INDIVIDUAL,
	RAINBOW_CENTRICAL1,
	RAINBOW_CENTRICAL2,
	RAINBOW_CENTRICAL3,
	RAINBOW_CENTRICAL4,
	RAINBOW_DIAGONAL1,
	RAINBOW_DIAGONAL2,
	RAINBOW_VERTICAL,
	RAINBOW_HORIZONTAL,
};

enum ConcentricType {
	CONCENTRIC_CIRCLE,
	CONCENTRIC_RHOMB,
	CONCENTRIC_RECT,
};

enum MoveType {
	MOVE_TYPE_MOVE,
	MOVE_TYPE_COPY,
	MOVE_TYPE_SWAP,
	MOVE_TYPE_STEP,
};

enum Step {
	STEP_BACK = -1,
	STEP_NONE = 0,
	STEP_FORW = +1,
};

typedef int Index;
typedef int Size;
constexpr Index NO_INDEX = (Index)-1;

class Cell {
public:
	Index y;
	Index x;
	Color color;
	Cell(Index y_, Index x_, Color color_): y(y_), x(x_), color(color_) {}
};

constexpr bool TEXT_RAINBOW_SKIPS_COLOR_FOR_SPACE = true;

class Grid {
public:
	Grid(Size size_y, Size size_x);
	Grid(Size size_y, Size size_x, Color bg_color);

	Size get_size_y();
	Size get_size_x();
	Color get_bg_color();
	Color get_color(Index y, Index x);
	const char *get_color_name(Color color);
	string get_color_name(Index y, Index x);
	void set_color(Index y, Index x, Color color, bool ignore_rainbow = false);
	void swap_colors(Index y1, Index x1, Index y2, Index x2);

	sigc::signal<void(Index, Index, Color)> signal_on_set_color;
	sigc::signal<void(bool, bool)> signal_on_change_undo_redo;

	void draw_line(Index y1, Index x1, Index y2, Index x2, Color color);

	void draw_rect(Index y1, Index x1, Index y2, Index x2, Color color, bool without_corners = false);
	void draw_filled_rect(Index y1, Index x1, Index y2, Index x2, Color color, bool without_corners = false);
	void draw_filled_rect_2(Index y1, Index x1, Index y2, Index x2, Color color1, Color color2, bool without_corners = false);

	void iterate_circle_eighth(Size radius, function <bool (Index yd, Index xd)> code);
	void draw_circle(Index y0, Index x0, Size radius, Color color);
	void draw_filled_circle(Index y0, Index x0, Size radius, Color color);
	void draw_filled_circle_2(Index y0, Index x0, Size radius, Color color1, Color color2);
	Size get_circle_diagonal_delta(Size radius);
	void get_circle_triangle_delta(Size radius, Size &yd_, Size &xd_);

	void draw_rhomb(Index y0, Index x0, Size radius, Color color);
	void draw_filled_rhomb(Index y0, Index x0, Size radius, Color color);
	void draw_filled_rhomb_2(Index y0, Index x0, Size radius, Color color1, Color color2);

	void draw_square(Index y0, Index x0, Size radius, Color color);
	void draw_filled_square(Index y0, Index x0, Size radius, Color color);
	void draw_filled_square_2(Index y0, Index x0, Size radius, Color color1, Color color2);

	void draw_circle_crest(Index y0, Index x0, Size radius, Color color1, Color color2, int axes_or_diagonal);
	void draw_clock(Index y0, Index x0, Size radius, Color color1, Color color2, Color color3, unsigned int hours = 3, unsigned int minutes = 0);
	void draw_smile(Index y0, Index x0, Size radius, Color color1, Color color2);

	void draw_triangle(Index y1, Index x1, Index y2, Index x2, Index y3, Index x3, Color color);
	void draw_filled_triangle(Index y1, Index x1, Index y2, Index x2, Index y3, Index x3, Color color);
	void draw_filled_triangle_2(Index y1, Index x1, Index y2, Index x2, Index y3, Index x3, Color color1, Color color2);

	void draw_char(Index y0, Index x0, char ch, Color fg_color, Color bg_color = NO_COLOR);
	void draw_text(Index y0, Index x0, string str, Color fg_color, Color bg_color = NO_COLOR, int y_offset = 0, int x_offset = 0);

	bool is_coord_visible(Index y, Index x);
	bool is_coord_passable(Index y, Index x);
	bool is_color_real(Color color);

	Color get_next_color_nowrap(Color color);

	bool save(ostream &os);
	bool save(string filename);
	bool load(istream &is);
	bool load(string filename);

	Size get_line_size(Index y1, Index x1, Index y2, Index x2);

	void show();
	void clear();
	bool is_clear();

	void start_collecting();
	void stop_collecting();
	template <typename... Args>
	vector <shared_ptr <Cell>> collect(void (Grid::*draw_func)(Args... args), Args... args);

	void start_rainbow(RainbowType rainbow_type);
	void stop_rainbow();

	bool has_undo();
	void push_undo(bool always = false);
	void undo();
	bool has_redo();
	void redo();
	void move(Index y1, Index x1, Index y2, Index x2, Size y_offset, Size x_offset, MoveType type = MOVE_TYPE_MOVE);

protected:
	Size size_y;
	Size size_x;
	Color bg_color;
	vector <vector <Color>> colors;
	ostringstream bug;
	Size passable_threshold;
	bool is_collecting;
	vector <shared_ptr <Cell>> collected_cells;
	vector <vector <vector <Color>>> undo_layers;
	vector <vector <vector <Color>>> redo_layers;
	bool remove_redo_on_change;
	bool push_undo_pending;

	class Rainbow {
		bool started = false;
		RainbowType start_type;
		int defined = 0;
		RainbowType type;
		Index y1, x1, y2, x2;
		ConcentricType concentric_type;
		int individual_offset;
		void exit_with_bug(const string &error);
		Grid &grid;

	public:
		Rainbow(Grid &grid);
		bool is_started();
		void start(RainbowType type);
		void stop();
		bool is_defined();
		void push(RainbowType best_type, Index y1, Index x1, Index y2, Index x2, ConcentricType = CONCENTRIC_CIRCLE);
		void push(RainbowType best_type, Index y0, Index x0, Size radius, ConcentricType = CONCENTRIC_CIRCLE);
		void push_none();
		void pop();
		void reset_individual_color();
		void start_new_individual_color(Color fg_color = NO_COLOR, Color bg_color = NO_COLOR);
		Color get_color(Color color, Index y, Index x);
	};

	Rainbow rainbow = *this;

	void exit_with_bug();
	void assert_coord_visible(Index y, Index x);
	void assert_coord_passable(Index y, Index x);
	void assert_color_real(Color color);
	void push_undo_layer();
	void push_redo_layer();
};

#endif /* __GRID_H__ */
