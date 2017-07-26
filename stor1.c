#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include"header.h"
int main(int argc, char **argv)
{
	struct super
	{
		int free_inode;
		int free_sp;
	}s_block[1];

	static struct fil
	{
		char filename[10];
		int size;
		int off_set;
		short int per;
		int a_time;
		int m_time;
		int c_time;
		int u_id;
		int g_id;
		int block[3000];               
	}inode[3];

	struct stat s;
	
	int i,j,k,fd1,fd2,r_val,fd3,m,n,cal,u_space;
	char data[300];
	bzero(data,300);
	j=2; // skip ./a.out(0) and vfs(1) string 

	fd1=open(argv[1],O_WRONLY);  // open vfs file 

	// for loop to fillup inode structure 
	for(i=0;i<3;i++){      
		if(j<argc){
			inode[i].per=0;
			fd2=open(argv[j],O_RDONLY);  // open sample1.c , sample2.c sample3.c on loop iterations( next by next)
			bzero(inode[i].filename,4);
			strcpy(inode[i].filename,argv[j]);
			printf("Name of File %s\n ",inode[i].filename);
			
			r_val=lseek(fd2,0,SEEK_END); // get file size 
			inode[i].size=r_val;
			printf("Size of File   %d\n",inode[i].size);
			
			// now find the file offset		
			if(j==2){
				inode[i].off_set=sizeof(inode)+sizeof(s_block); // first fileoffet will start after the sizeof no. of inode plus superblock size 
			}
			else{
				inode[i].off_set=inode[i-1].off_set+inode[i-1].size; // file offset from second onwards
			}
			printf(" Offset of File  %d\n",inode[i].off_set);

			
			m = stat(argv[j] , &s);
			if(m == -1){
				perror("open");
				exit(1);
			}

			n=s.st_mode & S_IRWXU;
			printf("%d  %lo ", n , n);

			if(n==RWXU)

				inode[i].per=inode[i].per|RWXU;

			if(n==RUSR)

				inode[i].per=inode[i].per|RUSR;

			if(n==WUSR)

				inode[i].per=inode[i].per|WUSR;

			if(n==XUSR)

				inode[i].per=inode[i].per|XUSR;

			if(n==RWUSR)

				inode[i].per=inode[i].per|RWUSR;

			if(n==WXUSR)

				inode[i].per=inode[i].per|WXUSR;

			if(n==RXUSR)

				inode[i].per=inode[i].per|RXUSR;

			n=s.st_mode & S_IRWXG;
printf("\n  user  %d  %lo \n  ", inode[i].per , inode[i].per);


			if(n==RWXG)

				inode[i].per=inode[i].per|RWXG;

			if(n==RGRP)

				inode[i].per=inode[i].per|RGRP;

			if(n==WGRP)

				inode[i].per=inode[i].per|WGRP;

			if(n==XGRP)

				inode[i].per=inode[i].per|XGRP;

			if(n==RWGRP)

				inode[i].per=inode[i].per|RWGRP;

			if(n==WXGRP)

				inode[i].per=inode[i].per|WXGRP;

			if(n==RXGRP)

				inode[i].per=inode[i].per|RXGRP;

			n=s.st_mode & S_IRWXO;
printf("\n user + group %d  %lo \n  ", inode[i].per , inode[i].per);
			if(n==RWXO)

				inode[i].per=inode[i].per|RWXO;

			if(n==ROTH)

				inode[i].per=inode[i].per|ROTH;

			if(n==WOTH)

				inode[i].per=inode[i].per|WOTH;

			if(n==XOTH)

				inode[i].per=inode[i].per|XOTH;

			if(n==RWOTH)

				inode[i].per=inode[i].per|RWOTH;

			if(n==WXOTH)

				inode[i].per=inode[i].per|WXOTH;

			if(n==RXOTH)

				inode[i].per=inode[i].per|RXOTH;

printf("finaly.. inode permission is  ugo %d   %lo :" , inode[i].per , inode[i].per);	
			n=(s.st_mode & S_IFMT);

			if(n==FDIR){

				inode[i].per=inode[i].per|FDIR;
				printf("%d\n",inode[i].per);
				printf("directory\n");
			}
			else{  
				printf(" Type of File  ");
				printf(" Regular File\n");
			} 
			inode[i].a_time=s.st_atime;
			printf("Acess Time %d\n",inode[i].a_time);
			inode[i].m_time=s.st_mtime;
			printf(" Modification Time %d\n",inode[i].m_time);
			inode[i].c_time=s.st_ctime;
			printf("Current Time %d\n",inode[i].c_time);
			inode[i].u_id=s.st_uid;
			printf("User Id %d\n",inode[i].u_id);
			inode[i].g_id=s.st_gid;
			printf("Group Id %d\n",inode[i].g_id);           
			close(fd2);
			u_space=inode[i].off_set+inode[i].size;
			printf("--------------------------------------\n");
			j++;
		}

	}

	s_block[0].free_sp=MAX-u_space;
	s_block[0].free_inode=3-(argc-2);
	printf("free inodes=%d   free space=%d \n",s_block[0].free_inode,s_block[0].free_sp);
	
	write(fd1,s_block,sizeof(s_block));
	i=0;
	j=0;
	k=inode[i].off_set;
	inode[i].off_set=sizeof(inode)+sizeof(s_block);
	while(i<3 && j<3000){
		cal=(inode[i].size/10)+1;
		cal=cal*10;
		if(k<cal+inode[i].off_set){
			inode[i].block[j]=k;
			j=j+1;
			k=k+10;
		}else{
			i=i+1;
			j=0;
		}
	}

	write(fd1,inode,sizeof(inode));

	i=0;
	for(j=2;j<argc;j++){
		fd3=open(argv[j],O_RDONLY);
		i=0;
		while(i<inode[j-2].size){
			bzero(data, 10);
			k=read(fd3,data,10);
			write(fd1,data,10);
			i=i+10;
		}

	}
	close(fd3);

}


