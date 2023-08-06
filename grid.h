#ifndef __GRID_H__
#define __GRID_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <sigc++/sigc++.h>
#include "font3x5.h"
using namespace std;

//#define NAIVE_LINE_ALGORYTHM 1
bool const TEXT_RAINBOW_SKIPS_COLOR_FOR_SPACE = true;
bool const TEXT_RAINBOW_SKIPS_WHITE_ON_NO_BG = true;

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

typedef unsigned int Index;
typedef unsigned int Size;
constexpr Index NO_INDEX = (Index)-1;

class Coord {
	Index y;
	Index x;
public:
	Coord(Index y0, Index x0): y(y0), x(x0) {}
	friend class Grid;
};

class Grid {
	Size size_y;
	Size size_x;
	vector <vector <Color>> colors;

public:
	Size get_size_y() {
		return size_y;
	}

	Size get_size_x() {
		return size_x;
	}

	Color get_color(Index y, Index x) {
		return colors[y][x];
	}

	const char *get_color_name(Color color) {
		return
			color == Re ? "red" :
			color == Wh ? "white" :
			color == Ye ? "yellow" :
			color == Or ? "orange" :
			color == Bl ? "blue" :
			color == Gr ? "green" :
			throw "Invalid color";
	}

	string get_color_name(Index y, Index x) {
		Color color = get_color(y, x);
		return get_color_name(color);
	}

	sigc::signal<void(Index, Index, Color)> signal_on_set_color;

	void set_color(Index y, Index x, Color color) {
		assert_coords(y, x);
		colors[y][x] = color;
		signal_on_set_color.emit(y, x, color);
	}

	int min(int a, int b) {
		return a < b ? a : b;
	}

	int max(int a, int b) {
		return a > b ? a : b;
	}

	unsigned int sqrt(unsigned int n) {
		int k = 1;
		for (; k * k <= n; k++);
		return k - 1;
	}

	void set_rect_color(Index y1, Index x1, Index y2, Index x2, Color color, bool without_corners = false) {
		assert_coords(y1, x1);
		assert_coords(y2, x2);

		for (Index y = min(y1, y2); y <= max(y1, y2); y++) {
			for (Index x = min(x1, x2); x <= max(x1, x2); x++) {
				if (without_corners
					? (y == y1 || y == y2) && x != x1 && x != x2 || (x == x1 || x == x2) && y != y1 && y != y2
					: y == y1 || y == y2 || x == x1 || x == x2
				)
					set_color(y, x, color);
			}
		}
	}

	void set_filled_rect_color(Index y1, Index x1, Index y2, Index x2, Color color, bool without_corners = false) {
		assert_coords(y1, x1);
		assert_coords(y2, x2);

		for (Index y = min(y1, y2); y <= max(y1, y2); y++) {
			for (Index x = min(x1, x2); x <= max(x1, x2); x++) {
				if (!(without_corners && (y == y1 || y == y2) && (x == x1 || x == x2)))
					set_color(y, x, color);
			}
		}
	}

	void set_filled_rect_2_color(Index y1, Index x1, Index y2, Index x2, Color color1, Color color2, bool without_corners = false) {
		set_filled_rect_color(y1, x1, y2, x2, color1, without_corners);
		set_rect_color(y1, x1, y2, x2, color2, without_corners);
	}

	void set_filled_rect_rainbow_color(Index y1, Index x1, Index y2, Index x2, Color color0 = Re, bool without_corners = false) {
		Index min_y = min(y1, y2);
		Index max_y = max(y1, y2);
		Index min_x = min(x1, x2);
		Index max_x = max(x1, x2);
		Size size_y = max_y - min_y;
		Size size_x = max_x - min_x;
		Size size_min = min(size_y, size_x);

		Color color = color0;
		for (int s = 0; s * 2 <= size_min; s++) {
			set_filled_rect_color(min_y + s, min_x + s, max_y - s, max_x - s, color, without_corners);
			color = get_next_color(color);
		}
	}

	void set_rhomb_color(Index y0, Index x0, Size radius, Color color) {
		assert_coords(y0, x0);

		if (radius == 0) {
			set_color(y0, x0, color);
			return;
		}

		for (Index q = 0; q < 4; q++) {
			for (Index r = 0; r < radius; r++) {
				Index y =
					q == 0 ? y0 - radius + r :
					q == 1 ? y0 + r :
					q == 2 ? y0 + radius - r :
					q == 3 ? y0 - r :
					0;
				Index x =
					q == 0 ? x0 + r :
					q == 1 ? x0 + radius - r :
					q == 2 ? x0 - r :
					q == 3 ? x0 - radius + r :
					0;
				set_color(y, x, color);
			}
		}
	}

	void set_filled_rhomb_color(Index y0, Index x0, Size radius, Color color) {
		for (Index r = 0; r <= radius; r++) {
			set_rhomb_color(y0, x0, r, color);
		}
	}

	void set_filled_rhomb_2_color(Index y0, Index x0, Size radius, Color color1, Color color2) {
		set_filled_rhomb_color(y0, x0, radius, color1);
		set_rhomb_color(y0, x0, radius, color2);
	}

	void set_filled_rhomb_rainbow_color(Index y0, Index x0, Size radius, Color color0 = Re) {
		Color color = color0;
		for (int r = radius; r >= 0; r--) {
			set_rhomb_color(y0, x0, r, color);
			color = get_next_color(color);
		}
	}

	void set_circle_color(Index y0, Index x0, Size radius, Color color) {
		assert_coords(y0, x0);

		if (radius == 0) {
			set_color(y0, x0, color);
			return;
		}

		Index xd = 0;
		Index yd = radius;
		int decesion_threashold = 3 - 2 * radius;

		while (true) {
			set_color(y0 + yd, x0 + xd, color);
			set_color(y0 + yd, x0 - xd, color);
			set_color(y0 - yd, x0 + xd, color);
			set_color(y0 - yd, x0 - xd, color);
			set_color(y0 + xd, x0 + yd, color);
			set_color(y0 + xd, x0 - yd, color);
			set_color(y0 - xd, x0 + yd, color);
			set_color(y0 - xd, x0 - yd, color);

			if (yd < xd) break;

			xd++;
			if (decesion_threashold > 0) {
				yd--;
				decesion_threashold += 4 * (xd - yd) + 10;
			} else {
				decesion_threashold += 4 * xd + 6;
			}
		}
	}

	void set_filled_circle_color(Index y0, Index x0, Size radius, Color color) {
		assert_coords(y0, x0);

		if (radius == 0) {
			set_color(y0, x0, color);
			return;
		}

		Index xd = 0;
		Index yd = radius;
		int decesion_threashold = 3 - 2 * radius;

		while (true) {
			set_line_color(y0 + yd, x0 + xd, y0 + yd, x0 - xd, color);

			set_line_color(y0 - yd, x0 + xd, y0 - yd, x0 - xd, color);

			set_line_color(y0 + xd, x0 + yd, y0 + xd, x0 - yd, color);

			set_line_color(y0 - xd, x0 + yd, y0 - xd, x0 - yd, color);

			if (yd < xd) break;

			xd++;
			if (decesion_threashold > 0) {
				yd--;
				decesion_threashold += 4 * (xd - yd) + 10;
			} else {
				decesion_threashold += 4 * xd + 6;
			}
		}
	}

	void set_filled_circle_2_color(Index y0, Index x0, Size radius, Color color1, Color color2) {
		set_filled_circle_color(y0, x0, radius, color1);
		set_circle_color(y0, x0, radius, color2);
	}

	void set_filled_circle_rainbow_color(Index y0, Index x0, Size radius, Color color0 = Re) {
		Color color = color0;
		for (int r = radius; r >= 0; r--) {
			set_filled_circle_color(y0, x0, r, color);
			color = get_next_color(color);
		}
	}

	Size get_circle_diagonal_delta(Size radius) {
		Index xd = 0;
		Index yd = radius;
		int decesion_threashold = 3 - 2 * radius;

		while (true) {
			if (yd == xd) return xd;
			if (yd < xd) return yd;

			xd++;
			if (decesion_threashold > 0) {
				yd--;
				decesion_threashold += 4 * (xd - yd) + 10;
			} else {
				decesion_threashold += 4 * xd + 6;
			}
		}
	}

	/* axes_or_diagonal may be 0 (no crest), 1 (axes crest), 2 (diagonal crest) or 3 (both) */
	void set_circle_crest_color(Index y0, Index x0, Size radius, Color color1, Color color2, int axes_or_diagonal) {
		if (axes_or_diagonal <= 0 || axes_or_diagonal >= 4) {
			cout << "Ignoring invalid axes_or_diagonal argument (" << axes_or_diagonal << "), should be 1, 2 or 3" << endl;
		}
		if (axes_or_diagonal == 1 || axes_or_diagonal == 3) {
			set_line_color(y0, x0 - radius, y0, x0 + radius, color2);
			set_line_color(y0 - radius, x0, y0 + radius, x0, color2);
		}
		if (axes_or_diagonal == 2 || axes_or_diagonal == 3) {
			Size delta = get_circle_diagonal_delta(radius);
			set_line_color(y0 - delta, x0 - delta, y0 + delta, x0 + delta, color2);
			set_line_color(y0 - delta, x0 + delta, y0 + delta, x0 - delta, color2);
		}
		set_circle_color(y0, x0, radius, color1);
	}

	void set_clock_color(Index y0, Index x0, Size radius, Color color1, Color color2, Color color3, unsigned int hours = 3, unsigned int minutes = 0) {
		set_circle_color(y0, x0, radius, color1);
		set_line_color(y0, x0, y0, x0 + radius - 1 - (int)(radius / 3), color3);
		set_line_color(y0, x0, y0 - radius + 1 + (int)(radius / 8), x0, color2);
	}

	void set_line_color(Index y1, Index x1, Index y2, Index x2, Color color) {
		assert_coords(y1, x1);
		assert_coords(y2, x2);

		Size y_len = abs((int)y2 - (int)y1) + 1;
		Size x_len = abs((int)x2 - (int)x1) + 1;
		Step y_step = y2 > y1 ? STEP_FORW : y2 == y1 ? STEP_NONE : STEP_BACK;
		Step x_step = x2 > x1 ? STEP_FORW : x2 == x1 ? STEP_NONE : STEP_BACK;

		Index y = y1;
		Index x = x1;

//		cout << "Drawing line (" << y1 << ", " << x1 << ") - (" << y2 << ", " << x2 << ")" << endl;
//		cout << "    y_len=" << y_len << ", xlen=" << x_len << ", y_step=" << y_step << ", x_step=" << x_step << endl;

		if (y_len > x_len) {
			int a = 2 * (x_len - 1);
			int b = a - 2 * (y_len - 1);
			int p = a - (y_len - 1);

			for (; ; y += (int)y_step) {
				set_color(y, x, color);
#ifdef NAIVE_LINE_ALGORYTHM
				x = x1 + (abs((int)y + (int)y_step - (int)y1) / (float)y_len * (float)x_len) * (int)x_step;
#else
				if (p >= 0) x += (int)x_step;
				p += (p < 0 ? a : b);
#endif
				if (y == y2) break;
			}
		} else {
			int a = 2 * (y_len - 1);
			int b = a - 2 * (x_len - 1);
			int p = a - (x_len - 1);

			for (; ; x += (int)x_step) {
//				cout << "a=" << a << ", b=" << b << ", p=" << p << ", y=" << y << ", x=" << x << endl;
				set_color(y, x, color);
#ifdef NAIVE_LINE_ALGORYTHM
				y = y1 + (abs((int)x + (int)x_step - (int)x1) / (float)x_len * (float)y_len) * (int)y_step;
#else
				if (p >= 0) y += (int)y_step;
				p += (p < 0 ? a : b);
#endif
				if (x == x2) break;
			}
		}
	}

	void set_char_color(Index y0, Index x0, char ch, Color fg_color, Color bg_color=NO_COLOR) {
		const int *bitmap = get_font3x5_char_bitmap(ch);
		for (int yd = -1; yd < 6; yd++) {
			for (int xd = -1; xd < 4; xd++) {
				Index y = y0 + yd;
				Index x = x0 + xd;
				if (y >= size_y || x >= size_x)
					continue;
				if (yd < 0 || yd >= 5 || xd < 0 || xd >= 5) {
					if (bg_color != NO_COLOR)
						set_color(y, x, bg_color);
					continue;
				}
				bool is_point_set = bitmap[yd] & (1 << xd);
				if (is_point_set)
					set_color(y, x, fg_color);
				if (!is_point_set && bg_color != NO_COLOR)
					set_color(y, x, bg_color);
			}
		}
	}

	void set_text_color(Index y0, Index x0, string str, Color fg_color, Color bg_color = NO_COLOR, int y_offset = 0, int x_offset = 0) {
		assert_coords(y0, x0);

		for (int c = 0; c < str.length(); c++) {
			set_char_color(y0 + c * y_offset, x0 + c * (4 + x_offset), str[c], fg_color, bg_color);
		}
	}

	void set_text_rainbow_color(Index y0, Index x0, string str, Color fg_color0 = Re, Color bg_color = NO_COLOR, int y_offset = 0, int x_offset = 0) {
		Color fg_color = fg_color0;
		assert_coords(y0, x0);

		for (int c = 0; c < str.length(); c++) {
			if (fg_color == bg_color || TEXT_RAINBOW_SKIPS_WHITE_ON_NO_BG && fg_color == Wh && bg_color == NO_COLOR)
				fg_color = get_next_color(fg_color);
			set_char_color(y0 + c * y_offset, x0 + c * (4 + x_offset), str[c], fg_color, bg_color);
			if (str[c] != ' ' || !TEXT_RAINBOW_SKIPS_COLOR_FOR_SPACE)
				fg_color = get_next_color(fg_color);
		}
	}

	bool are_coords_valid(Index y, Index x) {
		return y < size_y && x < size_x;
	}

	void assert_coords(Index y, Index x) {
		if (!are_coords_valid(y, x)) {
			show();
			cout << "Bug in code, invalid coords (" << y << ", " << x << ")" << endl;
			exit(1);
		}
	}

	string get_color_term_str(Index y, Index x) {
		string str;
		Color color = get_color(y, x);
		int term_color =
			color == Wh ? 97 :
			color == Ye ? 93 :
			color == Gr ? 92 :
			color == Bl ? 94 :
			color == Re ? 31 :
			color == Or ? 33 :
			0;
		str += "[";
		str += to_string(term_color);
		str += "m█▉[m";

		return str;
	}

	Color get_next_color_nowrap(Color color) {
		int next_color = ((int)color + 1);
		return (Color)next_color;
	}

	Color get_next_color(Color color) {
		int next_color = ((int)color + 1) % NUM_COLORS;
		return (Color)next_color;
	}

	Color get_prev_color(Color color) {
		int prev_color = ((int)color - 1) % NUM_COLORS;
		return (Color)prev_color;
	}

	char get_save_char_for_color(Color color) {
		return
			color == Re ? '#' :
			color == Wh ? '.' :
			color == Ye ? ':' :
			color == Or ? 'O' :
			color == Bl ? '*' :
			color == Gr ? '@' :
			throw "Invalid color";
	}

	Color get_color_for_save_char(char ch) {
		return
			ch == get_save_char_for_color(Re) ? Re :
			ch == get_save_char_for_color(Wh) ? Wh :
			ch == get_save_char_for_color(Ye) ? Ye :
			ch == get_save_char_for_color(Or) ? Or :
			ch == get_save_char_for_color(Bl) ? Bl :
			ch == get_save_char_for_color(Gr) ? Gr :
			throw string("Invalid save char ") + ch;
	}

	char get_color_save_char(Index y, Index x) {
		Color color = get_color(y, x);
		return get_save_char_for_color(color);
	}

	bool save(ostream &os) {
		os << "# Save file for grid " << size_x << "x" << size_y << endl;

		for (Index y = 0; y < size_y; y++) {
			for (Index x = 0; x < size_x; x++) {
				os << get_color_save_char(y, x);
			}
			os << endl;
		}

		return true;
	}

	bool save(string filename) {
		fstream fs;

		fs.open(filename.c_str(), ios_base::out);
		if (!fs.is_open()) {
			cerr << "Can't open file " << filename << " for saving" << endl;
			return false;
		}

		bool success = save(fs);

		fs.close();
		if (fs.is_open()) {
			cerr << "Can't close file " << filename << " for saving" << endl;
			return false;
		}

		return success;
	}

	bool load(istream &is) {
		char first_line[40];
		is.getline(first_line, 40);

		string first_line_str = first_line;
		string expected_prefix = "# Save file for grid ";
		if (first_line_str.compare(0, expected_prefix.size(), expected_prefix) != 0) {
			cerr << "Invalid file format, no expected line (" << expected_prefix << "XxY) found" << endl;
			return false;
		}

		string expected_size_str = to_string(size_x) + 'x' + to_string(size_y);
		string actual_size_str = first_line_str.substr(expected_prefix.size(), first_line_str.size());
		if (actual_size_str != expected_size_str) {
			cerr << "Wrong size (" << actual_size_str << ") when expected (" << expected_size_str << ")" << endl;
			return false;
		}

		for (Index y = 0; y < size_y; y++) {
			for (Index x = 0; x < size_x; x++) {
				char ch;
				is >> ch;
				Color color = get_color_for_save_char(ch);
				set_color(y, x, color);
			}
			is.ignore();
		}

		return true;
	}

	bool load(string filename) {
		fstream fs;

		fs.open(filename.c_str(), ios_base::in);
		if (!fs.is_open()) {
			cerr << "Can't open file " << filename << " for loading" << endl;
			return false;
		}

		bool success = load(fs);

		fs.close();
		if (fs.is_open()) {
			cerr << "Can't close file " << filename << " for loading" << endl;
			return false;
		}

		return success;
	}

	Size get_line_size(Index y1, Index x1, Index y2, Index x2) {
		Index min_y = min(y1, y2);
		Index max_y = max(y1, y2);
		Index min_x = min(x1, x2);
		Index max_x = max(x1, x2);
		Size size_y = max_y - min_y;
		Size size_x = max_x - min_x;
		return sqrt(size_y * size_y + size_x * size_x);
	}

	void show() {
		for (Index y = 0; y < size_y; y++) {
			for (Index x = 0; x < size_x; x++) {
				cout << get_color_term_str(y, x);
			}
			cout << endl;
		}
	}

	void clear() {
		for (Index y = 0; y < size_y; y++) {
			for (Index x = 0; x < size_x; x++) {
				set_color(y, x, Wh);
			}
		}
	}

	Grid(Size size_y0, Size size_x0): size_y(size_y0), size_x(size_x0) {
		colors = vector <vector <Color>>(size_y0, vector <Color>(size_x0));
		clear();
	}
};

#endif /* __GRID_H__ */
