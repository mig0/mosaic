#include "graphics.h"
#include "program.h"

enum CircleType {
	CIRCLE_TYPE_OUTLINE,
	CIRCLE_TYPE_FILLED,
	CIRCLE_TYPE_FILLED_2,
	CIRCLE_TYPE_CLOCK,
	CIRCLE_TYPE_CREST,
	CIRCLE_TYPE_CREST_DIAG,
	CIRCLE_TYPE_CREST_BOTH,
	CIRCLE_TYPE_RHOMB_OUTLINE,
	CIRCLE_TYPE_RHOMB_FILLED,
	CIRCLE_TYPE_RHOMB_FILLED_2,
	CIRCLE_TYPE_SQUARE_OUTLINE,
	CIRCLE_TYPE_SQUARE_FILLED,
	CIRCLE_TYPE_SQUARE_FILLED_2,
	CIRCLE_TYPE_TRIANGLE,
	CIRCLE_TYPE_TRIANGLE_FILLED,
	CIRCLE_TYPE_TRIANGLE_FILLED_2,
};

vector<Glib::ustring> center_type_strings = {
	"Circle Outline",
	"Circle Filled",
	"Circle Filled 2",
	"Clock",
	"Crest",
	"Crest diag",
	"Crest both",
	"Rhomb Outline ",
	"Rhomb Filled",
	"Rhomb Filled 2",
	"Square Outline",
	"Square Filled",
	"Square Filled 2",
	"Triangle Outline",
	"Triangle Filled",
	"Triangle Filled 2",
};

enum RectType {
	RECT_TYPE_OUTLINE,
	RECT_TYPE_FILLED,
	RECT_TYPE_FILLED_2,
	RECT_TYPE_LINE,
	RECT_TYPE_TRIANGLE,
	RECT_TYPE_TRIANGLE_FILLED,
	RECT_TYPE_TRIANGLE_FILLED_2,
};

vector<Glib::ustring> rect_type_strings = {
	"Rect Outline",
	"Rect Filled",
	"Rect Filled 2",
	"Line",
	"Triangle Outline",
	"Triangle Filled",
	"Triangle Filled 2",
};

enum TriangleType {
	TRIANGLE_TYPE_OUTLINE,
	TRIANGLE_TYPE_FILLED,
	TRIANGLE_TYPE_FILLED_2,
};

vector<Glib::ustring> triangle_type_strings = {
	"Triangle Outline",
	"Triangle Filled",
	"Triangle Filled 2",
};

vector<Glib::ustring> rainbow_type_strings = {
	"No rainbow",
	"Rainbow best",
	"Concentric",
	"Individual",
	"Centrical 1",
	"Centrical 2",
	"Centrical 3",
	"Centrical 4",
	"Diagonal 1",
	"Diagonal 2",
	"Vertical",
	"Horizontal",
};

vector<Glib::ustring> move_type_strings = {
	"Move",
	"Copy",
	"Swap",
	"Step",
};

MosaicWindow::MosaicWindow(Grid &grid0) : grid(grid0) {
	ostringstream str_stream;
	Size size_y = grid.get_size_y();
	Size size_x = grid.get_size_x();

	set_title("Mosaic application");
	set_default_size(200, 200);

	css_provider = Gtk::CssProvider::create();
	css_provider->load_from_data(
#include "mosaic-css.h"
	);

	set_child(main_box);
	main_box.set_orientation(Gtk::Orientation::HORIZONTAL);

	main_box.append(main_frame);
	main_frame.set_margin(10);

	str_stream << "Mosaic " << size_x << "x" << size_y;
	main_frame.set_label(str_stream.str());
	main_frame.set_label_align(Gtk::Align::CENTER);

	main_frame.set_child(main_grid);

	main_box.append(control_frame);
	control_frame.set_label("Operations");
	control_frame.set_label_align(Gtk::Align::CENTER);
	control_frame.set_margin(10);
	control_frame.set_margin_start(0);
	control_frame.set_expand(true);

	control_frame.set_child(control_box);
	control_box.set_orientation(Gtk::Orientation::VERTICAL);
	control_box.set_valign(Gtk::Align::FILL);
	control_box.set_margin(10);

	control_box.append(mini_button_box);
	mini_button_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	mini_button_box.set_halign(Gtk::Align::CENTER);
	mini_button_box.set_margin(2);

	mini_button_box.append(undo_button);
	undo_button.set_icon_name("edit-undo");
	undo_button.set_tooltip_text("Undo changes");
	undo_button.set_margin(4);
	undo_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::undo));

	mini_button_box.append(redo_button);
	redo_button.set_icon_name("edit-redo");
	redo_button.set_tooltip_text("Redo changes");
	redo_button.set_margin(4);
	redo_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::redo));

	Gtk::Separator mini_button_separator(Gtk::Orientation::HORIZONTAL);
	mini_button_box.append(mini_button_separator);
	mini_button_separator.set_margin(4);

	mini_button_box.append(move_l_button);
	move_l_button.set_icon_name("go-previous");
	move_l_button.set_tooltip_text("Move area 1 point left");
	move_l_button.set_margin(4);
	move_l_button.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MosaicWindow::move), 0, -1));

	mini_button_box.append(move_u_button);
	move_u_button.set_icon_name("go-up");
	move_u_button.set_tooltip_text("Move area 1 point up");
	move_u_button.set_margin(4);
	move_u_button.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MosaicWindow::move), -1, 0));

	mini_button_box.append(move_d_button);
	move_d_button.set_icon_name("go-down");
	move_d_button.set_tooltip_text("Move area 1 point down");
	move_d_button.set_margin(4);
	move_d_button.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MosaicWindow::move), +1, 0));

	mini_button_box.append(move_r_button);
	move_r_button.set_icon_name("go-next");
	move_r_button.set_tooltip_text("Move area 1 point right");
	move_r_button.set_margin(4);
	move_r_button.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MosaicWindow::move), 0, +1));

	control_box.append(action_box);
	action_box.set_orientation(Gtk::Orientation::VERTICAL);
	action_box.set_valign(Gtk::Align::START);
	action_box.set_margin(2);
	action_box.set_expand(true);

	action_box.append(active_color_box);
	active_color_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	active_color_box.set_halign(Gtk::Align::FILL);
	active_color_box.set_margin(5);
	active_color_box.set_spacing(8);

	active_color_box.append(active_color_label);
	active_color_label.set_label("Active Color");
	active_color_label.set_halign(Gtk::Align::START);
	active_color_label.set_expand(true);

	rainbow_dropdown = Gtk::DropDown(rainbow_type_strings);
	rainbow_dropdown.set_selected(0);
	rainbow_dropdown.set_size_request(150);
	active_color_box.append(rainbow_dropdown);

	active_color_box.append(active_color_menu_button);
	active_color_button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	set_button_color(active_color_button, active_color);
	active_color_menu_button.set_child(active_color_button);
	active_color_menu_button.set_always_show_arrow(true);

	active_color_menu_button.set_popover(active_color_popover);
	active_color_popover.set_child(color_selection_box);
	color_selection_box.set_orientation(Gtk::Orientation::VERTICAL);
	color_selection_box.set_halign(Gtk::Align::CENTER);
	color_selection_box.set_margin(0);
	for (Color color = COLOR_FIRST; color <= COLOR_LAST; color = grid.get_next_color_nowrap(color)) {
		Gtk::Button &color_selection_button = *(new Gtk::Button);
		color_selection_button.set_margin(3);
		color_selection_button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
		color_selection_button.signal_clicked().connect([this, color]() { set_active_color(color); });
		set_button_color(color_selection_button, color);
		color_selection_box.append(color_selection_button);
	}

	active_color_box.append(active_color2_menu_button);
	active_color2_button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	set_button_color(active_color2_button, active_color2);
	active_color2_menu_button.set_child(active_color2_button);
	active_color2_menu_button.set_always_show_arrow(true);

	active_color2_menu_button.set_popover(active_color2_popover);
	active_color2_popover.set_child(color2_selection_box);
	color2_selection_box.set_orientation(Gtk::Orientation::VERTICAL);
	color2_selection_box.set_halign(Gtk::Align::CENTER);
	color2_selection_box.set_margin(0);
	for (Color color = COLOR_FIRST; color <= NO_COLOR; color = grid.get_next_color_nowrap(color)) {
		Gtk::Button &color_selection_button = *(new Gtk::Button);
		color_selection_button.set_margin(3);
		color_selection_button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
		color_selection_button.signal_clicked().connect([this, color]() { set_active_color2(color); });
		set_button_color(color_selection_button, color);
		color2_selection_box.append(color_selection_button);
	}

	action_box.append(active_cell_box);
	active_cell_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	active_cell_box.set_halign(Gtk::Align::FILL);
	active_cell_box.set_margin(5);
	active_cell_box.set_spacing(8);

	active_cell_box.append(active_cell_label);
	active_cell_label.set_label("Active Cell");
	active_cell_label.set_halign(Gtk::Align::START);
	active_cell_label.set_expand(true);

	active_cell_box.append(active_cell_button);
	active_cell_button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	set_active_cell(NO_INDEX, NO_INDEX);

	active_cell_box.append(active_cell2_button);
	active_cell2_button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	set_active_cell2(NO_INDEX, NO_INDEX);

	active_cell_box.append(active_cell3_button);
	active_cell3_button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	set_active_cell3(NO_INDEX, NO_INDEX);

	action_box.append(draw_text_box);
	draw_text_box.set_sensitive(false);
	draw_text_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	draw_text_box.set_halign(Gtk::Align::FILL);
	draw_text_box.set_margin(5);
	draw_text_box.set_spacing(8);

	draw_text_box.append(draw_text_label);
	draw_text_label.set_label("Text");
	draw_text_label.set_halign(Gtk::Align::START);
	draw_text_label.set_expand(true);

	draw_text_box.append(draw_text_entry);
	draw_text_entry.set_max_length(size_x / 4 + 1);

	draw_text_box.append(draw_text_button);
	draw_text_button.set_label("Draw");
	draw_text_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::draw_text));

	action_box.append(draw_circle_box);
	draw_circle_box.set_sensitive(false);
	draw_circle_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	draw_circle_box.set_halign(Gtk::Align::FILL);
	draw_circle_box.set_margin(5);
	draw_circle_box.set_spacing(8);

	draw_circle_box.append(draw_circle_label);
	draw_circle_label.set_label("Circle/More");
	draw_circle_label.set_halign(Gtk::Align::START);
	draw_circle_label.set_expand(true);

	auto draw_circle_adj = Gtk::Adjustment::create(size_y / 4, 0, min(size_y - 1, size_x - 1));
	draw_circle_radius_spin.set_adjustment(draw_circle_adj);
	draw_circle_box.append(draw_circle_radius_spin);

	draw_circle_type_dropdown = Gtk::DropDown(center_type_strings);
	draw_circle_type_dropdown.set_selected(0);
	draw_circle_type_dropdown.set_size_request(180);
	draw_circle_box.append(draw_circle_type_dropdown);

	draw_circle_box.append(draw_circle_button);
	draw_circle_button.set_label("Draw");
	draw_circle_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::draw_circle));

	action_box.append(draw_rect_box);
	draw_rect_box.set_sensitive(false);
	draw_rect_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	draw_rect_box.set_halign(Gtk::Align::FILL);
	draw_rect_box.set_margin(5);
	draw_rect_box.set_spacing(8);

	draw_rect_box.append(draw_rect_label);
	draw_rect_label.set_label("Rectangle/Line");
	draw_rect_label.set_halign(Gtk::Align::START);
	draw_rect_label.set_expand(true);

	draw_rect_type_dropdown = Gtk::DropDown(rect_type_strings);
	draw_rect_type_dropdown.set_selected(0);
	draw_rect_type_dropdown.set_size_request(180);
	draw_rect_box.append(draw_rect_type_dropdown);

	draw_rect_box.append(draw_rect_button);
	draw_rect_button.set_label("Draw");
	draw_rect_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::draw_rect));

	action_box.append(draw_triangle_box);
	draw_triangle_box.set_sensitive(false);
	draw_triangle_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	draw_triangle_box.set_halign(Gtk::Align::FILL);
	draw_triangle_box.set_margin(5);
	draw_triangle_box.set_spacing(8);

	draw_triangle_box.append(draw_triangle_label);
	draw_triangle_label.set_label("Triangle");
	draw_triangle_label.set_halign(Gtk::Align::START);
	draw_triangle_label.set_expand(true);

	draw_triangle_type_dropdown = Gtk::DropDown(triangle_type_strings);
	draw_triangle_type_dropdown.set_selected(0);
	draw_triangle_type_dropdown.set_size_request(180);
	draw_triangle_box.append(draw_triangle_type_dropdown);

	draw_triangle_box.append(draw_triangle_button);
	draw_triangle_button.set_label("Draw");
	draw_triangle_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::draw_triangle));

	action_box.append(move_area_box);
	move_area_box.set_sensitive(false);
	move_area_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	move_area_box.set_halign(Gtk::Align::FILL);
	move_area_box.set_margin(5);
	move_area_box.set_spacing(8);

	move_type_dropdown = Gtk::DropDown(move_type_strings);
	move_type_dropdown.set_selected(0);
	move_type_dropdown.set_size_request(80);
	move_area_box.append(move_type_dropdown);

	move_area_box.append(move_area_label);
	move_area_label.set_label("area to ±x, ±y");
	move_area_label.set_halign(Gtk::Align::START);
	move_area_label.set_expand(true);

	auto move_x_offset_adj = Gtk::Adjustment::create(0, 1 - size_x, size_x - 1);
	move_x_offset_spin.set_adjustment(move_x_offset_adj);
	move_area_box.append(move_x_offset_spin);

	auto move_y_offset_adj = Gtk::Adjustment::create(0, 1 - size_y, size_y - 1);
	move_y_offset_spin.set_adjustment(move_y_offset_adj);
	move_area_box.append(move_y_offset_spin);

	move_area_box.append(move_area_button);
	move_area_button.set_label("Draw");
	move_area_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::move_area));

	control_box.append(button_box);
	button_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	button_box.set_halign(Gtk::Align::CENTER);
	button_box.set_margin(2);

	button_box.append(clear_button);
	clear_button.set_label("Clear");
	clear_button.set_margin(4);
	clear_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::clear));

	button_box.append(save_button);
	save_button.set_label("Save");
	save_button.set_margin(4);
	save_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::save));

	button_box.append(load_button);
	load_button.set_label("Load");
	load_button.set_margin(4);
	load_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::load));

	button_box.append(about_button);
	about_button.set_label("About");
	about_button.set_margin(4);
	about_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::show_about_dialog));

	button_box.append(quit_button);
	quit_button.set_label("Quit");
	quit_button.set_margin(4);
	quit_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::quit));

	main_grid.set_margin(10);
	main_grid.set_expand(true);
	main_grid.set_row_homogeneous(true);
	main_grid.set_column_homogeneous(true);

	for (Index y = 0; y < size_y; y++) {
		for (Index x = 0; x < size_x; x++) {
			Gtk::Button &button = *(new Gtk::Button);
			main_grid.attach(button, x, y);
			button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
			set_button_coord_tooltip(button, y, x);
			button.signal_clicked().connect([this, y, x]() {
				if (has_active_cell()) {
					set_active_cell2(y, x);
				} else {
					set_grid_cell_active_color(y, x);
				}
			});
			auto button2_controller = Gtk::GestureClick::create();
			button2_controller->set_button(GDK_BUTTON_MIDDLE);
			button2_controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
			button2_controller->signal_pressed().connect(sigc::bind(sigc::mem_fun(*this, &MosaicWindow::on_grid_button2_press), y, x));
			button.add_controller(button2_controller);
			auto button3_controller = Gtk::GestureClick::create();
			button3_controller->set_button(GDK_BUTTON_SECONDARY);
			button3_controller->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
			button3_controller->signal_pressed().connect(sigc::bind(sigc::mem_fun(*this, &MosaicWindow::on_grid_button3_press), y, x));
			button.add_controller(button3_controller);
		}
	}
	reload_grid();
	grid.signal_on_set_color.connect(sigc::mem_fun(*this, &MosaicWindow::set_grid_cell_color_callback));
	grid.signal_on_change_undo_redo.connect(sigc::mem_fun(*this, &MosaicWindow::set_undo_redo_sensitive_callback));
	set_undo_redo_sensitive_callback(grid.has_undo(), grid.has_redo());
	set_move_buttons_sensitive();

	auto controller = Gtk::EventControllerKey::create();
	controller->signal_key_pressed().connect(
		sigc::mem_fun(*this, &MosaicWindow::on_window_key_pressed), false);
	add_controller(controller);
}

void MosaicWindow::set_undo_redo_sensitive_callback(bool has_undo, bool has_redo) {
	undo_button.set_sensitive(has_undo);
	redo_button.set_sensitive(has_redo);
}

void MosaicWindow::set_move_buttons_sensitive() {
	move_l_button.set_sensitive(has_active_cell2() && min(active_cell_x, active_cell2_x) > 0);
	move_u_button.set_sensitive(has_active_cell2() && min(active_cell_y, active_cell2_y) > 0);
	move_d_button.set_sensitive(has_active_cell2() && max(active_cell_y, active_cell2_y) < grid.get_size_y() - 1);
	move_r_button.set_sensitive(has_active_cell2() && max(active_cell_x, active_cell2_x) < grid.get_size_x() - 1);
}

void MosaicWindow::set_button_color(Gtk::Widget &button, Color color) {
	vector<Glib::ustring> css_classes = { color == NO_COLOR ? "none" : grid.get_color_name(color) };
	int cell_y, cell_x, width, height;
	if (button.get_parent() == &main_grid)
		main_grid.query_child(button, cell_x, cell_y, width, height);
	else
		cell_y = cell_x = NO_INDEX;
	if (is_active_cell(cell_y, cell_x) || is_active_cell2(cell_y, cell_x) || is_active_cell3(cell_y, cell_x) ||
		&button == &active_cell_button || &button == &active_cell2_button || &button == &active_cell3_button)
		css_classes.push_back("active-cell");
	button.set_css_classes(css_classes);
}

void MosaicWindow::set_button_coord_tooltip(Gtk::Widget &button, Index y, Index x) {
	ostringstream str_stream;
	if (grid.is_coord_visible(y, x))
		str_stream << "(" << (x + 1) << ", " << (y + 1) << ")";
	button.set_tooltip_text(str_stream.str());
}

void MosaicWindow::reload_grid_cell(Index y, Index x) {
	set_button_color(*main_grid.get_child_at(x, y), grid.get_color(y, x));
}

void MosaicWindow::reload_grid() {
	for (Index y = 0; y < grid.get_size_y(); y++) {
		for (Index x = 0; x < grid.get_size_x(); x++) {
			reload_grid_cell(y, x);
		}
	}
}

void MosaicWindow::set_grid_cell_color(Index y, Index x, Color color) {
	grid.push_undo();
	grid.set_color(y, x, color);
}

void MosaicWindow::set_grid_cell_color_callback(Index y, Index x, Color color) {
	reload_grid_cell(y, x);
}

void MosaicWindow::set_grid_cell_active_color(Index y, Index x) {
	set_grid_cell_color(y, x, active_color);
}

void MosaicWindow::set_grid_cell_active_color2(Index y, Index x) {
	Color color = active_color2 != NO_COLOR ? active_color2 : grid.get_bg_color();
	set_grid_cell_color(y, x, color);
}

void MosaicWindow::set_active_color(Color color) {
	active_color = color;
	set_button_color(active_color_button, active_color);
	active_color_popover.popdown();
}

void MosaicWindow::set_active_color2(Color color) {
	active_color2 = color;
	set_button_color(active_color2_button, active_color2);
	active_color2_popover.popdown();
}

void MosaicWindow::set_active_cell(int y, int x) {
	bool old_has_active_cell = has_active_cell();
	int old_active_cell_y = active_cell_y;
	int old_active_cell_x = active_cell_x;

	active_cell_y = y;
	active_cell_x = x;
	set_button_color(active_cell_button, has_active_cell() ? grid.get_color(y, x) : NO_COLOR);

	if (has_active_cell())
		set_button_coord_tooltip(active_cell_button, y, x);
	else
		active_cell_button.set_tooltip_text("Press Right mouse button to set active cell");

	if (old_has_active_cell)
		reload_grid_cell(old_active_cell_y, old_active_cell_x);
	if (has_active_cell())
		reload_grid_cell(active_cell_y, active_cell_x);

	set_active_cell2(NO_INDEX, NO_INDEX);

	draw_text_box.set_sensitive(has_active_cell());
	draw_circle_box.set_sensitive(has_active_cell());
}

bool MosaicWindow::has_active_cell() {
	return active_cell_y >= 0 && active_cell_x >= 0;
}

bool MosaicWindow::is_active_cell(int y, int x) {
	return has_active_cell() && y == active_cell_y && x == active_cell_x;
}

void MosaicWindow::set_active_cell2(int y, int x) {
	bool old_has_active_cell2 = has_active_cell2();
	int old_active_cell2_y = active_cell2_y;
	int old_active_cell2_x = active_cell2_x;

	active_cell2_y = y;
	active_cell2_x = x;
	set_button_color(active_cell2_button, has_active_cell2() ? grid.get_color(y, x) : NO_COLOR);
	if (has_active_cell2())
		set_button_coord_tooltip(active_cell2_button, y, x);
	else if (has_active_cell())
		active_cell2_button.set_tooltip_text("Press Left mouse button to set active cell #2");
	else
		active_cell2_button.set_tooltip_text("Set active cell #1 by pressing Right mouse button first");

	if (old_has_active_cell2)
		reload_grid_cell(old_active_cell2_y, old_active_cell2_x);
	if (has_active_cell2())
		reload_grid_cell(active_cell2_y, active_cell2_x);

	set_active_cell3(NO_INDEX, NO_INDEX);

	draw_rect_box.set_sensitive(has_active_cell2());
	move_area_box.set_sensitive(has_active_cell2());
	set_move_buttons_sensitive();
}

bool MosaicWindow::has_active_cell2() {
	return active_cell2_y >= 0 && active_cell2_x >= 0;
}

bool MosaicWindow::is_active_cell2(int y, int x) {
	return has_active_cell2() && y == active_cell2_y && x == active_cell2_x;
}

void MosaicWindow::set_active_cell3(int y, int x) {
	bool old_has_active_cell3 = has_active_cell3();
	int old_active_cell3_y = active_cell3_y;
	int old_active_cell3_x = active_cell3_x;

	active_cell3_y = y;
	active_cell3_x = x;
	set_button_color(active_cell3_button, has_active_cell3() ? grid.get_color(y, x) : NO_COLOR);
	if (has_active_cell3())
		set_button_coord_tooltip(active_cell3_button, y, x);
	else if (has_active_cell2())
		active_cell3_button.set_tooltip_text("Press Middle mouse button to set active cell #3");
	else if (has_active_cell())
		active_cell3_button.set_tooltip_text("Set active cell #2 by pressing Left mouse button first");
	else
		active_cell3_button.set_tooltip_text("Set active cell #1 by pressing Right mouse button first");

	if (old_has_active_cell3)
		reload_grid_cell(old_active_cell3_y, old_active_cell3_x);
	if (has_active_cell3())
		reload_grid_cell(active_cell3_y, active_cell3_x);

	draw_triangle_box.set_sensitive(has_active_cell3());
}

bool MosaicWindow::has_active_cell3() {
	return active_cell3_y >= 0 && active_cell3_x >= 0;
}

bool MosaicWindow::is_active_cell3(int y, int x) {
	return has_active_cell3() && y == active_cell3_y && x == active_cell3_x;
}

void MosaicWindow::on_grid_button2_press(int n, double x, double y, Index cell_y, Index cell_x) {
	if (has_active_cell2()) {
		set_active_cell3(cell_y, cell_x);
	}
	else if (has_active_cell()) {
		Size new_radius = grid.get_line_size(active_cell_y, active_cell_x, cell_y, cell_x);
		draw_circle_radius_spin.set_value(new_radius);
	}
	else {
		set_grid_cell_active_color2(cell_y, cell_x);
	}
}

void MosaicWindow::on_grid_button3_press(int n, double x, double y, Index cell_y, Index cell_x) {
	if (cell_y == active_cell_y && cell_x == active_cell_x) {
		cell_y = NO_INDEX;
		cell_x = NO_INDEX;
	}
	set_active_cell(cell_y, cell_x);
}

bool MosaicWindow::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state) {
	Gdk::ModifierType modifier_mask = state & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK | Gdk::ModifierType::ALT_MASK);

	if (keyval == GDK_KEY_1 && modifier_mask == Gdk::ModifierType::ALT_MASK) {
		cout << "Pressed Alt-1" << endl;
		return true;
	}
	else if (keyval == GDK_KEY_Escape || keyval == GDK_KEY_q && modifier_mask == Gdk::ModifierType::CONTROL_MASK || keyval == GDK_KEY_x && modifier_mask == Gdk::ModifierType::ALT_MASK) {
		// close the window, when Escape or Ctrl-Q or Alt-X keys pressed
		quit();
		return true;
	}
	else if (keyval == GDK_KEY_z && modifier_mask == Gdk::ModifierType::CONTROL_MASK) {
		// undo grid changes, when Ctrl-Z pressed
		undo();
		return true;
	}
	else if (keyval == GDK_KEY_y && modifier_mask == Gdk::ModifierType::CONTROL_MASK) {
		// redo grid changes, when Ctrl-Y pressed
		redo();
		return true;
	}
	else if (keyval == GDK_KEY_KP_Left || keyval == GDK_KEY_Left/* && modifier_mask == Gdk::ModifierType::CONTROL_MASK*/) {
		move(0, -1);
		return true;
	}
	else if (keyval == GDK_KEY_KP_Up || keyval == GDK_KEY_Up/* && modifier_mask == Gdk::ModifierType::CONTROL_MASK*/) {
		move(-1, 0);
		return true;
	}
	else if (keyval == GDK_KEY_KP_Down || keyval == GDK_KEY_Down/* && modifier_mask == Gdk::ModifierType::CONTROL_MASK*/) {
		move(+1, 0);
		return true;
	}
	else if (keyval == GDK_KEY_KP_Right || keyval == GDK_KEY_Right/* && modifier_mask == Gdk::ModifierType::CONTROL_MASK*/) {
		move(0, +1);
		return true;
	}

	// the event has not been handled
	return false;
}

void MosaicWindow::draw_text() {
	grid.push_undo();

	grid.start_rainbow((RainbowType)rainbow_dropdown.get_selected());

	grid.draw_text(active_cell_y, active_cell_x, draw_text_entry.get_text(), active_color, active_color2);

	grid.stop_rainbow();
}

void MosaicWindow::draw_circle() {
	Size radius = draw_circle_radius_spin.get_value();
	auto type = draw_circle_type_dropdown.get_selected();

	grid.push_undo();

	grid.start_rainbow((RainbowType)rainbow_dropdown.get_selected());

	Color color1 = active_color;
	Color color2 = active_color2 != NO_COLOR ? active_color2 : color1 == Gr ? Bl : Gr;
	Color color3 = color2 == Re ? color1 == Or ? Gr : Or : color1 == Re ? color2 == Gr ? Bl : Gr : Re;

	switch (type) {
	case CIRCLE_TYPE_OUTLINE:
		grid.draw_circle(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CIRCLE_TYPE_FILLED:
		grid.draw_filled_circle(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CIRCLE_TYPE_FILLED_2:
		grid.draw_filled_circle_2(active_cell_y, active_cell_x, radius, color1, color2);
		break;
	case CIRCLE_TYPE_CLOCK:
		grid.draw_clock(active_cell_y, active_cell_x, radius, color1, color2, color3);
		break;
	case CIRCLE_TYPE_CREST:
	case CIRCLE_TYPE_CREST_DIAG:
	case CIRCLE_TYPE_CREST_BOTH:
	{
		int axes_or_diagonal = type == CIRCLE_TYPE_CREST ? 1 : type == CIRCLE_TYPE_CREST_DIAG ? 2 : 3;
		grid.draw_circle_crest(active_cell_y, active_cell_x, radius, color1, color2, axes_or_diagonal);
		break;
	}
	case CIRCLE_TYPE_RHOMB_OUTLINE:
		grid.draw_rhomb(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CIRCLE_TYPE_RHOMB_FILLED:
		grid.draw_filled_rhomb(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CIRCLE_TYPE_RHOMB_FILLED_2:
		grid.draw_filled_rhomb_2(active_cell_y, active_cell_x, radius, color1, color2);
		break;
	case CIRCLE_TYPE_SQUARE_OUTLINE:
		grid.draw_square(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CIRCLE_TYPE_SQUARE_FILLED:
		grid.draw_filled_square(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CIRCLE_TYPE_SQUARE_FILLED_2:
		grid.draw_filled_square_2(active_cell_y, active_cell_x, radius, color1, color2);
		break;
	case CIRCLE_TYPE_TRIANGLE:
	case CIRCLE_TYPE_TRIANGLE_FILLED:
	case CIRCLE_TYPE_TRIANGLE_FILLED_2:
		if (radius <= 1) {
			show_message_dialog("Such 0 or 1 radius is not supported", true);
			break;
		}
		Size yd, xd;
		grid.get_circle_triangle_delta(radius, yd, xd);
		Index y1 = active_cell_y - radius;
		Index x1 = active_cell_x;
		Index y2 = active_cell_y + yd;
		Index x2 = active_cell_x + xd;
		Index y3 = active_cell_y + yd;
		Index x3 = active_cell_x - xd;
		if (type == CIRCLE_TYPE_TRIANGLE)
			grid.draw_triangle(y1, x1, y2, x2, y3, x3, active_color);
		else if (type == CIRCLE_TYPE_TRIANGLE_FILLED)
			grid.draw_filled_triangle(y1, x1, y2, x2, y3, x3, active_color);
		else
			grid.draw_filled_triangle_2(y1, x1, y2, x2, y3, x3, color1, color2);
		break;
	}

	grid.stop_rainbow();
}

void MosaicWindow::draw_rect() {
	auto type = draw_rect_type_dropdown.get_selected();

	grid.push_undo();

	grid.start_rainbow((RainbowType)rainbow_dropdown.get_selected());

	Color color1 = active_color;
	Color color2 = active_color2 != NO_COLOR ? active_color2 : color1 == Gr ? Bl : Gr;

	switch (type) {
	case RECT_TYPE_OUTLINE:
		grid.draw_rect(active_cell_y, active_cell_x, active_cell2_y, active_cell2_x, active_color);
		break;
	case RECT_TYPE_FILLED:
		grid.draw_filled_rect(active_cell_y, active_cell_x, active_cell2_y, active_cell2_x, active_color);
		break;
	case RECT_TYPE_FILLED_2:
		grid.draw_filled_rect_2(active_cell_y, active_cell_x, active_cell2_y, active_cell2_x, color1, color2);
		break;
	case RECT_TYPE_LINE:
		grid.draw_line(active_cell_y, active_cell_x, active_cell2_y, active_cell2_x, active_color);
		break;
	case RECT_TYPE_TRIANGLE:
	case RECT_TYPE_TRIANGLE_FILLED:
	case RECT_TYPE_TRIANGLE_FILLED_2:
		Index y1 = max(active_cell_y, active_cell2_y);
		Index x1 = min(active_cell_x, active_cell2_x);
		Index y2 = max(active_cell2_y, active_cell_y);
		Index x2 = max(active_cell2_x, active_cell_x);
		Index y3 = min(active_cell_y, active_cell2_y);
		Index x3 = (x1 + x2) / 2;
		if (type == RECT_TYPE_TRIANGLE)
			grid.draw_triangle(y1, x1, y2, x2, y3, x3, active_color);
		else if (type == RECT_TYPE_TRIANGLE_FILLED)
			grid.draw_filled_triangle(y1, x1, y2, x2, y3, x3, active_color);
		else
			grid.draw_filled_triangle_2(y1, x1, y2, x2, y3, x3, color1, color2);
		break;
	}

	grid.stop_rainbow();
}

void MosaicWindow::draw_triangle() {
	auto type = draw_triangle_type_dropdown.get_selected();

	grid.push_undo();

	grid.start_rainbow((RainbowType)rainbow_dropdown.get_selected());

	Color color1 = active_color;
	Color color2 = active_color2 != NO_COLOR ? active_color2 : color1 == Gr ? Bl : Gr;

	switch (type) {
	case TRIANGLE_TYPE_OUTLINE:
		grid.draw_triangle(active_cell_y, active_cell_x, active_cell2_y, active_cell2_x, active_cell3_y, active_cell3_x, active_color);
		break;
	case TRIANGLE_TYPE_FILLED:
		grid.draw_filled_triangle(active_cell_y, active_cell_x, active_cell2_y, active_cell2_x, active_cell3_y, active_cell3_x, active_color);
		break;
	case TRIANGLE_TYPE_FILLED_2:
		grid.draw_filled_triangle_2(active_cell_y, active_cell_x, active_cell2_y, active_cell2_x, active_cell3_y, active_cell3_x, color1, color2);
		break;
	}

	grid.stop_rainbow();
}

void MosaicWindow::show_about_dialog() {
	auto dialog = new Gtk::AboutDialog();

	dialog->set_program_name("Mosaic");
	dialog->set_version(VERSION);
	dialog->set_license_type(Gtk::License::GPL_3_0);
	dialog->set_comments("Conveniently create nice pictores for GAN Mosaic");
	dialog->set_website("https://github.com/mig0/mosaic/");
	dialog->set_authors({ "Mikhael Goikhman", "Daniel Goikhman" });
	auto logo = Gtk::MediaFile::create();
	logo->set_filename("mosaic-logo.png");
	dialog->set_logo(logo);

	dialog->show();
}

void MosaicWindow::show_message_dialog(const Glib::ustring &message, bool is_error) {
	if (message_dialog) {
		auto existing_dialog = message_dialog;
		message_dialog = nullptr;
		delete existing_dialog;
	}
	auto dialog = message_dialog = new Gtk::MessageDialog(is_error ? "Error" : "Info", false, is_error ? Gtk::MessageType::ERROR : Gtk::MessageType::INFO, Gtk::ButtonsType::CLOSE);
	dialog->set_transient_for(*this);
	// BUG in gtkmm-4.10, this leaves the button in pressed state forever, so do not call set_modal()
//	dialog->set_modal(true);
	dialog->set_secondary_text(message);
	// BUG in gtkmm-4.10, the default close response does not work, implement own response
//	dialog->set_default_response(Gtk::ResponseType::CLOSE);
	dialog->signal_response().connect(sigc::bind(
		sigc::mem_fun(*this, &MosaicWindow::on_message_dialog_close), dialog));
	dialog->show();
}

void MosaicWindow::show_file_dialog(bool is_save) {
	auto dialog = new Gtk::FileChooserDialog(
		"Please choose a file",
		is_save ? Gtk::FileChooser::Action::SAVE : Gtk::FileChooser::Action::OPEN);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
		sigc::mem_fun(*this, is_save ? &MosaicWindow::on_file_dialog_save : &MosaicWindow::on_file_dialog_load), dialog));

	// add response buttons to the dialog
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button(is_save ? "_Save" : "_Load", Gtk::ResponseType::OK);

	// add filters to select only certain file types
	auto filter_sav = Gtk::FileFilter::create();
	filter_sav->set_name("SAV files");
	filter_sav->add_pattern("*.sav");
	dialog->add_filter(filter_sav);

	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);

	auto filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog->add_filter(filter_any);

	dialog->show();
}

void MosaicWindow::clear() {
	grid.push_undo();
	grid.clear();
}

void MosaicWindow::on_message_dialog_close(int response_id, Gtk::Dialog* dialog) {
	message_dialog = nullptr;
	if (dialog) delete dialog;
}

void MosaicWindow::on_file_dialog_save(int response_id, Gtk::FileChooserDialog* dialog) {
	if (response_id == Gtk::ResponseType::OK)
		grid.save(dialog->get_file()->get_path());
	delete dialog;
}

void MosaicWindow::on_file_dialog_load(int response_id, Gtk::FileChooserDialog* dialog) {
	if (response_id == Gtk::ResponseType::OK)
		grid.load(dialog->get_file()->get_path());
	delete dialog;
	reload_grid();
}

void MosaicWindow::save() {
	show_file_dialog(true);
}

void MosaicWindow::load() {
	grid.push_undo();
	show_file_dialog(false);
}

void MosaicWindow::quit() {
	hide();
}

bool MosaicWindow::on_close_request() {
	on_hide();
	return false;
}

void MosaicWindow::on_hide() {
	cout << "Have a nice day!" << endl;
}

void MosaicWindow::undo() {
	if (grid.has_undo())
		grid.undo();
	else
		show_message_dialog("No more undo layers available", true);
}

void MosaicWindow::redo() {
	if (grid.has_redo())
		grid.redo();
	else
		show_message_dialog("No redo layers available", true);
}

void MosaicWindow::move(int y_offset, int x_offset) {
	MoveType type = (MoveType)move_type_dropdown.get_selected();

	if (!has_active_cell()) {
		show_message_dialog("To move area, active cell #1 should be selected first", true);
		return;
	}
	if (!has_active_cell2()) {
		show_message_dialog("To move area, active cell #2 should be selected first", true);
		return;
	}
	if (y_offset == 0 && x_offset == 0) {
		show_message_dialog("Moving area with zero offset is nonsense", true);
		return;
	}

	int new_active_cell_y  = active_cell_y  + y_offset;
	int new_active_cell_x  = active_cell_x  + x_offset;
	int new_active_cell2_y = active_cell2_y + y_offset;
	int new_active_cell2_x = active_cell2_x + x_offset;
	if (!grid.is_coord_visible(new_active_cell_y, new_active_cell_x) || !grid.is_coord_visible(new_active_cell2_y, new_active_cell2_x)) {
		show_message_dialog("Can't move selected area out of grid", true);
		return;
	}

	grid.push_undo();
	grid.move(active_cell_y, active_cell_x, active_cell2_y, active_cell2_x, y_offset, x_offset, type);

	if (type != MOVE_TYPE_COPY) {
		set_active_cell(new_active_cell_y, new_active_cell_x);
		set_active_cell2(new_active_cell2_y, new_active_cell2_x);
	}
}

void MosaicWindow::move_area() {
	move(move_y_offset_spin.get_value(), move_x_offset_spin.get_value());
}
