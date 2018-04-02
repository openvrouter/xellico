
#include "xellico.h"
#include "port_conf.h"

const struct rte_eth_conf port_conf = {
  .rxmode = {
    .split_hdr_size = 0,
    .header_split   = 0, /**< Header Split disabled */
    .hw_ip_checksum = 0, /**< IP checksum offload disabled */
    .hw_vlan_filter = 0, /**< VLAN filtering disabled */
    .jumbo_frame    = 0, /**< Jumbo Frame Support disabled */
    .hw_strip_crc   = 1, /**< CRC stripped by hardware */
    .mq_mode = ETH_MQ_RX_RSS,
  },
  .txmode = {
    .mq_mode = ETH_MQ_TX_NONE,
  },
  .rx_adv_conf = {
    .rss_conf = {
      .rss_key = NULL,
      .rss_hf = ETH_RSS_IP|ETH_RSS_TCP|ETH_RSS_UDP,
    },
  },
};

