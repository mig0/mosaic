#include "graphics.h"

MosaicWindow::MosaicWindow(Grid &grid0) : grid(grid0) {
	ostringstream str_stream;
	Size size_y = grid.get_size_y();
	Size size_x = grid.get_size_x();

	set_title("Mosaic application");
	set_default_size(200, 200);

	set_child(main_box);
	main_box.set_orientation(Gtk::Orientation::HORIZONTAL);

	main_box.append(main_frame);
	main_frame.set_margin(10);

	str_stream << "Mosaic " << size_x << "x" << size_y;
	main_frame.set_label(str_stream.str());
	main_frame.set_label_align(Gtk::Align::CENTER);

	main_frame.set_child(main_grid);

	main_box.append(control_frame);
	control_frame.set_margin(10);
	control_frame.set_margin_start(0);
	control_frame.set_expand(true);

	control_frame.set_child(control_box);
	control_box.set_orientation(Gtk::Orientation::VERTICAL);
	control_box.set_valign(Gtk::Align::END);
	control_box.set_margin(10);

	control_box.append(quit_button);
	quit_button.set_label("Quit");
	quit_button.set_margin(5);
	quit_button.signal_clicked().connect(sigc::mem_fun(*this, &MosaicWindow::quit));

	main_grid.set_margin(10);
	main_grid.set_expand(true);
	main_grid.set_row_homogeneous(true);
	main_grid.set_column_homogeneous(true);

	Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
	css_provider->load_from_data(" \
		button.red    { background-image: image(rgb(180, 30, 30));   border-color: rgb(80, 10, 10); } \
		button.white  { background-image: image(rgb(240, 240, 240)); border-color: rgb(120, 120, 120); } \
		button.yellow { background-image: image(rgb(240, 210, 30));  border-color: rgb(120, 100, 10); } \
		button.orange { background-image: image(rgb(240, 100, 30));  border-color: rgb(120, 50, 10);  } \
		button.blue   { background-image: image(rgb(30, 30, 180));   border-color: rgb(10, 10, 90); } \
		button.green  { background-image: image(rgb(30, 180, 30));   border-color: rgb(10, 100, 10); } \
		button { border: solid 1px; font: 8px \"Sans\"; margin: 0px; padding: 0px; min-width: 20px; min-height: 20px; } \
		button:hover { background-image: image(pink); } \
		button:active { background-image: image(purple); } \
	");

	for (Index y = 0; y < size_y; y++) {
		for (Index x = 0; x < size_x; x++) {
			ostringstream str_stream;
			str_stream << "(" << x << ", " << y << ")";
			Gtk::Button button;
			main_grid.attach(button, x, y);
			set_grid_button_class(y, x);
			button.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
			button.set_tooltip_text(str_stream.str());
		}
	}

	auto controller = Gtk::EventControllerKey::create();
	controller->signal_key_pressed().connect(
		sigc::mem_fun(*this, &MosaicWindow::on_window_key_pressed), false);
	add_controller(controller);

	main_grid.show();
	main_frame.show();
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

void MosaicWindow::set_grid_button_class(Index y, Index x) {
	vector<Glib::ustring> css_classes = { grid.get_color_name(y, x) };
	main_grid.get_child_at(x, y)->set_css_classes(css_classes);
}
