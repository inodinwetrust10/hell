#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

int execute_command(char **token_array) {
  pid_t pid;
  int status;

  if (token_array[0] == NULL)
    return 1;

  if (strcmp(token_array[0], "cd") == 0) {
    if (token_array[1] == NULL) {
      fprintf(stderr, "shell: expected argument to \"cd\"\n");
    } else {
      if (chdir(token_array[1]) != 0) {
        perror("shell");
      }
    }
    return 1;
  } else if (strcmp(token_array[0], "exit") == 0) {
    return 0;
  }

  pid = fork();
  if (pid == 0) {
    if (execvp(token_array[0], token_array) == -1) {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("shell");
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

int main(void) {
  int status;
  char *command;
  char **token_array;
  do {
    printf("$   ");
    command = read_line();
    token_array = parse_line(command);
    status = execute_command(token_array);
  } while (status);
}
