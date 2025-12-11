#pragma once

typedef enum {
  STORAGE_OK,
  STORAGE_ERR_NOT_FOUND,
  STORAGE_ERR_MALLOC_FAIL,
  STORAGE_ERR_PATH_TOO_LONG,
  STORAGE_ERR_INVALID_PATH,
  STORAGE_ERR_PERMISSION,
  STORAGE_ERR_INVALID_ARGUMENT,
  STORAGE_ERR_NO_PERMISSION,
  STORAGE_FAILED_TO_CREATE_DIRECTORY,
  STORAGE_FAILED_TO_CREATE_FILE,
  STORAGE_ERR_GETCWD_FAILED,
  STORAGE_DIARY_DIR_ALREADY_CREATED,
  STORAGE_DIARY_FILE_ALREADY_CREATED,
} StorageError;

const char *storage_strerror(StorageError err);

StorageError storage_get_dir(char *directory);

StorageError storage_create_dir(void);

StorageError storage_init(void);

StorageError storage_get_current_diary_file_dir(char *out_dir);

StorageError storage_create_diary_file(void);

StorageError storage_append_diary_text(const char *text);

void storage_free_dir(const char *dir);
