
# -- fisch --

3d engine, refactor of [bovengine](https://github.com/phil-stein/bovengine), which was a refactor of [mooh_engine](https://github.com/phil-stein/mooh_engine) <br>
written in C<br>

**still under development | unstable**

using:      [glfw3](https://www.glfw.org/), [glad](https://glad.dav1d.de/), [stb_image & stb_ds](https://github.com/nothings/stb), [nuklear](https://github.com/Immediate-Mode-UI/Nuklear), [thread.h](https://github.com/mattiasgustavsson/libs/blob/main/docs/thread.md), [tiny file dialogs](https://github.com/LazyJazz/tinyfiledialogs) [miniaudio](https://github.com/mackron/miniaudio) <br>
and my own: [math-library](https://github.com/phil-stein/math), [global-defines](https://github.com/phil-stein/global), [serialization-library](https://github.com/phil-stein/serialization), [text-library](https://github.com/phil-stein/text)


build-sys: cmake -> [mingw64](https://github.com/niXman/mingw-builds-binaries?tab=readme-ov-file) and [msvc, visual studio 22](https://visualstudio.microsoft.com/vs/), [used before: tdm64](https://github.com/jmeubank/tdm-gcc)<br>
  -> mingw64 settings: gcc version 13.2.0, 64bit, posix-threads, msvcrt<br>
  -> visual studio 22: 64bit, [clang-cl](https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/build/clang-support-cmake.md)

main resources:  
  - [learnopengl.com](https://learnopengl.com/)
  - "3D Math Primer for Graphics and Game Development" by Dunn Parberry
  - "Game Physics Engine Development" by Ian Millington
  - ['the cherno' youtube](https://www.youtube.com/c/TheChernoProject)
  - ['winterdev' youtube](https://www.youtube.com/c/Winterdev)


## current capabilities

<!-- <img src="https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot03.png" alt="logo" width="800"> <br> -->

| ![screenshot03](https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot03.png) | ![screenshot04](https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot04.png) |
| ---            | ---            |
| [screenshot03](https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot03.png) | [screenshot04](https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot04.png) |
| ![screenshot05](https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot05.png) | ![screenshot02](https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot02.png) |
| [screenshot05](https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot05.png) | [screenshot02](https://github.com/phil-stein/fisch/blob/main/_assets/project_files/screenshot02.png) |
- renderer <br>
    - pbr & ibl & hdr <br>
    - direct draw <br>
  - ui <br>
  - serialization <br>
  - scene-manager <br>
  - level editor <br>
  - physics<br>
    - dynamics <br>
    - sphere <br>
    - aabb <br>

# -- todo --

## next steps
  -> `X`   important <br>
  -> `WIP` work in progress
  - *organization*
    - [release system](#organization) `X`
    - [rest of organization](#organization) `WIP`
  - *buggs*
    - [all buggs](#buggs)
  - *optimizations* 
    - [custom asset archive files](#optimizations) `WIP`
    - [multithreading](#multithreading) `WIP` 
    - [batch renderer](#optimizations) `WIP`
  - *base*
  - [ui (mui)](#in-game ui - mui) `WIP`
  - [multiple meshes](#base) ? `WIP`
  - *editor*
    - [undo/redo](#level editor) `WIP`
  - *graphics*
    - [blending](#renderer) `X`
    - [cascaded shadows](#renderer) `X`
    - [ssao](#renderer) `X`
    - [aa](#renderer) `X` `WIP`
    - [bloom](#renderer)
    - [particle system](#base) `X` `WIP` 
    - [trail renderer](#base) `WIP`
  - *advanced*
    - [physics engine](#physics-engine) `X` `WIP`
    - [audio](#audio-system) `X`
    - [animation](#animation-system) `X`
    - [chunking for ents/phys_objs](#optimizations)
    - [hot reloading](https://github.com/clibequilibrium/hot-reloading-sample)

## port from mooh
  - __todo__
    - [ ] clean up this todo list
    - [ ] finish scripting
      - [x] remove scripts
        - [ ] test REMOVE_SCRIPT()
    - [x] terrain isnt working
      - make terrain addon ?
        so have it be entirely optional ?
      - [ ] make it save materials into .terrain
    - [x] camera shake
      - coroutines ? (also mentioned below in -c)
    - [ ] set window title from app.c / test.c, currently in program.c 
    - [ ] changed camera_set_front(), maybe derive pitch/yaw from dir-vec
    - [ ] put members of core_data/app_data/game_data into anonymous structs
    - assetm
      - [ ] save .tex files as image.png -> image.png.tex, so image.png isnt the same as image.jpg
        - [ ] .tex
        - [ ] .mesh
          - [ ] change asset_get_mesh("name") -> asset_get_mesh("name.fbx")
      - [ ] integrate audio files
      - [ ] make shader be in .sh files or some with vertex and fragment combined
      - [ ] compile
        - [ ] spir-v ?
        - [ ] turn shaders into header files
      - [ ] add like // $[color_func_location] or some to add custom shader code automatically 
        - like have a .sh file that specifies wich .sh file it extends and then combine them at runtime
        - #include in glsl ?
    - editor
      - [ ] app_data window / settings window
      - [x] save/load editor cam pos&front selected_id and window state
        - [ ] serialize ui_rect for floating-windows so they are in the same spot, size
        - [ ] editor save also saves if exit during play-mode, not sure if i want that
      - [ ] add icons to executables, .rc file
        - [x] kinda works but doesnt update
        - [x] doesnt show in window or sidebar
        - [ ] [do in glfw](https://stackoverflow.com/questions/44321902/load-icon-function-for-glfwsetwindowicon)
      - [ ] local / global gizmos
        - [x] move
        - [ ] rotate
        - [ ] scale
        - [ ] gizmo scales with scale
      - [ ] set app_data->mouse_over_ui in each _ win() func, using nk_window_get_bounds(ctx);
      - [ ] editor tools ?
        - like state_scripts.h but for editor windows, so games can extend editor with custom tools ?
      - [x] debug graph with total frame time and renderer/scripts/editor/physics/etc.
      - [x] use operations.c to check if quitting with unsaved changes and guve tinyfiledialogs.h notification 
    - phys
      - [ ] add excluding objs to raycasts
        - [x] by id
        - [ ] by tag
          - make tag arr's in state
      - [ ] compile phys in/out via -DPHYS_IMPLEMENTATION or some
        - [ ] put program_sync_phys() in own file
        - so i can add jolt / bullet in future
      - [ ] multithread phys 
      - [ ] terrain collisions
      - [ ] ray v. triangle for terrain, see casey muratory simd video
    - c
      - [ ] make string type in global ?
        - with length in struct, is more secure
      - [ ] check out [msan, asan, tsan](https://skia.org/docs/dev/testing/xsan/)
      - [x] check out [c attributes](https://en.cppreference.com/w/c/language/attributes)
        - only in c23, using c18
        - [ ] destructor for cleanup functions
      - [x] static assert
        - [ ] test
      - [ ] coroutines
        - [cute_coroutines.h](https://github.com/RandyGaul/cute_headers_deprecated/blob/master/cute_coroutine.h), [video](https://www.youtube.com/watch?v=MuCpdoIEpgA)
        - [minicoro](https://github.com/edubart/minicoro)
      - [ ] hot reloading [cr.h](https://github.com/fungos/cr) [cr.h-example](https://github.com/clibequilibrium/hot-reloading-sample) 
      - [ ] tracing
        - [gcc-page](https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html)
        - [some-blog](https://www.linuxjournal.com/content/simplifying-function-tracing-modern-gcc)
        - [stackoverflow-seems-interesting](https://stackoverflow.com/questions/3899870/how-to-print-a-stack-trace-whenever-a-certain-function-is-called)
        - [gnu-page](https://www.gnu.org/software/libc/manual/html_node/Tracing-malloc.html)
        - [gprof](https://yuchen52.medium.com/profiling-with-gprof-64-bit-window-7-5e06ef614ba8)
      - [ ] get symbols into [remedybg](https://remedybg.itch.io/remedybg), [strip-exe-for-.debug-file](https://stackoverflow.com/questions/4679097/gcc-equivalent-of-pdbs)[same](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Separate-Debug-Files.html), [gcc-docs](https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)
      - [ ] [gdb](https://stackoverflow.com/questions/70830151/gdb-on-windows-machine), [sample-session](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Sample-Session.html#Sample-Session), [winpty-for-term-gui](https://stackoverflow.com/questions/77720184/cannot-enable-the-tui-when-output-is-not-a-terminal-gdb-with-gitbash), [term-gui-workaround](https://stackoverflow.com/questions/69474919/tui-working-on-cmd-line-but-not-in-mintty)
      - [ ] __COUNTER__ in msvc, nuklear has __COUNTER__ in nk _.._ hashed
    - mui
      - [ ] ui-scenes, aka. have save_sys_scene make scene with one entity with mui code, (main-menu, settings, etc.)
      - [ ] mui nine-patch images
      - [ ] mui shader based shapes
        - [ ] rounded rectangle `WIP`
          - kinda works, need to 'draw circle' at each corner instead of one big circle
      - [ ] mui slider
      - [ ] mui drop-down
      - [ ] make mui_draw_img() func
      - [ ] mui MUI_LEFT / MUI_RIGHT seems to change wich side of the screen text appears on
      - [ ] make mui submodule
      - [ ] 3d ui, i.e. nametags/healthbars above players/enemies
    - renderer
      - [ ] specular-occlusion to fix the fresnel effect being to intense
    - audio
      - [ ] stop / continue sound clips
      - [ ] have multiple music queues, i.e. game, pause-menu, etc. 3 should be enough
        - have sound_type_flag like SOUND_TYPE_MUSIC | SOUND_MUSIC_QUEUE_01
      - [ ] playing same sound again stops last sound from playing
      - [ ] fade in/out music queue
      - [ ] @UNSURE: store sound_t's in assetm, at least for clips, prob.
  - __optimizations__
    - [ ] ? multithreading asset_io
      - [ ] not really faster
    - [ ] make bump/arena allocator [optimizations](#optimizations) `WIP`
      - [ ] replace malloc/calloc/realloc with bump_alloc `WIP` 
    - [ ] use glGetUniformLocation() to not have to specify uniforms by name, is faster
    - [x] dont malloc names in assetm
      - turn string names into u32, assetm_str_to_u64()
    - [ ] INLINE serialization functions 
    - [x] dont asset_get_...() in renderer get mesh_t* arr = assetm_get_mesh_arr(), etc. `X`
    - [ ] debug_draw_...() with GL_LINE_STRIP not 10 debug_draw_line()
      - [ ] debug_draw_box()
      - [ ] debug_draw_circle()
  - __buggs__
    - [ ] highlights are weird pattern makes object look weird [[file:_assets/project_files/highlight_bugg_01.png|img]] `X` `WIP`
      - brdf_lut in pbr.frag  
        - may be bc. is rg not rgb and loading .tex is wrong
        - generating brdf_lut in core_data fixes it
        - brdf_lut is 16bit float but saved as 8bit int
    - [ ] save_sys_write_to_terrain() gets called even if core_data->terrain_name isnt set
    - [ ] fix tracing
    - [ ] input_get_mouse_pressed(MOUSE_LEFT) doesnt work in game
      - prob stolen by nuklear
    - [ ] tex_viewer doesnt compile since changing make with fisch.cmake etc. 
    - [ ] fix including math header causing errors
    - [ ] __physics doesnt seem to scale forces according to delta_t properly__ `X`
    - [ ] exiting play-mode sometimes causes error: tried attaching point light to entity that already has one -> file: C:\Workspace\C\fisch\src\core\state\state.c -> func: state_point_light_add, line: 800
      - robot enemy with point-light
    - [x] glfw mouse button & scroll callbacks `X`
      - works in nuklear, look at that
      - actually nuklear is stealing our callback from glfw
      - [x] mouse button
      - [x] scroll
    - [ ] mui mouse over obj ist super accurate
    - [ ] mui buttons dont work in game
      - prob. input mouse being screwed in game like left-click
    - [ ] input set cursor visible doesnt work in game 
      - prob. input mouse being screwed in game like left-click
    - [ ] setting ASSERT_FIX_USE_FIX doesnt work
          - crashes in game anyway in mui.c mui_text()
          - but first ASSERT_FIX seems to work, maybe ASSERT_FIX doesnt
  - __sus__
    - [ ] state.c line 68 prob. the same as next
    - [ ] state.c line 55 -> 60, template_entity_idxs_arr
    - [ ] mui.c mui_text() positioning, mul by 1.02 and 0.98 and no idea why
      - prob. bc. text is pixel-space and shapes are screen-spac, but idk

## buggs
  - [ ] obj sizes are different than in blender
    - there is a apply transform option but its experimental
  - [ ] terrain-chunks dont get culled properly 
  - [ ] minimizing window to sys tray causes framebuffer crash [also mentioned here]()
  - [ ] freeing cubemap doesnt seem to actually free any memory
  - [ ] changing WINDOW_MIN, _ MAX, _ FULL doesnt work
  - [ ] changing THREAD_MAX in threadm.c doesnt affect its speed [also mentioned](#multithreading)
  - [ ] gizmo is fcked [also mentioned](#base)
  - [ ] emissive textures overwrite roughness/metallic, dont think thats right
  - [ ] _bin\Debug folder has a lot of unnecessary files in it

## optimizations
  - [ ] [multithreading](#multithreading) 
  - [ ] use textures with r = roughness, g = metallic, ...
  - [ ] change the white tex for no normal
  - [ ] occlusion culling 
  - [ ] batch renderer `WIP`
    - [x] keep track of draw calls 
    - [x] keep track of all template ents
    - [ ] mark static, in template / editor ?
    - [ ] instanced draw all static templates
  - [ ] lod system ?
  - [ ] octree or something for chunks, for entities / phys_objs
  - [ ] custom asset formats
    - [ ] cubemaps 
  - [ ] custom asset archive files | not in use
    - all textures, etc. back to back in one file with a simple header
    - type specific or agnostic ?
      - i.e. just serialize a bunch of buffers or have mesh/texture
    - screws with multithreading, kind of, also very big files
    - maybe just a couple at a time ?
    - [x] textures | not in use 
    - [ ] meshes
    - [ ] shaders
  - [ ] only clear outline buffer when deselecting
  - [ ] memory allocation optimization
    - [ ] read game engine architecture page 427-
    - [ ] stack allocator   ?
    - [ ] frame allocator   ?
    - [ ] pool allocator    ?
    - [ ] chunk allocator   ?
    - [ ] arena allocator   ?
    - [ ] randy memory pool ?
  - [x] figure out why core_data_init() is taking so long: shaders
    - [ ] shader spir-v ?
  - [ ] undo/redo -> john jackman or some on yt pixeleditor
  - [ ] [text batch rendering](#in-game ui - mui)
  - [ ] [mui batch rendering](#in-game ui - mui)
  - [ ] m_lerp(), vec3_lerp() arent working correct see program.c
  - [ ] ecs_manager_updat() entity model update & child/parent logic refactor, is kinda wonky
  - [x] replace cursed if tree in input with switch 
  - [ ] scale forces properly

## sus amogus
  - when parenting broke and i fixed it by setting 'is_moved' in 'state_update_global_model()'
    it worked before and something changed in the code that made it no longer work that way
    what exactly changed ???
  - ENTITY_SET_POS() doesnt work properly in program_sync_physics(), need to set velocity to 0
  - zip archive not faster than straight files
  - __init__() gets called before main loop regardless if editor or game

## organization
  - [ ] release system
    - [ ] package assets & exe
      - obv, just the custom / needed assets
      - >editor -pack_assets / >editor -pack_game
        - go through entity table and pack all mentioned meshes / materials->textures
        - default assets mentioned in material_template.c, etc.
          - textures.archive & meshes.archive
          - index at the to with u32 identifier for asset "name", u32 index into file and u32 length
      - [ ] set asset_path to cwd/assets
    - [ ] maybe check if folder build_01 exist and name build_02, etc.
    - [ ] put date in folder / exe name or .txt file or some
    - [ ] use static lib instead of dll
      - [ ] glfw
      - [ ] flecs (compiling as target in cmake does this, but not linking directly)
  - [ ] seperate save_sys.c into terrain & entities
  - [ ] program_app_default_logic() -> cant do debug stuff in game
    - setting defines in makefile gives error
  - [ ] implement glfw opengl debug context, learnopengl page 439 ?
  - [ ] check shaders via reference compiler, page 444, debug_opengl.h
  - [ ] framebuffer debug, page 444, debug_opengl.h
  - [ ] add missing P_TYPE() funcs to global.h
  - [ ] add icons to exe
    - [ ] texture_view
    - [ ] editor
  - [x] make global submodule again, check if phys, serialization are still submodules
  - [ ] remove pos  maybe also cast_shadow, etc. from dir light
  - [ ] add [tracing](https://balau82.wordpress.com/2010/10/06/trace-and-profile-function-calls-with-gcc/)
    - either figure out a way to decode function pointer to names in app, print on ERR()
    - or do it in a file like the tutorial
  - [ ] add [profiling](https://ftp.gnu.org/old-gnu/Manuals/gprof-2.9.1/html_mono/gprof.html), [tutorial](https://www.thegeekstuff.com/2012/08/gprof-tutorial/)
  - [ ] make most funcs dbg error checked ? (tracing would make this redundant) 
    - [ ] use dbg funcs for materials in assetm ?
    - [x] make dbg check a macro to set easy and compile out ?
      - when compiled out file & line cause errors
      - are file & line impacting performance ?
    - [ ] state
    - [ ] etc.
  - [ ] go through all files check for unecessary header includes, to remove clutter
  - [ ] make functions with entity->id as arg have entity_t* version also, i.e. state_entity_remove(entity_t* e)
    - state_entity_remove(entity_t* e);
      INLINE state_entity_remove_id(int id) { state_entity_get(id); remove }
  - [ ] finish math_test.h `WIP`
  - [ ] make state.c world_arr & world_arr_len available in core_data as pointer
  - [ ] move structure save/load from save_sys.c to editor_save.c ?
  - [ ] track memory allcations through MALLOC(), FREE(), etc. and stb_ds.h
  - [x] move internal shaders, aka. deferred, lighting, etc. to core/templates/shader_templates

## tools
  - [ ] binary dump
    - [x] empty scene
    - [ ] empty terrain
  - [ ] "model-viewer/-editor" for shaders / materials / anim / particles
  - [ ] tracing / profiling ?

## base
  - [ ] asset manager
    - [ ] free assets
      - [x] textures
      - [ ] meshes
      - [ ] cubemaps [doesnt work](#buggs) `WIP`
      - [ ] shaders
    - [ ] load shaders from zip  ?
    - [ ] load cubemaps from zip ?
    - [ ] handle missing assets
  - [ ] hot-reload shaders
  - [ ] make seperate editor map file so f.e. structures can be stored special and edited all at once ? 
  - [ ] setup 32bit (-m32) : [tdm docs](https://github.com/jmeubank/tdm-distrib/blob/master/tdm64/core/README-gcc-tdm64.md)
    - [ ] reset all dynamic objects ? 
      - this way we don't have to reload the entire thing 
      - not sure if needed in final game though
      - requires restructuring probably
      - reload suprisingly fast
    - [ ] game saves system
      - single map file, never changed
      - multiple variants of that with changes
        - i.e. 'save01.scene', 'mygame.scene', etc.
  - [ ] billboards
  - [ ] particle system
    - cpu / gpu side or mix ?
    - [ ] pooling
    - [ ] billboards
    - [ ] different spawn "shapes"
      - [ ] circle
      - [ ] cone
      - [ ] sphere
      - [ ] cylinder ?
    - [ ] animated textures
    - [ ] integrate trail renderer
  - [ ] trail renderer
  - [ ] debug tools
    - [ ] make debug_draw_circle() func use mat4 and vec3 instead of pythag `WIP`
      - precompute x, y, z circles for axis aligned, like sphere collider
    - [ ] add profiler [also mentioned](#organization)
  - [ ] controller support ?
  - [x] keep_transform when parenting / un-parenting 
  - [ ] multiple meshes per entity `WIP`
  - [ ] input make key-locale
    - glfw uses qwerty, no matter the actual keyboard locale, i.e. qwertz in my case
    - i remapped z to y as a hotfix
    - maybe make key_locale array that mapps qwertz, etc. to qwerty

## renderer
  - [ ] blending
    - learnopengl weighted blending 'OIT'
    - [[file:C:/Users/phili/Documents/#04_LearnOpenGL/#guest_01_OIT/learnopengl-com-Guest-Articles-2020-OIT-Introduction.pdf|introduction]]
    - [[file:C:/Users/phili/Documents/#04_LearnOpenGL/#guest_01_OIT/learnopengl-com-Guest-Articles-2020-OIT-Weighted-Blended.pdf|tutorial]]
  - [ ] cascaded shadows
    - from cherno video 
    - also on learnopengl
  - [ ] ssao
  - [ ] bloom
  - [ ] custom anti aliasing, i.e. fxaa
    - [x] try built in again 
      - dont work, white around objs
  - [ ] lut tone mapping
  - [ ] water
  - [x] emissive texture (easy)

## entity-system

## level-editor
  - [x] pause mid game
  - [ ] generate struct literal for 
    - [x] materials
    - [ ] entities
    - [ ] shaders
  - [ ] particle system editor
    - seperate ?
  - [ ] make gui less shit
  - [ ] ask to save if unsaved changes
    - use operation.c
  - [ ] seperate windows into their own .c files
  - [ ] undo `WIP`
    - [ ] keep track of changes
  - [ ] redo
  - [x] deselect on play, reselect after
    - save custom editor info to file 

## physics-engine
  - [miniphysics](https://github.com/mackron/miniphysics/blob/master/miniphysics.h), [ntcd](https://github.com/Grieverheart/ntcd)
  - 3rd party engine ?
    - [jolt physics](https://github.com/jrouwe/JoltPhysics)
      - [dagor engine](https://github.com/GaijinEntertainment/DagorEngine) 
    - [bullet](https://github.com/bulletphysics/bullet3)
      - [bullet candle, c binding](https://github.com/EvilPudding/bullet.candle) 
      - _deprecated_ [unofficial c-bindings](https://github.com/chriscamacho/bulletCapi)
  - [ ] chunking for phys_obj_t
    - [quadtree](https://github.com/Antymon/quadtree), [magnum-engine-octree](https://magnum.graphics/showcase/octree/), [flecs-octree](https://github.com/flecs-hub/flecs-systems-physics/blob/master/src/octree.c)
    - [ ] collisison detection in chunk
    - [ ] ray casting in chunk
  - [x] ray casting
    - [x] v sphere
    - [x] v aabb
  - [x] swept collision detection
    - [x] sphere v sphere
    - [x] aabb v aabb
    - [x] aabb v sphere
  - [ ] finish only checking collisions
        for every possible combination
        phys_update_new()
    - need to to do both objs in phys_resolution()
    - objs vibrating
    - can just break in phys_update_old(), line: 362

## multithreading
  - maybe make abstraction for thread stuff ?
  - add [profiler](#base) first
  - architecture
    - main (input, gameplay, etc.)
    - worker (asset-loading, terrain, etc.)
    - renderer
    - physics
  - [ ] asset loading
    - [x] textures
      - [ ] changing THREAD_MAX in threadm.c doesnt affect its speed [also mentioned](#buggs)
      - [ ] experiment with thread count
      - [ ] load textures with pbo's, is supposed to be faster `WIP`
      - [ ] multithreaded when not loading arr
    - [ ] meshes
    - [ ] shaders
    - [ ] scene ?
    - [ ] terrain
  - [ ] physics
  - [ ] rendering ?
    - decouple entities from main-thread
    - duplicate world, etc. -> merge
  - [ ] terrain

## audio-system
  - using miniaudio, soloud, sokol, etc.
  - [ ] clips
  - [ ] loops
  - [ ] sfx ?
  - [ ] system
    - queue bg music, ambient, etc.

## animation-system
  - [ ] learnopengl.com
  - [ ] system
  - [ ] blending animations

## artstyle look-dev
  - [ ] draw sketch of scene
    - the base: old wooden house, trees, 
      rusty equipment, overgrown
  - [ ] make 3d polished scene 
    - [ ] stones
      - [ ] small
      - [x] middle
      - [ ] big
    - [ ] trees, [video, kinda also rocks and plants and stuff](https://www.youtube.com/watch?v=q8WL1Z1W4Gs)
      - [ ] normal
      - [x] big (fucked normals)
    - [ ] bush
    - [ ] fence
    - [ ] old hut

## terrain generation
  - [ ] normals & tangents on edges
    - button in gui to calc for all chunks

## in-game ui - mui
  - [x] text (already wrote lib)
  - mui_text_f(0.5f, 0.75f) funcs
  - hook rendering into core
  - [x] draw quads 
    - [x] scale with screen
    - [x] textured quads
  - [ ] aligned group hierarchy
    - [x] row / column
    - [ ] centered
    - [ ] left / right
    - [ ] up / down
  - [ ] batch rendering 

## game design
  - [ ] game design doc
  - [ ] ...
