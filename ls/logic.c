#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include"function.h"
#include<stdlib.h>
struct dirent** list;
void mode_chooser(int argc,char** argv)
{
	int i=1;
	if(argc==1)
	{
		ls(".");
	}
	else
	{
		if(strcmp(argv[1],"-l")==0)
		{
			if(argc==2)
			{
				ls_l(".");
			}
			else
			{
				while(i<argc)
				{
					ls_l(argv[i]);
					i++;
				}
			}
		}
		else if(strcmp(argv[1],"-R")==0)
		{
			if(argc==2)
			{
				ls_R(".");
			}
			else
			{
				while(i<argc)
				{
					ls_R(argv[i]);
					i++;
				}
			}
		}
		else if(strcmp(argv[1],"-d")==0)
		{
			if(argc==2)
			{
				ls_d(".");
			}
			else
			{
				while(i<argc)
				{
					ls_d(argv[i]);
					i++;
				}
			}
		}
		else if(strcmp(argv[1],"-a")==0)
		{
			if(argc==2)
			{
				ls_a(".");
			}
			else
			{
				while(argc>2)
				{
					ls_a(argv[argc-1]);
					argc--;
				}
			}
		}
		else
		{
			while(i<argc)
			{
				if(argc>2)
				{
					printf("%s:\n",argv[i]);
				}
				ls(argv[i]);
				i++;
			}
		}
	}
}
void ls(char* path)
{
    	struct dirent **myfile;
	int n=scandir(path,&myfile,0,alphasort);
	int i=0;
	while(n--)
    	{
        	if(myfile[i]->d_name[0]!='.')
		{
        		printf("%s\t", myfile[i]->d_name);
		}
		//printf("\n");
		i++;
    	}
}
void ls_l(char* path)
{
//	DIR *d;
	struct dirent **de;
	struct stat* buf;
	int i,j=0;
	char P[10]="rwxrwxrwx",AP[10]=" ";
	struct passwd *p;
	struct group *g;
	struct tm *t;
	char time[26];
	int n=scandir(path,&de,NULL,alphasort);
	buf=(struct stat*)malloc(sizeof(struct stat)*n);
//	d=opendir(path);
//	readdir(d);
//	readdir(d);
	int k=0;
	while(k<n)
	{
		stat(de[k]->d_name,&buf[k]);

		// File Type
		if(S_ISDIR(buf[k].st_mode))
		printf("d");
		else if(S_ISREG(buf[k].st_mode))
		printf("-");
		else if(S_ISCHR(buf[k].st_mode))
		printf("c");
		else if(S_ISBLK(buf[k].st_mode))
		printf("b");
		else if(S_ISLNK(buf[k].st_mode))
		printf("l");
		else if(S_ISFIFO(buf[k].st_mode))
		printf("p");
		else if(S_ISSOCK(buf[k].st_mode))
		printf("s");
		//File Permissions P-Full Permissions AP-Actual Permissions
		for(i=0,j=(1<<8);i<9;i++,j>>=1)
		AP[i]= (buf[k].st_mode & j ) ? P[i] : '-' ;
		printf("%s",AP);
		//No. of Hard Links
		printf("%5d",buf[k].st_nlink);
		//User Name
		p=getpwuid(buf[k].st_uid);
		printf(" %.8s",p->pw_name);
		//Group Name
		g=getgrgid(buf[k].st_gid);
		printf(" %-8.8s",g->gr_name);
		//File Size
		printf(" %8d",buf[k].st_size);
		//Date and Time of modification
		t=localtime(&buf[k].st_mtime);
		strftime(time,sizeof(time),"%b %d %H:%M",t);
		printf(" %s",time);
		//File Name
		printf(" %s\n",de[k]->d_name);
		k++;
	}
}
void ls_R(char* name)
{
	 DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printf("%s:\n", entry->d_name);
            ls_R(path);
        }
        else
            printf("%s  ",  entry->d_name);
    } while (entry = readdir(dir));
    closedir(dir);
}
void ls_d(char* path)
{
	struct dirent** list;
	int n=scandir(path,&list,NULL,alphasort);
	struct stat* buf;
	buf=(struct stat*)malloc(sizeof(struct stat)*n);
	int i=0;
	while(n--)
		{
			if(S_ISDIR(buf[i].st_mode))
				printf("%s\n",list[i]->d_name);
			i++;
		}
}
void ls_a(char* path)
{
    	struct dirent **myfile;
	int n=scandir(path,&myfile,0,alphasort);
	int i=0;
	while(n--)
    	{
        	
        	printf("%s\t", myfile[i]->d_name);
		
		//printf("\n");
		i++;
    	}
	printf("LS_a");
}
	
