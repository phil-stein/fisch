#ifndef EDITOR_SAVE_H
#define EDITOR_SAVE_H


#define EDITOR_SAVE_VERSION     2
#define EDITOR_SAVE_EXTENSION   ".editor_info"
#define EDITOR_SAVE_NAME        EDITOR_SAVE_EXTENSION // "editor"EDITOR_SAVE_EXTENSION

#ifdef __cplusplus
extern "C" {
#endif

void editor_save_init();
void editor_save_cleanup();

void editor_save_info();
void editor_load_info();

void editor_save_write_info_to_file();
void editor_save_load_info_from_file();

#ifdef __cplusplus
} // extern c
#endif

#endif // EDITOR_SAVE_H
