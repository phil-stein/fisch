#ifndef CORE_FILE_IO_H
#define CORE_FILE_IO_H


#include "global/global.h"
#include "core/types/mesh.h"
#include "global/global_print.h"

#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

// @DOC: used to return info from file_io_search_dir_for_file_type()
typedef  void (search_dir_return_f)(const char* path, const char* name);

typedef enum
{
  FILE_PERMISSION_READ    = FLAG(0),
  FILE_PERMISSION_WRITE   = FLAG(1),
  FILE_PERMISSION_EXECUTE = FLAG(2),
  
} file_permissions_flag;
typedef struct
{
  char* path;

  bool exists;

  file_permissions_flag permissions;
 
  u32 size;

  time_t t_created;
  time_t t_last_opened;
  time_t t_last_changed;
  // time_t last_status_change;  // idk

} file_info_t;
INLINE void file_io_print_file_info_t(file_info_t* info, const char* name)
{
  PF_COLOR(PF_CYAN); _PF("%s", name);         PF_STYLE_RESET(); _PF(":\n");
  PF_COLOR(PF_CYAN); _PF("  path");           PF_STYLE_RESET(); _PF(":           %s\n", info->path);
  PF_COLOR(PF_CYAN); _PF("  exists");         PF_STYLE_RESET(); _PF(":         %s\n", STR_BOOL(info->exists));
  if (!info->exists) { return; }
  PF_COLOR(PF_CYAN); _PF("  size");           PF_STYLE_RESET(); _PF(":           %d\n", info->size);
  PF_COLOR(PF_CYAN); _PF("  t_created");      PF_STYLE_RESET(); _PF(":      %s", ctime(&info->t_created));
  PF_COLOR(PF_CYAN); _PF("    -> ");          PF_STYLE_RESET(); _PF("%lld sec\n", info->t_created);
  PF_COLOR(PF_CYAN); _PF("  t_last_opened");  PF_STYLE_RESET(); _PF(":  %s", ctime(&info->t_last_opened));
  PF_COLOR(PF_CYAN); _PF("    -> ");          PF_STYLE_RESET(); _PF("%lld sec\n", info->t_last_opened);
  PF_COLOR(PF_CYAN); _PF("  t_last_changed"); PF_STYLE_RESET(); _PF(": %s", ctime(&info->t_last_changed));
  PF_COLOR(PF_CYAN); _PF("    -> ");          PF_STYLE_RESET(); _PF("%lld sec\n", info->t_last_changed);
  (void)info; (void)name;
}
#define P_FILE_INFO_T(_info)  file_io_print_file_info_t(&(_info), #_info)


// @DOC: returns true if the file under the specified path "file_path" exists, otherwise false
//       file_path: path to the file, including file name
int file_io_check_exists_dbg(const char* file_path, const char* _file, const int _line);
#define file_io_check_exists(p)   file_io_check_exists_dbg((p), __FILE__, __LINE__)

// @DOC: get info about file at path
//       see file_info_t
//       file_info_t.exists:false on fail
//       ~ use ctime() to stringify time_t
file_info_t file_io_get_info(const char* path);

// @DOC: read a text-file and output all contents as a char*
//       !!! free() the returned char* as it gets allocated
//       taken from: https://github.com/jdah/minecraft-weekend/blob/master/src/gfx/shader.c
//       file_path: path to the file, including file name
char* file_io_read_dbg(const char* file_path, const char* _file, const int _line);
#define file_io_read(p)   file_io_read_dbg((p), __FILE__, __LINE__)

// @DOC: read the given lengt of a text-file and output all contents as a char*
//       !!! free() the returned char* as it gets allocated
//       taken from: https://github.com/jdah/minecraft-weekend/blob/master/src/gfx/shader.c
//       file_path: path to the file, including file name
//       length:    gets set to the length of the returned file
char* file_io_read_len_dbg(const char* file_path, int* length, const char* _file, const int _line);
#define  file_io_read_len(file_path, length) file_io_read_len_dbg((file_path), (length), __FILE__, __LINE__)

// @DOC: read binary file
//       file_path: path to the file, including file name
//       length:    gets set to the length of the returned file
u8* file_io_read_bytes_dbg(const char* file_path, int* length, const char* _file, const int _line);
#define file_io_read_bytes(path, len) file_io_read_bytes_dbg((path), (len), __FILE__, __LINE__)

// @DOC: writes text 'txt' into file at 'file_path', creates file if it doesnt exist
//       file_path: path to the file, including file name
//       txt:       text to be written
//       len:       the length of 'txt', or shorter if you want to cut off the string
void file_io_write_dbg(const char* file_path, const char* txt, int len, const char* _file, const int _line);
#define file_io_write(p, txt, l)  file_io_write_dbg((p), (txt), (l), __FILE__, __LINE__)

// @DOC: writes bytes 'data' into file at 'file_path', creates file if it doesnt exist
//       file_path: path to the file, including file name
//       data:      bytes to be written
//       len:       the length of 'txt', or shorter if you want to cut off the string
void file_io_write_bytes_dbg(const char* file_path, const u8* data, int len, const char* _file, const int _line);
#define file_io_write_bytes(p, d, l)    file_io_write_bytes_dbg((p), (d), (l), __FILE__, __LINE__)

// @DOC: search a directory for all files with the extension ext, all found files are passed to the given
//       function return_f as complete path and file name
//       dir_path: directory to be searched
//       ext:      extension to be checked, i.e. '.jpg', '.txt', etc., not necesseraly starting in .
//       return_f: this function gets called for every found file and passed the path and name of that file
void file_io_search_dir_for_file_type(const char* dir_path, const char* ext, search_dir_return_f* return_f);

#ifdef __cplusplus
} // extern C
#endif

#endif
