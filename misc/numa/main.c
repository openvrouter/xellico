
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "pre_rte.h"

int main(int argc, char** argv)
{
  printf(" #socket: %u\n", pre_rte_socket_count());
  printf(" #core  : %u\n", pre_rte_lcore_count());
  printf("\n\n");
  printf(" core    socket id      \n");
  printf("------------------------\n");
  const uint32_t n_cores = pre_rte_lcore_count();
  for (uint32_t lcore_id=0; lcore_id<n_cores; lcore_id++) {
    printf(" %-8u%u\n", lcore_id, pre_rte_lcore_to_socket_id(lcore_id));
  }
}

