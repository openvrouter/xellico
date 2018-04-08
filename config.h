
#pragma once
#include <vector>

struct queue_conf
{
  uint32_t port_id;
  uint32_t queue_id;
  uint32_t lcore_id;
} __rte_cache_aligned;

struct lcore_conf
{
  std::vector <struct queue_conf> qconf;
  struct rte_eth_dev_tx_buffer *tx_buffer[RTE_MAX_ETHPORTS];
};

typedef struct xellico_conf
{
  std::vector <struct queue_conf> all_qconf;
  struct lcore_conf lcore_conf[RTE_MAX_LCORE];
  size_t tx_buffer_size;
} xellico_conf_t;

extern xellico_conf_t* xeconf;

xellico_conf_t* new_xeconf (const char* filename);
void free_xeconf (xellico_conf_t* conf);

