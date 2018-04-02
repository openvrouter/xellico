
#pragma once
#include <stddef.h>

extern struct rte_eth_conf port_conf;
void init_port_conf (struct rte_eth_conf* conf);
void port_init (size_t nb_rxq);
void port_fini (void);

