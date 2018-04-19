
#include <stdio.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_lcore.h>
#include <rte_cycles.h>
#define MEGA (1000 * 1000)
#define GIGA (1000 * 1000 * 1000)


static inline void
dirty_looped_delay_10ns (uint32_t nsec)
{
  volatile uint32_t vcount = nsec*8 + nsec;
  while (vcount --);
}

static void
test_dirty_looped_delay_10ns (size_t nsec, size_t trycnt)
{
  size_t sum = 0;
  for (size_t i=0; i<trycnt; i++)
    {
      size_t before = rte_get_tsc_cycles ();
      dirty_looped_delay_10ns (nsec);
      sum += rte_get_tsc_cycles () - before;
    }
  size_t hz = rte_get_tsc_hz ();
  size_t lat_clk = sum/trycnt;
  double lat_ns = ((double)lat_clk*GIGA)/hz;
  double accr = lat_ns / nsec / 10;

        /* dely, lat_clk, lat_ns, accur*/
  printf ("%10zd, %10zd, %10lf, %10lf\n",
          nsec*10, lat_clk, lat_ns, accr);
}

static void
test_rte_delay_us_block (size_t usec, size_t trycnt)
{
  size_t sum = 0;
  for (size_t i=0; i<trycnt; i++)
    {
      size_t before = rte_get_tsc_cycles ();
      rte_delay_us_block (usec);
      sum += rte_get_tsc_cycles () - before;
    }

  size_t hz = rte_get_tsc_hz ();
  size_t lat_clk = sum/trycnt;
  double lat_us = (double)(lat_clk)/hz*MEGA;
  double accr = lat_us / usec;

        /* dely, lat_clk, lat_us, accur*/
  printf ("%10zd, %10zd, %10lf, %10lf\n",
          usec, lat_clk, lat_us, accr);
}

static int
func (__attribute__((unused)) void *arg)
{
  printf ("freq: %zd\n\n", rte_get_tsc_hz ());
  size_t trycnt = MEGA;

  printf ("#%9s, %10s, %10s, %10s\n",
      "dly[us]", "clk", "latency", "accur");
  test_rte_delay_us_block (1, trycnt);
  test_rte_delay_us_block (2, trycnt);
  test_rte_delay_us_block (3, trycnt);
  test_rte_delay_us_block (4, trycnt);
  test_rte_delay_us_block (5, trycnt);
  test_rte_delay_us_block (6, trycnt);
  test_rte_delay_us_block (7, trycnt);
  test_rte_delay_us_block (8, trycnt);
  test_rte_delay_us_block (9, trycnt);
  test_rte_delay_us_block (10, trycnt);

  trycnt = MEGA;
  printf ("#%9s, %10s, %10s, %10s\n",
      "dly[ns]", "clk", "latency", "accur");
  test_dirty_looped_delay_10ns (1, trycnt);
  test_dirty_looped_delay_10ns (2, trycnt);
  test_dirty_looped_delay_10ns (3, trycnt);
  test_dirty_looped_delay_10ns (4, trycnt);
  test_dirty_looped_delay_10ns (5, trycnt);
  test_dirty_looped_delay_10ns (6, trycnt);
  test_dirty_looped_delay_10ns (7, trycnt);
  test_dirty_looped_delay_10ns (8, trycnt);
  test_dirty_looped_delay_10ns (9, trycnt);
  test_dirty_looped_delay_10ns (10, trycnt);
	return 0;
}

int
main (int argc, char **argv)
{
  rte_log_set_global_level (RTE_LOG_EMERG);
	int ret = rte_eal_init (argc, argv);
	if (ret < 0)
		rte_panic ("Cannot init EAL\n");

  rte_eal_remote_launch (func, NULL, 1);
	rte_eal_mp_wait_lcore ();
	return 0;
}

