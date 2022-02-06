#include "headers.h"


int copyfuncFile(string sourcedes,string finaldes)
{
    int count=0;
    char buf[1024];
    struct stat sinfo,dinfo;
	int f1, f2,rw,tot;
    const char*s=sourcedes.c_str();
    const char*d=finaldes.c_str();
	f1 = open(s, O_RDONLY);

	if (f1 == -1 )
	{
		
		close(f1);
		return 0;
	}
	f2= open(d, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); 
	
    if(f2==-1)
    {

        close(f2);
        return 0;
    }
    while((rw = read(f1,buf,sizeof(buf)))>0)
    {
        write(f2,buf,rw);
    }
     if(stat(s,&sinfo)<0)
    {
       
        return 0;
    }
    if(stat(d,&dinfo)<0)
    {
        
        return 0;
    }
   
    tot = chown(d,sinfo.st_uid,sinfo.st_gid);
    if(tot!=0)
    {
        //printf("Error in ownership\t");
        return 0;
    }
    tot = chmod(d,sinfo.st_mode);
    if(tot!=0)
    {
        //printf("Error in permissions\t");
        return 0;
    }
    close(f1);
	close(f2);
    return 1;
}





int copyfuncDir(string sourcedes, string finaldes)
{
    //int count=0;
    struct stat sinfo;
    int tot;
    DIR *dir=opendir(sourcedes.c_str());
    struct dirent*dt;

    string cps,cpd;
    if(dir!=NULL)
    {
        while((dt=readdir(dir))!=NULL)
        {
            if(string(dt->d_name)!="." && string(dt->d_name)!="..")
            {
                    cps=sourcedes+"/"+dt->d_name;
                    cpd=finaldes+"/"+dt->d_name;
                    struct stat st;
                    if(stat(cps.c_str(),&st)==-1)
                    {
                        return 0;
                    }
                    if(S_ISDIR(st.st_mode))
                    {
                        mkdir(cpd.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                        copyfuncDir(cps,cpd);
                    }
                    else
                    {
                        copyfuncFile(cps,cpd);
                        
                    }
            }
        
            
        }
    if(stat(sourcedes.c_str(),&sinfo)<0)
    {
        //printf("Error opening file\t");
        return 0;
    }
    chown(finaldes.c_str(),sinfo.st_uid,sinfo.st_gid);
    chmod(finaldes.c_str(),sinfo.st_mode);
    closedir(dir);
    return 1;
    }
    else
    {
        //printf("Error");
        return 0;
    }
}




int copyfunc(vector<string>&checkCommand)
{
    int count=0;
    string fname,srcPath,destPath;
    int len=int(checkCommand.size());
    if(len<3)
    {
        //printf("incomplete command\t");
        return 0;
    }
    string destDir=Abspath(checkCommand[len-1]);
    for(int i=1; i<len-1; i++)
    {
        fname=checkCommand[i];
        srcPath=presentPath+"/"+fname;
        destPath=destDir+"/"+fname;
        struct stat st;
        if(stat(srcPath.c_str(),&st)==0)
        {
            if(st.st_mode & S_IFDIR)
            {
                mkdir(destPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                count+=copyfuncDir(srcPath, destPath);
                
            }
            else
            {
                count+=copyfuncFile(srcPath, destPath);
            }
        }
        /*else
        {
            //printf("File does not exist\t");
            return 0;
        }*/
    }
    return count;
}