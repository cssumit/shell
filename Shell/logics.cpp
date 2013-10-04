#include "header.h"
pid_t pid;
Mode mode_Chooser(int argc, char** argv, char** cmd1, char** cmd2) {
  Mode result = NEITHER;
  int split = -1;
  char* str=(char*)malloc(10000);
  strcpy(str,"");
  for (int i=0; i<argc; i++) {
    if (strcmp(argv[i], "|") == 0) {
      result = PIPE;
      split = i;
    } else if (strcmp(argv[i], ">>") == 0) {
      result = REDIRECT;
      split = i;
    }
	else if(strcmp(argv[i],"pwd")==0){
		result=PWD;
	}
	else if(strcmp(argv[i],"cd") == 0) {
	result = CD;
	}
	else if(strcmp(argv[i],"history")==0) {
		result=HISTORY;
	}
	strcat(str,argv[i]);
  }
  history(str);
  if (result != NEITHER) {
    for (int i=0; i<split; i++)
      cmd1[i] = argv[i];
    int count = 0;
    for (int i=split+1; i<argc; i++) {
      cmd2[count] = argv[i];
      count++;
    }
    cmd1[split] = NULL;
    cmd2[count] = NULL;
  }
  return result;
}
int count_Args(char **argv) {
  char *cstr;
  string arg;
  int argc = 0;
  while (cin >> arg) {
    if (if_EXIT(arg)) {
      cout << "Goodbye!\n";
      exit(0);
    }
    cstr = new char[arg.size()+1];
    strcpy(cstr, arg.c_str());
    argv[argc] = cstr;
    argc++;
    if (cin.get() == '\n')
      break;
  }
  argv[argc] = NULL;
  return argc;
}
void if_PIPE(char** cmd1, char** cmd2) {
  int fds[2]; // file descriptors
  pipe(fds);
  pid_t pid;
  if (fork() == 0) {
    dup2(fds[0], 0);
    close(fds[1]);
    execvp(cmd2[0], cmd2);
    perror("execvp failed");
  } else if ((pid = fork()) == 0) {
    dup2(fds[1], 1);
    close(fds[0]);
    execvp(cmd1[0], cmd1);
    perror("execvp failed");
  } else
    waitpid(pid, NULL, 0);
}

void if_REDIRECT(char** cmd, char** file) {
  int fds[2]; // file descriptors
  int count;  // used for reading from stdout
  int fd;     // single file descriptor
  char c;     // used for writing and reading a character at a time
  pid_t pid;  // will hold process ID; used with fork()
  pipe(fds);
  if (fork() == 0) {
    fd = open(file[0], O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
      printf("Error: %s\n", strerror(errno));
      return;
    }
    dup2(fds[0], 0);
    close(fds[1]);
    while ((count = read(0, &c, 1)) > 0)
      write(fd, &c, 1); // Write to file.
    execlp("echo", "echo", NULL);
  } else if ((pid = fork()) == 0) {
    dup2(fds[1], 1);
    close(fds[0]);
    execvp(cmd[0], cmd);
    perror("execvp failed");
  } else {
    waitpid(pid, NULL, 0);
    close(fds[0]);
    close(fds[1]);
  }
}
void if_CD(char** argv){
  char path[1024];
  char *temp;
  if(argv[1]==NULL || (strcmp(argv[1],"~"))==0){
  	temp = getenv("HOME");
        strcpy(path,temp);
  }else if( (strcmp(argv[1],"/"))==0){
	strcpy(path,argv[1]);
  }else{
	getcwd(path,1024);
        strcat(path,"/");
	strcat(path,argv[1]);
  }

  if(chdir(path)!=0){
		cout<<"[Avengers]~: cd: "<<argv[1]<<": No such file or directory\n";
		exit (1);
  }

}
void history (char* command) {
 FILE* file = fopen("history.txt","a+");
 fprintf(file,"%s\n",command);
 fclose(file);
}

void if_HISTORY(){
 int i=1;
 FILE* file = fopen ("history.txt", "rt");
 if(file!=NULL){
	char line[256];
	while(fgets ( line, 256, file ) != NULL){
		printf("%-3d  %s",i++,line);
	}
 }
 else
	printf("Error in displaying history !!\n"); 	

 fclose(file);	

}
void sigstop(int p){
    signal(SIGTSTP,&sigstop);
    kill(pid,SIGSTOP);

    fflush(stdout);
}

// Ctrl+C
void sigkill(int p){
    signal(SIGINT,&sigkill);  
    kill(pid,SIGKILL);

    fflush(stdout);
}
void if_PWD(){
	char cwd[2000];
	getcwd(cwd,2000);
	cout<<cwd<<"\n";
}
void if_NOTHING(int argc, char** argv) {
  //pid_t pid;
  const char *amp;
  amp = "&";
  bool found_amp = false;
  if (strcmp(argv[argc-1], amp) == 0)
    found_amp = true;
  pid = fork();
  if (pid < 0)
    perror("Error (pid < 0)");
  else if (pid == 0) {
    if (found_amp) {
      argv[argc-1] = NULL;
	signal(SIGTSTP,&sigstop);      //for Ctrl+Z
    signal(SIGINT,&sigkill);       //for Ctrl+C
	argc--;
    }
    execvp(argv[0], argv);
    perror("execvp error");
  } else if (!found_amp)
    waitpid(pid, NULL, 0); // only wait if no ampersand
}
bool if_EXIT(string choice) {
  for (unsigned int i=0; i<choice.length(); i++)
    choice[i] = tolower(choice[i]);

  return (choice == "exit");
}
