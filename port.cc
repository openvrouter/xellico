
#include <vector>
#include "xellico.h"
#include "config.h"
#include "port.h"

#define NB_MBUF 8192
#define MEMPOOL_CACHE_SIZE 256

struct rte_eth_conf port_conf;
struct rte_mempool* pktmbuf_pool[RTE_MAX_ETHPORTS];

void
init_port_conf (struct rte_eth_conf* conf)
{
  conf->rxmode.hw_strip_crc = 1;
  conf->rxmode.mq_mode = ETH_MQ_RX_RSS;
  conf->txmode.mq_mode = ETH_MQ_TX_NONE;
  conf->rx_adv_conf.rss_conf.rss_key = NULL;
  conf->rx_adv_conf.rss_conf.rss_hf = ETH_RSS_IP|ETH_RSS_TCP|ETH_RSS_UDP;
}

static void
port_mempool_init ()
{
  const uint8_t nb_ports = rte_eth_dev_count ();
  for (size_t i=0; i<nb_ports; i++)
    {
      char str[128];
      snprintf (str, sizeof (str), "pktmbuf_pool[%zd]", i);

      pktmbuf_pool[i] = rte_pktmbuf_pool_create (str, NB_MBUF,
        MEMPOOL_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE,
        rte_eth_dev_socket_id(i));

      if (pktmbuf_pool[i] == NULL)
        rte_exit (EXIT_FAILURE, "Cannot init mbuf pool %s\n", str);

      RTE_LOG (INFO, XELLICO,
          "create mempool %s on socket %u\n",
          str, rte_eth_dev_socket_id(i));
    }
}

static size_t
get_nb_rxq (uint32_t port_id)
{
  size_t cnt = 0;
  for (size_t i=0; i<xeconf->all_qconf.size(); i++) {
    if (xeconf->all_qconf[i].port_id == port_id) cnt ++;
  }
  return cnt;
}

void
port_init (void)
{
  init_port_conf (&port_conf);

  const uint8_t nb_ports = rte_eth_dev_count ();
  if (nb_ports == 0)
    rte_exit (EXIT_FAILURE, "No Ethernet ports - bye\n");

  RTE_LOG (INFO, XELLICO, "%u Ethernet ports found\n", nb_ports);
  port_mempool_init ();

  uint8_t nb_ports_available = nb_ports;
  for (uint8_t portid = 0; portid < nb_ports; portid++)
    {
      const size_t nb_rxq = get_nb_rxq (portid);
      const size_t nb_txq = rte_lcore_count();
      printf("Initializing port %u... \n", (unsigned) portid);
      int ret = rte_eth_dev_configure (portid, nb_rxq, nb_txq, &port_conf);
      if (ret < 0)
        rte_exit (EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n",
            ret, (unsigned) portid);

      uint16_t nb_rxd = 128;
      uint16_t nb_txd = 512;
      ret = rte_eth_dev_adjust_nb_rx_tx_desc (portid, &nb_rxd, &nb_txd);
      if (ret < 0)
        rte_exit(EXIT_FAILURE,
           "Cannot adjust number of descriptors: err=%d, port=%u\n",
           ret, (unsigned) portid);

      /* init one RX queue */
      for (uint32_t q=0; q<nb_rxq; q++)
        {
          ret = rte_eth_rx_queue_setup (portid, q, nb_rxd,
                     rte_eth_dev_socket_id (portid),
                     NULL, pktmbuf_pool[portid]);
          if (ret < 0)
            rte_exit(EXIT_FAILURE,
                "rte_eth_rx_queue_setup:err=%d, port=%u, queue=%u\n",
                ret, (unsigned) portid, q);
        }

      /* init one TX queue on each port */
      for (uint32_t q=0; q<nb_txq; q++)
        {
          ret = rte_eth_tx_queue_setup (portid, q, nb_txd,
              rte_eth_dev_socket_id (portid), NULL);
          if (ret < 0)
            rte_exit(EXIT_FAILURE,
                "rte_eth_tx_queue_setup:err=%d, port=%u, queue=%u\n",
                ret, (unsigned) portid, q);
        }
    }

  for (uint8_t portid = 0; portid < nb_ports; portid++)
    {
      int ret = rte_eth_dev_start (portid);
      if (ret < 0)
        rte_exit(EXIT_FAILURE, "rte_eth_dev_start:err=%d, port=%u\n",
            ret, (unsigned) portid);

      printf("done: \n");
      rte_eth_promiscuous_enable (portid);
    }

  if (!nb_ports_available)
    {
      rte_exit(EXIT_FAILURE,
        "All available ports are disabled. Please set portmask.\n");
    }
}

void
port_fini (void)
{
  const uint8_t nb_ports = rte_eth_dev_count ();
  for (uint8_t portid = 0; portid < nb_ports; portid++)
    {
      printf ("Closing port %d...", portid);
      rte_eth_dev_stop (portid);
      rte_eth_dev_close (portid);
      printf (" Done\n");
    }
}

