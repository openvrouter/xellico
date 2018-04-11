
Delayの実験方法
====================

ブラックボックスNFテスト方法について
--------------------------------------

本レポートでの性能計測は, 原則として帯域と遅延に関して行った.
どちらもpktge-dpdkを用いて性能計測を行った.
計測はパケットサイズを64,128,192,...,1518と変更しながらそれぞれ
計測を行った. RSSを利用した性能計測を行うため, pktgenはrangeモードを
有効かし, pktgenから流れるトラフィックはそれぞれ別々のRSS Queueに
流れるように行った. ptkgen-dpdkは別途, DPDK用のサーバを用いて行った.

内部のマイクロレイテンシ
-------------------------

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


Delayの準備
------------

DPDKには ``rte_delay_ms()`` , ``rte_delay_us_block()`` といったdelayを作り出す
関数が存在している. 性能計測等の目的で使用することが想定されているようなので,
正確なdelayを挿入することができるとてもありがたい関数である一方で,
10GbE以上の性能計測を行う場合においては, より細かい制度でのdelayを挿入したい.

本節では, 性能計測を行う上で用いるdelayの方法に関しての整理と, それらを用いた
数理的な性能見積もりに関して論ずる.

rte_delay_us関数
^^^^^^^^^^^^^^^^

DPDKに用意されているdelayの関数であるため, 信頼性がたかい(と筆者は思っている)
rdtscによる遅延計測でも安定した値を出していた.


空ループdelay
^^^^^^^^^^^^^^^^

nano secオーダのdelayが欲しかったが, さすがにCPUのクロック周波数が2GHz程度では,
その細かさの正確なdelayは作れなかったので, 10nsオーダのなんとなく正しいdelay
関数を無理やり実装した. この方法は完全なdirty-hackなため推奨されるべきものではない.
以下に実装を示す.  筆者の環境ではなんとかこれでそれっぽい精度で動いている.

.. code-block:: none

  static inline void
  dirty_looped_delay_10ns (uint32_t nsec)
  {
    volatile uint32_t vcount = nsec*8 + nsec;
    while (vcount --);
  }



