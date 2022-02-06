#include "headers.h"

int searching(string cc,string curdir)
{
    string tosend;
    DIR*d;
    if((d=opendir(curdir.c_str()))!=NULL)
    {
        struct dirent*dt;
        for((dt=readdir(d)); dt!=NULL; dt=readdir(d))
        {
            
            if(string(dt->d_name)==cc)
            {
                //printf("True\n");
                return 1;
            }
            if(dt->d_type==DT_DIR && string(dt->d_name)!="." && string(dt->d_name)!="..")
            {
                tosend=curdir+"/"+string(dt->d_name);
                int abc=searching(cc,tosend);
                if(abc==1)
                return 1;
            }
        }
        closedir(d);
    }
    //printf("False\n");
    return 0;
}