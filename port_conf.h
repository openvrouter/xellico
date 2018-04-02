#ifndef __PORT_CONFIG_H__
#define __PORT_CONFIG_H__

extern struct rte_eth_conf port_conf;
void init_port_conf (struct rte_eth_conf* conf);

#endif /* __PORT_CONFIG_H__ */
