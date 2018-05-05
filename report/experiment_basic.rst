
==========
基本実験
==========

送受信関数の実験方法
====================

本レポートでは, TxBufferとRSSを用いて最適化をおこなう方法に関して再検討するが,
それらの性能を正確に評価するにあたり, DPDKで多く呼び出される送受信関数の遅延
を見積もる必要がある. 例えば, 以下のようなプログラムを書くとする.

.. code-block:: c

  while (true)
    {
      rte_mbuf* pkts[32];
      size_t n_rx = rte_eth_rx_burst (0, 0, pkts, 32);
      for (size_t i=0; i<n_rx; i++)
        {
          process_packet (pkts[i]);
          rte_eth_tx_burst (1, 0, &pkts[i], 1)
        }
    }

上記のコードの ``process_packet`` 関数には40nsの遅延があるとする場合.
14Mppsを達成するには, 単純計算すると以下のようになる.
ただし, ループのオーバヘッド等は省略している.

.. code-block:: none

  14Mpps -> 72ns/packet

  while内部の1loopの遅延を
  72 * 32 == rx(32) + 32 ( tx(1) + 40 )
  と考える時, この時のrx(n), tx(n)はいくらか

本研究では, 上記の実行オーバヘッドを参考情報として測る.
計測は,前述のrdtscを用いた遅延計測で行う. 検査対象は以下の関数である.

- ``rte_eth_rx_burst`` 関数 (burst-sizeを1-32と変化させる)
- ``rte_eth_tx_burst`` 関数 (burst-sizeを1-32と変化させる)
- ``rte_eth_tx_buffer`` 関数


TxBufferの実験方法
==================

TxBufferとはDPDKを利用した時に, パケット送信をBulk Transmissionするために
一時的にパケットをためておくBufferingAPIである.
サンプルアプリケーションを含む多くのDPDK VNFの実装では, TxBufferが用いられており,
高性能に通信を行う上で必要不可欠であると思われる.
多くは32bulkで使用されているが, 実際にどのくらいのgainがあるのかがきになるため,
調べることにした.  DPDKを用いた実装の多くは以下のようなデザインになる.

.. code-block:: c

  while (true)
    {
      rte_mbuf* pkts[32];
      size_t n_rx = rte_eth_rx_burst (0, 0, pkts, 32);
      for (size_t i=0; i<n_rx; i++)
        {
          process_packet (pkts[i]);
          rte_eth_tx_burst (1, 0, &pkts[i], 1)
        }
    }

この実装では, Rx処理は32burstで行えているが,Tx処理が1パケットずつに生じてしまう.
PCIeNICに対する送信処理は, 高コストであるため,一定量のパケットが貯まるまで
パケットをためておいて, いくつかのパケットを一気に送信をしたい.
TxBufferをもちることで, そのような実装を簡単に行うことができる.
以下にTxBufferを用いたサンプルコードを示す。

.. code-block:: c

  uint64_t prev_tsc = 0;
  const uint64_t drain_tsc =
      (rte_get_tsc_hz () + US_PER_S - 1)
      / US_PER_S * BURST_TX_DRAIN_US;
  while (true)
    {

      uint64_t cur_tsc = rte_rdtsc ();
      uint64_t diff_tsc = cur_tsc - prev_tsc;
      if (diff_tsc > drain_tsc)
        {
          rte_eth_tx_buffer_flush (1, 0, txbuffer);
          prev_tsc = cur_tsc;
        }

      rte_mbuf* pkts[32];
      size_t n_rx = rte_eth_rx_burst (0, 0, pkts, 32);
      for (size_t i=0; i<n_rx; i++)
        {
          process_packet (pkts[i]);
          rte_eth_tx_buffer(1, 0, txbuffer, pkts[i]);
        }
    }

少しコードが複雑になるが, Tx,Rx,process_packetの処理のみが高コストであると
考えると, 上記のコードは以下のコードと同じ意味合いと考えることができる.

.. code-block:: c

  while (true)
    {
      rte_mbuf* pkts[32];
      size_t n_rx = rte_eth_rx_burst (0, 0, pkts, 32);
      for (size_t i=0; i<n_rx; i++)
        process_packet (pkts[i]);
      rte_eth_tx_burst (1, 0, pkts, 32)
    }

このようにTxBufferは送信パケットをBufferingすることにより高コストなTx処理の
呼び出し回数を低減する利点がある.

実験の詳細
----------

通常, TxBufferを用いる多くの実装では, TxBufferのサイズは32で固定されて利用されるが,
本実験では, それを再検討する. Bufferサイズを1-64まで変化させることで, 送信性能に
どのような変化が現れるかを検証することが, 本実験の目的である.
検証は, 通常通り, 前節で定義したブラックボックスNFテストを実行し,
Bufferサイズを1-64として変化させた時の性能の変化を示す.


