
研究の方法
==========

ここでは,本レポート作成と研究の方法と方針にいてまとめる.
本レポートはXellicoというソフトウェアルータ
(現在はルーティング機能はない)を用いてRSSとTxBufferの機能の
評価を行う. 性能は帯域と遅延に関して行う. 計測にはpktgen-dpdk
を用いた, また, 性能計測結果の正確な考察を行うために,
TimeStampClockを用いてDPDKの関数のマイクロレイテンシを
計測した.

計算機環境
-----------

本研究で用いた計算機環境を示す.
計算機は, DUT, TrafficGenerator/Anlyzerの計2台の計算機を用いた.
片方のサーバ(DUT)でXellicoを動作させ, 他方のサーバで
pktgen-dpdkを動作させた.

実験に使用した計算機2台のスペックを以下に示す.
まずDUTに使用した計算機のスペックを以下に示す.

.. csv-table::
  :header: 要素, 概要
  :widths: 5, 10

  OS            , Ubuntu 16.04 LTS
  Linux Kernel  , 4.4.0-116-generic
  DPDK          , 17.11 (forked from dpdk.org)
  NIC           , Intel X540-AT2 (10GbE)
  CPU           , Intel(R) Xeon(R) Silver 4108 CPU @ 1.80GHz x2
  RAM           , DDR4 32GB
  HyperThreading, Disabled

次にTesterに使用した計算機のスペックを以下に示す.

.. csv-table::
  :header: 要素, 概要
  :widths: 5, 10

  OS            , Ubuntu 16.04 LTS
  Linux Kernel  , 4.4.0-116-generic
  DPDK          , 17.11 (forked from dpdk.org)
  NIC           , Intel X540-AT2 (10GbE)
  CPU           , Intel(R) Xeon(R) Silver 4108 CPU @ 1.80GHz x2
  RAM           , DDR4 32GB
  HyperThreading, Disabled

性能計測時は :numref:`pnw` に示す通りに接続して検証をおこなった.

.. figure:: img/evaluation_network.png
	:name: pnw

	実験環境の物理ネットワーク

性能計測方法
-------------

ブラックボックスNFテスト
^^^^^^^^^^^^^^^^^^^^^^^^^^

本レポートでの性能計測は, 原則として帯域と遅延に関して行った.
どちらもpktge-dpdkを用いて性能計測を行った.
計測はパケットサイズを64,128,192,...,1518と変更しながらそれぞれ
計測を行った. RSSを利用した性能計測を行うため, pktgenはrangeモードを
有効かし, pktgenから流れるトラフィックはそれぞれ別々のRSS Queueに
流れるように行った. ptkgen-dpdkは別途, DPDK用のサーバを用いて行った.

内部のマイクロレイテンシ
^^^^^^^^^^^^^^^^^^^^^^^^^


Xellico vRouterによる計測
-------------------------

本レポートで取得したデータは, Xellicoというソフトウェアルータを用いた.
Xellicoは筆者が開発しているDPDKのソフトウェアルータとして参照実装を
なることを目指して開発を行っているものである.

XellicoはGithubでOSSとして開発しており, https://github.com/slankdev/xellico
で全ソースコードにアクセス可能である. XellicoはDPDKのl3fwdサンプル
アプリケーションをforkし, 汎用のVNFとして使用できるように拡張を行っている.
本説では, Xellicoの設計と実装に関して簡単に説明を行う.

設計
^^^^^

まずは, 設計について説明する. XellicoはRun to Completionモデルで
実装されており, NICのRSSを用いて同じ役割のスレッドを多重に並列動作させる.
スレッドモデルを :numref:`threadmodel` に示す.

.. figure:: img/threadmodel.png
  :name: threadmodel

  Xellicoのスレッドモデル

Xellicoは実行時にconfigを入力することで動作を調節することができる.
configで設定可能な項目は以下のものがある. configは実行時引数の
``-f <filename>`` で指定可能である.

- RSSの構成 (# of rx-queue)
- Tx Bufferの構成 (# of bulk tx)

サンプルのconfigファイルを以下に示す.
このファイルは各ポートのRx Queueの数を4に設定し, 32bulk Transmissionを
行うようにXellicoを設定するconfigファイルである.

.. code-block:: none

  nrxq : 4
  btx : 32



