#include "program.h"
#include "testgrid.h"
#include <fcntl.h>
#include <unistd.h>

bool DEBUG = false;
bool MANUAL = false;
bool MANUAL_LAST = false;
bool SIMULATE_GridDrawTest_FAILURE = false;
bool SIMULATE_GridDrawTest_SUCCESS = false;

const char *PASSED_PREFIX = "\e[92m[PASSED]\e[m ";
const char *FAILED_PREFIX = "\e[91m[FAILED]\e[m ";

TestGrid grid(34, 50);

void ok(bool success, string_view test_descr, string_view expected_value) {
	if (success)
		cout << PASSED_PREFIX << test_descr << endl;
	else
		cout << FAILED_PREFIX << test_descr << " (expected: " << expected_value << ")" << endl;
}

void nok(bool success, string_view test_descr, string_view actual_value) {
	if (!success)
		cout << PASSED_PREFIX << test_descr << endl;
	else
		cout << FAILED_PREFIX << test_descr << " (got: " << actual_value << ")" << endl;
}

void eq(int actual, int expected, string value_descr) {
	ok(actual == expected, value_descr + " is " + to_string(actual), to_string(expected));
}

void ne(int actual, int expected, string value_descr) {
	nok(actual == expected, value_descr + " is not " + to_string(expected), to_string(actual));
}

class GridDrawTest {
public:
	string descr;
	string expected_descr;
	string descr_prefix;
	vector <vector <Color>> actual_colors;
	GridDrawTest(string_view descr_, string_view expected_descr_ = "") {
		descr = descr_;
		expected_descr = expected_descr_;
		if (DEBUG) {
			ostringstream prefix;
			prefix << "\e[93;7m[" << descr << "]\e[m ";
			descr_prefix = prefix.str();
		}
		grid.clear();
	};
	void cut() {
		actual_colors = grid.get_colors();
		if (DEBUG) {
			cout << descr_prefix << "Actual grid:" << endl;
			grid.show();
		}
		grid.clear();
	}
	~GridDrawTest() {
		auto expected_colors = grid.get_colors();
		if (DEBUG) {
			cout << descr_prefix << "Expected grid:" << endl;
			grid.show();
		}
		bool success = SIMULATE_GridDrawTest_FAILURE ? false : SIMULATE_GridDrawTest_SUCCESS ? true : actual_colors == expected_colors;
		ok(success, descr + " compared to expected grid", expected_descr.size() ? expected_descr : "different grid");
	}
};

class GridManualUsageTest {
public:
	string descr;
	vector <vector <Color>> cleared_colors;
	string_view last_subtest_descr;
	GridManualUsageTest(string_view descr_) {
		descr = descr_;
		grid.clear();
		cleared_colors = grid.get_colors();
		grid.push_undo();
	};
	void cut(string_view subtest_descr) {
		if (MANUAL)
			prompt(subtest_descr);
		if (MANUAL_LAST)
			last_subtest_descr = subtest_descr;
		grid.undo(true);
		grid.push_undo(true);
	}
	~GridManualUsageTest() {
		if (MANUAL_LAST) {
			bool has_redo = grid.has_redo();
			if (has_redo) grid.redo();
			prompt(last_subtest_descr);
			if (has_redo) grid.undo(true);
		}
		grid.undo(true);
		auto actual_colors = grid.get_colors();
		bool success = actual_colors == cleared_colors;
		ok(success, descr + " after undo", "empty grid");
	}
protected:
	void prompt(string_view subtest_descr) {
		grid.show();
		cout << "After " << subtest_descr << endl << "If OK, press Enter. Or Ctrl-C to abort. ";
		string answer;
		getline(cin, answer);
	}
};

class TemporarilyDisableStderr {
public:
	int stored_stderr_fd;
	TemporarilyDisableStderr() {
		stored_stderr_fd = ::dup(2);
		// redirect stderr to null, but only if not in debug mode
		if (!DEBUG) {
			int null_fd = ::open("/dev/null", O_WRONLY);
			::dup2(null_fd, 2);
			::close(null_fd);
		}
	}
	~TemporarilyDisableStderr() {
		dup2(stored_stderr_fd, 2);
	}
};

// Tests for common functionality

void test_sqrt() {
	eq(sqrt(25), 5, "sqrt(25)");
	eq(sqrt(26), 5, "sqrt(26)");
}

void test_version() {
	ok(stoi(VERSION) >= 1, "version string is sane", "higher equal than 1");
}

void test_parse_options() {
	int argc = 0;
	map <string, int> preferences = parse_options(argc, nullptr, nullptr);

	eq(preferences["size_x"], 30, "default program size_x");
	eq(preferences["size_y"], 30, "default program size_y");
}

void test_get_line_size() {
	Size size = grid.get_line_size(16, 28, 29, 8);
	eq(size, 24, "Distance between (16, 28) and (29, 8)");

	size = grid.get_line_size(10, 15, 20, 15);
	eq(size, 10, "Distance between (10, 15) and (20, 15)");

	size = grid.get_line_size(12, 12, 12, 12);
	eq(size, 0, "Distance between (12, 12) and (12, 12)");
}

// Unit tests for Grid draw methods

void test_set_color() {
	GridDrawTest test("set_color", "changing vector manually");

	grid.set_color(10, 20, Bl);

	test.cut();

	(grid.get_colors())[10][20] = Bl;
}

void test_draw_text() {
	GridDrawTest test("draw_text \"Danik\"", "5 chars");

	grid.draw_text(3, 3, "Danik", Gr);

	test.cut();

	grid.draw_char(3, 3, 'D', Gr);
	grid.draw_char(3, 7, 'a', Gr);
	grid.draw_char(3, 11, 'n', Gr);
	grid.draw_char(3, 15, 'i', Gr);
	grid.draw_char(3, 19, 'k', Gr);
}

void test_draw_square() {
	GridDrawTest test("draw_square", "corresponding rect");

	grid.draw_square(10, 10, 7, Gr);

	test.cut();

	grid.draw_rect(3, 3, 17, 17, Gr);
}

void test_draw_rhomb() {
	GridDrawTest test("draw_rhomb with radius 0", "center point");

	grid.draw_rhomb(15, 15, 0, Bl);

	test.cut();

	grid.set_color(15, 15, Bl);
}

void test_draw_circle() {
	GridDrawTest test("draw_circle with radius 0", "center point");

	grid.draw_circle(15, 15, 0, Gr);

	test.cut();

	grid.set_color(15, 15, Gr);
}

void test_draw_triangle() {
	GridDrawTest test("draw_triangle", "corresponding 3 lines");

	grid.draw_triangle(15, 15, 23, 15, 10, 9, Gr);

	test.cut();

	grid.draw_line(10, 9, 15, 15, Gr);
	grid.draw_line(10, 9, 23, 15, Gr);
	grid.draw_line(23, 15, 15, 15, Gr);
}

// Tests for draw_filled_<figure> methods

void test_draw_filled_circle() {
	GridDrawTest test("draw_filled_circle with radius 2", "2 lines and rect");

	grid.draw_filled_circle(15, 15, 2, Gr);

	test.cut();

	grid.draw_line(14, 13, 16, 13, Gr);
	grid.draw_line(14, 17, 16, 17, Gr);
	grid.draw_filled_rect(13, 14, 17, 16, Gr);
}

void test_draw_filled_rect() {
	GridDrawTest test("draw_filled_rect 10x4", "2 concentric rects");

	grid.draw_filled_rect(4, 8, 13, 11, Re);

	test.cut();

	grid.draw_rect(4, 8, 13, 11, Re);
	grid.draw_rect(5, 9, 12, 10, Re);
}

void test_draw_filled_rhomb() {
	GridDrawTest test("draw_filled_rhomb with radius 4", "5 concentric rhombs");

	grid.draw_filled_rhomb(15, 15, 4, Re);

	test.cut();

	grid.draw_rhomb(15, 15, 4, Re);
	grid.draw_rhomb(15, 15, 3, Re);
	grid.draw_rhomb(15, 15, 2, Re);
	grid.draw_rhomb(15, 15, 1, Re);
	grid.draw_rhomb(15, 15, 0, Re);
}

void test_draw_filled_square() {
	GridDrawTest test("draw_filled_square with radius 4", "5 concentric squares");

	grid.draw_filled_square(15, 15, 4, Or);

	test.cut();

	grid.draw_square(15, 15, 4, Or);
	grid.draw_square(15, 15, 3, Or);
	grid.draw_square(15, 15, 2, Or);
	grid.draw_square(15, 15, 1, Or);
	grid.draw_square(15, 15, 0, Or);
}

void test_draw_filled_triangle() {
	GridDrawTest test("draw_filled_triangle", "2 filled triangles");

	grid.draw_filled_triangle(19, 3, 7, 15, 19, 27, Re);

	test.cut();

	grid.draw_filled_triangle(7, 15, 19, 15, 19, 3, Re);
	grid.draw_filled_triangle(8, 16, 19, 16, 19, 27, Re);
}

// Tests for draw_filled_<figure>_2 methods

void test_draw_filled_triangle_2() {
	GridDrawTest test("draw_filled_triangle_2", "2 triangles");

	grid.draw_filled_triangle_2(1, 15, 15, 15, 7, 3, Re, Gr);

	test.cut();

	grid.draw_filled_triangle(1, 15, 15, 15, 7, 3, Re);
	grid.draw_triangle(1, 15, 15, 15, 7, 3, Gr);
}

void test_draw_filled_circle_2() {
	GridDrawTest test("draw_filled_circle_2", "2 circles");

	grid.draw_filled_circle_2(15, 15, 9, Re, Gr);

	test.cut();

	grid.draw_filled_circle(15, 15, 9, Re);
	grid.draw_circle(15, 15, 9, Gr);
}

void test_draw_filled_rect_2() {
	GridDrawTest test("draw_filled_rect_2", "2 rects");

	grid.draw_filled_rect_2(15, 15, 30, 30, Re, Gr);

	test.cut();

	grid.draw_filled_rect(16, 16, 29, 29, Re);
	grid.draw_rect(15, 15, 30, 30, Gr);
}

void test_draw_filled_rhomb_2() {
	GridDrawTest test("draw_filled_rhomb_2", "2 rhombs");

	grid.draw_filled_rhomb_2(15, 15, 7, Re, Gr);

	test.cut();

	grid.draw_filled_rhomb(15, 15, 6, Re);
	grid.draw_rhomb(15, 15, 7, Gr);
}

void test_draw_filled_square_2() {
	GridDrawTest test("draw_filled_square_2", "2 squares");

	grid.draw_filled_square_2(15, 15, 7, Re, Gr);

	test.cut();

	grid.draw_filled_square(15, 15, 6, Re);
	grid.draw_square(15, 15, 7, Gr);
}

// Tests for draw_<figure> methods with rainbow

void test_draw_text_rainbow() {
	GridDrawTest test("draw_text with rainbow", "own implementation");

	grid.start_rainbow(RAINBOW_BEST);
	grid.draw_text(1, 1, "Welcome to    ", Re);
	grid.draw_text(grid.get_size_y() - 6, 1, "Mosaic!    ", Bl, Gr, 0, 1);
	grid.stop_rainbow();

	test.cut();

	grid.draw_text_rainbow(1, 1, "Welcome to    ", Re);
	grid.draw_text_rainbow(grid.get_size_y() - 6, 1, "Mosaic!    ", Bl, Gr, 0, 1);
}

void test_draw_filled_circle_rainbow() {
	GridDrawTest test("draw_filled_circle with rainbow", "own implementation");

	grid.start_rainbow(RAINBOW_BEST);
	grid.draw_filled_circle(15, 15, 9, Gr);
	grid.stop_rainbow();

	test.cut();

	grid.draw_filled_circle_rainbow(15, 15, 9, Gr);
}

void test_draw_filled_rect_rainbow() {
	GridDrawTest test("draw_filled_rect with rainbow", "own implementation");

	grid.start_rainbow(RAINBOW_BEST);
	grid.draw_filled_rect(3, 3, 20, 20, Re);
	grid.stop_rainbow();

	test.cut();

	grid.draw_filled_rect_rainbow(3, 3, 20, 20, Re);
}

void test_draw_filled_rhomb_rainbow() {
	GridDrawTest test("draw_filled_rhomb with rainbow", "own implementation");

	grid.start_rainbow(RAINBOW_BEST);
	grid.draw_filled_rhomb(15, 15, 9, Gr);
	grid.stop_rainbow();

	test.cut();

	grid.draw_filled_rhomb_rainbow(15, 15, 9, Gr);
}

void test_draw_filled_square_rainbow() {
	GridDrawTest test("draw_filled_square with rainbow", "own implementation");

	grid.start_rainbow(RAINBOW_BEST);
	grid.draw_filled_square(15, 15, 9, Re);
	grid.stop_rainbow();

	test.cut();

	grid.draw_filled_square_rainbow(15, 15, 9, Re);
}

// Unit tests of non-draw Grid methods

void test_grid_save_load() {
	GridDrawTest test("save + set_color + load + save + load", "empty grid");

	grid.save("text-example.sav");
	grid.set_color(10, 10, Gr);
	grid.load("text-example.sav");
	grid.save("text-example.sav");
	grid.load("text-example.sav");

	test.cut();

	unlink("text-example.sav");
}

void test_grid_save_load_failed() {
	TemporarilyDisableStderr disabler;
	nok(grid.save("/no/dir/unexisting"), "save(unexisting-file) fails", "success");
	nok(grid.load("non-existing.sav"), "load(unexisting-file) fails", "success");
	if (access("tests", R_OK) == 0)
		nok(grid.load("tests"), "load(existing-non-sav-file) fails", "success");
}

void test_grid_move() {
	GridDrawTest test("move rect", "other rect");

	grid.draw_filled_rect_2(5, 5, 10, 10, Or, Gr);
	grid.move(5, 5, 10, 10, -2, +2);

	test.cut();

	grid.draw_filled_rect_2(3, 7, 8, 12, Or, Gr);
}

// Manual tests for Grid methods by prompting user (or automatic undo in non manual mode)

void test_grid_usage_manually() {
	GridManualUsageTest test("Comprehensive Grid API");

	grid.set_color(20, 20, Bl);
	test.cut("set_color(20, 20, Bl)");

	test.cut("do nothing");

	grid.draw_line(17, 28, 22, 6, Bl);
	grid.draw_line(16, 29, 18, 4, Re);
	test.cut("draw_line(17, 28, 22, 6, Bl) + draw_line(16, 29, 18, 4, Re)");

	grid.draw_rhomb(4, 3, 3, Re);
	grid.draw_rhomb(15, 18, 10, Ye);
	test.cut("draw_rhomb(4, 3, 3, Re) + draw_rhomb(15, 18, 10, Ye)");

	grid.draw_filled_rhomb(4, 3, 3, Re);
	grid.draw_filled_rhomb(15, 18, 10, Ye);
	test.cut("draw_filled_rhomb(4, 3, 3, Re) + draw_filled_rhomb(15, 18, 10, Ye)");

	grid.draw_filled_rhomb_2(4, 3, 3, Re, Bl);
	grid.draw_filled_rhomb_2(15, 18, 10, Ye, Gr);
	test.cut("draw_filled_rhomb_2(4, 3, 3, Re, Bl) + draw_filled_rhomb_2(15, 18, 10, Ye, Gr)");

	grid.draw_square(12, 12, 10, Gr);
	test.cut("draw_square(12, 12, 10, Gr)");

	grid.draw_filled_square(12, 12, 10, Gr);
	grid.draw_filled_square_2(12, 12, 10, Gr, Bl);
	test.cut("draw_filled_square(12, 12, 10, Gr) + grid.draw_filled_square_2(12, 12, 10, Gr, Bl)");

	grid.draw_circle(4, 3, 3, Re);
	grid.draw_circle(15, 18, 10, Ye);
	test.cut("draw_circle(4, 3, 3, Re) + draw_circle(15, 18, 10, Ye)");

	grid.draw_filled_circle(4, 3, 3, Re);
	grid.draw_filled_circle(15, 18, 10, Ye);
	test.cut("draw_filled_circle(4, 3, 3, Re) + draw_filled_circle(15, 18, 10, Ye)");

	grid.draw_filled_circle_2(4, 3, 3, Re, Or);
	grid.draw_filled_circle_2(15, 18, 10, Ye, Gr);
	test.cut("draw_filled_circle_2(4, 3, 3, Re, Or) + draw_filled_circle_2(15, 18, 10, Ye, Gr)");

	grid.draw_rect(25, 6, 2, 20, Re);
	grid.draw_rect(16, 28, 19, 8, Ye, true);
	test.cut("draw_rect(25, 6, 2, 20, Re) + draw_rect(16, 28, 19, 8, Ye, true)");

	grid.draw_filled_rect(16, 28, 19, 8, Ye, false);
	grid.draw_filled_rect(24, 4, 1, 1, Gr, true);
	test.cut("draw_filled_rect(16, 28, 19, 8, Ye, false) + draw_filled_rect(24, 4, 1, 1, Gr, true)");

	grid.draw_filled_rect_2(25, 6, 2, 20, Bl, Re);
	grid.draw_filled_rect_2(25, 22, 2, 28, Bl, Re, true);
	test.cut("draw_filled_rect_2(25, 6, 2, 20, Bl, Re) + draw_filled_rect_2(25, 22, 2, 28, Bl, Re, true)");

	grid.draw_circle_crest(10, 17, 3, Re, Gr, 2);
	grid.draw_circle_crest(20, 9, 9, Gr, Or, 3);
	test.cut("draw_circle_crest(10, 17, 3, Re, Gr, 2) + draw_circle_crest(20, 9, 9, Gr, Or, 5)");

	grid.draw_circle_crest(4, 4, 4, Bl, Bl, 2);
	grid.draw_circle_crest(23, 24, 5, Or, Bl, 1);
	test.cut("draw_circle_crest(4, 4, 4, Bl, Bl, 2) + draw_circle_crest(23, 24, 5, Or, Bl, 1)");

	grid.draw_clock(4, 4, 3, Re, Or, Gr);
	grid.draw_clock(15, 15, 9, Gr, Re, Or);
	test.cut("draw_clock(4, 4, 3, Re, Or, Gr) + draw_clock(15, 15, 9, Gr, Re, Or)");

	grid.draw_smile(15, 15, 10, Re, Ye);
	test.cut("draw_smile(15, 15, 10, Re, Ye)");

	grid.draw_triangle(0, 0, 1, 29, 29, 10, Gr);
	test.cut("draw_triangle(0, 0, 1, 29, 29, 10, Gr)");

	grid.draw_char(10, 10, 'D', Gr, Ye);
	test.cut("draw_char(10, 10, 'D', Gr, Ye)");

	grid.draw_text(14, 10, "Danik", Bl, Gr);
	grid.draw_text(9, 1, "Irisha", Re, NO_COLOR, -1, 1);
	grid.draw_text(20, 1, "Lion", Gr, Ye, 1, 0);
	test.cut("draw_text(14, 10, \"Danik\", Bl, Gr) + draw_text(9, 1, \"Irisha\", Re, NO_COLOR, -1, 1) + draw_text(20, 1, \"Lion\", Gr, Ye, 1, 0)");

	grid.draw_text(2, 14, "I", Re);
	grid.draw_text(7, 10, "love", Re);
	grid.draw_text(12, 14, "you", Re, NO_COLOR, 3);
	test.cut("draw_text(2, 14, \"I\", Re) + draw_text(7, 10, \"love\", Re) + draw_text(12, 14, \"you\", Re, NO_COLOR, 3)");

	grid.start_rainbow(RAINBOW_BEST);
	grid.draw_filled_rect(0, 0, 29, 29, Gr);
	grid.stop_rainbow();
	test.cut("start_rainbow(RAINBOW_BEST) + draw_filled_rect(0, 0, 29, 29, Gr) + stop_rainbow()");

	grid.draw_smile(15, 15, 10, Re, Ye);
	grid.push_undo();
	grid.draw_clock(4, 4, 3, Re, Or, Gr);
	grid.undo();
	test.cut("draw_smile(15, 15, 10, Re, Ye) + push_undo() + draw_clock + undo()");

	grid.draw_circle(5, 5, 5, Bl);
	grid.move(0, 0, 10, 10, 1, 2);
	test.cut("move circle from left-top to y+1, x+2");
}

// main - call all tests

int main(int argc, char **argv) {
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'd')
		DEBUG = true;
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'm')
		MANUAL = true;
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'M')
		MANUAL_LAST = true;
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'f')
		SIMULATE_GridDrawTest_FAILURE = true;
	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 's')
		SIMULATE_GridDrawTest_SUCCESS = true;

	test_sqrt();
	test_version();
	test_parse_options();
	test_get_line_size();
	test_set_color();
	test_draw_text();
	test_draw_square();
	test_draw_rhomb();
	test_draw_circle();
	test_draw_triangle();
	test_draw_filled_circle();
	test_draw_filled_rect();
	test_draw_filled_rhomb();
	test_draw_filled_square();
	test_draw_filled_triangle();
	test_draw_filled_circle_2();
	test_draw_filled_rect_2();
	test_draw_filled_rhomb_2();
	test_draw_filled_square_2();
	test_draw_filled_triangle_2();
	test_draw_text_rainbow();
	test_draw_filled_circle_rainbow();
	test_draw_filled_rect_rainbow();
	test_draw_filled_rhomb_rainbow();
	test_draw_filled_square_rainbow();
	test_grid_save_load();
	test_grid_save_load_failed();
	test_grid_move();
	test_grid_usage_manually();
}
