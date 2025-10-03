#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFFER 64
#define TOKEN_DELIMITER "\t\r\n\a"

char *read_line() {
  char *commmand = NULL;

  size_t buffer = 0;
  if (getline(&commmand, &buffer, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);
    } else {
      perror("getline");
      exit(EXIT_FAILURE);
    }
  }

  return commmand;
}

char **parse_line(char *command) {
  int buffer = TOKEN_BUFFER;
  int position = 0;
  char **token_array = malloc(buffer * sizeof(char *));
  char *token; // to store the temporary token from the command

  if (!token_array) {
    fprintf(stderr, "Allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(command, TOKEN_DELIMITER);
  while (token != NULL) {
    token_array[position] = token;
    position++;
    if (position >= buffer) {
      buffer += TOKEN_BUFFER;
      token_array = realloc(token_array, buffer * sizeof(char *));
      if (!token_array) {
        fprintf(stderr, "Reallocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(
        NULL,
        TOKEN_DELIMITER); // Passing NULL as the first argument tells strtok to
                          // continue tokenizing the same string it was working
                          // on before, starting from where it left off
  }
  token_array[position] = NULL; // to mark the end of the array
  return token_array;
}

int main(void) {
  char *line = read_line();
  return EXIT_SUCCESS;
}
