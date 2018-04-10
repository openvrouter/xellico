
Delayの準備
-----------

DPDKには ``rte_delay_ms()`` , ``rte_delay_us_block()`` といったdelayを作り出す
関数が存在している. 性能計測等の目的で使用することが想定されているようなので,
正確なdelayを挿入することができるとてもありがたい関数である一方で,
10GbE以上の性能計測を行う場合においては, より細かい制度でのdelayを挿入したい.

本節では, 性能計測を行う上で用いるdelayの方法に関しての整理と, それらを用いた
数理的な性能見積もりに関して論ずる.

rte_delay_us関数
-----------------

DPDKに用意されているdelayの関数であるため, 信頼性がたかい(と筆者は思っている)
rdtscによる遅延計測でも安定した値を出していた.


空ループdelay
-------------

nano secオーダのdelayが欲しかったが, さすがにCPUのクロック周波数が2GHz程度では,
その細かさの正確なdelayは作れなかったので, 10nsオーダのなんとなく正しいdelay
関数を無理やり実装した. この方法は完全なdirty-hackなため推奨されるべきものではない.
以下に実装を示す.

.. code-block:: none

  static inline void
  dirty_looped_delay_10ns (uint32_t nsec)
  {
    volatile uint32_t vcount = nsec*8 + nsec;
    while (vcount --);
  }

筆者の環境ではなんとかこれでそれっぽい精度で動いている.
ベンチマーク結果は後述する.

rdtscを用いたdelayの評価
------------------------

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



