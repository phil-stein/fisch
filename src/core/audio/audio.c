#include "core/audio/audio.h"
#include "core/camera.h"
#include "core/core_data.h"

#include "global/global_types.h"
#include "miniaudio/extras/miniaudio_split/miniaudio.h"
#include "miniaudio/miniaudio.h"

#include "stb/stb_ds.h"

INLINE void audio_print_ma_sound_flag(ma_sound_flags f, const char* name, const char* _file, const char* _func, const int _line)
{
  PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(":\n");
 
  P_FLAG_MEMBER(f, MA_SOUND_FLAG_STREAM);
  P_FLAG_MEMBER(f, MA_SOUND_FLAG_DECODE);
  P_FLAG_MEMBER(f, MA_SOUND_FLAG_ASYNC);
  P_FLAG_MEMBER(f, MA_SOUND_FLAG_WAIT_INIT);
  P_FLAG_MEMBER(f, MA_SOUND_FLAG_UNKNOWN_LENGTH);
  P_FLAG_MEMBER(f, MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT);
  P_FLAG_MEMBER(f, MA_SOUND_FLAG_NO_PITCH);
  P_FLAG_MEMBER(f, MA_SOUND_FLAG_NO_SPATIALIZATION);
  _PF("\n");
  _PF_IF_LOC(_file, _func, _line);
 
  (void)f; (void)name;
  (void)_file; (void)_func; (void)_line;
}
#define P_MA_SOUND_FLAGS(_f) audio_print_ma_sound_flag(_f, #_f, __FILE__, __func__, __LINE__)

ma_result result;
ma_engine engine;
ma_device device;
// ma_decoder decoder;
ma_resource_manager resourceManager;

u32 engine_clip_bus_pos = SOUND_CLIP_BUS_START;

// cant have stb-arr bc. need the u32 to be in the same spot in memory
// bc. using pointer to u32 in soundConfig.pEndCallbackUserData
// u32* music_queue_arr = NULL;
// u32  music_queue_arr_len = 0;
#define MUSIC_QUEUE_MAX 12
u32  music_queue[MUSIC_QUEUE_MAX];
int  music_queue_len   = 0;  // length of valid entries in music_arr
int  music_queue_pos   = 0;  // current position playing in music_arr
bool music_queue_playing = false;
#define MUSIC_QUEUE_NEXT()   { music_queue_pos = music_queue_pos+1 >= music_queue_len ? 0 : music_queue_pos +1; }
#define MUSIC_QUEUE_PREV()   { music_queue_pos = music_queue_pos-1 < 0 ? music_queue_len -1 : music_queue_pos -1; }

// ma_sound sound;
// ma_sound_config sound_config;
// ma_fence fence;
#define SOUND_T_PATH_MAX 256
typedef struct
{
  sound_type_flag type;
  ma_sound sound;
  ma_fence fence;
  bool has_fence;
  f32 volume;
  ma_uint64 stopped_pos;
  #ifdef EDITOR
  char path[SOUND_T_PATH_MAX];
  #endif

} sound_t;

// cant use stb_ds dyn array, bc. audio gets streamed from memory
// sound_t* sounds_arr = NULL;
// int      sounds_arr_len = 0; 
// @TODO: make linked list type thing
//        maybe linked list of sound_t[8], etc.
#define SOUNDS_ARR_MAX 32
sound_t sounds_arr[SOUNDS_ARR_MAX];
u32     sounds_arr_len = 0; 



void audio_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
void audio_music_end_callback(void* _p, ma_sound* sound);

#define AUDIO_CAM_LISTENER_IDX 0

void audio_init()
{
  { // device 
    ma_device_config deviceConfig;
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    // deviceConfig.playback.format   = decoder.outputFormat;
    // deviceConfig.playback.channels = decoder.outputChannels;
    // deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = audio_data_callback;
    // deviceConfig.pUserData         = &decoder;
    if ( ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS ) 
    {
      // ma_decoder_uninit(&decoder);
      ERR("failed to load sound device\n");
    }
  }
 
  { // resource manager
    ma_resource_manager_config config;
    config = ma_resource_manager_config_init();
    // config.decodedFormat     = device.playback.format;
    // config.decodedChannels   = device.playback.channels;
    // config.decodedSampleRate = device.sampleRate;
    config.decodedFormat     = ma_format_f32;    /* ma_format_f32 should almost always be used as that's what the engine (and most everything else) uses for mixing. */
    config.decodedChannels   = 0;                /* Setting the channel count to 0 will cause sounds to use their native channel count. */
    config.decodedSampleRate = 48000;            /* Using a consistent sample rate is useful for avoiding expensive resampling in the audio thread. This will result in resampling being performed by the loading thread(s). */

    if ( ma_resource_manager_init(&config, &resourceManager) != MA_SUCCESS )
    {
      ERR("Failed to initialize the resource manager.");
      ma_device_uninit(&device);
    }
  }
  // { // context, manages devices
  //     /* We're going to want a context so we can enumerate our playback devices. */
  //   result = ma_context_init(NULL, 0, NULL, &context);
  //   if (result != MA_SUCCESS) {
  //       printf("Failed to initialize context.");
  //   }
  //   /*
  //   Now that we have a context we will want to enumerate over each device so we can display them to the user and give
  //   them a chance to select the output devices they want to use.
  //   */
  //   result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, NULL, NULL);
  //   if (result != MA_SUCCESS) {
  //       printf("Failed to enumerate playback devices.");
  //       ma_context_uninit(&context);
  //   }
  // }
  { // clip engine
    ma_engine_config engineConfig;
    engineConfig = ma_engine_config_init();
    engineConfig.pDevice = &device;
    engineConfig.pResourceManager = &resourceManager;
    engineConfig.listenerCount = 1; // spacial audio, MA_ENGINE_MAX_LISTENERS

    if ( ma_engine_init(&engineConfig, &engine) != MA_SUCCESS ) 
    { ERR("failed to init clip audio engine\n"); }
    ma_engine_set_volume(&engine, core_data->audio.clip_master_volume);
    ma_engine_listener_set_world_up( &engine, AUDIO_CAM_LISTENER_IDX, 0, 1, 0);
    // ma_engine_listener_set_cone(     &engine, listenerIndex, innerAngleInRadians, outerAngleInRadians, outerGain);
    // spacial audio:
    //   // positive X points right, positive Y points up and negative Z points forward
    //   ma_engine_listener_set_position(&engine, listenerIndex, worldPosX, worldPosY, worldPosZ);
    //   ma_engine_listener_set_direction(&engine, listenerIndex, forwardX, forwardY, forwardZ);
    //   ma_engine_listener_set_world_up(&engine, listenerIndex, 0, 1, 0);
    //   ma_engine_listener_set_cone(&engine, listenerIndex, innerAngleInRadians, outerAngleInRadians, outerGain);
    // time:
    //   ma_engine_get_time_in_pcm_frames()
    //   ma_engine_set_time_in_pcm_frames()
  }
}

void audio_update()
{
  vec3 p, f;
  vec3_copy(core_data->cam.pos, p);
  camera_get_front(f);
  ma_engine_listener_set_position( &engine, AUDIO_CAM_LISTENER_IDX, p[0], p[1], p[2]);
  ma_engine_listener_set_direction(&engine, AUDIO_CAM_LISTENER_IDX, f[0], f[1], f[2]);
}

void audio_cleanup()
{
  audio_clear();

  ma_device_uninit(&device);
  // ma_decoder_uninit(&decoder);
  ma_engine_uninit(&engine);
  // ma_engine_uninit(&music_engine);
  ma_resource_manager_uninit(&resourceManager);
}

void audio_clear()
{
  for (int i = 0; i < (int)sounds_arr_len; ++i)
  { ma_sound_uninit(&sounds_arr[i].sound); }
  sounds_arr_len  = 0; 
  music_queue_len = 0;  
  music_queue_pos = 0;
}

u32 audio_load_audio(const char* name, sound_type_flag type, f32 volume)
{
  if ( HAS_FLAG(type, SOUND_TYPE_CLIP) && HAS_FLAG(type, SOUND_TYPE_MUSIC) )
  {
    P_SOUND_TYPE_FLAG(type);
    ERR("can only have one of SOUND_TYPE_... in flag\n");
    return AUDIO_INVALID_IDX;
  }
  if ( HAS_FLAG(type, SOUND_TYPE_MUSIC) && music_queue_len >= MUSIC_QUEUE_MAX )
  {
    ERR("tried to add sound '%s' to music queue but already at max capacity: %d|%d\n", 
          name, music_queue_len, MUSIC_QUEUE_MAX);
    return AUDIO_INVALID_IDX;
  }

  if (sounds_arr_len+1 >= SOUNDS_ARR_MAX) { ERR("cant add anymore sounds\n"); }

  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX +64, path, "%saudio/%s", core_data->asset_path, name);

  // sound_t sound = {0};
  // arrput(sounds_arr, sound);
  sound_t* s = &sounds_arr[sounds_arr_len++];
  s->type        = type;
  s->volume      = volume;
  s->has_fence   = false;
  s->stopped_pos = 0;
  #ifdef EDITOR
  ASSERT(strlen(path) < SOUND_T_PATH_MAX);
  STRCPY(s->path, path);
  #endif
  u32 idx = sounds_arr_len -1;

  // _PF("sound: %u | ", idx); P_STR(name);
  { // sound
    // MA_SOUND_FLAG_DECODE: decode sound file before playing, plays faster
    // MA_SOUND_FLAG_ASYNC:  load async. needs fence 
    // MA_SOUND_FLAG_STREAM: stream from file, for larger sound files
    // MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT: dont attach to sound-graph, i think pGroup needs to be NULL
    // MA_SOUND_FLAG_NO_SPATIALIZATION: disable spacialization
    ma_sound_config soundConfig;
    soundConfig = ma_sound_config_init();
    soundConfig.pFilePath   = path; // Set this to load from a file path.
    // soundConfig.pDataSource = NULL; // Set this to initialize from an existing data source. // @TODO:
    // soundConfig.pInitialAttachment = NULL;
    if (HAS_FLAG(type, SOUND_TYPE_MUSIC))
    { 
      // attach all music to same bus
      soundConfig.initialAttachmentInputBusIndex = SOUND_MUSIC_BUS_INDEX;
      // put idx into queue-arr
      music_queue[music_queue_len++]   = idx;  // queue still has space check done at start
      soundConfig.endCallback          = audio_music_end_callback;
      soundConfig.pEndCallbackUserData = &music_queue[idx];  // ptr to idx
      soundConfig.flags                = MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION; 
      s->has_fence = false;
      // _PF(" -> music: %u | %u | ", idx, music_queue[idx]); P_STR(name);
    }
    else 
    {
      // clips get unique bus
      soundConfig.initialAttachmentInputBusIndex = engine_clip_bus_pos++; 
      soundConfig.flags       = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC;
                                // | (MA_SOUND_FLAG_NO_SPATIALIZATION * !HAS_FLAG(type, SOUND_SPATIAL));
      if ( ma_fence_init(&s->fence) != MA_SUCCESS ) { ERR("failed to init sound fence\n"); }
      soundConfig.pDoneFence  = &s->fence;
      s->has_fence = true;
      // _PF(" -> clip:  %u | ", idx); P_STR(name);
    }
    // P_MA_SOUND_FLAGS(soundConfig.flags);

    // soundConfig.channelsIn  = 1;
    // soundConfig.channelsOut = 0;    // Set to 0 to use the engine's native channel count.
    soundConfig.isLooping   = false;
    if ( ma_sound_init_ex(&engine, &soundConfig, &s->sound) != MA_SUCCESS )
    { ERR("failed to load sound file, %s\n", path); }
  }

  // { // decoder
  //   ma_decoder_config decoderConfig = ma_decoder_config_init_default();
  //   result = ma_decoder_init_file(path, &decoderConfig, &decoder);
  //   if (result != MA_SUCCESS) 
  //   { ERR("failed to load sound file\n"); }
  // }

  // // if ( ma_sound_init_from_file(&engine, path, 0, NULL, NULL, &sound) != MA_SUCCESS ) 
  // if ( ma_sound_init_from_file(&engine, path, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, NULL, &s->fence, &s->sound)  != MA_SUCCESS )
  // { ERR("failed to load sound file\n"); }
  // ma_sound_set_pan()
  // spatial audio:
  //   ma_sound_set_spatialization_enabled(&sound, isSpatializationEnabled); // true by default
  //   ma_sound_set_pinned_listener_index(&sound, listenerIndex);
  //   ma_sound_set_positioning(&sound, ma_positioning_relative);
  //   ma_sound_set_position(&sound, posX, posY, posZ);
  //   ma_sound_set_direction(&sound, forwardX, forwardY, forwardZ);
  //   ma_sound_set_cone(&sound, innerAngleInRadians, outerAngleInRadians, outerGain);
  //   ma_sound_set_velocity(&sound, velocityX, velocityY, velocityZ);
  //   ma_sound_set_attenuation_model(&sound, ma_attenuation_model_inverse);
  //   -> ma_attenuation_model_inverse // default
  //   -> ma_attenuation_model_linear
  //   -> ma_attenuation_model_exponential
  //   ma_sound_set_rolloff(&sound, rolloff);
  //   ma_sound_set_min_gain(&sound, minGain);
  //   ma_sound_set_max_gain(&sound, maxGain);
  //   ma_sound_set_min_distance(&sound, minDistance);
  //   ma_sound_set_max_distance(&sound, maxDistance);
  //   ma_sound_set_doppler_factor(&sound, dopplerFactor);
  // fading:
  //   // Fade in over 1 second.
  //   ma_sound_set_fade_in_milliseconds(&sound, 0, 1, 1000);
  //   // ... sometime later ...
  //   // Fade out over 1 second, starting from the current volume.
  //   ma_sound_set_fade_in_milliseconds(&sound, -1, 0, 1000);
  // start stop:
  //   // Start the sound in 1 second from now.
  //   ma_sound_set_start_time_in_pcm_frames(&sound, ma_engine_get_time_in_pcm_frames(&engine) + (ma_engine_get_sample_rate(&engine) * 1));
  //   // Stop the sound in 2 seconds from now.
  //   ma_sound_set_stop_time_in_pcm_frames(&sound, ma_engine_get_time_in_pcm_frames(&engine) + (ma_engine_get_sample_rate(&engine) * 2));
  //   ma_sound_is_playing()        // takes fading, sceduling etc. into acount
  //   ma_sound_at_end()            // should never be true for looping sound
  //   ma_sound_set_end_callback()  // void my_end_callback(void* pUserData, ma_sound* pSound), can also use ma_sound_config with _ex load func 
  // looping:
  //   ma_sound_set_looping()
  //   ma_sound_is_looping()
  // data source:
  //   ma_sound_seek_to_pcm_frame(&sound, frameIndex);
  //   ma_sound_get_data_format(&sound, &format, &channels, &sampleRate, pChannelMap, channelMapCapacity);
  //   ma_sound_get_cursor_in_pcm_frames(&sound, &cursor);
  //   ma_sound_get_length_in_pcm_frames(&sound, &length);

  return idx;
}

void audio_play_sound_complex(u32 idx, f32 volume, bool spatial, vec3 pos)
{
  // ERR_CHECK(idx >= 0, "sound idx smaller than 0, is invalid: %d\n", idx);
  ERR_CHECK(idx <  sounds_arr_len, "sounds idx: %d, is bigger than arr_len: %d\n", idx, sounds_arr_len);
  sound_t* s = &sounds_arr[idx];

  // wait for audio to load
  // ma_fence_wait(&s->fence);
  // P_V(s->fence.counter);
  if (s->has_fence && s->fence.counter > 0)
  { P_INFO("didnt play sound: %d, hadnt loaded yet\n", idx); return; }

  if (ma_sound_is_playing(&s->sound))
  {
    // // 60862
    // ma_node_dettach_output_bus
    // // 60904, 64420, 64424
    // ma_node_attach_output_bus
    // ma_sound_stop(&sound); 
    ma_sound_seek_to_pcm_frame(&s->sound, 0); // go to start
  }

  ma_sound_set_volume(&s->sound, volume);
  if (spatial)
  { ma_sound_set_position(&s->sound, pos[0], pos[1], pos[2]); }
  else
  { ma_sound_set_spatialization_enabled(&s->sound, false); } // true by default

  if (ma_sound_start(&s->sound) != MA_SUCCESS) 
  {
    ma_sound_uninit(&s->sound);
    // ma_decoder_uninit(&decoder);
    ERR("failed to play sound\n");
  }
  // ma_sound_stop()

  // if (ma_device_start(&device) != MA_SUCCESS) 
  // {
  //   ma_device_uninit(&device);
  //   ma_decoder_uninit(&decoder);
  //   P_ERR("failed to play sound\n");
  // }
}

void audio_music_queue_start()
{
  ERR_CHECK(music_queue_len > 0, "trying to play music queue without adding songs first, use audio_load_audio()\n");
  u32 idx = music_queue[0];
  music_queue_pos = 0;
  
  // P_INT(music_queue_len);
  // P_INT(music_queue_pos);
  // for (int i = 0;  i < music_queue_len; ++i)
  // {
  //   P_INT(i);
  //   P_INT(music_queue[i]);
  //   P_STR(sounds_arr[music_queue[i]].path);
  // }
  
  audio_play_sound(idx, sounds_arr[idx].volume);
  music_queue_playing = true;
}
void audio_music_queue_stop()
{
  if (!music_queue_playing) { return; }
  ERR_CHECK(music_queue_len > 0, "trying to play music queue without adding songs first, use audio_load_audio()\n");
  sound_t* s = &sounds_arr[music_queue[music_queue_pos]];
  ma_sound_get_cursor_in_pcm_frames(&s->sound, &s->stopped_pos);
  ma_sound_stop(&s->sound);
  music_queue_playing = false;
}
void audio_music_queue_resume()
{  
  if (music_queue_playing) { return; }
  ERR_CHECK(music_queue_len > 0, "trying to play music queue without adding songs first, use audio_load_audio()\n");
  u32 idx = music_queue[music_queue_pos];
  sound_t* s = &sounds_arr[idx];
  audio_play_sound(idx, s->volume);
  ma_sound_seek_to_pcm_frame(&s->sound, s->stopped_pos); 
  music_queue_playing = true;
}
bool audio_music_queue_toggle()
{
  if (music_queue_playing)
  { audio_music_queue_stop(); }
  else 
  { audio_music_queue_resume(); }
  return music_queue_playing;
}
void audio_music_queue_next()
{
  bool was_playing = false;
  if (music_queue_playing) 
  { 
    audio_music_queue_stop(); 
    was_playing = true;
  }
  MUSIC_QUEUE_NEXT();
  u32 idx = music_queue[music_queue_pos];
  sound_t* s = &sounds_arr[idx];
  s->stopped_pos = 0;
  if (was_playing) { audio_music_queue_resume(); }
}
void audio_music_queue_prev()
{
  bool was_playing = false;
  if (music_queue_playing) 
  { 
    audio_music_queue_stop(); 
    was_playing = true;
  }
  MUSIC_QUEUE_PREV();
  P_V(music_queue_pos);
  u32 idx = music_queue[music_queue_pos];
  P_V(idx);
  sound_t* s = &sounds_arr[idx];
  s->stopped_pos = 0;
  if (was_playing) { audio_music_queue_resume(); }
}
char* audio_music_queue_get_current(u32* idx)
{
  ERR_CHECK(music_queue_len > 0, 
            "trying to get song from music queue without adding songs first, use audio_load_audio()\n");
  *idx = music_queue[music_queue_pos];
  #ifdef EDITOR
  return sounds_arr[music_queue[music_queue_pos]].path;
  #else
  return "sound.path only available when EDITOR defined";
  #endif
}

void audio_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  (void)pDevice; (void)pInput;
  // PF("%s, %p, %p, %p, %u\n", __func__, pDevice, pOutput, pInput, frameCount);
  ma_engine_read_pcm_frames(&engine, pOutput, frameCount, NULL);
  // ma_engine_read_pcm_frames((ma_engine*)pDevice->pUserData, pOutput, frameCount, NULL);

  // ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
  // if (pDecoder == NULL) 
  // {
  //   P_ERR("decoder is NULL\n");
  //   return;
  // }
  // ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
  // (void)pInput;
}

void audio_music_end_callback(void* _p, ma_sound* sound)
{
  (void)_p; (void)sound;
  if (music_queue_len <= 0) { ERR("shouldt happen\n"); return; }
  // music_queue_pos++;
  // u32 idx = music_queue_pos >= music_queue_len ? music_queue[0] : music_queue[music_queue_pos];
  
  MUSIC_QUEUE_NEXT();
  u32 idx = music_queue[music_queue_pos];
  P_V(sounds_arr_len);
  P_V(music_queue_len);
  P_V(music_queue_pos);
  P_V(idx);
  P_SOUND_TYPE_FLAG(sounds_arr[idx].type);
  audio_play_sound(idx, sounds_arr[idx].volume);
  // ma_sound_stop_with_fade_in_milliseconds
}
