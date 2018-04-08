
#include <assert.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "xellico.h"
#include "config.h"
#include "json.hpp"


static void
read_conf (const char* configfile, xellico_conf_t* xeconf)
{
  std::ifstream f (configfile);
  nlohmann::json json;
  f >> json;

  const size_t n_qconf = json["qconf"].size ();
  for (size_t i=0; i<n_qconf; i++)
    {
      auto ele = json["qconf"][i];
      auto port_id = ele["port_id"].get<uint32_t> ();
      auto queue_id = ele["queue_id"].get<uint32_t> ();
      auto lcore_id = ele["lcore_id"].get<uint32_t> ();
      printf ("(%u,%u,%u)\n", port_id, queue_id, lcore_id);

      xeconf->all_qconf.push_back ({port_id, queue_id, lcore_id});
    }
  xeconf->tx_buffer_size = json["txbulk"].get<size_t> ();
  printf ("tx_buffer_size: %zd\n", xeconf->tx_buffer_size);
}

static bool
validate_conf (xellico_conf_t* xeconf)
{
  std::vector<struct queue_conf>& all_qconf = xeconf->all_qconf;

  /* Check each port's rx-queue-id
   * is starting from 0 in order. */
  const size_t n_port = rte_eth_dev_count ();
  for (size_t pid=0; pid<n_port; pid++)
    {
      std::vector <size_t> queue_ids;
      for (auto& qconf: all_qconf)
        if (qconf.port_id == pid)
          queue_ids.push_back (qconf.queue_id);
      std::sort (queue_ids.begin (), queue_ids.end ());

      auto is_in_order = [] (std::vector<size_t>& vec)
        {
          size_t min = *std::min_element (vec.begin (), vec.end ());
          size_t max = *std::max_element (vec.begin (), vec.end ());
          size_t expect_total = (min + max) * vec.size() / 2;
          size_t actual_total = [&] { size_t c=0; for (auto v : vec) c+=v; return c; } ();
          return actual_total == expect_total;
        };

      bool ret0 = is_in_order (queue_ids);
      bool ret1 = queue_ids[0] == 0;
      if (!(ret0 & ret1)) return false;
    }
  return true;
}

static void
create_lcore_conf (xellico_conf_t* xeconf)
{
  const size_t n_lcores = rte_lcore_count ();
  for (size_t i=0; i<n_lcores; i++)
    {
      for (size_t j=0; j<xeconf->all_qconf.size(); j++)
        if (xeconf->all_qconf[j].lcore_id == i)
          xeconf->lcore_conf[i].qconf.push_back (xeconf->all_qconf[j]);
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

          rte_eth_tx_buffer_init (txbuff, xeconf->tx_buffer_size);
          xeconf->lcore_conf[i].tx_buffer[pid] = txbuff;
        }
    }
}

xellico_conf_t*
new_xeconf (const char* filename)
{
  xellico_conf_t* conf = new xellico_conf_t;
  read_conf (filename, conf);
  if (!validate_conf (conf))
    {
      fprintf (stderr, "invalid configuration\n");
      exit(1);
    }
  create_lcore_conf (conf);
  return conf;
}

void
free_xeconf (xellico_conf_t* conf)
{
  delete conf;
}

