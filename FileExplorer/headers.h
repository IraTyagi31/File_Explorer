#include <bits/stdc++.h>
#include<unistd.h>
#include<signal.h>
#include<termios.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdio.h>
#include<iostream>
#include<errno.h>
#include<sys/ioctl.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<cmath>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<filesystem> //to get curr directory name

//=========defines==========//

#define clearScreen printf("\033[H\033[2J\033[3J")
//#define gotoTop printf("%c[%d;%df",0x1B,1,1)
#define ARROW_UP 'A'
#define ARROW_DOWN 'B'
#define ARROW_LEFT 'D'
#define ARROW_RIGHT 'C'
 
using namespace std;


//=========class=========//
#ifndef PROPERTIES_H
#define PROPERTIES_H

class properties
{
    public:
    string dname;
    string dsize;
    string dpermissions;
    string downer;
    string dgroup;
    string dmodified;
    string dmtime;
};
#endif
//=========DS and var============//
extern stack<string>back_VisitedDir;
extern stack<string>fwd_VisitedDir;
extern string presentPath;
extern string MyHome;
extern int curr_x;
extern int curr_y;
extern int scr_height;
extern int len;
extern int start;
extern int endrec;
//extern string opstr;
extern vector<properties>data;

//=========functions=======//
void gotoxy(int,int);
void list_dir(string);
void sig_handler(int);
void enableRawMode();
int enableCommandMode();
void gotoTop();
void gotoBottom();
void RefreshScreen();
void initTerminalS();
int TerminalS(int);
string Abspath(string);
void dispLK();
//void copyfunc(vector<string>);
//void copyfuncFile(string,string);
//void copyfuncDir(string,string);
