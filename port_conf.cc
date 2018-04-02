
#include "xellico.h"
#include "port_conf.h"

struct rte_eth_conf port_conf;

void
init_port_conf (struct rte_eth_conf* conf)
{
  conf->rxmode.hw_strip_crc = 1;
  conf->rxmode.mq_mode = ETH_MQ_RX_RSS;
  conf->txmode.mq_mode = ETH_MQ_TX_NONE;
  conf->rx_adv_conf.rss_conf.rss_key = NULL;
  conf->rx_adv_conf.rss_conf.rss_hf = ETH_RSS_IP|ETH_RSS_TCP|ETH_RSS_UDP;
}


