#include "program.h"
#include "testgrid.h"

bool debug = false;

TestGrid grid(36, 50);

void die(string s) {
	cerr << s << endl;
	exit(1);
}

void ok(bool result, string_view test_descr, string_view expected_value) {
	if (result)
		cout << "\e[92m[PASSED]\e[m " << test_descr << endl;
	else
		cout << "\e[91m[FAILED]\e[m " << test_descr << " (expected: " << expected_value << ")" << endl;
}

void eq(int actual, int expected, string value_descr) {
	ok(actual == expected, value_descr + " == " + to_string(actual), to_string(expected));
}

class GridDrawTest {
public:
	string descr;
	string expected_descr;
	vector <vector <Color>> actual_colors;
	GridDrawTest(string_view descr_, string_view expected_descr_ = "") {
		descr = descr_;
		expected_descr = expected_descr_;
		grid.clear();
	};
	void cut() {
		actual_colors = grid.get_colors();
		if (debug) {
			cout << "Actual grid:" << endl;
			grid.show();
		}
		grid.clear();
	}
	~GridDrawTest() {
		auto expected_colors = grid.get_colors();
		if (debug) {
			cout << "Expected grid:" << endl;
			grid.show();
		}
		ok(actual_colors == expected_colors, descr + " compared to expected grid", expected_descr.size() ? expected_descr : "different grid");
	}
};

// Test functions for common functionality

void test_sqrt() {
	eq(sqrt(25), 5, "sqrt(25)");
	eq(sqrt(26), 5, "sqrt(26)");
}

void test_version() {
	ok(stoi(VERSION) >= 1, "version string", "higher than 1");
}

void test_parse_options() {
	int argc = 0;
	map <string, int> preferences = parse_options(argc, nullptr, nullptr);

	eq(preferences["size_x"], 30, "default program size_x");
	eq(preferences["size_y"], 30, "default program size_y");
}

// Test functions for draw_<figure> methods

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

// Test functions for draw_filled_<figure> methods

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

	grid.draw_filled_square(15, 15, 4, Re);

	test.cut();

	grid.draw_square(15, 15, 4, Re);
	grid.draw_square(15, 15, 3, Re);
	grid.draw_square(15, 15, 2, Re);
	grid.draw_square(15, 15, 1, Re);
	grid.draw_square(15, 15, 0, Re);
}

void test_draw_filled_triangle() {
	GridDrawTest test("draw_filled_triangle", "2 filled triangles");

	grid.draw_filled_triangle(19, 3, 7, 15, 19, 27, Re);

	test.cut();

	grid.draw_filled_triangle(7, 15, 19, 15, 19, 3, Re);
	grid.draw_filled_triangle(8, 16, 19, 16, 19, 27, Re);
}

// Test functions for draw_filled_<figure>_2 methods

void test_draw_filled_triangle_2() {
	GridDrawTest test("draw_filled_triangle_2", "2 triangles");

	grid.draw_filled_triangle_2(1, 15, 15, 15, 7, 3, Re, Gr);

	test.cut();

	grid.draw_filled_triangle(1, 15, 15, 15, 7, 3, Re);
	grid.draw_triangle(1, 15, 15, 15, 7, 3, Gr);
}

void test_draw_filled_circle_2() {
	GridDrawTest test("draw_filled_circle_2");

	grid.draw_filled_circle_2(15, 15, 4, Re, Gr);

	test.cut();

	grid.draw_filled_circle(15, 15, 3, Re);
	grid.draw_circle(15, 15, 4, Gr);
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

// Test functions for draw_<figure> methods with rainbow

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
	grid.draw_filled_circle(15, 15, 9, Re);
	grid.stop_rainbow();

	test.cut();

	grid.draw_filled_circle_rainbow(15, 15, 9, Re);
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
	grid.draw_filled_rhomb(15, 15, 9, Re);
	grid.stop_rainbow();

	test.cut();

	grid.draw_filled_rhomb_rainbow(15, 15, 9, Re);
}

void test_draw_filled_square_rainbow() {
	GridDrawTest test("draw_filled_square with rainbow", "own implementation");

	grid.start_rainbow(RAINBOW_BEST);
	grid.draw_filled_square(15, 15, 9, Re);
	grid.stop_rainbow();

	test.cut();

	grid.draw_filled_square_rainbow(15, 15, 9, Re);
}

// main - call all tests

int main(int argc, char **argv) {
	if (argc > 1)
		debug = true;

	test_sqrt();
	test_version();
	test_parse_options();
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
}
