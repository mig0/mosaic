#include "graphics.h"

enum CenterType {
	CENTER_TYPE_OUTLINE,
	CENTER_TYPE_FILLED,
	CENTER_TYPE_FILLED_2,
	CENTER_TYPE_RAINBOW,
	CENTER_TYPE_CLOCK,
	CENTER_TYPE_CREST,
	CENTER_TYPE_CREST_DIAG,
	CENTER_TYPE_CREST_BOTH,
	CENTER_TYPE_RHOMB_OUTLINE,
	CENTER_TYPE_RHOMB_FILLED,
	CENTER_TYPE_RHOMB_FILLED_2,
	CENTER_TYPE_RHOMB_RAINBOW,
};

vector<Glib::ustring> center_type_strings = {
	"Outline",
	"Filled",
	"Filled 2",
	"Rainbow",
	"Clock",
	"Crest",
	"Crest diag",
	"Crest both",
	"Rhomb Outline ",
	"Rhomb Filled",
	"Rhomb Filled 2",
	"Rhomb Rainbow",
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
	set_button_color(active_cell_button, NO_COLOR);

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
	draw_circle_label.set_label("Circle");
	draw_circle_label.set_halign(Gtk::Align::START);
	draw_circle_label.set_expand(true);

	draw_circle_type_dropdown = Gtk::DropDown(center_type_strings);
	draw_circle_type_dropdown.set_selected(0);
	draw_circle_type_dropdown.set_size_request(200);
	draw_circle_box.append(draw_circle_type_dropdown);

	auto draw_circle_adj = Gtk::Adjustment::create(size_y / 4, 0, min(size_y, size_x));
	draw_circle_radius_spin.set_adjustment(draw_circle_adj);
	draw_circle_box.append(draw_circle_radius_spin);

	draw_circle_box.append(draw_circle_button);
	draw_circle_button.set_label("Draw");
	draw_circle_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::draw_circle));

	control_box.append(button_box);
	button_box.set_orientation(Gtk::Orientation::HORIZONTAL);
	button_box.set_halign(Gtk::Align::CENTER);
	button_box.set_margin(2);

	button_box.append(save_button);
	save_button.set_label("Save");
	save_button.set_margin(4);
	save_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::save));

	button_box.append(load_button);
	load_button.set_label("Load");
	load_button.set_margin(4);
	load_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::load));

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
			button.signal_clicked().connect([this, y, x]() { set_grid_cell_active_color(y, x); });
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

	auto controller = Gtk::EventControllerKey::create();
	controller->signal_key_pressed().connect(
		sigc::mem_fun(*this, &MosaicWindow::on_window_key_pressed), false);
	add_controller(controller);
}

void MosaicWindow::set_button_color(Gtk::Widget &button, Color color) {
	vector<Glib::ustring> css_classes = { color == NO_COLOR ? "none" : grid.get_color_name(color) };
	int cell_y, cell_x, width, height;
	main_grid.query_child(button, cell_x, cell_y, width, height);
	if (has_active_cell() && cell_y == active_cell_y && cell_x == active_cell_x || &button == &active_cell_button)
		css_classes.push_back("active-cell");
	button.set_css_classes(css_classes);
}

void MosaicWindow::set_button_coord_tooltip(Gtk::Widget &button, Index y, Index x) {
	ostringstream str_stream;
	if (y != NO_INDEX && x != NO_INDEX)
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
	grid.set_color(y, x, color);
}

void MosaicWindow::set_grid_cell_color_callback(Index y, Index x, Color color) {
	reload_grid_cell(y, x);
}

void MosaicWindow::set_grid_cell_active_color(Index y, Index x) {
	set_grid_cell_color(y, x, active_color);
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
	active_cell_y = y;
	active_cell_x = x;
	set_button_color(active_cell_button, has_active_cell() ? grid.get_color(y, x) : NO_COLOR);
	set_button_coord_tooltip(active_cell_button, y, x);
	draw_text_box.set_sensitive(has_active_cell());
	draw_circle_box.set_sensitive(has_active_cell());
}

bool MosaicWindow::has_active_cell() {
	return active_cell_y >= 0 && active_cell_x >= 0;
}

void MosaicWindow::on_grid_button2_press(int n, double x, double y, Index cell_y, Index cell_x) {
	if (has_active_cell()) {
		Size new_radius = grid.get_line_size(active_cell_y, active_cell_x, cell_y, cell_x);
		draw_circle_radius_spin.set_value(new_radius);
	}
}

void MosaicWindow::on_grid_button3_press(int n, double x, double y, Index cell_y, Index cell_x) {
	int old_active_cell_y = active_cell_y;
	int old_active_cell_x = active_cell_x;
	bool old_has_active_cell = has_active_cell();
	if (cell_y == active_cell_y && cell_x == active_cell_x) {
		cell_y = NO_INDEX;
		cell_x = NO_INDEX;
	}
	set_active_cell(cell_y, cell_x);
	if (old_has_active_cell)
		reload_grid_cell(old_active_cell_y, old_active_cell_x);
	if (has_active_cell())
		reload_grid_cell(active_cell_y, active_cell_x);
}

bool MosaicWindow::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state)
{
	Gdk::ModifierType modifier_mask = state & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK | Gdk::ModifierType::ALT_MASK);

	if (keyval == GDK_KEY_1 && modifier_mask == Gdk::ModifierType::ALT_MASK)
	{
		cout << "Pressed Alt-1" << endl;
		return true;
	}
	else if (keyval == GDK_KEY_Escape || keyval == GDK_KEY_q && modifier_mask == Gdk::ModifierType::CONTROL_MASK || keyval == GDK_KEY_x && modifier_mask == Gdk::ModifierType::ALT_MASK)
	{
		// close the window, when Escape or Ctrl-Q or Alt-X keys pressed
		quit();
		return true;
	}

	// the event has not been handled
	return false;
}

void MosaicWindow::draw_text() {
	grid.set_text_color(active_cell_y, active_cell_x, draw_text_entry.get_text(), active_color, active_color2);
}

void MosaicWindow::draw_circle() {
	Size radius = draw_circle_radius_spin.get_value();
	auto type = draw_circle_type_dropdown.get_selected();

	Color color1 = active_color;
	Color color2 = active_color2 != NO_COLOR ? active_color2 : color1 == Gr ? Bl : Gr;
	Color color3 = color2 == Re ? color1 == Or ? Gr : Or : color1 == Re ? color2 == Gr ? Bl : Gr : Re;

	switch (type) {
	case CENTER_TYPE_OUTLINE:
		grid.set_circle_color(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CENTER_TYPE_FILLED:
		grid.set_filled_circle_color(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CENTER_TYPE_FILLED_2:
		grid.set_filled_circle_2_color(active_cell_y, active_cell_x, radius, color1, color2);
		break;
	case CENTER_TYPE_RAINBOW:
		grid.set_filled_circle_rainbow_color(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CENTER_TYPE_CLOCK:
		grid.set_clock_color(active_cell_y, active_cell_x, radius, color1, color2, color3);
		break;
	case CENTER_TYPE_CREST:
	case CENTER_TYPE_CREST_DIAG:
	case CENTER_TYPE_CREST_BOTH:
	{
		int axes_or_diagonal = type == CENTER_TYPE_CREST ? 1 : type == CENTER_TYPE_CREST_DIAG ? 2 : 3;
		grid.set_circle_crest_color(active_cell_y, active_cell_x, radius, color1, color2, axes_or_diagonal);
		break;
	}
	case CENTER_TYPE_RHOMB_OUTLINE:
		grid.set_rhomb_color(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CENTER_TYPE_RHOMB_FILLED:
		grid.set_filled_rhomb_color(active_cell_y, active_cell_x, radius, active_color);
		break;
	case CENTER_TYPE_RHOMB_FILLED_2:
		grid.set_filled_rhomb_2_color(active_cell_y, active_cell_x, radius, color1, color2);
		break;
	case CENTER_TYPE_RHOMB_RAINBOW:
		grid.set_filled_rhomb_rainbow_color(active_cell_y, active_cell_x, radius, active_color);
		break;
	}
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
