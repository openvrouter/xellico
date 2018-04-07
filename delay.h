
#pragma once
#include <stdint.h>

inline void dirty_looped_delay (uint32_t nb_loop)
{
  volatile uint32_t vcount = nb_loop;
  while (vcount --);
}

