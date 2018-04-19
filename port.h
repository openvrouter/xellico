
#pragma once
#include <stddef.h>

void init_port_conf (struct rte_eth_conf* conf);
void port_init (void);
void port_fini (void);
void dump_pktmbuf_pool (void);

