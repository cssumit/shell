#include "header.h"
int main() {
  char *argv[MAX_ARGS], *cmd1[MAX_ARGS], *cmd2[MAX_ARGS];
  Mode pipe_redirect;
  int argc;
  while (true) {
    cout << "[Avenger]~: ";
    argc = count_Args(argv);
    pipe_redirect = mode_Chooser(argc, argv, cmd1, cmd2);
    if (pipe_redirect == PIPE)          // piping
      if_PIPE(cmd1, cmd2);
    else if (pipe_redirect == REDIRECT) // redirecting
      if_REDIRECT(cmd1, cmd2);
    else if(pipe_redirect == CD)
	if_CD(argv);
    else if(pipe_redirect == PWD)
	if_PWD();
    else if(pipe_redirect == HISTORY)
	if_HISTORY();
    else
      if_NOTHING(argc, argv);              // neither
    
    for (int i=0; i<argc; i++)
      argv[i] = NULL;
  }
  return 0;
}
