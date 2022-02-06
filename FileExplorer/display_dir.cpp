#include "headers.h"

vector<properties>data;
int start=0,endrec=0,len=0;


string Human_read(size_t sizee)
{
    vector<string>A={"B","KB","MB", "GB", "TB", "PB"};
    string x;
    int i=0,res;
    double f=sizee;
    while(f>1024)
    {
        f/=1024;
        i++;
    }
    
    res=round(f);
    x=to_string(res)+" "+A[i];
    return x;

}

size_t totalS(string path)
{
    unsigned int dirS;
    struct stat sinfo;
    int tot;
    DIR *dir=opendir(path.c_str());
    struct dirent*dt;
    string cps;
    if(dir!=NULL)
    {
        while((dt=readdir(dir))!=NULL)
        {
            if(string(dt->d_name)!="." && string(dt->d_name)!="..")
            {
                    cps=path+"/"+dt->d_name;
                    struct stat stt;
                    if(stat(cps.c_str(),&stt)!=-1)
                    {
                    if(S_ISDIR(stt.st_mode))
                    {
                        dirS+=totalS(cps);
                    }
                    else
                    {
                        dirS+=stt.st_size;
                        
                    }
                    }
            }
        
            
        }
    }
    return dirS;
}

string permissions(mode_t mode)
{
    string perm="-";
    //for file type
    if ( S_ISDIR(mode) )  perm = 'd'; 
    else if ( S_ISCHR(mode) )  perm = 'c';    
    else if ( S_ISBLK(mode) )  perm = 'b';
    //for user
    mode & S_IRUSR? perm+="r" : perm+="-";   
    mode & S_IWUSR? perm+="w" : perm+="-";
    mode & S_IXUSR? perm+="x" : perm+="-";
    //for groups
    mode & S_IRGRP? perm+="r" : perm+="-";
    mode & S_IWGRP? perm+="w" : perm+="-";
    mode & S_IXGRP? perm+="x" : perm+="-";
    //for others
    mode & S_IROTH? perm+="r" : perm+="-";
    mode & S_IWOTH? perm+="w" : perm+="-";
    mode & S_IXOTH? perm+="x" : perm+="-";

    return perm;
}

string owner_name(uid_t uid)
{
    string oname;
    struct passwd *ptr;
    ptr=getpwuid(uid);
    if(ptr==NULL)
    oname=uid;
    else
    oname=ptr->pw_name;
    return oname;
}

string grp_name(gid_t gid)
{
    string gname;
    struct group *ptr;
    ptr=getgrgid(gid);
    if(ptr==NULL)
    gname=gid;
    else
    gname=ptr->gr_name;
    return gname;
}

string last_modified(time_t lmt)
{
    struct tm lmod;
    lmod=*(localtime(&lmt));
    
    string dt=to_string(lmod.tm_mday)+"-";
    dt+=to_string(lmod.tm_mon)+"-";
    dt+=to_string(lmod.tm_year+1900);
    return dt;
}

string last_modified_time(time_t lmt)
{
    struct tm lmod;
    lmod=*(localtime(&lmt));
    string ddt=to_string(lmod.tm_hour)+":";
    ddt+=to_string(lmod.tm_min)+":";
    ddt+=to_string(lmod.tm_sec);
    return ddt;
}


bool comparator(properties& n,properties& m)
{
    return n.dname<m.dname;
}

void display()
{
    clearScreen;
    sort(data.begin(), data.end(), &comparator);
    auto it=data.begin();
    len=data.size();
    endrec=min(len,scr_height);
    for(int y=0; y<endrec;y++)
    {
        if(it==data.end())
        break;
        /*if((*it).dname.size()>20)
        {
            cout<<left<<setw(20)<<(*it).dname.substr(0,18)+"..";
        }
        else
        cout<<left<<setw(20)<<(*it).dname;*/
        cout<<left<<setw(20)<<(*it).dpermissions;
        cout<<left<<setw(10)<<(*it).downer;
        cout<<left<<setw(10)<<(*it).dgroup;
        cout<<left<<setw(10)<<(*it).dsize;
        cout<<left<<setw(15)<<(*it).dmodified;
        cout<<left<<setw(15)<<(*it).dmtime;
        if((*it).dname.size()>20)
        {
            cout<<left<<setw(20)<<(*it).dname.substr(0,18)+".."<<endl;
        }
        else
        cout<<left<<setw(20)<<(*it).dname<<endl;

        it++;
    }
    //printf("%s",presentPath.c_str());
    gotoTop();
}

void dispLK()
{
    clearScreen;
    for(int i=start; i<=start+endrec-1; i++)
    {
        cout<<left<<setw(20)<<data[i].dpermissions;
        cout<<left<<setw(10)<<data[i].downer;
        cout<<left<<setw(10)<<data[i].dgroup;
        cout<<left<<setw(10)<<data[i].dsize;
        cout<<left<<setw(15)<<data[i].dmodified;
        cout<<left<<setw(15)<<data[i].dmtime;
        if(data[i].dname.size()>20)
        {
            cout<<left<<setw(20)<<data[i].dname.substr(0,18)+".."<<endl;
        }
        else
        cout<<left<<setw(20)<<data[i].dname<<endl;
    }

}


void list_dir(string dirname)
{
    data.clear();
    chdir(dirname.c_str());
    start=0,endrec=0,len=0;
    properties obj;
    DIR* directory;
    directory = opendir(dirname.c_str());
    
    struct dirent *D_F;
    if (directory == NULL) {
        return;
    }
    for(D_F=readdir(directory); D_F!=NULL; D_F=readdir(directory))
    {
        string temp=D_F->d_name;
        obj.dname=temp;
        struct stat st;
        if(stat(temp.c_str(),&st)!=0)
        {
            cout<<0<<endl;
        }
        /*if(st.st_mode & S_IFDIR )
        {
            string namee;
            if(temp==".")
            namee=presentPath;
            else if(temp=="..")
            obj.dsize=Human_read(st.st_size);
            else
            namee=presentPath+"/"+temp;
            obj.dsize=Human_read(totalS(namee));
        }
        else*/
            obj.dsize=Human_read(st.st_size);

        obj.dpermissions=permissions(st.st_mode);
        obj.downer=owner_name(st.st_uid);
        obj.dgroup=grp_name(st.st_gid);
        obj.dmodified=last_modified(st.st_mtime);
        obj.dmtime=last_modified_time(st.st_mtime);
        data.push_back(obj);
    }
    display();
    closedir(directory);

}




//what func is what ?
//================================================
//readdir() function returns a pointer to a structure representing the directory 
//entry at the current position in the directory stream specified by the argument 
//dirp, and positions the directory stream at the next entry. It returns a null 
//pointer upon reaching the end of the directory stream.
//==================================================
//The opendir() function shall open a directory stream corresponding to the 
//directory named by the dirname argument. The directory stream is positioned at 
//the first entry. If the type DIR is implemented using a file descriptor, 
//applications shall only be able to open up to a total of {OPEN_MAX} files and 
//directories.
//=================================================

