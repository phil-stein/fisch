#ifndef CORE_AUDIO_AUDIO_H
#define CORE_AUDIO_AUDIO_H

#include "global/global.h"
#include "math/math_inc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  SOUND_TYPE_CLIP  = FLAG(0),  // cant have SOUND_TYPE_..., need one 
  SOUND_TYPE_MUSIC = FLAG(1),  // cant have SOUND_TYPE_..., need one 
  SOUND_SPATIAL    = FLAG(2),
  SOUND_LOOP       = FLAG(3),

} sound_type_flag;
INLINE void audio_print_sound_type_flag(sound_type_flag state, const char* name, const char* _file, const char* _func, const int _line)
{
  PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(":\n");
 
  PF_COLOR(HAS_FLAG(state, SOUND_TYPE_CLIP)  ? PF_GREEN : PF_RED); _PF("SOUND_TYPE_MUSIC"); PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, SOUND_TYPE_MUSIC) ? PF_GREEN : PF_RED); _PF("SOUND_TYPE_MUSIC"); PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, SOUND_SPATIAL)    ? PF_GREEN : PF_RED); _PF("SOUND_SPATIAL");    PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, SOUND_LOOP)       ? PF_GREEN : PF_RED); _PF("SOUND_LOOP");       // PF_STYLE_RESET(); _PF(" | ");
  _PF("\n");
  _PF_IF_LOC(_file, _func, _line);
}
#define P_SOUND_TYPE_FLAG(v)  audio_print_sound_type_flag((v), #v, __FILE__, __func__, __LINE__)

// @DOC: overflown or (UINT_MAX-1000)->(UINT_MAX), are invalid
//       this way -1 is invalid
#define AUDIO_INVALID_IDX         (UINT_MAX - 1000) // has to overflow 1001 to break
#define AUDIO_IDX_IS_VALID(_idx)  ( (_idx) < AUDIO_INVALID_IDX)
                          
// bus given to all SOUND_TYPE_MUSIC sounds
#define SOUND_MUSIC_BUS_INDEX  0   
// bus given to SOUND_TYPE_CLIP sounds, gets incremented for each clip
#define SOUND_CLIP_BUS_START  (SOUND_MUSIC_BUS_INDEX +1)   

void audio_init();
void audio_update();
void audio_cleanup();

u32 audio_load_audio(const char* name, sound_type_flag type, f32 volume);
#define audio_load_clip(_name, _type)     audio_load_audio(_name, (_type) | SOUND_TYPE_CLIP, 1.0f)
#define audio_load_music(_name, _volume)  audio_load_audio(_name, SOUND_TYPE_MUSIC , _volume)

void audio_play_sound_complex(u32 idx, f32 volume, bool spatial, vec3 pos);
#define audio_play_sound(_idx, _volume) audio_play_sound_complex(_idx, _volume, false, VEC3(0))
#define audio_play_sound_spatial(_idx, _volume, _pos) audio_play_sound_complex(_idx, _volume, true, _pos)

void audio_music_queue_start();
void audio_music_queue_stop();
void audio_music_queue_resume();
bool audio_music_queue_toggle();
void audio_music_queue_next();
void audio_music_queue_prev();
char* audio_music_queue_get_current(u32* idx);


#ifdef __cplusplus
} // extern C
#endif

#endif  // CORE_AUDIO_AUDIO_H
