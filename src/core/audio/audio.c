#include "core/audio/audio.h"
#include "core/camera.h"
#include "core/core_data.h"

#include "global/global_types.h"
#include "miniaudio/extras/miniaudio_split/miniaudio.h"
#include "miniaudio/miniaudio.h"

#include "stb/stb_ds.h"


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
u32  music_queue_len   = 0;  // length of valid entries in music_arr
u32  music_queue_pos   = 0;  // current position playing in music_arr
bool music_queue_playing = false;
#define MUSIC_QUEUE_NEXT(_idx)   { music_queue_pos++; _idx = music_queue_pos >= music_queue_len ? music_queue[0] : music_queue[music_queue_pos]; }

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

sound_t* sounds_arr = NULL;
int      sounds_arr_len = 0; 


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
    engineConfig.listenerCount = 1; // spacial audio

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
  ma_device_uninit(&device);
  // ma_decoder_uninit(&decoder);
  ma_engine_uninit(&engine);
  // ma_engine_uninit(&music_engine);
  ma_resource_manager_uninit(&resourceManager);
}

u32 audio_load_audio(const char* name, sound_type_flag type, f32 volume)
{
  if ( HAS_FLAG(type, SOUND_TYPE_CLIP) && HAS_FLAG(type, SOUND_TYPE_MUSIC) )
  {
    P_SOUND_TYPE_FLAG(type);
    ERR("can only have one of SOUND_TYPE_... in flag\n");
  }
  if ( HAS_FLAG(type, SOUND_TYPE_MUSIC) && music_queue_pos >= MUSIC_QUEUE_MAX )
  {
    P_ERR("tried to add sound '%s' to music queue but already at max capacity: %d\n", name, MUSIC_QUEUE_MAX);
    return -1;
  }

  char path[ASSET_PATH_MAX +64];
  SPRINTF(ASSET_PATH_MAX +64, path, "%saudio/%s", core_data->asset_path, name);

  sound_t sound = {0};
  arrput(sounds_arr, sound);
  sound_t* s = &sounds_arr[sounds_arr_len];
  s->type        = type;
  s->volume      = volume;
  s->has_fence   = false;
  s->stopped_pos = 0;
  #ifdef EDITOR
  ASSERT(strlen(path) < SOUND_T_PATH_MAX);
  STRCPY(s->path, path);
  #endif

  
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
      music_queue[music_queue_len++]   = sounds_arr_len;  // queue still has space check done at start
      soundConfig.endCallback          = audio_music_end_callback;
      soundConfig.pEndCallbackUserData = &music_queue[sounds_arr_len];  // ptr to idx
      soundConfig.flags                = MA_SOUND_FLAG_STREAM; 
    }
    else 
    {
      // clips get unique bus
      soundConfig.initialAttachmentInputBusIndex = engine_clip_bus_pos++; 
      soundConfig.flags       = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC;
      if ( ma_fence_init(&s->fence) != MA_SUCCESS ) { ERR("failed to init sound fence\n"); }
      soundConfig.pDoneFence  = &s->fence;
      s->has_fence = true;
    }
    // soundConfig.channelsIn  = 1;
    // soundConfig.channelsOut = 0;    // Set to 0 to use the engine's native channel count.
    soundConfig.isLooping   = false;
    if ( ma_sound_init_ex(&engine, &soundConfig, &s->sound) != MA_SUCCESS )
    { ERR("failed to load sound file, %s\n", path); }
  }
    
  if (HAS_FLAG(type, SOUND_TYPE_MUSIC)) // music specific
  {

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

  return sounds_arr_len++;
}

void audio_play_sound_complex(u32 idx, f32 volume, bool spatial, vec3 pos)
{
  ERR_CHECK(idx >= 0, "sound idx smaller than 0, is invalid: %d\n", idx);
  ERR_CHECK(idx <  sounds_arr_len, "sounds idx: %d, is bigger than arr_len: %d\n", idx, sounds_arr_len);
  sound_t* s = &sounds_arr[idx];

  // wait for audio to load
  // ma_fence_wait(&s->fence);
  // P_V(s->fence.counter);
  if (s->has_fence && s->fence.counter > 0)
  { P_INFO("didnt play sound: %d, hadnt loaded yet\n", idx); return; }

  if (ma_sound_is_playing(&s->sound))
  { 
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

void audio_start_music_queue()
{
  ERR_CHECK(music_queue_len > 0, "trying to play music queue without adding songs first, use audio_load_audio()\n");
  u32 idx = music_queue[0];
  music_queue_pos = 0;
  
  // ERR_CHECK(idx >= 0, "sound idx smaller than 0, is invalid: %d\n", idx);
  // ERR_CHECK(idx <  sounds_arr_len, "sounds idx: %d, is bigger than arr_len: %d\n", idx, sounds_arr_len);
  // sound_t* s = &sounds_arr[idx];
  // // wait for audio to load
  // ma_fence_wait(&s->fence);
  
  audio_play_sound(idx, sounds_arr[idx].volume);
  music_queue_playing = true;
}
void audio_stop_music_queue()
{
  if (!music_queue_playing) { return; }
  sound_t* s = &sounds_arr[music_queue[music_queue_pos]];
  ma_sound_get_cursor_in_pcm_frames(&s->sound, &s->stopped_pos);
  ma_sound_stop(&s->sound);
  music_queue_playing = false;
}
void audio_resume_music_queue()
{  
  if (music_queue_playing) { return; }
  u32 idx = music_queue[music_queue_pos];
  sound_t* s = &sounds_arr[idx];
  audio_play_sound(idx, s->volume);
  ma_sound_seek_to_pcm_frame(&s->sound, s->stopped_pos); 
  music_queue_playing = true;
}
void audio_toggle_music_queue()
{
  if (music_queue_playing)
  { audio_stop_music_queue(); }
  else 
  { audio_resume_music_queue(); }
}

void audio_data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
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
  // music_queue_pos++;
  // u32 idx = music_queue_pos >= music_queue_len ? music_queue[0] : music_queue[music_queue_pos];
  u32 idx;
  MUSIC_QUEUE_NEXT(idx);
  audio_play_sound(idx, sounds_arr[idx].volume);
}
