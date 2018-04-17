
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


