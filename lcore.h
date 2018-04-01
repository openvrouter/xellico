#ifndef __LCORE_H__
#define __LCORE_H__
#define MAX_RX_QUEUE_PER_LCORE 16

struct lcore_port_queue_list
{
  uint32_t port_id;
  uint32_t queue_id;
} __rte_cache_aligned;

struct lcore_queue_conf
{
  uint32_t n_rx_port;
  struct lcore_port_queue_list rx_port_queue_list[MAX_RX_QUEUE_PER_LCORE];
  struct rte_eth_dev_tx_buffer *tx_buffer[RTE_MAX_ETHPORTS];
} __rte_cache_aligned;


void dump_queue_conf (struct lcore_queue_conf* qconf);
void dump_queue_confs (struct lcore_queue_conf* qconfs, size_t n_qconfs);
// void init_queue_conf_buffer_init (void);
void init_queue_conf_buffer_init (struct lcore_queue_conf* confs);

#endif /* __LCORE_H__ */
