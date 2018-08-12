#include "../inc/leaks_detector.h"

extern char **environ;

static char *name_preload = "LD_PRELOAD";
static char *path_preload = "./shared_lib/shim.so";
static char *pr_name = "PRG_NAME";
static char cmd[BUF] = "nm -nr ";
static char *filename = "file_stack_func";

static void wait_status(int status)
{
  if (WIFEXITED(status)) {
    fprintf(stderr, "child exited, status=%d\n",
    WEXITSTATUS(status));
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

static void save_stack_func(FILE *fp)
{
  FILE *newfile;
  char buf[BUF];

  newfile = fopen(filename, "w");
  if (newfile == NULL) {
    HANDLE_ERROR("fopen");
  }
  while (fgets(buf, BUF, fp) != NULL) {
    if (strstr(buf, "_libc_csu_init")) {
      while ((fgets(buf, BUF, fp) != NULL)
      && !strstr(buf, "frame_dummy")) {
        fprintf(newfile,"%s", buf);
      }
    }
  }
  fclose(newfile);
}

int main(int argc, char *argv[])
{
  int status;
  pid_t child_pid;
  FILE *fp;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s [path to prog]\n", argv[0]);
    return 0;
  }
  memcpy(cmd + strlen(cmd), argv[1], strlen(argv[1]) + 1);
  if ((fp = popen(cmd, "r")) == NULL) {
    HANDLE_ERROR("peopen");
  }
  save_stack_func(fp);
  if ((status = pclose(fp)) == -1){
    wait_status(status);
  }
  if (setenv(pr_name, argv[1], 0) == -1) {
    HANDLE_ERROR("setenv");
  }
  if (setenv(name_preload, path_preload, 0) == -1) {
    HANDLE_ERROR("setenv");
  }
  switch(child_pid = fork()){
    case -1:
      HANDLE_ERROR("fork failed");
    case 0:
      execve(argv[1], argv + 1, environ);
      exit(EXIT_FAILURE);
    default:
      wait(&status);
  }
  return 0;
}