#include <gtkmm.h>
#include <string>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#define SIZE 100
#define MAX_RGB 65535
#define MIN_RGB 60000
typedef sigc::signal< void > SIGNAL;
typedef sigc::signal< bool > FLAG_SIGNAL;
typedef sigc::signal< int,int,int > AROUND_SIGNAL;

struct UserOption{
  int mine_num;
  int matrix_v;
  int matrix_h;
  bool time_try_flag;
  bool continue_flag;
  bool window_state_flag;
};

class SetUpWin : public Gtk::Window
{
  Gtk::ProgressBar m_progbar;
  double m_val;
 protected:
  Gtk::HBox mw_box;
  Gtk::VBox mv_box,radio_box;
  Gtk::Button skip_bt,exit_bt;
  Gtk::RadioButton m_radio1, m_radio2, m_radio3;
  Gtk::Alignment m_Alignment;
  Glib::ustring bar_title;
  Gtk::Label t_label,level;
  Gtk::CheckButton m_chk;
  Gtk::Tooltips m_tooltip;
 public:
  SetUpWin(UserOption *p);
 private:
  string load;
  string percent;
  bool on_timeout();
  void get_check_state();
  void get_radio_state();
  UserOption *user_struct;
  void on_exit_bt_clicked();
  void on_skip_bt_clicked();
  void set_user_struct(int b,int mv,int mh);
};

class ContinueDialog:Gtk::Window
{
 private:
  bool continue_flag;
  bool restart_flag;

 public:
  ContinueDialog(bool flag);
  bool get_continue();
  bool get_restart();

};

class MyButton:public Gtk::Button
{
 private:
  int matrix_x;
  int matrix_y;
  bool mine_flag;
  bool open_flag;
  bool flag_state;
  Gtk::Image flag_img;
  UserOption *user_struct;
  SIGNAL hide_call_signal;
  SIGNAL open_call_signal;
  FLAG_SIGNAL flag_call_signal;
  AROUND_SIGNAL around_state_signal;
  Glib::RefPtr< Gdk::Pixbuf > f_pixbuf;
  Glib::ustring button_name,opened,closed;
  
 protected:
  virtual void on_clicked();
 public:
  MyButton();
  bool set_mine();
  void open_button();
  void get_matrix(int x,int y);
  void set_struct(UserOption *p);
  bool get_open_flag(){return open_flag; };
  bool get_mine_flag(){return mine_flag; };
  SIGNAL hide_connect(){ return hide_call_signal; };
  SIGNAL open_connect(){ return open_call_signal; };
  FLAG_SIGNAL flag_connect(){ return flag_call_signal; };
  AROUND_SIGNAL around_connect(){ return around_state_signal; };
};



class MainWin:public Gtk::Window
{
 private:
  int size_v;
  int size_h;
  double limit;
  int score_time;
  bool flag_state;
  int open_counter;
  int mines_number;
  bool switch_flag;
  bool on_timeout();
  bool set_vision();
  bool time_try_flag;
  bool answer_click_flag;
  Gdk::Color col;
  Gtk::MenuBar mw_MenuBar;
  Gtk::ProgressBar m_progbar;
  Gtk::Menu preference;
  Gtk::Image mine_img[SIZE],explosion_img[SIZE];
  Glib::RefPtr< Gdk::Pixbuf > m_pixbuf,e_pixbuf;
  Gtk::CheckButton chk_bt;
  Gtk::Button quit_bt,answer_bt,reset_bt;
  UserOption *user_struct;
  Glib::ustring opened,closed;
  virtual void on_clicked ();
  void answer_on_clicked();
  void hide_call();
  bool switch_icon();
  void setting_win();
  void check_bt_on_clicked();
  int set_around_state(int matrix_x,int matrix_y);
  int around_mine_counter(int matrix_h,int matrix_v);
  bool get_flag_state(){ return flag_state; };

 public:
  MainWin (UserOption *p);
  unsigned long random(unsigned long seed);

 protected:
  Gtk::HBox hbox;
  Gtk::Table ms_grid;
  MyButton square[SIZE][SIZE];
  Gtk::VBox board;
};
