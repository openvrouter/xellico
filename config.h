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


