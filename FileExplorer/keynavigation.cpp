#include "headers.h"


//=================global===============//

int curr_x=1, curr_y=1;
int scr_height;
struct termios original;

//=================================Raw Mode and Terminal Size============================//

void kill(const char *s) {
  clearScreen;
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original) == -1)
    kill("tcsetattr");
}

int TerminalS(int *rows) 
{
  struct winsize ts;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ts) == -1 || ts.ws_col == 0)
  {
    return -1;
  } 
  else 
  {
    *rows = ts.ws_row-1; //sets the height of the terminal
    return 0;
  }
}

void initTerminalS() 
{
  if (TerminalS(&scr_height) == -1) 
  kill("TerminalS");
}


void enableRawMode() {
  
  signal(SIGWINCH, sig_handler);
  atexit(disableRawMode);
  struct termios temp = original;
  temp.c_lflag &= ~(ECHO | ICANON );
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &temp);
  gotoxy(curr_x,curr_y);
}

//======================================Key navigation===============================//

void ArrowKeys(char key) {
  switch (key)
   {

    case ARROW_UP: 
    {
      if(curr_x==1)
      break;
      curr_x--;
      gotoxy(curr_x,curr_y);
      break;
    }

    case ARROW_DOWN: 
    {
      if(curr_x==endrec)
      break;
      curr_x++;
      gotoxy(curr_x,curr_y);
      break;

    }
    
    case ARROW_LEFT:
    {
      if(back_VisitedDir.empty())
      break;
      fwd_VisitedDir.push(presentPath);
      presentPath=back_VisitedDir.top();
      back_VisitedDir.pop();
      //chdir(presentPath.c_str());
      list_dir(presentPath);
      break;
    }

    case ARROW_RIGHT:
    {
      if(fwd_VisitedDir.empty())
      break;
      back_VisitedDir.push(presentPath);
      presentPath=fwd_VisitedDir.top();
      fwd_VisitedDir.pop();
      list_dir(presentPath);
      break;
    }
  }
}



char ReadKey() 
{
  char c;
  fflush(stdin);
  c=cin.get();
  if(c==0x1b)
  {
    char seq[2];
    seq[0]=cin.get();
    seq[1]=cin.get();
    if (seq[0]!='[') 
    return 0x1b;
    if (seq[0] == '[') 
    {
      switch (seq[1]) 
      {
        case 'A': 
        return ARROW_UP;
        case 'B': 
        return ARROW_DOWN;
        case 'C': 
        return ARROW_RIGHT;
        case 'D': 
        return ARROW_LEFT;
      }
    }
    return 0x1b;
  } 
  else
  {
    return c;
  }
}


void ProcessKeypress() 
{
  char c = ReadKey();
  switch (c) 
  {
    case 'q':
      clearScreen;
      exit(0);
      break;

    case 0x1b:
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      ArrowKeys(c);
      break;

    case 10:
    {
    string pointed_name;
    if(presentPath=="/home/..")
    {
      pointed_name="/";
    }
    else
    pointed_name=presentPath+"/";
    string sel=data[curr_x-1+start].dname;
    //printf("%s",pointed_name.c_str());
    pointed_name+=sel;
    struct stat st;
    if(stat(pointed_name.c_str(),&st)==0)
    {
      if(st.st_mode & S_IFDIR)
      {
        if(sel==".")
          break;
      
        back_VisitedDir.push(presentPath);
        if(sel=="..")
        {
          if(presentPath=="/home/..")
          presentPath="";
          presentPath=presentPath.substr(0, presentPath.find_last_of("/\\"));
    
        }
        else
        {
          presentPath=pointed_name;
        }
        list_dir(pointed_name);
      }
      else if(st.st_mode & S_IFREG)
      {
        pid_t f=fork();
        if(f==-1)
        perror("fork");
        else if(f==0)
        {
          char c[300];
          strcpy(c,pointed_name.c_str());
          string v="vi";
          char*exn=(char*)v.c_str();
          char*exargs[]={exn, c, NULL};
          execv("/usr/bin/vi",exargs);
        }
        else
        {
          wait(0);
          //presentPath=presentPath.substr(0, presentPath.find_last_of("/\\"));
          RefreshScreen();
        }
      }
    }
    
    break;
    }

    case 127:
    {
      if(presentPath=="/home/..")
      break;
      back_VisitedDir.push(presentPath);
      presentPath=presentPath.substr(0, presentPath.find_last_of("/\\"));
      if(presentPath.empty())
      {
        presentPath="/home/..";
      }
      list_dir(presentPath);
      break;
    }

    case 'H':
    case 'h':
    {
      while(!fwd_VisitedDir.empty())
      fwd_VisitedDir.pop();
      while(!back_VisitedDir.empty())
      back_VisitedDir.pop(); //==================reset left and right on pressing home
      presentPath=MyHome;
      list_dir(presentPath);
      break;
    }

    case ':':
    {
      enableCommandMode();
      RefreshScreen();
    }

    case 'l':
    case 'L':
    { 
      if(curr_x==endrec && len>start+endrec)
      {
        start+=1;
        dispLK();
        gotoxy(scr_height,1);
      }
      break;
    }

    case 'k':
    case 'K':
    {
      if(curr_x==1 && start-1>=0)
      {
        start-=1;
        dispLK();
        gotoTop();
      }
      break;
    }
  }
}

void RefreshScreen() {
  clearScreen;
  disableRawMode();
  list_dir(presentPath);
  enableRawMode();
}

void NormalMode()
{
    tcgetattr(STDIN_FILENO, &original);
    enableRawMode();
    curr_x=1;
    curr_y=1;
    gotoxy(curr_x,curr_y);
    while (1) 
    {
      //RefreshScreen();
      ProcessKeypress();
    }
}
















