#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *commmand = NULL;

  size_t buffer = 100;

  getline(&commmand, &buffer, stdin);

  free(commmand);
}
