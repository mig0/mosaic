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
	Color active_color = Re;
	Gtk::MenuButton active_color_button;
	Gtk::Box button_box;
	Gtk::Button save_button;
	Gtk::Button load_button;
	Gtk::Button quit_button;
	Glib::RefPtr<Gtk::CssProvider> css_provider;
	Grid &grid;

	void reload_grid();
	void reload_grid_cell(Index y, Index x);
	void set_grid_cell_color(Index y, Index x, Color color);
	void set_grid_cell_active_color(Index y, Index x);
	void set_button_color(Gtk::Widget &button, Color color);
	bool on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state);
	void on_file_dialog_save(int response_id, Gtk::FileChooserDialog* dialog);
	void on_file_dialog_load(int response_id, Gtk::FileChooserDialog* dialog);
	void show_file_dialog(bool is_save);
	void save() { show_file_dialog(true); }
	void load() { show_file_dialog(false); }
	void quit() { hide(); }
	bool on_close_request() { on_hide(); return false; }
	void on_hide() { cout << "Have a nice day!" << endl; }

public:
	MosaicWindow(Grid &grid0);
};

