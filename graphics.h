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
	Gtk::MenuButton active_color_menu_button;
	Gtk::Button active_color_button;
	Gtk::Popover active_color_popover;
	Gtk::Box color_selection_box;
	Gtk::Box active_cell_box;
	Gtk::Label active_cell_label;
	int active_cell_y = -1;
	int active_cell_x = -1;
	Gtk::ToggleButton active_cell_button;
	Gtk::Box draw_text_box;
	Gtk::Label draw_text_label;
	Gtk::Entry draw_text_entry;
	Gtk::Button draw_text_button;
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
	void set_active_color(Color color);
	void set_active_cell(int y, int x);
	bool has_active_cell();
	void set_button_color(Gtk::Widget &button, Color color);
	void on_grid_button2_press(int n, double x, double y);
	void on_grid_button3_press(int n, double x, double y, Index cell_y, Index cell_x);
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

