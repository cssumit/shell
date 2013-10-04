#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
const int MAX_ARGS = 256;
enum Mode {PIPE, REDIRECT,CD,PWD,HISTORY,NEITHER};
int count_Args(char**);
Mode mode_Chooser(int, char**, char**, char**);
void if_PIPE(char**, char**);
void if_REDIRECT(char**, char**);
void if_CD(char**);
void if_PWD();
void if_HISTORY();
void history(char*);
void if_NOTHING(int, char**);
bool if_EXIT(string);
void sigkill(int);
void sigstop(int);
