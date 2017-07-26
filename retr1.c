#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include"header.h"
int cmd_found;

main()
{
	int fd1,n,n1,n2=0,fd2,fd3,fd4,i,i1=0,i2,le,j,k,k1=0,rt,fd,cmp,cal1,cmp1,cmp2,cpy;
	int cal2,cal3,cal4,cal5,a1;
	char cmnd[10],src_file[10],lock[15],sr[25],des_file[10];
	char data1[300]; 
	char buf[30000];
	char data[36301];
	char data2[300];
	bzero(data,36301);
	bzero(data1,300);
	bzero(data2,300);
	fd1=open("vfs",O_RDONLY);
	char buffer[120];
	struct superblock
	{ 
		int free_inode;        /*superblock*/
		int free_space;
	}s_block[1];


	static struct s
	{
		char file_name[10];
		int size;
		int off_set;
		short int per;         /*inode*/
		int a_time;
		int m_time;
		int c_time;
		int u_id;
		int g_id;
		int block_no [3000];
	}inode[3];
	struct temp
	{
		char f_name[10];
		int  f_size;
		int  f_offset;
		short int f_per;
		int f_atime;
		int f_mtime;         /*temporary inode*/
		int f_ctime;
		int f_uid;
		int f_gid;
		int f_lock;
	}t;
        printf("%d\n",fd1);
	read(fd1,s_block,sizeof(s_block));
	n=read(fd1,inode,sizeof(inode));       
	for(i=0;i<3;i++){
		printf("%s  %d %d\n ",inode[i].file_name,inode[i].size,inode[i].off_set);
		//printf("working ");
		printf("Access time=%s\n",ctime(&inode[i].a_time));
		printf("Modification time=%s\n",ctime(&inode[i].m_time));
		printf("current time=%s\n",ctime(&inode[i].c_time));
		printf("********************************************************************\n");
		n1=(inode[i].size/10)+1;
		n1=n1*10;
		n2=n2+n1;
	}

	while(1){
		cmd_found = 0;
		printf("\nefs>  ");
		scanf("%s",cmnd);
		{
			cmp=strcmp(cmnd,"myls");
			if(cmp==0){
				cmd_found = 1;                          /*myls command*/
				for(i=0;i<3;i++){
					printf("%s\n",inode[i].file_name);
				}
			}
			cmp=strcmp(cmnd,"myls-x");
			if(cmp==0){
				cmd_found = 1;
				for(i=0;i<3;i++){                     /*myls-x command */ 
					printf("%s\t",inode[i].file_name);
				}
			}
			cmp=strcmp(cmnd,"myls-i");
			if(cmp==0){
				cmd_found = 1;
				for(i=0;i<3;i++){                   /*myls-i command*/
					if(strcmp(inode[i].file_name,"\0")==0)
						break;
					else
						printf("%s\t %d\n",inode[i].file_name,i);
				}
			}
			cmp=strcmp(cmnd,"mplayer");
			if(cmp==0){
				cmd_found =1;
				printf("enter the source file\n");  /*mplayer command*/
				scanf("%s",src_file);
				for(i=0;i<3;i++){
					cmp1=strcmp(src_file,inode[i].file_name);
					if(cmp1==0){
						printf("enter the dest file\n");
						scanf("%s",des_file);
						lseek(fd1,inode[i].block_no[0],SEEK_SET); 
						cal1=(inode[i].size/10)+1;
						cal1=cal1*10;
						rt=read(fd1,buf,cal1);
						fd3=open(des_file,O_WRONLY);
						write(fd3,buf,rt);
					}
				}
			}

			cmp= strcmp(cmnd,"myls-l");
			if(cmp==0){
				cmd_found = 1;
				for(i=0;i<3;i++){
					printf("%d\n",inode[i].size);   /*myls-l command*/
				}
			}

			cmp=strcmp(cmnd,"cp");
			if(cmp==0){
				cmd_found=1;
				printf("Enter the source file = ");    /*cp command*/
				scanf("%s",src_file);
				fflush(0);
				printf("Enter the target file=  ");
				scanf("%s",des_file);
				fd4=open(des_file,O_WRONLY);
				for(i=0;i<3;i++){
					cmp1 =strcmp(inode[i].file_name,"\0 ");
					if(cmp1==0){
						strcpy(inode[i].file_name,des_file);
					}
				}
				for(j=0;j<3;j++){
					cmp2= strcmp(inode[j].file_name,src_file);
					if(cmp2==0){                        
						inode[i-1].size=inode[j].size;
						inode[i-1].per=inode[j].per;
						inode[i-1].a_time=inode[j].a_time;
						inode[i-1].m_time=inode[j].m_time;
						inode[i-1].c_time=inode[j].c_time;
						inode[i-1].u_id=inode[j].u_id;
						inode[i-1].g_id=inode[j].g_id;
						inode[i-1].off_set=inode[i-2].size+inode[i-2].off_set;
						rt= lseek(fd1,inode[j].off_set,SEEK_SET);
						k=inode[i-2].block_no[0];
						cal2=(inode[i-2].size/10)+1;
						cal2=cal2*10;
						cal3=cal2+k;
						k=cal3;
						i2=0;
						cal4=(inode[j].size/10)+1;
						cal4=cal4*10;
						cal5=cal3+cal4;
						while(i2<10){
							if(k<cal5){
								inode[i-1].block_no [i2]=k;
								k=k+10;
								i2=i2+1;
							}
							else
								break;
						}                           
						rt=read(fd1,data2,inode[j].size);
						write(1,data2,rt);
						write(fd4,data2,rt); 
					}
				}
				fd1=open("out",O_WRONLY);
				write(fd1,inode,sizeof(inode));
				lseek(fd1,inode[i-1].block_no[0],SEEK_SET);
				write(fd1,data2,rt);
			}
			cmp=strcmp(cmnd,"mv");
			if(cmp==0){
				cmd_found=1;
				printf("Enter the source file = ");
				scanf("%s",src_file);
				fflush(0);
				printf("Enter the target file=  ");      /*mv command*/
				scanf("%s",des_file);
				for(i=0;i<3;i++){
					cmp1=strcmp(src_file,inode[i].file_name);
					if(cmp1==0){
						cpy=strcpy(inode[i].file_name,des_file);
					}
				}
				fd1=open("efs",O_WRONLY);
				write(fd1,inode,sizeof(inode));
				printf("name of file changed\n");
			}

			for(i=0;i<3;i++){
				cmp=strcmp(cmnd,inode[i].file_name);
				if(cmp==0){
					lseek(fd1,inode[i].block_no[0],SEEK_SET);
					cal1=(inode[i].size/10)+1;       
					cal1=cal1*10;                       /*file contents*/
					rt= read(fd1,data1,cal1);
					cmd_found = 1;
					write(1,data1,rt);
				}
			}

			cmp=strcmp(cmnd,"exit");
			if(cmp==0)                                        /*exit command*/ 
				exit(1);

			cmp=strcmp(cmnd,"help");
			if(cmp==0){                                      /*help command*/ 
				printf("\nCommand Set:-\nmyls\nmyls-l\nmyls*\nfind\nmstat\nfilename\nmv\ncp\nmplayer\n");
				continue;		
			}
			cmp=strcmp(cmnd,"mstat");
			if(cmp==0){
				printf("Enter the File Name\n");   
				scanf("%s",src_file);                       /*mstat command*/
				for(i=0;i<3;i++){
					cmp1=strcmp(src_file,inode[i].file_name);
					if(cmp1==0){
						printf(" inode no=  %d\n",i);
						cmd_found = 1;
						strcpy(t.f_name,inode[i].file_name);
						t.f_size=inode[i].size;
						t.f_offset=inode[i].off_set;
						printf("%s  ",t.f_name);
						printf("%d  %d\n",t.f_size,t.f_offset);
						t.f_per=inode[i].per;
						printf("%d\n",t.f_per);
						t.f_atime=inode[i].a_time;
						printf("%d\n",t.f_atime);
						t.f_mtime=inode[i].m_time;
						printf("%d\n",t.f_mtime);
						t.f_ctime=inode[i].c_time;
						printf("%d\n",t.f_ctime);        
						t.f_uid=inode[i].u_id;        
						printf("%d\n",t.f_uid);
						t.f_gid=inode[i].g_id;		
						printf("%d\n",t.f_gid);
						if(RWXU==(t.f_per&RWXU)){
							printf("user have all permission\n");
						}
						else
						    if(RWUSR==(t.f_per&RWUSR)){
						      printf("user have only read-write permission\n");
						       }  
						    else
						       if(WXUSR==(t.f_per&WXUSR)){
						         printf("user have only write-excute permission\n");
						          }
						       else
						          if(RXUSR==(t.f_per&RXUSR)){
							    printf("user have only read-execute permissions\n");                                                                          }
						          else
							    if(RUSR==(t.f_per&RUSR)){
							       printf("user have only read permission\n");
							      }
							     else                  
							       if(WUSR==(t.f_per&WUSR)){
							         printf("user have only write permission\n");
							         }
							         else
							           if(XUSR==(t.f_per&XUSR)){
							             printf("user have only execute permission\n");
								    }
							            else
								      if(RWXU!=(t.f_per&RWXU)){
								          printf("user don't have any permission");
									  }			

						printf("enter the advisory lock");
						scanf("%s",lock);
						if( strcmp(lock,"shared")==0){
							fd=open(t.f_name,O_RDONLY);
							printf("value of DESCRIPTOR %d\n",fd);
							flock(fd,LOCK_SH);
							printf("linux\n");
							t.f_lock=SHARED;
							printf("%d\n",t.f_lock);
							sleep(2);
						} 
						else{
							fd=open(t.f_name,O_RDWR);
							flock(fd,LOCK_EX);
							t.f_lock=EXCLUSIVE;
							printf("%d\n",t.f_lock);
						}
					}
				}
			}
			cmp=strcmp(cmnd,"myls*");
			{
				if(cmp==0){
					cmd_found = 1;
					printf("enter the name:");
					scanf("%s",src_file);
					le=strlen(src_file);             /*myls* command*/
					i=0;
					while(i<3){
						j=0;
						while(j<le){
							if(inode[i].file_name[j]==src_file[j])
								j++;
							else
								break;                                       
						}
						if(j==le){
							printf("%s\n",inode[i].file_name);
						}
						i++;
					}
				}
			}

			cmp= strcmp(cmnd,"find"); 
			if(cmp==0){

				cmd_found = 1;                      /*find command*/
				printf("enter the content to be searched\n");
				scanf("%s",sr);
				le=strlen(sr);
				i=0;
				a1=0;
				read(fd1,data,n2);
				cal1=sizeof(inode) +sizeof(s_block);
				cal2=(inode[0].size/10)+1;
				cal2=cal2*10;

				for(j=0;j<n2;j++){

					if(data[j]==sr[a1]){
						a1=a1+1;

						if(a1==le){


							while(i<3){

								if(j<=cal2){
									a1=0;
									printf("*****************************\n");

									printf("file name=%s\n",inode[i].file_name);
									i=i+1;
									cal3=(inode[i].size/10)+1;
									cal3=cal3*10;
									cal2=cal2+cal3;
									break;
								}
								else{

									i=i+1;
									cal3=(inode[i].size/10)+1;
									cal3=cal3*10;
									cal2=cal2+cal3;
								}
							}
						}
					}
					else
						a1=0;


				} 
			}                   
		}
		if(cmd_found == 0)
		{
			printf("Command not found.\nCommand Set:-\nmyls\nmyls-l\nmyls*\nsearch\nopen\nfilename\n");
		}
	}
} 
