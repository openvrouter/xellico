
#include "xellico.h"
#include "lcore.h"
#include "forwarder.h"
#include "force_quit.h"

uint32_t l2fwd_dst_ports[RTE_MAX_ETHPORTS];
extern struct lcore_conf lcore_conf[RTE_MAX_LCORE];

static inline void
l2fwd_simple_forward (struct rte_mbuf *m, unsigned portid)
{
  uint32_t lcore_id = rte_lcore_id();
  unsigned dst_port = l2fwd_dst_ports[portid];
  unsigned dst_queue = lcore_id;
  struct rte_eth_dev_tx_buffer* buffer = lcore_conf[lcore_id].tx_buffer[dst_port];
  rte_eth_tx_buffer(dst_port, dst_queue, buffer, m);
}

void
forwarder (void)
{
  unsigned lcore_id = rte_lcore_id ();
  if (lcore_conf[lcore_id].qconf.size() == 0)
    {
      RTE_LOG (INFO, XELLICO, "lcore %u has nothing to do\n", lcore_id);
      return;
    }

  RTE_LOG (INFO, XELLICO, "entering main loop on lcore %u\n", lcore_id);

  for (size_t i = 0; i < lcore_conf[lcore_id].qconf.size(); i++)
    {
      uint32_t portid = lcore_conf[lcore_id].qconf[i].port_id;
      uint32_t queueid = lcore_conf[lcore_id].qconf[i].queue_id;
      RTE_LOG (INFO, XELLICO,
          " -- lcoreid=%u portid=%u queueid=%u\n",
          lcore_id, portid, queueid);
    }

  uint64_t prev_tsc = 0;
  const uint64_t drain_tsc =
      (rte_get_tsc_hz () + US_PER_S - 1)
      / US_PER_S * BURST_TX_DRAIN_US;

  printf ("Staring Main_loop on lcore%u \n", rte_lcore_id());
  while (!force_quit)
    {
      /*
       * TX burst queue drain
       */
      uint64_t cur_tsc = rte_rdtsc ();
      uint64_t diff_tsc = cur_tsc - prev_tsc;
      if (unlikely (diff_tsc > drain_tsc))
        {
          for (size_t i = 0; i < lcore_conf[lcore_id].qconf.size(); i++)
            {
              uint32_t dst_portid = l2fwd_dst_ports[lcore_conf[lcore_id].qconf[i].port_id];
              uint32_t dst_queueid = rte_lcore_id();
              struct rte_eth_dev_tx_buffer *buffer = lcore_conf[lcore_id].tx_buffer[dst_portid];
              rte_eth_tx_buffer_flush (dst_portid, dst_queueid, buffer);
            }
          prev_tsc = cur_tsc;
        }

      /*
       * Read packet from RX queues
       */
      for (size_t i = 0; i < lcore_conf[lcore_id].qconf.size(); i++)
        {
          struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
          uint32_t in_portid = lcore_conf[lcore_id].qconf[i].port_id;
          uint32_t in_queueid = lcore_conf[lcore_id].qconf[i].queue_id;
          uint32_t nb_rx = rte_eth_rx_burst (in_portid,
              in_queueid, pkts_burst, MAX_PKT_BURST);

          for (size_t j = 0; j < nb_rx; j++)
            {
              struct rte_mbuf *m = pkts_burst[j];
              rte_prefetch0 (rte_pktmbuf_mtod (m, void *));
              rte_delay_us_block (1);
              l2fwd_simple_forward (m, in_portid);
            }
        }
    }
}

void
init_fib (void)
{
  /*
   * Each logical core is assigned a dedicated TX queue on each port.
   */
  const size_t nb_ports = rte_eth_dev_count ();
  uint8_t last_port = 0;
  unsigned nb_ports_in_mask = 0;
  memset (l2fwd_dst_ports, 0x0, sizeof (l2fwd_dst_ports));
  for (uint8_t portid = 0; portid < nb_ports; portid++)
    {
      if (nb_ports_in_mask % 2)
        {
          l2fwd_dst_ports[portid] = last_port;
          l2fwd_dst_ports[last_port] = portid;
        }
      else
        last_port = portid;

      nb_ports_in_mask++;
    }
  if (nb_ports_in_mask % 2)
    {
      printf("Notice: odd number of ports in portmask.\n");
      l2fwd_dst_ports[last_port] = last_port;
    }
}
