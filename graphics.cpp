#include "graphics.h"

MosaicWindow::MosaicWindow(Grid &grid0) : grid(grid0) {
	ostringstream str_stream;
	Size size_y = grid.get_size_y();
	Size size_x = grid.get_size_x();

	set_title("Mosaic application");
	set_default_size(200, 200);

	css_provider = Gtk::CssProvider::create();
	css_provider->load_from_data(" \
		button.red    { background-image: image(rgb(180, 30, 30));   border-color: rgb(80, 10, 10); } \
		button.white  { background-image: image(rgb(240, 240, 240)); border-color: rgb(120, 120, 120); } \
		button.yellow { background-image: image(rgb(240, 210, 30));  border-color: rgb(120, 100, 10); } \
		button.orange { background-image: image(rgb(240, 140, 30));  border-color: rgb(120, 70, 10);  } \
		button.blue   { background-image: image(rgb(30, 30, 180));   border-color: rgb(10, 10, 90); } \
		button.green  { background-image: image(rgb(30, 180, 30));   border-color: rgb(10, 100, 10); } \
		button.red:hover    { background-image: image(rgb(220, 48, 48)); } \
		button.white:hover  { background-image: image(rgb(255, 190, 190)); } \
		button.yellow:hover { background-image: image(rgb(255, 240, 48)); } \
		button.orange:hover { background-image: image(rgb(255, 170, 48)); } \
		button.blue:hover   { background-image: image(rgb(48, 48, 220)); } \
		button.green:hover  { background-image: image(rgb(48, 220, 48)); } \
		button.red:active    { background-image: image(rgb(140, 18, 18)); } \
		button.white:active  { background-image: image(rgb(190, 190, 190)); } \
		button.yellow:active { background-image: image(rgb(190, 170, 18)); } \
		button.orange:active { background-image: image(rgb(190, 100, 18)); } \
		button.blue:active   { background-image: image(rgb(18, 18, 140)); } \
		button.green:active  { background-image: image(rgb(18, 140, 18)); } \
		button { border: solid 1px; font: 8px \"Sans\"; margin: 0px; padding: 0px; min-width: 20px; min-height: 20px; } \
	");

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
	active_color_box.set_halign(Gtk::Align::CENTER);
	active_color_box.set_margin(2);

	active_color_box.append(active_color_label);
	active_color_label.set_label("Active Color");

	active_color_box.append(active_color_button);
	active_color_button.set_margin(5);
	Gtk::Button current_color_button;
	current_color_button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
	set_button_color(current_color_button, active_color);
	active_color_button.set_child(current_color_button);
	active_color_button.set_always_show_arrow(true);
//	active_color_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::quit));

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
			ostringstream str_stream;
			str_stream << "(" << x << ", " << y << ")";
			Gtk::Button &button = *(new Gtk::Button);
			main_grid.attach(button, x, y);
			button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
			button.set_tooltip_text(str_stream.str());
			button.signal_clicked().connect([this, y, x]() { set_grid_cell_active_color(y, x); });
		}
	}
	reload_grid();

	auto controller = Gtk::EventControllerKey::create();
	controller->signal_key_pressed().connect(
		sigc::mem_fun(*this, &MosaicWindow::on_window_key_pressed), false);
	add_controller(controller);
}

void MosaicWindow::set_button_color(Gtk::Widget &button, Color color) {
	vector<Glib::ustring> css_classes = { grid.get_color_name(color) };
	button.set_css_classes(css_classes);
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
	reload_grid_cell(y, x);
}

void MosaicWindow::set_grid_cell_active_color(Index y, Index x) {
	set_grid_cell_color(y, x, active_color);
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
