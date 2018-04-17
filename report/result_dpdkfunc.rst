
送受信関数の実験結果
====================

以下のデータはまだdummy値である.

以下に, burstサイズを変化させた時のDPDKのパケットの送受信関数の遅延を示す.
まずはrxburstとtxburstの結果を示す.

以下にrxburstの遅延結果を表で示す.

.. csv-table::
  :header: #burst, rx(64byte), rx(128byte), rx(256byte), rx(512byte), rx(1024byte)
  :widths: 1, 1, 1, 1, 1, 1
  :file: img/rxburst_delay.csv

.. csv-table::
  :header: #burst, tx(64byte), tx(128byte), tx(256byte), tx(512byte), tx(1024byte)
  :widths: 1, 1, 1, 1, 1, 1
  :file: img/txburst_delay.csv

上記の内容を図として示す.

.. figure:: img/rxburst_delay.png
  :name: rxburst_delay

  rte_eth_rx_burstの遅延

.. figure:: img/txburst_delay.png
  :name: txburst_delay

  rte_eth_tx_burstの遅延


次にtxbufferの結果を示す.
txbufferはshotでしか送ることができない.


