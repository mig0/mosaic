#include "grid.h"
#include "font3x5.h"

//#define NAIVE_LINE_ALGORYTHM 1
bool const TEXT_RAINBOW_SKIPS_COLOR_FOR_SPACE = true;
bool const TEXT_RAINBOW_SKIPS_WHITE_ON_NO_BG = true;

Size Grid::get_size_y() {
	return size_y;
}

Size Grid::get_size_x() {
	return size_x;
}

Color Grid::get_color(Index y, Index x) {
	return colors[y][x];
}

const char *Grid::get_color_name(Color color) {
	return
		color == Re ? "red" :
		color == Wh ? "white" :
		color == Ye ? "yellow" :
		color == Or ? "orange" :
		color == Bl ? "blue" :
		color == Gr ? "green" :
		throw "Invalid color";
}

string Grid::get_color_name(Index y, Index x) {
	Color color = get_color(y, x);
	return get_color_name(color);
}

void Grid::set_color(Index y, Index x, Color color, bool ignore_rainbow/* = false*/) {
	assert_coord_passable(y, x);
	assert_color_real(color);
	if (!is_coord_visible(y, x))
		return;

	if (!ignore_rainbow && rainbow.is_defined())
		color = rainbow.get_color(color, y, x);

	if (is_collecting) {
		collected_cells.push_back(make_shared <Cell>(y, x, color));
		return;
	}

	colors[y][x] = color;
	signal_on_set_color.emit(y, x, color);
}

int min(int a, int b) {
	return a < b ? a : b;
}

int min(int a, int b, int c) {
	return min(min(a, b), c);
}

int max(int a, int b) {
	return a > b ? a : b;
}

int max(int a, int b, int c) {
	return max(max(a, b), c);
}

unsigned int sqrt(unsigned int n) {
	int k = 1;
	for (; k * k <= n; k++);
	return (k * k - n) * 1 <= (n - (k - 1) * (k - 1)) * 2 ? k : k - 1;
}

void Grid::draw_line(Index y1, Index x1, Index y2, Index x2, Color color) {
	assert_coord_passable(y1, x1);
	assert_coord_passable(y2, x2);

	Size y_len = abs((int)y2 - (int)y1) + 1;
	Size x_len = abs((int)x2 - (int)x1) + 1;
	Step y_step = y2 > y1 ? STEP_FORW : y2 == y1 ? STEP_NONE : STEP_BACK;
	Step x_step = x2 > x1 ? STEP_FORW : x2 == x1 ? STEP_NONE : STEP_BACK;

	rainbow.push(y_len > x_len ? RAINBOW_VERTICAL : RAINBOW_HORIZONTAL, y1, x1, y2, x2, CONCENTRIC_RECT);

	Index y = y1;
	Index x = x1;

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

	rainbow.pop();
}

void Grid::draw_rect(Index y1, Index x1, Index y2, Index x2, Color color, bool without_corners/* = false*/) {
	assert_coord_passable(y1, x1);
	assert_coord_passable(y2, x2);

	rainbow.push(RAINBOW_DIAGONAL1, y1, x1, y2, x2, CONCENTRIC_RECT);

	for (Index y = min(y1, y2); y <= max(y1, y2); y++) {
		for (Index x = min(x1, x2); x <= max(x1, x2); x++) {
			if (without_corners
				? (y == y1 || y == y2) && x != x1 && x != x2 || (x == x1 || x == x2) && y != y1 && y != y2
				: y == y1 || y == y2 || x == x1 || x == x2
			)
				set_color(y, x, color);
		}
	}

	rainbow.pop();
}

void Grid::draw_filled_rect(Index y1, Index x1, Index y2, Index x2, Color color, bool without_corners/* = false*/) {
	assert_coord_passable(y1, x1);
	assert_coord_passable(y2, x2);

	rainbow.push(RAINBOW_CONCENTRIC, y1, x1, y2, x2, CONCENTRIC_RECT);

	for (Index y = min(y1, y2); y <= max(y1, y2); y++) {
		for (Index x = min(x1, x2); x <= max(x1, x2); x++) {
			if (!(without_corners && (y == y1 || y == y2) && (x == x1 || x == x2)))
				set_color(y, x, color);
		}
	}

	rainbow.pop();
}

void Grid::draw_filled_rect_2(Index y1, Index x1, Index y2, Index x2, Color color1, Color color2, bool without_corners/* = false*/) {
	draw_filled_rect(y1, x1, y2, x2, color1, without_corners);
	rainbow.push_none();
	draw_rect(y1, x1, y2, x2, color2, without_corners);
	rainbow.pop();
}

void Grid::draw_filled_rect_rainbow(Index y1, Index x1, Index y2, Index x2, Color color0/* = Re*/, bool without_corners/* = false*/) {
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

void Grid::iterate_circle_eighth(Size radius, function <bool (Index yd, Index xd)> code) {
	Index xd = 0;
	Index yd = radius;
	int decesion_threashold = 3 - 2 * radius;

	while (true) {
		if (code(yd, xd)) break;
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

void Grid::draw_circle(Index y0, Index x0, Size radius, Color color) {
	assert_coord_visible(y0, x0);

	if (radius == 0) {
		set_color(y0, x0, color);
		return;
	}

	rainbow.push(RAINBOW_VERTICAL, y0, x0, radius);

	iterate_circle_eighth(radius, [=](Index yd, Index xd) {
		set_color(y0 + yd, x0 + xd, color);
		set_color(y0 + yd, x0 - xd, color);
		set_color(y0 - yd, x0 + xd, color);
		set_color(y0 - yd, x0 - xd, color);
		set_color(y0 + xd, x0 + yd, color);
		set_color(y0 + xd, x0 - yd, color);
		set_color(y0 - xd, x0 + yd, color);
		set_color(y0 - xd, x0 - yd, color);
		return false;
	});

	rainbow.pop();
}

void Grid::draw_filled_circle(Index y0, Index x0, Size radius, Color color) {
	assert_coord_visible(y0, x0);

	if (radius == 0) {
		set_color(y0, x0, color);
		return;
	}

	rainbow.push(RAINBOW_CONCENTRIC, y0, x0, radius);

	iterate_circle_eighth(radius, [=](Index yd, Index xd) {
		draw_line(y0 + yd, x0 + xd, y0 + yd, x0 - xd, color);
		draw_line(y0 - yd, x0 + xd, y0 - yd, x0 - xd, color);
		draw_line(y0 + xd, x0 + yd, y0 + xd, x0 - yd, color);
		draw_line(y0 - xd, x0 + yd, y0 - xd, x0 - yd, color);
		return false;
	});

	rainbow.pop();
}

void Grid::draw_filled_circle_2(Index y0, Index x0, Size radius, Color color1, Color color2) {
	draw_filled_circle(y0, x0, radius, color1);
	rainbow.push_none();
	draw_circle(y0, x0, radius, color2);
	rainbow.pop();
}

void Grid::draw_filled_circle_rainbow(Index y0, Index x0, Size radius, Color color0/* = Re*/) {
	rainbow.push_none();
	Color color = color0;
	for (int r = radius; r >= 0; r--) {
		draw_filled_circle(y0, x0, r, color);
		color = get_next_color(color);
	}
	rainbow.pop();
}

Size Grid::get_circle_diagonal_delta(Size radius) {
	Size sz = 0;
	iterate_circle_eighth(radius, [&](Index yd, Index xd) {
		if (yd == xd) { sz = xd; return true; }
		if (yd < xd) { sz = yd; return true; }
		return false;
	});
	return sz;
}

void Grid::get_circle_triangle_delta(Size radius, Size &yd_, Size &xd_) {
	iterate_circle_eighth(radius, [&](Index yd, Index xd) {
		if ((radius * 2 - yd) * (radius * 2 - yd) - 4 * xd * xd == 0) {
			yd_ = yd - 1;
			xd_ = xd + 1;
			return true;
		}
		return false;
	});
}

void Grid::draw_rhomb(Index y0, Index x0, Size radius, Color color) {
	assert_coord_visible(y0, x0);

	if (radius == 0) {
		set_color(y0, x0, color);
		return;
	}

	rainbow.push(RAINBOW_VERTICAL, y0, x0, radius);

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

	rainbow.pop();
}

void Grid::draw_filled_rhomb(Index y0, Index x0, Size radius, Color color) {
	rainbow.push(RAINBOW_CONCENTRIC, y0, x0, radius, CONCENTRIC_RHOMB);
	for (Index r = 0; r <= radius; r++) {
		draw_rhomb(y0, x0, r, color);
	}
	rainbow.pop();
}

void Grid::draw_filled_rhomb_2(Index y0, Index x0, Size radius, Color color1, Color color2) {
	draw_filled_rhomb(y0, x0, radius, color1);
	rainbow.push_none();
	draw_rhomb(y0, x0, radius, color2);
	rainbow.pop();
}

void Grid::draw_filled_rhomb_rainbow(Index y0, Index x0, Size radius, Color color0/* = Re*/) {
	rainbow.push_none();
	Color color = color0;
	for (int r = radius; r >= 0; r--) {
		draw_rhomb(y0, x0, r, color);
		color = get_next_color(color);
	}
	rainbow.pop();
}

void Grid::draw_square(Index y0, Index x0, Size radius, Color color) {
	rainbow.push(RAINBOW_DIAGONAL1, y0, x0, radius);
	draw_rect(y0 - radius, x0 - radius, y0 + radius, x0 + radius, color);
	rainbow.pop();
}

void Grid::draw_filled_square(Index y0, Index x0, Size radius, Color color) {
	rainbow.push(RAINBOW_CONCENTRIC, y0, x0, radius, CONCENTRIC_RECT);
	for (Index r = 0; r <= radius; r++) {
		draw_square(y0, x0, r, color);
	}
	rainbow.pop();
}

void Grid::draw_filled_square_2(Index y0, Index x0, Size radius, Color color1, Color color2) {
	draw_filled_square(y0, x0, radius, color1);
	rainbow.push_none();
	draw_square(y0, x0, radius, color2);
	rainbow.pop();
}

void Grid::draw_filled_square_rainbow(Index y0, Index x0, Size radius, Color color0/* = Re*/) {
	rainbow.push_none();
	Color color = color0;
	for (int r = radius; r >= 0; r--) {
		draw_square(y0, x0, r, color);
		color = get_next_color(color);
	}
	rainbow.pop();
}

/* axes_or_diagonal may be 0 (no crest), 1 (axes crest), 2 (diagonal crest) or 3 (both) */
void Grid::draw_circle_crest(Index y0, Index x0, Size radius, Color color1, Color color2, int axes_or_diagonal) {
	if (axes_or_diagonal <= 0 || axes_or_diagonal >= 4) {
		cout << "Ignoring invalid axes_or_diagonal argument (" << axes_or_diagonal << "), should be 1, 2 or 3" << endl;
	}
	rainbow.push_none();
	if (axes_or_diagonal == 1 || axes_or_diagonal == 3) {
		draw_line(y0, x0 - radius, y0, x0 + radius, color2);
		draw_line(y0 - radius, x0, y0 + radius, x0, color2);
	}
	if (axes_or_diagonal == 2 || axes_or_diagonal == 3) {
		Size delta = get_circle_diagonal_delta(radius);
		draw_line(y0 - delta, x0 - delta, y0 + delta, x0 + delta, color2);
		draw_line(y0 - delta, x0 + delta, y0 + delta, x0 - delta, color2);
	}
	rainbow.pop();
	draw_circle(y0, x0, radius, color1);
}

void Grid::draw_clock(Index y0, Index x0, Size radius, Color color1, Color color2, Color color3, unsigned int hours/* = 3*/, unsigned int minutes/* = 0*/) {
	draw_circle(y0, x0, radius, color1);
	rainbow.push_none();
	if (radius > 0) {
		draw_line(y0, x0, y0, x0 + radius - 1 - (int)(radius / 3), color3);
		draw_line(y0, x0, y0 - radius + 1 + (int)(radius / 8), x0, color2);
	}
	rainbow.pop();
}

void Grid::draw_smile(Index y0, Index x0, Size radius, Color color1, Color color2) {
	Color bg_color = get_color(y0, x0);
	draw_circle(y0, x0, radius, color1);
	if (radius > 0) {
		draw_circle(y0 - (int)(radius / 9), x0, radius - (int)(radius / 3), color2);
		draw_filled_rect(y0 + (int)(radius / 3), x0 - (int)(radius * 2 / 3) - 1, y0 - (int)(radius / 3), x0 + (int)(radius * 2 / 3) + 1, bg_color, true);
		draw_filled_circle(y0 - (int)(radius / 3), x0 - (int)(radius / 3) + 1, (int)(radius / 2), bg_color);
		draw_filled_circle(y0 - (int)(radius / 3), x0,                         (int)(radius / 2), bg_color);
		draw_filled_circle(y0 - (int)(radius / 3), x0 + (int)(radius / 3) - 1, (int)(radius / 2), bg_color);
		Size eye_delta = sqrt((int)(radius / 2 + 1) * (int)(radius / 2 + 1)) / 2;
		draw_filled_circle(y0 - eye_delta, x0 - eye_delta, (int)(radius / 9), color2);
		draw_filled_circle(y0 - eye_delta, x0 + eye_delta, (int)(radius / 9), color2);
	}
}

void Grid::draw_triangle(Index y1, Index x1, Index y2, Index x2, Index y3, Index x3, Color color) {
	rainbow.push(RAINBOW_CONCENTRIC, min(y1, y2, y3), min(x1, x2, x3), max(y1, y2, y3), max(x1, x2, x3), CONCENTRIC_RECT);
	draw_line(y1, x1, y2, x2, color);
	draw_line(y2, x2, y3, x3, color);
	draw_line(y3, x3, y1, x1, color);
	rainbow.pop();
}

void Grid::draw_filled_triangle(Index y1, Index x1, Index y2, Index x2, Index y3, Index x3, Color color) {
	rainbow.push(RAINBOW_CONCENTRIC, min(y1, y2, y3), min(x1, x2, x3), max(y1, y2, y3), max(x1, x2, x3), CONCENTRIC_RECT);

	int y_top, x_top;
	int y_mid, x_mid;
	int y_bot, x_bot;

	if (y1 < y2 && y1 < y3) {
		y_top = y1; x_top = x1;
		if (y2 < y3) {
			y_mid = y2; x_mid = x2;
			y_bot = y3; x_bot = x3;
		} else {
			y_mid = y3; x_mid = x3;
			y_bot = y2; x_bot = x2;
		}
	} else if (y2 < y1 && y2 < y3) {
		y_top = y2; x_top = x2;
		if (y1 < y3) {
			y_mid = y1; x_mid = x1;
			y_bot = y3; x_bot = x3;
		} else {
			y_mid = y3; x_mid = x3;
			y_bot = y1; x_bot = x1;
		}
	} else {
		y_top = y3; x_top = x3;
		if (y1 < y2) {
			y_mid = y1; x_mid = x1;
			y_bot = y2; x_bot = x2;
		} else {
			y_mid = y2; x_mid = x2;
			y_bot = y1; x_bot = x1;
		}
	}

	vector <shared_ptr <Cell>> line_top_bot_cells = collect(&Grid::draw_line, y_top, x_top, y_bot, x_bot, color);
	vector <shared_ptr <Cell>> line_top_mid_cells = collect(&Grid::draw_line, y_top, x_top, y_mid, x_mid, color);
	vector <shared_ptr <Cell>> line_mid_bot_cells = collect(&Grid::draw_line, y_mid, x_mid, y_bot, x_bot, color);

	vector <shared_ptr <Cell>> beg_cells = line_top_bot_cells;
	vector <shared_ptr <Cell>> end_cells = line_top_mid_cells;

	Index beg_i = 0;
	Index end_i = 0;
	bool is_switched = false;

	for (; beg_i < beg_cells.size() && end_i < end_cells.size(); beg_i++, end_i++) {
		shared_ptr <Cell> beg = beg_cells[beg_i];
		shared_ptr <Cell> end = end_cells[end_i];
		if (beg->y != end->y) {
			bug << "In draw_filled_triangle two ends of horizontal line have different y: " << beg->y << " and " << end->y;
			exit_with_bug();
		}
		draw_line(beg->y, beg->x, end->y, end->x, color);
		while (beg_i + 1 < beg_cells.size() && beg_cells[beg_i + 1]->y == beg->y) {
			beg_i++;
			set_color(beg_cells[beg_i]->y, beg_cells[beg_i]->x, color);
		}
		for (auto i : {1, 2}) {
			if (!is_switched && end_i + 1 == end_cells.size()) {
				is_switched = true;
				end_cells = line_mid_bot_cells;
				end_i = -1;
			}
			while (end_i + 1 < end_cells.size() && end_cells[end_i + 1]->y == beg->y) {
				end_i++;
				set_color(end_cells[end_i]->y, end_cells[end_i]->x, color);
			}
		}
	}

	rainbow.pop();
}

void Grid::draw_char(Index y0, Index x0, char ch, Color fg_color, Color bg_color/* = NO_COLOR*/) {
	const int *bitmap = get_font3x5_char_bitmap(ch);
	for (int yd = -1; yd < 6; yd++) {
		for (int xd = -1; xd < 4; xd++) {
			Index y = y0 + yd;
			Index x = x0 + xd;
			if (!is_coord_visible(y, x))
				continue;
			if (yd < 0 || yd >= 5 || xd < 0 || xd >= 5) {
				if (bg_color != NO_COLOR)
					set_color(y, x, bg_color, true);
				continue;
			}
			bool is_point_set = bitmap[yd] & (1 << xd);
			if (is_point_set)
				set_color(y, x, fg_color);
			if (!is_point_set && bg_color != NO_COLOR)
				set_color(y, x, bg_color, true);
		}
	}
}

void Grid::draw_text(Index y0, Index x0, string str, Color fg_color, Color bg_color/* = NO_COLOR*/, int y_offset/* = 0*/, int x_offset/* = 0*/) {
	assert_coord_visible(y0, x0);

	int len = str.length();

	rainbow.push(RAINBOW_VERTICAL, y0, x0, y0 + (len - 1) * y_offset + 4, x0 + (len - 1) * (4 + x_offset) + 2, CONCENTRIC_RECT);

	for (int c = 0; c < len; c++) {
		draw_char(y0 + c * y_offset, x0 + c * (4 + x_offset), str[c], fg_color, bg_color);
	}

	rainbow.pop();
}

void Grid::draw_text_rainbow(Index y0, Index x0, string str, Color fg_color0/* = Re*/, Color bg_color/* = NO_COLOR*/, int y_offset/* = 0*/, int x_offset/* = 0*/) {
	Color fg_color = fg_color0;
	assert_coord_visible(y0, x0);

	for (int c = 0; c < str.length(); c++) {
		if (fg_color == bg_color || TEXT_RAINBOW_SKIPS_WHITE_ON_NO_BG && fg_color == Wh && bg_color == NO_COLOR)
			fg_color = get_next_color(fg_color);
		draw_char(y0 + c * y_offset, x0 + c * (4 + x_offset), str[c], fg_color, bg_color);
		if (str[c] != ' ' || !TEXT_RAINBOW_SKIPS_COLOR_FOR_SPACE)
			fg_color = get_next_color(fg_color);
	}
}

void Grid::exit_with_bug() {
	show();
	cout << "Bug in code: " << bug.str() << endl;
	exit(1);
}

bool Grid::is_coord_visible(Index y, Index x) {
	return y >= 0 && y <= size_y - 1 && x >= 0 && x <= size_x - 1;
}

bool Grid::is_coord_passable(Index y, Index x) {
	return y >= 0 - passable_threshold && y <= size_y - 1 + passable_threshold && x >= 0 - passable_threshold && x <= size_x - 1 + passable_threshold;
}

void Grid::assert_coord_visible(Index y, Index x) {
	if (!is_coord_visible(y, x)) {
		bug << "invalid coords out of grid (" << y << ", " << x << ")";
		exit_with_bug();
	}
}

void Grid::assert_coord_passable(Index y, Index x) {
	if (!is_coord_passable(y, x)) {
		bug << "invalid coords out of operation (" << y << ", " << x << ")";
		exit_with_bug();
	}
}

bool Grid::is_color_real(Color color) {
	return color >= COLOR_FIRST && color <= COLOR_LAST;
}

void Grid::assert_color_real(Color color) {
	if (!is_color_real(color)) {
		bug << "invalid color (" << color << ")";
		exit_with_bug();
	}
}

Color Grid::get_next_color_nowrap(Color color) {
	int next_color = ((int)color + 1);
	return (Color)next_color;
}

Color Grid::get_next_color(Color color) {
	int next_color = ((int)color + 1) % NUM_COLORS;
	return (Color)next_color;
}

Color Grid::get_prev_color(Color color) {
	int prev_color = ((int)color - 1) % NUM_COLORS;
	return (Color)prev_color;
}

string get_color_term_str(Color color) {
	string str;
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

bool Grid::save(ostream &os) {
	os << "# Save file for grid " << size_x << "x" << size_y << endl;

	for (Index y = 0; y < size_y; y++) {
		for (Index x = 0; x < size_x; x++) {
			os << get_save_char_for_color(get_color(y, x));
		}
		os << endl;
	}

	return true;
}

bool Grid::save(string filename) {
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

bool Grid::load(istream &is) {
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

bool Grid::load(string filename) {
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

Size Grid::get_line_size(Index y1, Index x1, Index y2, Index x2) {
	return ::get_line_size(y1, x1, y2, x2);
}

void Grid::show() {
	for (Index y = 0; y < size_y; y++) {
		for (Index x = 0; x < size_x; x++) {
			cout << get_color_term_str(get_color(y, x));
		}
		cout << endl;
	}
}

void Grid::clear() {
	for (Index y = 0; y < size_y; y++) {
		for (Index x = 0; x < size_x; x++) {
			set_color(y, x, Wh);
		}
	}
}

Grid::Grid(Size size_y0, Size size_x0): size_y(size_y0), size_x(size_x0), is_collecting(false) {
	colors = vector <vector <Color>>(size_y, vector <Color>(size_x));
	passable_threshold = min(size_y - 1, size_x - 1);
	clear();
}

void Grid::start_collecting() {
	collected_cells.clear();
	is_collecting = true;
}

void Grid::stop_collecting() {
	is_collecting = false;
}

template <typename... Args>
vector <shared_ptr <Cell>> Grid::collect(void (Grid::*draw_func)(Args... args), Args... args) {
	start_collecting();
	invoke(draw_func, *this, args...);
	stop_collecting();
	return collected_cells;
}

void Grid::start_rainbow(RainbowType rainbow_type) {
	rainbow.start(rainbow_type);
}

void Grid::stop_rainbow() {
	rainbow.stop();
}

void Grid::Rainbow::exit_with_bug(const string &error) {
	cout << "Bug in Rainbow code: " << error.c_str() << endl;
	exit(1);
}

Grid::Rainbow::Rainbow() : started(false), defined(0) {}

bool Grid::Rainbow::is_started() {
	return started;
}

void Grid::Rainbow::start(RainbowType rainbow_type) {
	if (is_started())
		exit_with_bug("Can't start. Already started");
	if (is_defined())
		exit_with_bug("Already defined on start");
	started = true;
	start_type = rainbow_type;
}

void Grid::Rainbow::stop() {
	if (!is_started())
		exit_with_bug("Can't stop. Not started");
	if (is_defined())
		exit_with_bug("There are more push calls than pop calls on stop");
	started = false;
}

bool Grid::Rainbow::is_defined() {
	return started && defined > 0;
}

void Grid::Rainbow::push(RainbowType best_type, Index y1_, Index x1_, Index y2_, Index x2_, ConcentricType cc_type) {
	if (!is_started())
		return;
	if (defined++)
		return;
	type = start_type;
	if (type == RAINBOW_BEST)
		type = best_type;
	y1 = y1_;
	x1 = x1_;
	y2 = y2_;
	x2 = x2_;
	concentric_type = cc_type;
}

void Grid::Rainbow::push(RainbowType best_type, Index y0_, Index x0_, Size radius, ConcentricType cc_type) {
	push(best_type, y0_ - radius, x0_ - radius, y0_ + radius, x0_ + radius, cc_type);
}

void Grid::Rainbow::push_none() {
	push(RAINBOW_NONE, 0, 0, 0, 0);
	type = RAINBOW_NONE;
}

void Grid::Rainbow::pop() {
	if (!is_started())
		return;
	if (!is_defined())
		exit_with_bug("Can't pop. No corresponding push");
	--defined;
}

Color get_delta_color(Color color, int delta) {
	return (Color)((((int)color + delta) % NUM_COLORS + NUM_COLORS) % NUM_COLORS);
}

Color Grid::Rainbow::get_color(Color color, Index y, Index x) {
	if (!is_started())
		exit_with_bug("Called get_color without being started");
	if (!is_defined())
		exit_with_bug("Called get_color without being defined");

	int delta = 0;

	switch (type) {
	case RAINBOW_CONCENTRIC:
		if (concentric_type == CONCENTRIC_CIRCLE)
			delta = (y2 - y1) / 2 - ::get_line_size(y, x, (y2 + y1) / 2, (x2 + x1) / 2);
		else if (concentric_type == CONCENTRIC_RHOMB)
			delta = (y2 - y1) / 2 - abs(y - (y2 + y1) / 2) - abs(x - (x2 + x1) / 2);
		else {
			Size delta1_y = abs(y - y1);
			Size delta2_y = abs(y - y2);
			Size delta1_x = abs(x - x1);
			Size delta2_x = abs(x - x2);
			delta = min(min(delta1_y, delta2_y), min(delta1_x, delta2_x));
		}
		break;
	case RAINBOW_CENTRICAL1:
		delta = ::get_line_size(y, x, y1, x1);
		break;
	case RAINBOW_CENTRICAL2:
		delta = ::get_line_size(y, x, y1, x2);
		break;
	case RAINBOW_CENTRICAL3:
		delta = ::get_line_size(y, x, y2, x1);
		break;
	case RAINBOW_CENTRICAL4:
		delta = ::get_line_size(y, x, y2, x2);
		break;
	case RAINBOW_DIAGONAL1:
		delta = y - y1 + x - x1;
		break;
	case RAINBOW_DIAGONAL2:
		delta = y - y1 - x + x1;
		break;
	case RAINBOW_HORIZONTAL:
		delta = x - x1;
		break;
	case RAINBOW_VERTICAL:
		delta = y - y1;
		break;
	}

	return get_delta_color(color, delta);
}
