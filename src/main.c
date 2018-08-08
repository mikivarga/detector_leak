#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int arglen = 0;
  for (int i = 1; i < argc; i++)
    arglen += strlen(argv[i]) + 1;

  const char *preload = "LD_PRELOAD=./shared_lib/shim.so";
  arglen += strlen(preload) + 1;
  char data[arglen];
  strcpy(data, preload);
  for (int i = 1; i < argc; i++) {
    strcat(data, " ");
    strcat(data, argv[i]);
  }
  printf("%s\n", data);
  FILE *pipe = popen(data, "r");
  if (pipe == NULL)
    perror("Error opening piped file.");
  pclose(pipe);
}
