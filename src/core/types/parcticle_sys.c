#include "core/types/particle_sys.h"
#include "core/core_data.h"

#include "stb/stb_ds.h" 

particle_sys_t* particle_sys_arr = NULL;
u32             particle_sys_arr_len = 0;

u32*            particle_sys_dead_arr = NULL;
u32             particle_sys_dead_arr_len = 0;


bump_alloc_t*   particle_arr = NULL;
u32             particle_arr_len = 0;

u32*            particle_dead_arr = NULL;
u32             particle_dead_arr_len = 0;

int particle_sys_add(particle_sys_t ps)
{
  bump_alloc_t alloc;

  u32 particle_max = start_particle_count + ( particle_per_sec * lifetime );
  bump_alloc_init(&alloc, sizeof(particle_t) * particle_max);

  // replace dead or add new
  u32 particle_idx = -1; 
  if (particle_dead_arr_len > 0)
  {
    u32 idx = arrpop(particle_dead_arr);
    particle_dead_arr_len--;
    particle_arr[idx] = alloc;
    particle_idx = idx; 
  }
  else
  {
    arrput(particle_arr, alloc);
    particle_arr_len++;
    particle_idx = particle_arr_len -1;
  }

  // replace dead or add new
  u32 particle_sys_idx = -1;
  if (particle_sys_dead_arr_len > 0)
  {
    u32 idx = arrpop(particle_sys_dead_arr);
    particle_sys_dead_arr_len--;
    particle_sys_arr[idx] = ps;
    particle_sys_idx = idx; 
  }
  else
  {
    arrput(particle_sys_arr, ps);
    particle_sys_arr_len++;
    particle_sys_idx = particle_sys_arr_len -1;
  }
 
  // init particle_sys
  particle_sys_t* ps_ptr   = &particle_sys_arr[particle_sys_idx];
  ps_ptr->is_alive         = true;
  ps_ptr->particle_arr_idx = particle_idx;
  ps_ptr->particle_max     = particle_max;
  particle_sys_init(ps_ptr);

  return particle_sys_idx;
}
void particle_sys_init(particle_sys_t* ps)
{
  // add start particles, etc.

  bump_alloc_t* particles = &particle_arr[ps->particle_idx];
  // u32 particles_len       = particles->size / sizeof(particle_t);
  particle_t* p = bump_alloc(particles, sizeof(particle_t));
  // emit ...
}
void particle_sys_remove(int idx)
{
  // mark particle_sys_t dead
  partlicle_sys_arr[i].is_alive = false;
  arrput(particle_sys_dead_arr, idx);
  particle_sys_dead_arr_len++;

  // free particles and mark dead
  bump_alloc_free(&particle_arr[ps_ptr->particle_arr_idx]);
  arrput(particle_dead_arr, ps_ptr->particle_arr_idx);
  particle_dead_arr_len++;
}

void particle_sys_update()
{
  for (int i = 0; partlicle_sys_arr_len; ++i)
  {
    particle_sys_t* ps = &partlicle_sys_arr[i];
    
    // lifetime
    ps->lifetime -= core_data->delta_t;
    if (ps->lifetime <= 0) { particle_sys_remove(); continue; }

    // simulate particles, and add/remove
    bump_alloc_t* particles = &particle_arr[ps->particle_idx];
    u32 particles_len       = particles->size / sizeof(particle_t); 
    for (int p = 0; p < particles_len; ++p)
    {
      // simulate
      
      // add / remove particles
    }

  }
}

void particle_sys_render()
{
}

