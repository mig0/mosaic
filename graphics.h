#include <gtkmm.h>
#include <gdkmm.h>
#include "grid.h"
#include "screensaver.h"

enum ActionType {
	ACTION_DRAW,
	ACTION_SAVE,
	ACTION_LOAD,
	ACTION_UNDO,
	ACTION_REDO,
	ACTION_MOVE,
	ACTION_KEY_PRESS,
	ACTION_MOUSE_PRESS,
};

class IconButton : public Gtk::Button {
public:
	void set_icon_and_label(const Glib::ustring &icon_name, const Glib::ustring &name);

protected:
	Gtk::Box box;
	Gtk::Image image;
	Gtk::Label label;
};

class MosaicWindow : public Gtk::Window {
public:
	MosaicWindow(Grid &grid);

protected:
	Gtk::Box main_box;
	Gtk::Frame main_frame;
	Gtk::Grid main_grid;
	Gtk::Frame control_frame;
	Gtk::Box control_box;

	Gtk::Box mini_button_box;
	Gtk::Button undo_button;
	Gtk::Button redo_button;
	Gtk::Button move_l_button;
	Gtk::Button move_u_button;
	Gtk::Button move_d_button;
	Gtk::Button move_r_button;
	Gtk::Button screensaver_button;

	Gtk::Box action_box;

	Gtk::Box active_color_box;
	Gtk::Label active_color_label;
	Gtk::DropDown rainbow_dropdown;
	Color active_color = Re;
	Color active_color2 = NO_COLOR;
	Gtk::MenuButton active_color_menu_button;
	Gtk::Button active_color_button;
	Gtk::Popover active_color_popover;
	Gtk::Box color_selection_box;
	Gtk::MenuButton active_color2_menu_button;
	Gtk::Button active_color2_button;
	Gtk::Popover active_color2_popover;
	Gtk::Box color2_selection_box;

	Gtk::Box active_cell_box;
	Gtk::Label active_cell_label;
	int active_cell_y = NO_INDEX;
	int active_cell_x = NO_INDEX;
	Gtk::ToggleButton active_cell_button;
	int active_cell2_y = NO_INDEX;
	int active_cell2_x = NO_INDEX;
	Gtk::ToggleButton active_cell2_button;
	int active_cell3_y = NO_INDEX;
	int active_cell3_x = NO_INDEX;
	Gtk::ToggleButton active_cell3_button;

	Gtk::Box draw_text_box;
	Gtk::Label draw_text_label;
	Gtk::Entry draw_text_entry;
	Gtk::Button draw_text_button;

	Gtk::Box draw_circle_box;
	Gtk::Label draw_circle_label;
	Gtk::DropDown draw_circle_type_dropdown;
	Gtk::SpinButton draw_circle_radius_spin;
	Gtk::Button draw_circle_button;

	Gtk::Box draw_rect_box;
	Gtk::Label draw_rect_label;
	Gtk::DropDown draw_rect_type_dropdown;
	Gtk::Button draw_rect_button;

	Gtk::Box draw_triangle_box;
	Gtk::Label draw_triangle_label;
	Gtk::DropDown draw_triangle_type_dropdown;
	Gtk::Button draw_triangle_button;

	Gtk::Box move_area_box;
	Gtk::DropDown move_type_dropdown;
	Gtk::Label move_area_label;
	Gtk::SpinButton move_x_offset_spin;
	Gtk::SpinButton move_y_offset_spin;
	Gtk::Button move_area_button;

	Gtk::Box button_box;
	IconButton clear_button;
	IconButton save_button;
	IconButton load_button;
	IconButton about_button;
	IconButton quit_button;

	Screensaver screensaver;

	Glib::RefPtr<Gtk::CssProvider> css_provider;
	Grid &grid;

	bool register_action(ActionType type);

	void set_undo_redo_sensitive_callback(bool has_undo, bool has_redo);
	void set_move_buttons_sensitive();
	void set_button_color(Gtk::Widget &button, Color color);
	void set_button_coord_tooltip(Gtk::Widget &button, Index y, Index x);
	void reload_grid_cell(Index y, Index x);
	void reload_grid();

	void set_grid_cell_color(Index y, Index x, Color color);
	void set_grid_cell_color_callback(Index y, Index x, Color color);
	void set_grid_cell_active_color(Index y, Index x);
	void set_grid_cell_active_color2(Index y, Index x);
	void set_active_color(Color color);
	void set_active_color2(Color color);

	void set_active_cell(int y, int x);
	bool has_active_cell();
	bool is_active_cell(int y, int x);
	void set_active_cell2(int y, int x);
	bool has_active_cell2();
	bool is_active_cell2(int y, int x);
	void set_active_cell3(int y, int x);
	bool has_active_cell3();
	bool is_active_cell3(int y, int x);

	void on_grid_button1_press(Index cell_y, Index cell_x);
	void on_grid_button2_press(int n, double x, double y, Index cell_y, Index cell_x);
	void on_grid_button3_press(int n, double x, double y, Index cell_y, Index cell_x);
	bool on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state);
	void draw_text();
	void draw_circle();
	void draw_rect();
	void draw_triangle();

	void show_about_dialog();
	Gtk::MessageDialog* message_dialog = nullptr;
	void show_message_dialog(const Glib::ustring &message, bool is_error = false);
	void show_file_dialog(bool is_save);
	void clear();
	void on_message_dialog_close(int response_id, Gtk::Dialog* dialog);
	void on_file_dialog_save(int response_id, Gtk::FileChooserDialog* dialog);
	void on_file_dialog_load(int response_id, Gtk::FileChooserDialog* dialog);
	void save();
	void load();
	void quit();
	bool on_close_request();
	void on_hide();
	void undo();
	void redo();
	void move(int y_offset, int x_offset);
	void move_area();

	void start_screensaver();
	void stop_screensaver();
	void toggle_screensaver();
	bool is_screensaver_active();
};

