
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define PRE_RTE_MAX_NUMA_NODES 8 /* dpdk/config/common_base */
#define PRE_RTE_MAX_LCORE 128 /* dpdk/config/common_base */
#define PRE_NUMA_NODE_PATH "/sys/devices/system/node"

uint32_t
pre_eal_cpu_socket_id(uint32_t lcore_id)
{
  for (uint32_t sock=0; sock<PRE_RTE_MAX_NUMA_NODES; sock++) {
    char path[256];
    snprintf(path, sizeof(path), "%s/node%u/cpu%u",
        PRE_NUMA_NODE_PATH, sock, lcore_id);
    int ret = access("/node10/cpu0", F_OK);
    if (ret == 0) return sock;
  }
  return 0;
}

uint32_t
pre_rte_socket_count(void)
{
  int32_t prev_socket_id = -1;
  uint32_t numa_node_count = 0;
  for (uint32_t lcore_id=0; lcore_id<PRE_RTE_MAX_LCORE; lcore_id++) {
    uint32_t socket_id = pre_eal_cpu_socket_id(lcore_id);
    if (socket_id != prev_socket_id)
      numa_node_count ++;
    prev_socket_id = socket_id;
  }
  return numa_node_count;
}

int main(int argc, char** argv)
{
  printf(" #socket: %u\n", pre_rte_socket_count());
  printf("\n\n");
  printf(" core    socket id      \n");
  printf("------------------------\n");
  printf(" %-8u%u\n", 0, pre_eal_cpu_socket_id(0));
  printf(" %-8u%u\n", 1, pre_eal_cpu_socket_id(1));
}

