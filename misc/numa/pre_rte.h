
#ifndef _PRE_RTE_H_
#define _PRE_RTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define PRE_RTE_MAX_NUMA_NODES 8 /* dpdk/config/common_base */
#define PRE_RTE_MAX_LCORE 128 /* dpdk/config/common_base */
#define PRE_NUMA_NODE_PATH "/sys/devices/system/node"
#define PRE_SYS_CPU_DIR "/sys/devices/system/cpu/cpu%u"
#define PRE_CORE_ID_FILE "topology/core_id"
#define DIM(a) (sizeof(a) / sizeof((a)[0]))

inline extern int32_t pre_rte_lcore_to_socket_id(uint32_t lcore_id);
inline extern uint32_t pre_rte_lcore_count(void);
inline extern uint32_t pre_rte_socket_count(void);

inline static int
__socket_id_cmp(const void *a, const void *b)
{
  const int *lcore_id_a = a;
  const int *lcore_id_b = b;

  if (*lcore_id_a < *lcore_id_b)
    return -1;
  if (*lcore_id_a > *lcore_id_b)
    return 1;
  return 0;
}

inline static uint32_t
__pre_eal_cpu_socket_id(uint32_t lcore_id)
{
  for (uint32_t sock=0; sock<PRE_RTE_MAX_NUMA_NODES; sock++) {
    char path[256];
    snprintf(path, sizeof(path), "%s/node%u/cpu%u",
        PRE_NUMA_NODE_PATH, sock, lcore_id);
    int ret = access(path, F_OK);
    if (ret == 0) return sock;
  }
  return 0;
}

inline static int
__pre_eal_cpu_detected(unsigned lcore_id)
{
  char path[256];
  int len = snprintf(path, sizeof(path), PRE_SYS_CPU_DIR
    "/"PRE_CORE_ID_FILE, lcore_id);
  if (len <= 0 || (unsigned)len >= sizeof(path))
    return 0;
  if (access(path, F_OK) != 0)
    return 0;
  return 1;
}

inline int32_t
pre_rte_lcore_to_socket_id(uint32_t lcore_id)
{
  if (__pre_eal_cpu_detected(lcore_id) == 0)
    return -1;
  return __pre_eal_cpu_socket_id(lcore_id);
}

inline uint32_t
pre_rte_lcore_count(void)
{
  uint32_t lcore_count = 0;
  for (size_t lcore_id=0; lcore_id<PRE_RTE_MAX_LCORE; lcore_id++) {
    if (__pre_eal_cpu_detected(lcore_id) != 0)
      lcore_count++;
  }
  return lcore_count;
}

inline uint32_t
pre_rte_socket_count(void)
{
  uint32_t lcore_to_socket_id[PRE_RTE_MAX_LCORE];
  memset(lcore_to_socket_id, 0x0, sizeof(lcore_to_socket_id));
  for (uint32_t lcore_id=0; lcore_id<PRE_RTE_MAX_LCORE; lcore_id++) {
    uint32_t socket_id = __pre_eal_cpu_socket_id(lcore_id);
    lcore_to_socket_id[lcore_id] = socket_id;
  }
  qsort(lcore_to_socket_id, DIM(lcore_to_socket_id),
      sizeof(lcore_to_socket_id[0]), __socket_id_cmp);

  int32_t prev_socket_id = -1;
  uint32_t numa_node_count = 0;
  for (uint32_t lcore_id=0; lcore_id<PRE_RTE_MAX_LCORE; lcore_id++) {
    uint32_t socket_id = lcore_to_socket_id[lcore_id];
    if (socket_id != prev_socket_id)
      numa_node_count ++;
    prev_socket_id = socket_id;
  }
  return numa_node_count;
}

// TODO
// inline int32_t pre_rte_get_pcie_dev_socket_id(const char* pcie_addr)
// {
// }

#endif /* _PRE_RTE_H_ */

