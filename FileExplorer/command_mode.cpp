#include "headers.h"
#include "copy.cpp"
#include "createF_D.cpp"
#include "rename.cpp"
#include "searchF_D.cpp"
#include "deleteF_D.cpp"

vector<string>checkCommand;
vector<string>logs;
string opstr="";


string Abspath(string str)
{
    string p;
    if(str[0]=='~' && str[1]=='/') 
    {
        p=MyHome;
        return p+str.substr(1,str.length()-1);
    }
    else if(str[0]=='.' && str[1]=='/')
    {
        p=presentPath;
        return p+str.substr(1,str.length()-1);
    }
    else if(str[0]=='~')
    {
        return MyHome;
    }
    else if(str[0]=='.')
    {
        return presentPath;
    }
    else
    {
        return str;
    }
}

void toPrint()
{
    //clearScreen;
    gotoBottom();
    printf("\x1B[J");
}

int enableCommandMode()
{
    char c;
    string user_command;
    int flag=0;
    //clearScreen;
    gotoBottom();
while(1)
{
    //clearScreen;
    //gotoBottom();
    c='\0';
    user_command="";
    while(1)
    {
        fflush(stdin);
        c=cin.get();
        if(c==10 || c==27 )
        {
            break;
        }
        if(c=='q')
        {
            if(user_command.empty())
            {
                clearScreen;
                exit(0);
            }
            /*else
            {
                user_command+=c;
                printf("%c",c);
            }*/
        }
        if(c=='l')
        {
            if(user_command.empty())
            {
                clearScreen;
                gotoBottom();
                for(auto it=logs.begin(); it!=logs.end(); it++)
                {
                    cout<<*it<<endl;
                }
                sleep(4);
            
                logs.clear();
                return 0;
            }
            /*else
            {
                user_command+=c;
                printf("%c",c);
            }*/
        }
        if(c==127)
        {
            if(user_command.length()>0)
            {
                //curr_y--;
                user_command.pop_back();
                printf("\b");
                printf("\033[J");
            }
        }
        else
        {
            user_command+=c;
            printf("%c",c);
        }
    }
    if(c==10)
    {
        if(user_command.empty())
        {
            toPrint();
            //printf("\n");
            continue;
        }
        string word;
        logs.push_back(user_command);
        stringstream str(user_command);
        while(str>>word)
        {
            checkCommand.push_back(word);
        }
        //toPrint();
        if(checkCommand[0]=="copy")
        {
            int a=copyfunc(checkCommand);
            if(a==0)
            opstr="Operation Unsucessful";
            else
            {
                opstr=to_string(a)+" files copied";
            }
        }
        else if(checkCommand[0]=="move")
        {
            int a=moveFile(checkCommand);
            if(a==0)
            opstr="Operation Unsucessful";
            else
            opstr=to_string(a)+" files moved";
        }
        else if(checkCommand[0]=="rename")
        {
            if(renameFile(checkCommand))
            opstr="File renamed";
            else
            opstr="Operation unsuccessful";
            
        }
        else if(checkCommand[0]=="create_file")
        {
            int a=createfile(checkCommand);
            if(a==0)
            opstr="Operation Unsuccessful";
            else
            opstr=to_string(a)+" files created";
        }
        else if(checkCommand[0]=="create_dir")
        {
            int a=createdir(checkCommand);
            if(a==0)
            opstr="Operation Unsuccessful";
            else
            opstr=to_string(a)+" directories created";
        }
        else if(checkCommand[0]=="delete_file")
        {
            if(checkCommand.size()!=2)
            {
                opstr="Invalid number of args";
            }
            else
            {
                string abp=Abspath(checkCommand[1]);
                if(deleteF(abp))
                opstr="File deleted";
                else
                opstr="Operation unsuccessful";
            }
        }
        else if(checkCommand[0]=="delete_dir")
        {
            if(checkCommand.size()!=2)
            {
                opstr="Invalid number of args";
            }
            else
            {
                string abp=Abspath(checkCommand[1]);
                if(deleteD(abp))
                opstr="Directory deleted";
                else
                opstr="Operation unsuccessful";
            }
        }
        else if(checkCommand[0]=="goto")
        {
            if(checkCommand.size()!=2)
            opstr="Invalid number of args";
            else
            {
                string dirpath=Abspath(checkCommand[1]);
                struct stat st;
                if(stat(dirpath.c_str(),&st)==0)
                {
                    if(st.st_mode & S_IFDIR)
                    {
                        back_VisitedDir.push(presentPath);
                        presentPath=dirpath;
                        chdir(dirpath.c_str());
                        opstr="Directory changed";
                    }
                }
                else
                opstr="Wrong path";
            }
        }

        else if(checkCommand[0]=="search")
        {
            if(checkCommand.size()!=2)
            opstr="False";
            else
            {
                if(searching(checkCommand[1],presentPath))
                opstr="True";
                else
                opstr="False";
            }
        }
        else
        {
            opstr="Error: invalid command !";
        }
    
        list_dir(presentPath);
        toPrint();
        printf("%s",opstr.c_str());
        //printf("\033[1J");
        //list_dir(presentPath);
        opstr="";
        checkCommand.clear();
    }
    if(c==27)
    {
        return 0;
    }
}
    return 0;
}