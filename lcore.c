
#include "xellico.h"
#include "lcore.h"

void
dump_queue_conf (struct lcore_queue_conf* qconf)
{
  printf ("qconf@%p: n_rx_port=%u \n", qconf, qconf->n_rx_port);
  for (size_t j=0; j<qconf->n_rx_port; j++)
    printf ("   port%u queue%u\n",
        qconf->rx_port_queue_list[j].port_id,
        qconf->rx_port_queue_list[j].queue_id);
}

void
dump_queue_confs (struct lcore_queue_conf* qconfs, size_t n_qconfs)
{
  for (size_t i=0; i<n_qconfs; i++)
    {
      struct lcore_queue_conf* qconf = (qconfs + i);
      dump_queue_conf (qconf);
    }
}

void
init_queue_conf_buffer_init (struct lcore_queue_conf* confs)
{
  const size_t nb_ports = rte_eth_dev_count();
  for (size_t i=0; i<RTE_MAX_LCORE; i++)
    {
      for (size_t portid=0; portid<nb_ports; portid++)
        {
          struct rte_eth_dev_tx_buffer* txbuff =
            confs[i].tx_buffer[portid];
          rte_eth_tx_buffer_init (txbuff, MAX_PKT_BURST);
        }
    }
}

