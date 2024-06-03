#include "core/io/file_io.h"
#include "math/math_inc.h"

#include <ctype.h>
#include <direct.h>
#include <dirent/dirent.h>

// file_io_get_info()
#include <sys/stat.h>


int file_io_check_exists_dbg(const char* file_path, const char* _file, const int _line)
{
  (void)_file; (void)_line;
  TRACE();

  bool exists = true;
  FILE* f = fopen(file_path, "rb");
  if (f == NULL) 
  { exists = false; return false; }
  fclose(f);
  return exists;
}

// @TODO: theres more info in struct stat
file_info_t file_io_get_info(const char* path)
{
  file_info_t info;
  info.path   = (char*)path;
  info.exists = false;

  struct stat file_stats;
  if ( stat(path, &file_stats) != 0 ) { return info; }

  info.exists         = true;
  info.t_created      = file_stats.st_ctime;
  info.t_last_opened  = file_stats.st_atime;
  info.t_last_changed = file_stats.st_mtime;
  // info.last_status_change = file_stats.st_ctime;

  info.size = (u32)file_stats.st_size;

  info.permissions = 0;
  if (file_stats.st_mode & _S_IREAD)  { info.permissions |= FILE_PERMISSION_READ;    } 
  if (file_stats.st_mode & _S_IWRITE) { info.permissions |= FILE_PERMISSION_WRITE;   } 
  if (file_stats.st_mode & _S_IEXEC)  { info.permissions |= FILE_PERMISSION_EXECUTE; }  

  return info;
}


char* file_io_read_dbg(const char* file_path, const char* _file, const int _line)
{
  TRACE();

	printf("called read_text_file()\n");  // fucks this 
  FILE* f;
  char* text;
  long len;

  f = fopen(file_path, "rb");
  ERR_CHECK(f != NULL, "loading text-file at: %s \n  -> file: \"%s\", line: %d", file_path, _file, _line);

  // get len of file
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  ASSERT(len > 0);
  fseek(f, 0, SEEK_SET);

  // alloc memory 
  MALLOC(text, (size_t)len * sizeof(char));
  
  // fill text buffer
  fread(text, 1, (size_t)len, f);
  ASSERT(strlen(text) > 0);
  fclose(f);

  printf("text: %s\n", text); // fucks this then

  (void)_file; (void)_line;
  return text;
}
char* file_io_read_len_dbg(const char* file_path, int* length, const char* _file, const int _line)
{
  TRACE();

  FILE* f;
  char* text;
  long len;

  f = fopen(file_path, "rb");
  ERR_CHECK(f != NULL, "loading text-file at: %s \n  -> file: \"%s\", line: %d", file_path, _file, _line);

  // get len of file
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  ASSERT(len > 0);
  fseek(f, 0, SEEK_SET);

  // alloc memory 
  MALLOC(text, (size_t)len * sizeof(char));

  // fill text buffer
  fread(text, sizeof(char), (size_t)len, f);
  ASSERT(strlen(text) > 0);
  fclose(f);

  *length = len;
  
  (void)_file; (void)_line;
  return text;
}
u8* file_io_read_bytes_dbg(const char* file_path, int* length, const char* _file, const int _line)
{
  TRACE();

  FILE* f;
  u8* text;
  long len;

  f = fopen(file_path, "rb");
  if (f == NULL) { P_ERR("loading text-file at: %s \n  -> file: \"%s\", line: %d", file_path, _file, _line); return NULL; }

  // get len of file
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  ASSERT(len > 0);
  fseek(f, 0, SEEK_SET);

  // alloc memory 
  MALLOC(text, (size_t)len * sizeof(char));

  // fill text buffer
  fread(text, sizeof(char), (size_t)len, f);
  fclose(f);

  *length = len;
  
  (void)_file; (void)_line;
  return text;
}
void file_io_write_dbg(const char* file_path, const char* txt, int len, const char* _file, const int _line)
{
  TRACE();

  FILE* f;

  f = fopen(file_path, "wb");
  ERR_CHECK(f != NULL, "loading text-file at: %s \n  -> file: \"%s\", line: %d", file_path, _file, _line);

  int rtn = (int)fwrite(txt, sizeof(char), (size_t)len, f);
  ASSERT(rtn != EOF);
  // fprintf(f, "%s", txt);

  fclose(f);
  
  (void)_file; (void)_line;
}
void file_io_write_bytes_dbg(const char* file_path, const u8* data, int len, const char* _file, const int _line)
{
  TRACE();

  FILE* f;

  f = fopen(file_path, "wb");
  ERR_CHECK(f != NULL, "loading text-file at: %s \n  -> file: \"%s\", line: %d", file_path, _file, _line);

  int rtn = (int)fwrite(data, sizeof(u8), (size_t)len, f);
  ASSERT(rtn != EOF);
  // fprintf(f, "%s", txt);

  fclose(f);
  
  (void)_file; (void)_line;
}

void file_io_search_dir_for_file_type(const char* dir_path, const char* ext, search_dir_return_f* return_f)
{
  TRACE();

  char path[256];
  struct dirent* dp;
  DIR* dir = opendir(dir_path);
  // unable to open directory stream
  if (!dir) { return; }
  

  // recursively read the directory and its sub-directories
  while ((dp = readdir(dir)) != NULL)
  {
    // check that the file currently read isn't a directory
    if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
    {
      if (strlen(dp->d_name) <= strlen(ext)) { return;}

      char* file_ending = dp->d_name + strlen(dp->d_name) - strlen(ext);
      if (strcmp(file_ending, ext) == 0)
      {
        char buf[300];
        // add slash if missing
        char* slash = dir_path[strlen(dir_path) -1] == '/'  ? "" :
                      dir_path[strlen(dir_path) -1] == '\\' ? "" : "\\";
        SPRINTF(300, buf, "%s%s%s", dir_path, slash, dp->d_name);
        
        return_f(buf, dp->d_name);
      }

      // construct new path from our base path
      strcpy(path, dir_path);
      path[strlen(path) -1] = '\0';
      strcat(path, "\\");
      strcat(path, dp->d_name);
      
      file_io_search_dir_for_file_type(path, ext, return_f);
    }
  }

  // close the stream
  closedir(dir);
}

