#include "headers.h"

int createfile(vector<string>&checkCommand)
{
    string filepath,despath;
    int cf,count=0;
    mode_t mode=O_RDONLY | O_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int len=int(checkCommand.size());
    if(len<3)
    {
        //op="Incomplete command";
        return 0;
    }
    despath=Abspath(checkCommand[len-1]);
    //chdir(despath.c_str());
    if(len==3)
    {
        filepath=despath+"/"+checkCommand[1];
        cf=creat(filepath.c_str(),mode);
        if(cf==-1)
        {
            //op="File can't be created";
            return 0;
        }
        else
        {
            //op="File created";
            return 1;
        }
    }
    else
    {
        
        for(int i=1; i<len-1; i++)
        {
            string fname=checkCommand[i];
            filepath=despath+"/"+fname;
            cf=creat(filepath.c_str(),mode);
            if(cf=!-1)
            {
                //op+="File created\t";
                count++;
            }
        }
    }
    return count;
    //chdir(presentPath.c_str());
}

int createdir(vector<string>&checkCommand)
{
    string dirpath,despath;
    int cf,count=0;
    mode_t mode=S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
    int len=int(checkCommand.size());
    if(len<3)
    {
        //op="Incomplete command";
        return 0;
    }
    despath=Abspath(checkCommand[len-1]);
    //chdir(despath.c_str());
    if(len==3)
    {
        dirpath=despath+"/"+checkCommand[1];
        cf=mkdir(dirpath.c_str(),mode);
        if(cf==-1)
        {
            //op="Directory can't be created";
            return 0;
        }
        else
        {
            //op="Directoy created";
            return 1;
        }
    }
    else
    {
        for(int i=1; i<len-1; i++)
        {
            string dname=checkCommand[i];
            dirpath=despath+"/"+dname;
            cf=mkdir(dirpath.c_str(),mode);
            if(cf=!-1)
            {
                //op+="Directory created\t";
                count++;
            }
        }
    }
    //chdir(presentPath.c_str());
    return count;
}