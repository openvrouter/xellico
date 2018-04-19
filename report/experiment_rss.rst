
RSSの実験方法
=============

ここではRSS(Receive Side Scaling)に関する調査報告を行う.
RSSに関しては, Takuya Asada (syuu1228)氏のブログの記事が参考になるため,
そちらを参考にしてほしい.

- [参考] Takuya Asada, Linuxのネットワークスタックのスケーラビリティについて,
  http://syuu1228.hatenablog.com/entry/20101210/1291941459
- [参考] Receive-Side Scalingについての調査メモ,
  http://syuu1228.hatenablog.com/entry/20101219/1292725423

DPDKで高速なVNFを実装する場合, 40GbE以上のパケット転送を行う場合には,
RSSを用いることが定石である. ここでは, その方法とスケーラビリティに
関しての調査結果を示していく. 調査には前述しているXellicoを用いる.

DPDKでの設定方法
----------------

DPDKでRSSによるMultiQueueを有効化する場合, ``rte_eth_dev_configure``
関数で以下のようにするとよい.以下の内容では, RxQueueを4つ, TxQueueを8つに
設定をしているサンプルである.

.. code-block:: c

    struct rte_eth_conf port_conf;
    ...<省略>...
    size_t n_rxq = 4;
    size_t n_txq = 8;
    conf.rxmode.mq_mode = ETH_MQ_RX_RSS;
    conf.txmode.mq_mode = ETH_MQ_TX_NONE;
    conf.rx_adv_conf.rss_conf.rss_key = NULL;
    conf.rx_adv_conf.rss_conf.rss_hf = ETH_RSS_IP|ETH_RSS_TCP|ETH_RSS_UDP;
    rte_eth_dev_configure (portid, n_rxq, n_txq, &port_conf);

実験の詳細
-----------

本節では, 前述のTxBufferの値は, 32として固定して行う.
主要なDPDKのサンプルアプリケーションは32bulkで固定しているためであり,
現状この値が, DPDKアプリケーションの最適値であると考えているためである.

本節では, RSSのqueueの数を変化させた時のVNFのスケーラビリティを調べる.
TxBufferの検証と同じく, 検証にはXellico vRouterを用いた.
Xellicoはconfigファイルにより, RSSのコンフィグレーションを行うことが
できるため, 本節もソースコードの編集は行わずに, configとなるJSONファイル
の編集のみをおこなった.

また, 10GbEの場合, RSSを用いなくても割と性能が出てしまうため, パケット処理
部分にはdelayを挿入する. delayは前述している ``rte_delay_us_block`` 関数と
``dirty_looped_delay_10ns`` 関数を用いる. 本実験の目的は, いかなるパケット処理
プログラムであろうと, RSSによりスケールすることを確認することである.
よってスケーラビティが確認できるようにdelayを挿入する.

ここでは,RSSのRxQeuueに対して1つのコアを割り当てるようにXellicoを開発
しているので, RxQueueをいくつに設定するかが, 本実験のパラメータである.
RxQueueの数を1-8とふやすことで, 性能にどのような変化が合わられるかを検証する.

