#include "mines.h"

MainWin::MainWin (UserOption *p) : quit_bt("Quit"),answer_bt("Answer"),reset_bt("Reset")
{
  limit = 0;
  score_time = 0;
  open_counter = 0;
  switch_flag = false;
  flag_state = false;
  answer_click_flag = false;
  user_struct = p;
  user_struct -> window_state_flag = false;
  mines_number = user_struct -> mine_num;
  time_try_flag = user_struct -> time_try_flag;
  size_v = user_struct -> matrix_v ;
  size_h = user_struct -> matrix_h ;

  ms_grid.resize (size_v, size_h);

  for (int i = 0; i < size_v; i++){
    for (int j = 0; j < size_h; j++){
      ms_grid.attach (square[j][i], j, j + 1, i, i + 1); // square[横][縦]
      square[j][i].get_matrix(i,j);
      square[j][i].set_struct(user_struct);
      square[j][i].set_size_request(30,30);
      square[j][i].hide_connect().connect( sigc::mem_fun( *this, &MainWin::hide_call ) );
      square[j][i].open_connect().connect( sigc::mem_fun( *this, &MainWin::answer_on_clicked ) );
      square[j][i].around_connect().connect( sigc::mem_fun( *this, &MainWin::set_around_state ) );
      square[j][i].flag_connect().connect( sigc::mem_fun( *this, &MainWin::get_flag_state ) );
    }
  }

  random( (unsigned long)time(NULL) );
  while(mines_number > 0){
    if(square[random(0) % size_h][random(0) % size_v].set_mine()){
      mines_number--;
    }
  }

  m_pixbuf = Gdk::Pixbuf::create_from_file( "/home/yohei516/software/mines/Icon/mine.png" );
  e_pixbuf = Gdk::Pixbuf::create_from_file( "/home/yohei516/software/mines/Icon/explosion.png" );

  for(int img_num = 0; img_num < SIZE; img_num++){
    mine_img[img_num].set( m_pixbuf->scale_simple( m_pixbuf->get_width()/5,m_pixbuf->get_height()/5, Gdk::INTERP_NEAREST ) );
    explosion_img[img_num].set( e_pixbuf->scale_simple( e_pixbuf->get_width()/5,e_pixbuf->get_height()/5, Gdk::INTERP_NEAREST ) );
  }

  chk_bt.set_label("Flag");
  chk_bt.signal_clicked().connect(sigc::mem_fun( *this, &MainWin::check_bt_on_clicked ) );

  quit_bt.signal_clicked ().connect (sigc::mem_fun (*this, &MainWin::on_clicked));
  reset_bt.signal_clicked ().connect (sigc::mem_fun (*this, &MainWin::hide_call));
  answer_bt.signal_clicked ().connect (sigc::mem_fun (*this, &MainWin::answer_on_clicked));

  hbox.set_size_request(-1,30);

  Glib::signal_timeout().connect( sigc::mem_fun( *this, &MainWin::set_vision ), 600 );

  
  Gtk::Menu::MenuList& menulist = preference.items();
  menulist.push_back(Gtk::Menu_Helpers::MenuElem( "_Setting",sigc::mem_fun(*this, &MainWin::setting_win) ) );


  mw_MenuBar.items().push_back(Gtk::Menu_Helpers::MenuElem( "_Preference", preference) );
  board.pack_start(mw_MenuBar,Gtk::PACK_SHRINK);

  if(time_try_flag){
    m_progbar.set_size_request(-1,15);
    Glib::signal_timeout().connect( sigc::mem_fun( *this, &MainWin::on_timeout ), 1000 );
    board.pack_start(m_progbar,Gtk::PACK_SHRINK);
  }

  board.pack_start(ms_grid);
  hbox.pack_start(chk_bt,Gtk::PACK_SHRINK);
  hbox.pack_start(reset_bt);
  hbox.pack_start(answer_bt);
  hbox.pack_start(quit_bt);
  board.pack_start(hbox, Gtk::PACK_SHRINK);

  add (board);

  show_all_children ();
}

void MainWin::on_clicked (){
  user_struct -> continue_flag = false;
  hide ();
};

unsigned long MainWin::random(unsigned long seed){

  static unsigned long random_value;
    
  if ( seed ){
    random_value = seed;
  }
  random_value *= 1234567;
  random_value *= rand();
  random_value += 1397;
  return random_value;

}

void MainWin::hide_call( ){
  user_struct -> window_state_flag = true;
  hide();
}

int MainWin::set_around_state(int matrix_x,int matrix_y){
  if(!square[matrix_y][matrix_x].get_open_flag())
    open_counter++;

  int val = around_mine_counter(matrix_x,matrix_y);
  if(open_counter == size_v * size_h - user_struct -> mine_num){
    stringstream ss;
    ss << val;
    Glib::ustring m_num = ss.str();
    square[matrix_y][matrix_x].set_label(m_num);
    ContinueDialog dialog(true);
    if(!dialog.get_continue())
      user_struct -> continue_flag = false;
    hide_call();
  }

  return val;
}

void MainWin::answer_on_clicked(){
  if(!answer_click_flag)
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainWin::switch_icon), 800);
  answer_click_flag = true;
  int pre_num = 0;
  for(int x = 0; x < size_v; x++){
    for(int y = 0; y < size_h; y++){
      if(square[y][x].get_mine_flag()){
        square[y][x].set_image(mine_img[pre_num]);
        square[y][x].open_button();
        pre_num++;
      }
      else{
        int val = around_mine_counter(x,y);
        stringstream ss;
        ss << val;
        Glib::ustring m_num = ss.str();
        square[y][x].set_label(m_num);
        square[y][x].open_button();
      }
    }
  }
}

int MainWin::around_mine_counter(int matrix_h,int matrix_v){
  int val = 0;
   for(int x = -1; x < 2; x++){
    if(matrix_h + x < 0) continue;
    for(int y = -1; y < 2; y++){
      if(matrix_v + y < 0) continue;
      if(square[matrix_v + y][matrix_h + x].get_mine_flag())
        val++;
    }
  }
   return val;
}

bool MainWin::switch_icon(){
  int num = 0;
  for(int x = 0; x < size_v; x++){
    for(int y = 0; y < size_h; y++){
      if(square[y][x].get_mine_flag()){
        if(switch_flag){ square[y][x].set_image(mine_img[num]); }
        else{ square[y][x].set_image(explosion_img[num]); }
        num++;
      }
    }
  }
  if(switch_flag){ switch_flag = false; }
  else{switch_flag = true; }
  return true;
}

void MainWin::check_bt_on_clicked(){
  if(flag_state){flag_state = false; }
  else{flag_state = true; }
}

bool MainWin::on_timeout(){
  score_time++;
  limit += 0.1;
  if(limit >= 1.00)
    limit = 1.0;

  if(limit == 1.0){
    answer_on_clicked();
    ContinueDialog dialog(false);
    user_struct -> continue_flag = dialog.get_continue();
    hide_call();
  }

  m_progbar.set_fraction( limit );
}

void MainWin::setting_win(){
  
}

int i = 60000;
int x = 1000;
int ig = 60000;
int xg = 800;
int ib = 60000;
int xb = 500;
bool MainWin::set_vision(){
  i = i + x;
  ig = ig + xg;
  ib = ib + xb;
  if(i > 65535){
    x = -1000;
    i = 65535;
  }
  if(i < 50000){
    x = 500;
    i = 50000;
  }
  if(ig > 65535){
    xg = -800;
    ig = 65535;
  }
  if(ig < 50000){
    xg = 800;
    ig = 50000;
  }
  if(ib > 65535){
    xb = -500;
    ib = 65535;
  }
  if(ib < 50000){
    xb = 500;
    ib = 50000;
  }

  col.set_rgb(i,ig,ib);
  modify_bg( Gtk::STATE_NORMAL, col );
}
