#include "storage.h"
#include "string.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

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
  }
  return "unknown error";
}

StorageError storage_create_app_directory(void) {
  const char *appdir;
  storage_alloc_app_dir(&appdir);

  if (access(appdir, F_OK) != 0) {
    int status = mkdir(appdir, 0755);
    if (status != 0)
      printf("status: %d\n ", status);
    return STORAGE_FAILED_TO_CREATE_DIRECTORY;
  }

  storage_free_dir(appdir);
  return STORAGE_OK;
}

StorageError storage_init(void) {
  StorageError err = storage_create_app_directory();
  if (err != STORAGE_OK) {
    return err;
  }
  return STORAGE_OK;
};

StorageError storage_alloc_app_dir(const char **out_dir) {
  if (out_dir == NULL) {
    return STORAGE_ERR_INVALID_ARGUMENT;
  }

  const char *location = getenv("HOME");
  if (!location) {
    return STORAGE_ERR_NOT_FOUND;
  }

  const char *app_path = ".local/share/dia";

  size_t needed_len = strlen(location) + strlen(app_path) + 2;

  if (needed_len > PATH_MAX) {
    return STORAGE_ERR_PATH_TOO_LONG;
  }

  char *full_path = (char *)malloc(needed_len);

  if (full_path == NULL) {
    return STORAGE_ERR_MALLOC_FAIL;
  }

  int len = snprintf(full_path, needed_len, "%s/%s", location, app_path);

  if (len < 0 || (size_t)len > needed_len) {
    storage_free_dir(full_path);
    return STORAGE_ERR_INVALID_PATH;
  }

  *out_dir = full_path;
  return STORAGE_OK;
}

StorageError storage_create_file(const char *filename) {
  const char *appdir;
  StorageError err = storage_alloc_app_dir(&appdir);
  if (err != STORAGE_OK) {
    return err;
  }

  size_t path_len = strlen(appdir) + 1 + strlen(filename) + 1;

  char *fullpath = malloc(path_len);
  if (!fullpath) {
    storage_free_dir(appdir);
    return STORAGE_ERR_MALLOC_FAIL;
  }

  snprintf(fullpath, path_len, "%s/%s", appdir, filename);

  storage_free_dir(appdir);

  FILE *fptr = fopen(fullpath, "w");
  free(fullpath);
  fullpath = NULL;

  if (!fptr) {
    return STORAGE_ERR_NO_PERMISSION;
  }

  fprintf(fptr, "base text");
  fclose(fptr);

  return STORAGE_OK;
}

// StorageError storage_save_file(const char **out_dir) { return STORAGE_OK; }

void storage_free_dir(const char *dir) {
  free((void *)dir);
  dir = NULL;
}
