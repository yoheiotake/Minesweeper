#include "mines.h"


SetUpWin::SetUpWin(UserOption *p): m_val( 0 ),m_Alignment(0.5, 0.5, 0, 0),skip_bt("Skip"),exit_bt("Exit")
{
  user_struct = p;
  user_struct -> time_try_flag = false;
  user_struct -> continue_flag = true;

  load ="Loading";
  percent = "%/100%";
  bar_title = "Loading.  0%/100%";
  set_resizable();
  set_title("Set Up Window");

  t_label.set_text("TIME TRY OPTION");
  level.set_text("DIFFICULTY: Select Game Level");

  m_chk.set_label("Time Trial");

  m_radio1.set_label("BASIC");
  m_radio2.set_label("MEDIUM");
  m_radio3.set_label("HARD");

  m_tooltip.set_tip( m_radio1, Glib::locale_to_utf8 ("マス目:81個, 地雷:10個") );  // 9*9
  m_tooltip.set_tip( m_radio2, Glib::locale_to_utf8 ("マス目:256個, 地雷:40個") ); // 16*16
  m_tooltip.set_tip( m_radio3, Glib::locale_to_utf8 ("マス目:480個, 地雷:99個") ); // 30*16

  Gtk::RadioButton::Group gr = m_radio1.get_group();
  m_radio2.set_group( gr );
  m_radio3.set_group( gr );

  mv_box.set_border_width(10); // ボタンに縁を作成

  add(mv_box);

  mv_box.pack_start(m_Alignment, Gtk::PACK_SHRINK, 5);
  m_Alignment.add(m_progbar);
  m_progbar.set_text(bar_title);

  mw_box.pack_start(exit_bt);
  mw_box.pack_start(skip_bt);
  mv_box.pack_start(mw_box);

  mv_box.pack_start(t_label);

  mv_box.pack_start(m_chk);

  mv_box.pack_start(level);
  radio_box.pack_start(m_radio1);
  radio_box.pack_start(m_radio2);
  radio_box.pack_start(m_radio3);
  mv_box.pack_start(radio_box);

  Glib::signal_timeout().connect( sigc::mem_fun( *this, &SetUpWin::on_timeout ), 800 );
  m_progbar.set_size_request(500,50);

  exit_bt.signal_clicked().connect( sigc::mem_fun( *this, &SetUpWin::on_exit_bt_clicked ) );
  skip_bt.signal_clicked().connect( sigc::mem_fun( *this, &SetUpWin::on_skip_bt_clicked ) );

  show_all_children();
}


bool SetUpWin::on_timeout()
{
  m_val += 0.01;

  if(m_val >= 1.00)
    m_val = 1.00;

  stringstream ss; // 数値→文字列 変換

  int x = (int) (1000 * m_val)%30;
  int y = (int) 100*m_val;

  if(m_val == 1.00)
    ss <<"Complete!! "<< y << percent;
  else if( x < 10 )
    ss << load <<".  "<< y << percent;
  else if( 10 <= x && x < 20 )
    ss << load <<".. "<< y << percent;
  else
    ss << load <<"..."<< y << percent;

  bar_title = ss.str();
  m_progbar.set_text(bar_title);

  if(m_val == 1.00){
    get_check_state();
    get_radio_state();
    hide();
  }

  m_progbar.set_fraction( m_val );
}


void SetUpWin::on_exit_bt_clicked(){
  user_struct -> continue_flag = false;
  hide();
}

void SetUpWin::on_skip_bt_clicked(){
  get_check_state();
  get_radio_state();
  hide();
}

void SetUpWin::set_user_struct(int b,int mv,int mh){
  user_struct -> mine_num = b;
  user_struct -> matrix_v = mv;
  user_struct -> matrix_h = mh;
}

void SetUpWin::get_check_state(){ // 終了直前のCheck_Buttonの状態を取得
  if(m_chk.get_active())
    user_struct -> time_try_flag = true;
}

void SetUpWin::get_radio_state(){
  if( m_radio1.get_active() ) set_user_struct(10,9,9);
  else if( m_radio2.get_active() ) set_user_struct(40,16,16);
  else if( m_radio3.get_active() ) set_user_struct(99,16,30);
}
