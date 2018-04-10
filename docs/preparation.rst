
実験の準備
==========

DPDKには ``rte_delay_ms()`` , ``rte_delay_us_block()`` といったdelayを作り出す
関数が存在している. 性能計測等の目的で使用することが想定されているようなので,
正確なdelayを挿入することができるとてもありがたい関数である一方で,
10GbE以上の性能計測を行う場合においては, より細かい制度でのdelayを挿入したい.

本節では, 性能計測を行う上で用いるdelayの方法に関しての整理と, それらを用いた
数理的な性能見積もりに関して論ずる.

The DPDK Performance Deep Dive
------------------------------

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

今回はpacket processの部分にdelayを挿入して性能計測を行なっていくわけだが,
32 bulk rxと, 1tx bufferingでどの程度の計算機的コストを払っているかを
明らかにしなくては, 本来の性能計測の見積もりができそうにない.

なので, 今回はTxBufferの実験を用いて, rx/txのコストを計測していく.
基本的に計測対象は以下のようにして計測を行う.

.. code-block:: c

  static size_t sum = 0;
  static size_t cnt = 0;
  size_t before = rte_get_tsc_cycles ()
  //検査対象コード
  sum += rte_get_tsc_cycles () - before;
  cnt ++;
  if (cnt > 10000000)
  {
    printf ("latency: %zd \n", sum / cnt);
    cnt = sum = 0;
  }

このようにしてrdtscを用いてプログラムの実行速度を細かく測っていく.


rte_delay_us関数
-----------------

DPDKに用意されているdelayの関数であるため, 信頼性がたかい(と筆者は思っている)
rdtscによる遅延計測でも安定した値を出していた.



空ループdelay
-------------

rdtscを用いたdelayの評価
------------------------

.. code-block:: none

  jk6:~/git/xellico/misc/delay_eval:( make
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

delay量と性能の見積もり
-----------------------

