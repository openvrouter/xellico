
/*
 * The use of this software is limited to education, research, and evaluation
 * purposes only.  Commercial use is strictly prohibited.  For all other uses,
 * contact the author(s).
 * Copyright(c) 2018 Souta Kawahara
 * Copyright(c) 2018 Hiroki Shirokura
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <vector>
#include "xellico.h"
#include "dpdk_misc.h"
#include "lcore.h"
#include "forwarder.h"
#include "force_quit.h"
#include "port.h"

std::vector <struct queue_conf> all_qconf;
struct lcore_conf lcore_conf[RTE_MAX_LCORE];

static void
create_lcore_conf (void)
{
  const size_t n_lcores = rte_lcore_count ();
  for (size_t i=0; i<n_lcores; i++)
    {
      for (size_t j=0; j<all_qconf.size(); j++)
        if (all_qconf[j].lcore_id == i)
          lcore_conf[i].qconf.push_back (all_qconf[j]);
    }


  for (size_t i=0; i<n_lcores; i++)
    {
      const size_t n_ports = rte_eth_dev_count ();
      for (size_t pid=0; pid<n_ports; pid++)
        {
          struct rte_eth_dev_tx_buffer* txbuff = (struct rte_eth_dev_tx_buffer*)
            rte_zmalloc_socket ("tx_buffer",
              RTE_ETH_TX_BUFFER_SIZE (MAX_PKT_BURST), 0,
              rte_eth_dev_socket_id (pid));
          if (txbuff == NULL)
            rte_exit(EXIT_FAILURE, "Cannot allocate buffer for tx on port %u\n",
                (unsigned) pid);

          rte_eth_tx_buffer_init (txbuff, MAX_PKT_BURST);
          lcore_conf[i].tx_buffer[pid] = txbuff;
        }
    }
}

static void
init_conf (void)
{
  all_qconf.push_back ({0, 0, 2});
  all_qconf.push_back ({1, 0, 3});
  all_qconf.push_back ({0, 1, 4});
  all_qconf.push_back ({1, 1, 5});

  all_qconf.push_back ({0, 2, 6});
  all_qconf.push_back ({1, 2, 7});
  all_qconf.push_back ({0, 3, 8});
  all_qconf.push_back ({1, 3, 9});

  all_qconf.push_back ({0, 4, 10});
  all_qconf.push_back ({1, 4, 11});
  all_qconf.push_back ({0, 5, 12});
  all_qconf.push_back ({1, 5, 13});

  all_qconf.push_back ({0, 6, 14});
  all_qconf.push_back ({1, 6, 15});
  all_qconf.push_back ({0, 7, 16});
  all_qconf.push_back ({1, 7, 17});
}

static int
launcher (__attribute__ ((unused)) void *dummy)
{
  forwarder ();
  return 0;
}

static void
signal_handler (int signum)
{
  if (signum == SIGINT || signum == SIGTERM)
    {
      printf("\n\nSignal %d received, preparing to exit...\n", signum);
      force_quit = true;
    }
}

int
main (int argc, char **argv)
{
  int ret = xellico_boot_dpdk (argc, argv);
  argc -= ret;
  argv += ret;

  force_quit = false;
  signal (SIGINT, signal_handler);
  signal (SIGTERM, signal_handler);

  init_conf ();
  create_lcore_conf ();
  init_port_conf (&port_conf);
  port_init (8);
  init_fib ();

  /*
   * Threaad Launch
   */
  rte_eal_mp_remote_launch (launcher, NULL, SKIP_MASTER);
  rte_eal_mp_wait_lcore ();

  /*
   * Termination
   */
  port_fini ();
  printf ("Bye...\n");
  return ret;
}


