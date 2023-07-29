#include <gtkmm.h>
#include <gdkmm.h>
#include "grid.h"

class MosaicWindow : public Gtk::Window
{
protected:
	Gtk::Box main_box;
	Gtk::Frame main_frame;
	Gtk::Grid main_grid;
	Gtk::Frame control_frame;
	Gtk::Box control_box;
	Gtk::Box action_box;
	Gtk::Box active_color_box;
	Gtk::Label active_color_label;
	Gtk::MenuButton active_color_button;
	Gtk::Box button_box;
	Gtk::Button save_button;
	Gtk::Button load_button;
	Gtk::Button quit_button;
	Grid &grid;

	void set_grid_button_class(Index y, Index x);
	bool on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state);
	void quit() { hide(); }
	bool on_close_request() { on_hide(); return false; }
	void on_hide() { cout << "Have a nice day!" << endl; }

public:
	MosaicWindow(Grid &grid0);
};

