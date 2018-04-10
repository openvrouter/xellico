
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>

#include <rte_memory.h>
#include <rte_memzone.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <rte_cycles.h>

static inline void
dirty_looped_delay (uint32_t nb_loop)
{
  volatile uint32_t vcount = nb_loop;
  while (vcount --);
}

static int
func (__attribute__((unused)) void *arg)
{
  const size_t loopcnt = 1000000;
  size_t sum = 0;
  for (size_t i=0; i<loopcnt; i++)
    {
      size_t before = rte_get_tsc_cycles ();
      rte_delay_us_block (2);
      /* dirty_looped_delay (10); */
      sum += rte_get_tsc_cycles () - before;
    }
  size_t latency = sum/loopcnt;
  printf ("avg latency is %zd clock lcore%u\n",
      latency, rte_lcore_id ());
	return 0;
}

int
main (int argc, char **argv)
{
	int ret = rte_eal_init (argc, argv);
	if (ret < 0)
		rte_panic ("Cannot init EAL\n");

  printf ("\n\n");
  rte_eal_remote_launch (func, NULL, 1);
  rte_eal_remote_launch (func, NULL, 2);
  rte_eal_remote_launch (func, NULL, 3);
  rte_eal_remote_launch (func, NULL, 4);
  rte_eal_remote_launch (func, NULL, 5);
	rte_eal_mp_wait_lcore ();
	return 0;
}
