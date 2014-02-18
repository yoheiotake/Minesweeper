#include "mines.h"

ContinueDialog::ContinueDialog(bool flag)
{
  continue_flag = false;
  restart_flag = false;
  Gtk::MessageDialog diag( *this, "You are Lose!! \nTo be Continue?", false,Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
  diag.set_title( "To be Continue?" );
  if(flag)
    diag.set_message("You are Win!! \nTo be Continue?",false);

  switch( diag.run() ){

  case Gtk::RESPONSE_OK:
    continue_flag = true;
    break;

  case Gtk::RESPONSE_CANCEL:
    hide();
    break;
  }
}

bool ContinueDialog::get_continue(){
  return continue_flag;
}
