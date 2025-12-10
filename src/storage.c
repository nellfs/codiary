#include "storage.h"
#include "datetime.h"
#include "string.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

StorageError storage_get_dir(char *directory) {
  char cwd[PATH_MAX];

  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("getcwd");
    return STORAGE_ERR_GETCWD_FAILED;
  }

  const char *suffix = "/.dia";
  if (strlen(cwd) + strlen(suffix) + 1 > PATH_MAX) {
    fprintf(stderr, "Path too long\n");
    return STORAGE_ERR_PATH_TOO_LONG;
  }

  char dia_dir[PATH_MAX];
  int n = snprintf(dia_dir, sizeof(dia_dir), "%s/.dia", cwd);

  if (n < 0 || n >= (int)sizeof(dia_dir)) {
    fprintf(stderr, "path too long\n");
    return STORAGE_ERR_PATH_TOO_LONG;
  }

  if (access(dia_dir, F_OK) == 0) {
    directory = dia_dir;
    return STORAGE_DIR_ALREADY_CREATED;
  }
  return STORAGE_OK;
}

StorageError storage_create_dir(void) {
  char dia_dir[PATH_MAX];
  StorageError err = storage_get_dir(dia_dir);
  if (err != STORAGE_OK) {
    return err;
  }

  if (mkdir(dia_dir, 0755) != 0) {
    perror("mkdir");
    return STORAGE_FAILED_TO_CREATE_DIRECTORY;
  }

  return STORAGE_OK;
}

StorageError storage_init(void) {
  StorageError err = storage_create_dir();
  if (err != STORAGE_OK) {
    return err;
  }
  return STORAGE_OK;
}

StorageError storage_append_diary_file(const char **out_dir) {
  return STORAGE_OK;
}

StorageError storage_get_current_diary_file(char *out_dir) {
  char dia_dir[PATH_MAX];

  StorageError storage_err = storage_get_dir(dia_dir);
  if (storage_err != STORAGE_OK && storage_err != STORAGE_DIR_ALREADY_CREATED) {
     return storage_err;
  }

  char date[16];
  StorageError datetime_err = datetime_get_current_date(date, sizeof(date));
  if (datetime_err != STORAGE_OK) {
     return datetime_err;
  }


  const char *extension = ".txt";

  char fullpath[PATH_MAX];

  snprintf(fullpath, sizeof(fullpath), "%s%s%s", dia_dir, date, extension);

  out_dir = fullpath;
  return STORAGE_OK;
}

// StorageError storage_append_diary_text(void) {
//     FILE *fptr = fopen(fullpath, "a");
//       free(fullpath);
//       fullpath = NULL;

//       if (!fptr) {
//         return STORAGE_ERR_NO_PERMISSION;
//       }

//       char timestamp[32];
//       get_iso8601(timestamp, sizeof(timestamp));

//       fprintf(fptr, "%s\n%s", timestamp, text);
//       fclose(fptr);

//     return STORAGE_OK;
// }

void storage_free_dir(const char *dir) {
  free((void *)dir);
  dir = NULL;
}

const char *storage_strerror(StorageError err) {
  switch (err) {
  case STORAGE_OK:
    return "ok";
  case STORAGE_ERR_NOT_FOUND:
    return "directory not found";
  case STORAGE_ERR_MALLOC_FAIL:
    return "storage allocation failed";
  case STORAGE_ERR_PATH_TOO_LONG:
    return "directory path is too long";
  case STORAGE_ERR_INVALID_PATH:
    return "invalid directory path";
  case STORAGE_ERR_PERMISSION:
    return "permission denied";
  case STORAGE_ERR_INVALID_ARGUMENT:
    return "invalid argument";
  case STORAGE_ERR_NO_PERMISSION:
    return "storage permission error";
  case STORAGE_FAILED_TO_CREATE_DIRECTORY:
    return "storage failed to create directory";
  case STORAGE_ERR_GETCWD_FAILED:
    return "storage failed to get current directory";
  case STORAGE_DIR_ALREADY_CREATED:
    return "storage directory already created";
  }
  return "unknown error";
}
