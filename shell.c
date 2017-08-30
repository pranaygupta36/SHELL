<<<<<<< HEAD
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

=======
#include<stdio.h>
#include<sys/types.h>
#include<pwd.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>
#include<time.h>
#include<grp.h>
>>>>>>> ead21988b85c6b3d420754bb7f8d86286bf74f7c

#define Delim " \t\n\r\a"  

char home[100];

void getHome() {
	getcwd(home,100);
}

void getPrompt() {
	char *username = getenv("USERNAME");
	char *hostname = malloc(20*sizeof(char));
	char *currdir = malloc(100*sizeof(char)); 
	int i;
	getcwd(currdir,100);
	int currlen = strlen(currdir);
	int homelen = strlen(home);
	currdir[currlen++] = 47; 
	gethostname(hostname,20);
	if (currlen >= homelen) {
		int flag = 0;
		for(i=0;i<homelen;i++) {
			if(currdir[i] != home[i]) {
				flag = 1;
			}
		}
		int j = 0;
		if (flag == 0) {
			currdir[j++] = '~';
			currdir[j++] = '/';
			for(i=homelen;i<currlen;i++) currdir[j++] = currdir[i];
			while(j<currlen) currdir[j++] = 0;
		}
	}	
	printf("<%s@%s:%s> ",username,hostname,currdir);
	free(hostname);
	free(currdir);
}

char *readInput(void) {
	char *line;
	int bufsize = 1024;
	line = (char *)malloc(bufsize*sizeof(char));
	// error handling
	int c,pos=0;
	while(1) {
		c = getchar();
		if (c == EOF || c == '\n') {
			line[pos++] = '\0';
				//printf("%s%d\n",line,(int)strlen(line));
				return line;	
		}
		else line[pos++] = c;
		if (pos >= bufsize) {
			bufsize += bufsize;
			line = realloc(line,bufsize*sizeof(char));
			//error handling
		} 
	}
	free(line);
}

char **separateCmds(char *line) {
	int bufsize = 64,position = 0;
	char **tokens = malloc(bufsize*sizeof(char));
	//put error handling
	char *token;

	token = strtok(line,Delim);
	//printf("%s",token);
	while(token != NULL) {
		//printf("yay");
		tokens[position++] = token;
		if (position > bufsize) {
			bufsize += 64;
			tokens = (char**)realloc(tokens,bufsize*sizeof(char));
			//put error handling
		}
		token = strtok(NULL,Delim);
	}
	tokens[position] = NULL;
	//printf("%s",tokens[0]);
	return tokens;
	free(tokens);
}

int launchCmds(char **args) {
	int pid = fork(),wpid,status;
	if (pid == 0) {
		if (execvp(args[0],args) == -1) perror("shell");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) perror("shell");
	else { 
		wpid = waitpid(pid,&status,WUNTRACED);
		while ((WIFEXITED(status)<=0) && (WIFSIGNALED(status)<=0)) {
			wpid = waitpid(pid,&status,WUNTRACED);
		}
	}
	return 1;
}

int pwd(void) {
	char *currdir = malloc(1000*sizeof(char));
	getcwd(currdir,1000);
	printf("%s\n",currdir);
	free(currdir);
}

int echo(char **args) {
	int i=1;
	while (args[i] != NULL) printf("%s ",args[i++]);
	printf("\n");
	return 1;
}

int cd(char **args) {
	//printf("%s\n",home);
	if (args[1] == NULL) chdir(home);
	else {
		if (strcmp(args[1],"~") == 0) { 
			if (chdir(home) != 0) perror("shell");
		}	
		else {
			if(chdir(args[1]) != 0) perror("shell");		
		}
	}
	return 1;
}	

<<<<<<< HEAD
int pinfo(char **args,char *path)
{
	char ProcessPath[1000];
	strcpy(ProcessPath,"/proc/");
	if(args[1]==NULL)
		strcat(ProcessPath,"self");
	else
		strcat(ProcessPath,args[1]);

	// strcat(ProcessPath,"/stat");
	char finalpath[100],execpath[1000],relativepath[1000];
	strcpy(finalpath,ProcessPath);
	strcat(finalpath,"/stat");


	//to print pid and process status
	int pid;
	char status,name[50];
	FILE *stat=fopen(finalpath,"r");
	fscanf(stat,"%d %s %c",&pid,name,&status);
	fprintf(stdout," pid: %d\n Process Status: %c\n",pid,status);
	fclose(stat);


	//to print memory
	strcpy(finalpath,ProcessPath);
	strcat(finalpath,"/statm");

	int memsize;
	FILE *mem=fopen(finalpath,"r");
	fscanf(mem,"%d",&memsize);
	fprintf(stdout, "Memory: %d\n",memsize);
	fclose(mem);


	//to print executable path
	strcpy(finalpath,ProcessPath);
	strcat(finalpath,"/exe");
	readlink(finalpath,execpath,sizeof(execpath));

	int i=0,pathL=strlen(path);
	while(i<pathL)
	{
		if(path[i]!=execpath[i])
			break;
		i++;
	}

	if(i==pathL)
	{
		relativepath[0]='~';
		relativepath[1]='\0';
		strcat(relativepath,(const char *)&execpath[pathL]);
	}
	else
	{
		strcpy(relativepath,execpath);
		relativepath[strlen(execpath)]='\0';
	}
	int j=0;
	while(execpath[j])
		execpath[j++]='\0';

	fprintf(stdout,"Executable Path: %s\n",relativepath);
	return 1;

int ls(char **args) {
	struct dirent **dent;
	DIR* dir;
	struct stat mystat;
	int n,i = 1,l=0,a=0,pos = 0,j=0;
	char *buf;
	struct passwd *user;
	struct group *group;
	unsigned char mod[13];
	struct tm *strtime;
	time_t now;
	int year; 
	buf = malloc(1024*sizeof(char));
	while(args[i] != NULL) {
		j = 0;
		if (args[i][j] == 45) {
			j = 1;
			while(args[i][j] != 0) {
				if(args[i][j] == 108) l = 1;
				if(args[i][j] == 97)  a = 1;
				j++;
			}
		}
		else {
			if (i > 0) pos = i;
		}
		i++; 
	}
	if (pos == 0 || args[pos] == NULL) args[pos] = ".";
		//n = scandir(".",&dent,NULL,alphasort);		
	n = scandir(args[pos],&dent,NULL,alphasort);
	if (n < 0) perror("shell");
	else {
		for(i=0;i<n;i++) {
			if (l == 1) {
				if (a == 1) {
					sprintf(buf,"%s/%s",args[pos],dent[i]->d_name);
					stat(buf,&mystat);
					printf( (S_ISDIR(mystat.st_mode)) ? "d" : "-");
					printf( (mystat.st_mode & S_IRUSR) ? "r" : "-");
					printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
					printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
					printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
					printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
					printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
					printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
					printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
					printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");
					printf(" \t%d",(int)mystat.st_nlink);
					user = getpwuid(mystat.st_uid);
						printf(" \t%s", user->pw_name);
					group = getgrgid(mystat.st_gid);
						printf(" \t%s", group->gr_name);
					printf(" \t%lld",(long long)mystat.st_size);
					time(&now);
					year = localtime(&now)->tm_year;
					strtime = localtime(&mystat.st_ctime);
					if(strtime->tm_year == year)
						strftime(mod,13,"%b %e %R",strtime);
					else
						strftime(mod,13,"%b %e %Y",strtime);
					printf(" \t%s",mod );	 	
					printf(" \t%s\n",dent[i]->d_name);
				}
				else {
					if ((dent[i] ->d_name)[0] != 46) {
						sprintf(buf,"%s/%s",args[pos],dent[i]->d_name);
						stat(buf,&mystat);
						printf( (S_ISDIR(mystat.st_mode)) ? "d" : "-");
						printf( (mystat.st_mode & S_IRUSR) ? "r" : "-");
						printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
						printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
						printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
						printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
						printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
						printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
						printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
						printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");
						printf(" \t%d",(int)mystat.st_nlink);
						user = getpwuid(mystat.st_uid);
						printf(" \t%s", user->pw_name);
						group=getgrgid(mystat.st_gid);
						printf(" \t%s", group->gr_name);
						printf(" \t%lld",(long long)mystat.st_size);
						time(&now);
						year = localtime(&now)->tm_year;
						strtime = localtime(&mystat.st_ctime);
						if(strtime->tm_year == year)
							strftime(mod,13,"%b %e %R",strtime);
						else
							strftime(mod,13,"%b %e %Y",strtime);
						printf(" \t%s",mod );
						//printf(" \t%s",ctime(&mystat.st_mtime));
						printf(" \t%ld",mystat.st_mtime);
						printf(" \t%s\n",dent[i]->d_name);
					}	 
				}
			}	
			else {
				if (a == 1) {
					printf(" %s\n",dent[i]->d_name);	
				}
				else {
					if ((dent[i] ->d_name)[0] != 46) printf(" %s\n",dent[i]->d_name);
				}
			}	
			free(dent[i]);
			
		}
		free(dent);
	}	
	return 1; 

}

int executeCmds(char **args) {
	if (args[0] == NULL) return 1;
	//if (strcmp(args[0],"cd") == 0) return cd(args);
	else if (strcmp(args[0],"pwd") == 0) return pwd();
	else if (strcmp(args[0],"exit") == 0) exit(0);
	else if (strcmp(args[0],"echo") == 0) return echo(args);
	else if (strcmp(args[0],"cd") == 0) return cd(args);
	else if (strcmp(args[0],"pinfo")==0) return pinfo(args,home);
	else if (strcmp(args[0],"ls") == 0) return ls(args);
	else return launchCmds(args);
}

int main() {
	int i,status = 1;
	getHome();
		home[strlen(home)] = 47;
	do {
		//rintf("\n");
		getPrompt();
		char *line = readInput();

		char *temp;
		//printf("1\n");
		while (line) {
			temp = strsep(&line,";"); 
			char **args = separateCmds(temp);
			//printf("2\n");
			int status = executeCmds(args);
			//printf("\n");
			if (status == 0 ) break; 
			
		}
	} while(status);
	
	return 0;
}
