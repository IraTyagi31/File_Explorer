#include "headers.h"

int deleteF(string abp)
{
    const char*p=abp.c_str();
    int delf=remove(p);
    if(delf==-1)
    {
        //printf("Deletion failed\t");
        return 0;
    }
    else
    {
        //printf("Deletion successful\t");
        return 1;
    }
}

int deleteD(string abp)
{
    struct stat sinfo;
    int tot,ret;
    DIR *dir=opendir(abp.c_str());
    struct dirent*dt;
    string cps,cpd;
    if(dir!=NULL)
    {
        while((dt=readdir(dir))!=NULL)
        {
            if(string(dt->d_name)!="." && string(dt->d_name)!="..")
            {
                    cps=abp+"/"+dt->d_name;
                    
                    struct stat st;
                    if(stat(cps.c_str(),&st)==-1)
                    {
                        return 0;
                    }
                    if(S_ISDIR(st.st_mode))
                    {

                        ret=deleteD(cps);
                        if(ret==0)
                        return 0;
                    }
                    else
                    {
                        ret=deleteF(cps);
                        if(ret==0)
                        return 0;
                    }
            }
        
            
        }
        closedir(dir);
        int delD= rmdir(abp.c_str());
        if (delD == -1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
        
}