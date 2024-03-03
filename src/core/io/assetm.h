#ifndef CORE_IO_ASSETM_H
#define CORE_IO_ASSETM_H


#include "global/global.h"
#include "core/types/types.h"
#include "core/templates/shader_template.h"

#ifdef __cplusplus
extern "C" {
#endif


// @NOTE: mute or show messages from assetmanager
// #define ASSETM_P(txt)  P(txt)
// #define ASSETM_PF(...) PF(__VA_ARGS__)
#define ASSETM_P(txt)
#define ASSETM_PF(...)

#define TEXTURE_LOAD_DATA_T_NAME_MAX 128
typedef struct
{
  char name[TEXTURE_LOAD_DATA_T_NAME_MAX];
  bool srgb;
  int  idx;
}texture_load_data_t;

// @DOC: turn string into u32 number, unique to that string
//       used for indexing hashmaps in assetm.c
//       assetm_str_to_u32("textures/this/should/do/it/lol/maybe/not_that/be/good/too/images/test///testing/if/too/high/texture_image_albedo_color_normal_.png") = 23534756
INLINE u32  assetm_str_to_u32_len(const char* str, u32 len)
{
  u32 val = 0;
  for (int i = 0; i < len; ++i)
  { val += (u32)str[i] * ( ((u32)i +1) * 28 ); }
  return val;
}
#define assetm_str_to_u32(_str) assetm_str_to_u32_len(_str, strlen(_str)) 



// @DOC: initializes assetm, call this before any other call to assetm
void assetm_init();
// @DOC: frees all allocated resources, call when exiting program
void assetm_cleanup();

// @DOC: checks if .tex file exists or if .png/.jpg is newer than .tex file and converts new .tex file
//       gets compiled out if EDITOR isnt defined
#ifdef EDITOR
void assetm_check_texture_tex_file_func(const char* path, const char* name);
#define assetm_check_texture_tex_file(_path, _name) assetm_check_texture_tex_file_func(_path, _name)
#else // EDITOR
#define assetm_check_texture_tex_file(_path, _name)
#endif // EDITOR

// @DOC: get array of textures registered to get loaded by threadm
texture_load_data_t* assetm_get_texture_register_arr(u32* len);
// @DOC: get array pointer of textures registered to get loaded by threadm
texture_load_data_t** assetm_get_texture_register_arr_ptr(u32** len);
// @DOC: get array of texture_t loaded by assetm and its length
texture_t* assetm_get_texture_arr(int* len);

// @DOC: register a texture to be loaded 
int assetm_register_texture_for_load(const char* name, bool srgb);
// @DOC: overwrite the texture at index idx
void assetm_overwrite_texture_idx(int idx, texture_t* t);
// @DOC: get by its index 
//       idx: index into the assetm array of textures
texture_t* assetm_get_texture_by_idx(int idx);
int assetm_get_texture_idx_dbg(const char* name, bool srgb, const char* _file, const int _line);
texture_t* assetm_get_texture_dbg(const char* name, bool srgb, const char* _file, const int _line);
// void assetm_create_texture_dbg(const char* name, bool srgb, const char* _file, const int _line);
void assetm_create_texture_dbg(const char* name, bool srgb, const char* _file, const int _line);
void assetm_get_texture_data_dbg(const char* name, int* width, int* height, int* channel_num, u8** pixels, const char* _file, const int _line); // @NOTE: no longer in use
int assetm_add_texture(texture_t* tex, const char* name);
// @DOC: get a textures index in the assetm array of textures, used in f.e. assetm_get_texture_by_idx()
//       name: name of texture
//       srgb: if tex isnt loaded yet, this decides if its loaded as a srgb texture for hdr
#define assetm_get_texture_idx(name, srgb)  assetm_get_texture_idx_dbg(name, srgb, __FILE__, __LINE__)
// @DOC: get a texture by its name 
//       name: name of texture
//       srgb: if tex isnt loaded yet, this decides if its loaded as a srgb texture for hdr
#define assetm_get_texture(name, srgb)      assetm_get_texture_dbg(name, srgb, __FILE__, __LINE__)
// @DOC: load and create a texture if its not loaded yet
//       name: name of texture
//       srgb: if tex isnt loaded yet, this decides if its loaded as a srgb texture for hdr
#define assetm_create_texture(name, srgb)   assetm_create_texture_dbg(name, srgb, __FILE__, __LINE__)
// @DOC: get the data associated with a texture, i.e. pixels mostly
//       name:   name of texture
//       width:  gets set to image width in pixels
//       height: gets set to image height in pixels
//       channel_num: gets set to 1 if r 3 if rgb and 4 if rgba
//       pixels:      gets set to pointer to pixel data
#define assetm_get_texture_data(name, width, height, channel_num, pixels) \
                                            assetm_get_texture_data_dbg(name, width, height, channel_num, pixels, __FILE__, __LINE__)


mesh_t* assetm_get_mesh_arr(int* len);
mesh_t* assetm_get_mesh_by_idx_dbg(int idx, const char* _file, const int _line);
int assetm_get_mesh_idx_dbg(const char* name, const char* _file, const int _line);
mesh_t* assetm_get_mesh_dbg(const char* name, const char* _file, const int _line);
void assetm_create_mesh_dbg(const char* name, const char* _file, const int _line);
int assetm_add_mesh(mesh_t* mesh, const char* name);
// @DOC: get a mesh by its idx
//       idx: index into the assetm array of meshes
#define assetm_get_mesh_by_idx(idx)     assetm_get_mesh_by_idx_dbg(idx, __FILE__, __LINE__)
// @DOC: get a meshes idx by its name
//       name: name of the mesh
#define assetm_get_mesh_idx(name)       assetm_get_mesh_idx_dbg(name, __FILE__, __LINE__)
// @DOC: get a mesh by its name
//       name: name of the mesh
#define assetm_get_mesh(name)           assetm_get_mesh_dbg(name, __FILE__, __LINE__)
// @DOC: load/create a mesh by its name if its not already loaded
//       name: name of the mesh
#define assetm_create_mesh(name)        assetm_create_mesh(name, __FILE__, __LINE__)


shader_t* assetm_get_shader_arr(int* len);
shader_t* assetm_get_shader_by_idx_dbg(int idx, const char* _file, const int _line);
int assetm_get_shader_idx_dbg(int type, const char* _file, const int _line);
shader_t* assetm_get_shader_dbg(int type, const char* _file, const int _line);
void assetm_create_shader_dbg(int type, const char* _file, const int _line);
shader_t assetm_create_shader_from_template_dbg(int type, const char* _file, const int line);
// @DOC: get a shader by its idx
//       idx: index into the assetm array of shaders
#define assetm_get_shader_by_idx(idx)               assetm_get_shader_by_idx_dbg(idx, __FILE__, __LINE__)
// @DOC: get a shaders idx by its name
//       type: shader_template_type 
#define assetm_get_shader_idx(type)                 assetm_get_shader_idx_dbg(type, __FILE__, __LINE__)
// @DOC: get a shader by its name
//       type: shader_template_type 
#define assetm_get_shader(type)                     assetm_get_shader_dbg(type, __FILE__, __LINE__)
// @DOC: get a shader by its type
//       type: shader_template_type 
#define assetm_get_shader(type)                     assetm_get_shader_dbg(type, __FILE__, __LINE__)
// @DOC: load/create a shader by its type if its not already loaded
//       type: shader_template_type
#define assetm_create_shader(type)                  assetm_create_shader_dbg(type, __FILE__, __LINE__)
// @DOC: create a shader from the specified type
//       doesnt get put into the assetm array accsessed via assetm_get_shader()
//       call assetm_create_shader() instead
#define assetm_create_shader_from_template(type)    assetm_create_shader_from_template_dbg(type, __FILE__, __LINE__)


// @TODO: use '_dbg' func's and macros for __FILE__ & __LINE__ as well 

material_t* assetm_get_material_arr(int* len);
// @DOC: get a material by its idx
//       idx: index into the assetm array of materials
material_t* assetm_get_material_by_idx_dbg(int idx, const char* _file, const int _line);
#define assetm_get_material_by_idx(idx) assetm_get_material_by_idx_dbg((idx), __FILE__, __LINE__)
// @DOC: get a materials idx by its type
//       type: material_template_type
int assetm_get_material_idx(int type);
// @DOC: get a material by its type
//       type: material_template_type
material_t* assetm_get_material(int type);
// @DOC: load/create a material by its type if its not already loaded
//       type: material_template_type
void assetm_create_material(int type);

#ifdef __cplusplus
} // extern C
#endif

#endif
