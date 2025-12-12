#include "cli.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <threads.h>

int main(int argc, char **argv) {
  if (argc <= 1) {
    fprintf(stderr, "please provide an argument. (use -h to see options)\n");
    return 1;
  }

  if (strcmp(argv[1], "init") == 0) {
    StorageError err = storage_init();
    if (err == STORAGE_DIARY_DIR_ALREADY_CREATED) {
      printf("you're already ready to go :)\n");
      return 0;
    }
    if (err != STORAGE_OK) {
      fprintf(stderr, "storage initialization error: %s\n",
              storage_strerror(err));
      return 1;
    }
    printf("'.dia' created. you can write your diary\n");
    return 0;
  }

  char *text = join_arguments(argc, argv);

  StorageError err = storage_append_diary_text(text);
  if (err == STORAGE_ERR_NOT_FOUND) {
    fprintf(stderr,
            "diary directory not found, have you created it? run `dia init`\n");
  } else if (err != STORAGE_OK) {
    fprintf(stderr, "failed to create file: %s\n", storage_strerror(err));
  }

  return 0;
}
