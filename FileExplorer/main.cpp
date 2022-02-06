#include "headers.h"
#include "display_dir.cpp"
#include "keynavigation.cpp"
#include "command_mode.cpp"

stack<string>back_VisitedDir;
stack<string>fwd_VisitedDir;
string MyHome,presentPath;

//=========================for resizing===========================//

void sig_handler(int sig)
{
    if(SIGWINCH==sig)
    {
        initTerminalS();
        RefreshScreen();
        fflush(stdout);
    }

}

//=============================for cursor pos=======================//

void gotoxy(int x, int y)    
{
    printf("%c[%d;%df",0x1B,x,y);
}

void gotoTop()
{
    curr_x=1;
    curr_y=1;
    printf("%c[%d;%df",0x1B,1,1);
}

void gotoBottom()
{
    initTerminalS();
    curr_x=scr_height;
    curr_y=1;
    printf("%c[%d;%df",0x1B,curr_x+1,curr_y);
    printf(":");
    //curr_y++;
}

string CurrentDirName()
{
    char a[256];
    getcwd(a, 256);
    string D=a;
    return D;
}

//==============================MAIN=======================//

int main()
{
    printf("\0337\033[?47h");
    initTerminalS();
    MyHome=CurrentDirName();
    presentPath=MyHome;
    list_dir(MyHome);
    NormalMode();
    
    return 0;
}