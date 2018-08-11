#include "../inc/leaks_detector.h"
#include <fcntl.h>

extern char **__environ;
static char *pr_name = "PRG_NAME";
static char cmd[BUF] = "nm -n ";
static char patch[BUF] = "LD_PRELOAD=./shared_lib/shim.so ";
static char *filename = "1.txt";

static void wait_status(int status)
{
  if (WIFEXITED(status)) {
    fprintf(stderr, "child exited, status=%d\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    fprintf(stderr, "child killed by signal %d (%s)\n",
    WTERMSIG(status), strsignal(WTERMSIG(status)));
  } else if (WIFSTOPPED(status)) {
    fprintf(stderr, "child stopped by signal %d (%s)\n",
    WSTOPSIG(status), strsignal(WSTOPSIG(status)));
  } else {
    printf("what happened to this child? (status=%x)\n",
    (unsigned int) status);
  }
}

static void create_file(FILE *fp)
{
  FILE *newfile;
  char buf[BUF];

  newfile = fopen(filename, "w");
  if (newfile == NULL) {
    HANDLE_ERROR("fopen");
  }
  while (fgets(buf, BUF, fp) != NULL) {
    fprintf(newfile,"%s\n", buf);
  }
  fclose(newfile);
}

int main(int argc, char *argv[])
{
  int status;
  FILE *fp;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s [path to prog]\n", argv[0]);
    return 0;
  }
  if (setenv(pr_name, argv[1], 0) == -1) {
    HANDLE_ERROR("setenv");
  }
  memcpy(cmd + strlen(cmd), argv[1], strlen(argv[1]) + 1);
  if ((fp = popen(cmd, "r")) == NULL) {
    HANDLE_ERROR("peopen");
  }
  create_file(fp);
  if ((status = pclose(fp)) == -1){
    wait_status(status);
  }
  memcpy(patch + strlen(patch), argv[1], strlen(argv[1]) + 1);
  if ((fp = popen(patch, "r")) == NULL) {
    HANDLE_ERROR("peopen");
  }
  if ((status = pclose(fp)) == -1){
    wait_status(status);
  }
}