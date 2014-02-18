#include "mines.h"

MyButton::MyButton():Gtk::Button()
{
  f_pixbuf = Gdk::Pixbuf::create_from_file( "/home/yohei516/software/mines/Icon/flag.png" );
  flag_img.set( f_pixbuf->scale_simple( f_pixbuf->get_width()/3,f_pixbuf->get_height()/3, Gdk::INTERP_NEAREST ) );
  flag_state = false;
  mine_flag = false;
  open_flag = false;
}

void MyButton::on_clicked(){
  if(open_flag)
    return;

  if(flag_call_signal.emit()){
    if(flag_state){
      remove();
      flag_state = false;
    }
    else{
      set_image(flag_img);
      flag_state = true;
    }
  }
  else if(mine_flag){
    if(flag_state)
      return;
    open_call_signal.emit();
    ContinueDialog dialog(false);
    if(!dialog.get_continue()){
      user_struct -> continue_flag = false;
    }
    hide_call_signal.emit( );
    
  }
  else{
    if(flag_state)
      return;
    int tmp = around_state_signal.emit( matrix_x,matrix_y );
    stringstream ss;
    ss << tmp;
    Glib::ustring m_num;
    m_num = ss.str();
    set_label(m_num);
    open_flag = true;
  }
}


bool MyButton::set_mine(){
  if(mine_flag == false){
    mine_flag = true;
    return true;
  }
  else
    return false;
}

void MyButton::set_struct(UserOption *p){
  user_struct = p;
}

void MyButton::get_matrix(int x,int y){
  matrix_x = x;
  matrix_y = y;
}

void MyButton::open_button(){
  open_flag = true;
}
