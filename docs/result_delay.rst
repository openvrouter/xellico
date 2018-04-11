
Delayの実験結果
--------------------

性能計測はpktgen-dpdkを用いてブラックボックス性能テストを行うと前節では述べた.
では, 実際にpktgenを用いて計測した値に対してどの程度の信頼性があるのかを
計算する方法を整理する.

- 64Byteパケットで10Gbps Wirerateを出す場合は14Mppsになる.
- 14Mppsを出すには1パケットを67nsで処理する必要がある
- 動作周波数が2GHzのCPUの場合

  - 14Mpps の処理性能は 134clock/packet
  - 140Mpps の処理性能は 13clock/packet

このようにDPDK-VNFの性能計測は結構見積もれそうな気がしてきたので,
もう少ししっかりと考えてみよう. 多くのDPDKアプリケーションの場合,
パケット解析のプログラムの流れは以下のようになる.

.. code-block:: c

  size_t n_rx = rte_eth_rx_burst (0, 0, pkts, 32);
  for (size_t i=0; i<n_rx; i++)
    {
      process_packet (pkts[i])
      size_t n_tx = rte_eth_tx_burst (1, 0, &pkts[i], 1);
      if (n_tx != 1)
        rte_pktmbuf_free (pkts[i]);
    }

1行目に32burstで受信処理をおこない, 4行目でパケット処理をおこない, 5行目で
パケットの送信を行なっている. 今回は都合上, tx_buffer等の
bulk transmissionを利用して実装を行なったとする.

この時の性能に影響を与える部分は以下の3つに分けられる.

- 32 bulk rx
- packet process
- 1 tx buffering

性能評価
^^^^^^^^^^^

delayの検証用プログラムを以下に示す.

.. code-block:: c

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
    size_t latency = sum/trycnt;
    size_t hz = rte_get_tsc_hz ();
    printf ("dirty_looped_delay_10ns(%zd): "
            " lat=%zd [clk] try=%zd, delay=%zdns\n",
            nsec, latency, trycnt, GIGA*latency/hz);
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
    size_t latency = sum/trycnt;
    size_t hz = rte_get_tsc_hz ();
    printf ("rte_delay_us_block(%zd): "
            " lat=%zd [clk] try=%zd, delay=%zdus\n",
            usec, latency, trycnt, MEGA*latency/hz);
  }

  static int
  func (__attribute__((unused)) void *arg)
  {
    printf ("freq: %zd\n", rte_get_tsc_hz ());
    size_t trycnt = MEGA;
    test_rte_delay_us_block (1, trycnt);
    test_rte_delay_us_block (2, trycnt);
    test_rte_delay_us_block (3, trycnt);
    test_rte_delay_us_block (4, trycnt);
    trycnt = 100 * MEGA;
    test_dirty_looped_delay_10ns (1, trycnt);
    test_dirty_looped_delay_10ns (2, trycnt);
    test_dirty_looped_delay_10ns (3, trycnt);
    test_dirty_looped_delay_10ns (4, trycnt);
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

実行結果を以下に示す.

.. code-block:: none

  jk6:~/git/xellico/misc/delay_eval:) make
    CC main.o
    LD delayeval
    INSTALL-APP delayeval
    INSTALL-MAP delayeval.map
  jk6:~/git/xellico/misc/delay_eval:) make run
  sudo ./build/delayeval
  freq: 1795792297
  rte_delay_us_block(1):  lat=1841 [clk] try=1000000, delay=1us
  rte_delay_us_block(2):  lat=3686 [clk] try=1000000, delay=2us
  rte_delay_us_block(3):  lat=5460 [clk] try=1000000, delay=3us
  rte_delay_us_block(4):  lat=7250 [clk] try=1000000, delay=4us
  dirty_looped_delay_10ns(1):  lat=20 [clk] try=100000000, delay=11ns
  dirty_looped_delay_10ns(2):  lat=33 [clk] try=100000000, delay=18ns
  dirty_looped_delay_10ns(3):  lat=59 [clk] try=100000000, delay=32ns
  dirty_looped_delay_10ns(4):  lat=86 [clk] try=100000000, delay=47ns

