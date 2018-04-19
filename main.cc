
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "xellico.h"
#include "dpdk_misc.h"
#include "config.h"
#include "forwarder.h"
#include "force_quit.h"
#include "port.h"
#include "delay.h"

xellico_conf_t* xeconf;

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
      printf ("\n\nSignal %d received, preparing to exit...\n", signum);
      force_quit = true;
    }
}

void debug (void)
{
  while (!force_quit)
    {
      dump_pktmbuf_pool ();
      sleep (1);
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

  xeconf = new_xeconf ("config.json");
  port_init ();
  init_fib ();

  rte_eal_mp_remote_launch (launcher, NULL, SKIP_MASTER);
  rte_eal_mp_wait_lcore ();

  port_fini ();
  free_xeconf (xeconf);
  printf ("Bye...\n");
  return ret;
}


