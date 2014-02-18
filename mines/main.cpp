#include "mines.h"

int main (int argc, char *argv[])
{
  UserOption mines_option;

  Gtk::Main kit (argc, argv);
  SetUpWin setup(& mines_option);
  Gtk::Main::run (setup);

  while(mines_option.continue_flag){
    MainWin mainwin(& mines_option);
    Gtk::Main::run (mainwin);
    if(!mines_option.window_state_flag)
      break;
  }
  
  return 0;
}
