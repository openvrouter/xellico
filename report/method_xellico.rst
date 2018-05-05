
Xellico vRouterによる計測
=========================

本レポートで取得したデータは, Xellicoというソフトウェアルータを用いた.
Xellicoは筆者が開発しているDPDKのソフトウェアルータとして参照実装を
なることを目指して開発を行っているものである.

XellicoはGithubでOSSとして開発しており, https://github.com/slankdev/xellico
で全ソースコードにアクセス可能である. XellicoはDPDKのl3fwdサンプル
アプリケーションをforkし, 汎用のVNFとして使用できるように拡張を行っている.
本説では, Xellicoの設計と実装に関して簡単に説明を行う.

設計
----

本研究では,これらの性能計測を円滑に行うために,
Xellicoというソフトウェアネットワークアプライアンスを新規に開発し,
計測を行なった. Xellicoはオープンソースで開発を行なっており,
DPDKのexample/l3fwdをforkして開発開始を行なった.
本研究で調節を行たい, RSSとTxBufferの構成パターンを実行時に
jsonファイルから入力することができ,性能評価に適している.

まずは, 設計について説明する. XellicoはRun to Completionモデルで
実装されており, NICのRSSを用いて同じ役割のスレッドを多重に並列動作させる.
スレッドモデルを :numref:`threadmodel` に示す.

.. figure:: img/threadmodel.png
  :name: threadmodel
  :width: 60%

  Xellicoのスレッドモデル

configファイルによる設定
------------------------

xellicoはjsonファイルを用いて設定を行うことができる.
設定項目は現在はRSS, TxBufferの二種類を構成可能である.

Xellicoは実行時にconfigを入力することで動作を調節することができる.
configで設定可能な項目は以下のものがある. configは実行時引数の
``-f <filename>`` で指定可能である.

サンプルのconfigファイルを以下に示す.
このファイルは各ポートのRx Queueの数を4に設定し, 32bulk Transmissionを
行うようにXellicoを設定するconfigファイルである.

.. code-block:: none

  {
    "qconf": [
      { "port_id":0, "queue_id":0, "lcore_id":2 },
      { "port_id":1, "queue_id":0, "lcore_id":3 },
      { "port_id":0, "queue_id":1, "lcore_id":4 },
      { "port_id":1, "queue_id":1, "lcore_id":5 }
    ],
    "txbulk": 16
  }

