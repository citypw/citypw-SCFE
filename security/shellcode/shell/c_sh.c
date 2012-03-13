#include <unistd.h>

char *shell_path[2] = {"/bin/sh", NULL};

int main(int argc, char **argv[])
{
  execve(shell_path[0], shell_path, NULL);
  return 0;
}
