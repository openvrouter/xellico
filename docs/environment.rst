
作業環境
=========

ハードウェアとソフトウェア
--------------------------

本レポートで使用した作業環境を以下に示す

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


性能計測評価に関して
--------------------

本レポートでの性能計測は, 原則として帯域と遅延に関して行った.
どちらもpktge-dpdkを用いて性能計測を行った.
性能計測時は :numref:`graph` に示す通りに接続して検証をおこなった.
計測はパケットサイズを64,128,192,...,1518と変更しながらそれぞれ
計測を行った. RSSを利用した性能計測を行うため, pktgenはrangeモードを
有効かし, pktgenから流れるトラフィックはそれぞれ別々のRSS Queueに
流れるように行った. ptkgen-dpdkは別途, DPDK用のサーバを用いて行った.

.. figure:: img/evaluation_network.png
	:name: graph

	実験環境の物理ネットワーク


