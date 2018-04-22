
Delayの実験方法
====================

DPDKには ``rte_delay_ms()`` , ``rte_delay_us_block()`` といったdelayを作り出す
関数が存在している. 性能計測等の目的で使用することが想定されているようなので,
正確なdelayを挿入することができるとてもありがたい関数である一方で,
10GbE以上の性能計測を行う場合においては, より細かい制度でのdelayを挿入したい.
本節では, 性能計測を行う上で用いるdelayの方法に関しての整理をする.
より細かいdelayに関しては妥協した実装案について説明する.

rte_delay_us_block関数
-----------------------

- ドキュメント: http://dpdk.org/doc/api/rte__cycles_8h.html

DPDKに用意されているdelayの関数であるため, 信頼性が高いと思われる.


dirty_looped_delay_10ns関数(空ループdelay)
-------------------------------------------

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


性能評価
---------

delayの検証用プログラムを以下に示す.

- https://github.com/slankdev/xellico/blob/db5bd57f14390cc457a2574626d19a7506256282/misc/delay_eval/main.c


