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
    return STORAGE_ERR_PATH_TOO_LONG;
  }

  int n = snprintf(directory, PATH_MAX, "%s/.dia", cwd);

  if (n < 0 || n >= PATH_MAX) {
    return STORAGE_ERR_PATH_TOO_LONG;
  }

  struct stat st;
  if (stat(directory, &st) == 0) {
    return STORAGE_ERR_NOT_FOUND;
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

StorageError storage_create_diary_file(void) {
  char storage_dir[PATH_MAX];
  StorageError err = storage_get_current_diary_file_dir(storage_dir);
  if (err != STORAGE_OK) {
    return err;
  }

  struct stat st;
  if (stat(storage_dir, &st) == 0) {
    return STORAGE_DIARY_FILE_ALREADY_CREATED;
  }

  FILE *fptr = fopen(storage_dir, "w");
  if (!fptr) {
    perror("fopen");
    return STORAGE_FAILED_TO_CREATE_FILE;
  }

  fclose(fptr);
  return STORAGE_OK;
}

StorageError storage_append_diary_text(const char *text) {
  char date[16];
  StorageError datetime_err = datetime_get_current_date(date, sizeof(date));
  if (datetime_err != STORAGE_OK) {
    return datetime_err;
  }

  char filedir[PATH_MAX];
  StorageError dir_err = storage_get_current_diary_file_dir(filedir);
  if (dir_err != STORAGE_OK) {
    return dir_err;
  }

  StorageError file_err = storage_create_diary_file();
  if (file_err != STORAGE_OK &&
      file_err != STORAGE_DIARY_FILE_ALREADY_CREATED) {
    return file_err;
  }

  FILE *fptr = fopen(filedir, "a");

  fprintf(fptr, "%s\n%s", date, text);

  fclose(fptr);

  return STORAGE_OK;
}

StorageError storage_get_current_diary_file_dir(char *out_dir) {
  char dia_dir[PATH_MAX];

  StorageError storage_err = storage_get_dir(dia_dir);
  if (storage_err != STORAGE_OK &&
      storage_err != STORAGE_DIARY_DIR_ALREADY_CREATED) {
    return storage_err;
  }

  char date[16];
  StorageError datetime_err = datetime_get_current_date(date, sizeof(date));
  if (datetime_err != STORAGE_OK) {
    return datetime_err;
  }

  const char *extension = ".txt";

  int n = snprintf(out_dir, PATH_MAX, "%s/%s%s", dia_dir, date, extension);

  if (n < 0 || n >= PATH_MAX) {
    return STORAGE_ERR_PATH_TOO_LONG;
  }

  return STORAGE_OK;
}

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
  case STORAGE_FAILED_TO_CREATE_FILE:
    return "storage failed to create file";
  case STORAGE_ERR_GETCWD_FAILED:
    return "storage failed to get current directory";
  case STORAGE_DIARY_DIR_ALREADY_CREATED:
    return "storage diary directory is already set";
  case STORAGE_DIARY_FILE_ALREADY_CREATED:
    return "storage diary file is already set";
  }
  return "unknown error";
}
