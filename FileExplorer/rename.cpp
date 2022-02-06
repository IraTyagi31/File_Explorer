#include "headers.h"

int renameFile(vector<string>&checkCommand)
{
    string op;
    int len=checkCommand.size();
    if(len!=3)
    {
        //op="Invalid format";
        return 0;
    }
    const char*prev=checkCommand[1].c_str();
    const char*nnew=checkCommand[2].c_str();

	if (rename(prev, nnew) != 0)
        return 0;
		//op="No such file or directory";
    else
        return 1;
        //op="File renamed";
	//return op;
}

int moveFile(vector<string>&checkCommand)
{
    int count=0;
    int len=checkCommand.size();
    if(len<3)
    {
        //op="Invalid command";
        return 0;
    }
    string destination=Abspath(checkCommand[len-1]);
    for(int i=1; i<len-1; i++)
    {
        string tomove=presentPath+"/"+checkCommand[i];
        const char*prev=tomove.c_str();
        string dest=destination+"/"+checkCommand[i];
        const char*nnew=dest.c_str();
        if(rename(prev,nnew)==0)
        {
            count++;
        }
    }
    return count;
}