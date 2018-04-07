.. DPDK performance optimization documentation master file, created by
   sphinx-quickstart on Sat Apr  7 17:43:49 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

DPDK performance optimization
=============================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

はじめに
========


本レポートでは, DPDKの性能チューニングに関する要素技術の調査と, 性能評価を行った.
具体的には, RSSのコンフィギレーションと, Tx Buffer (Bulk Transmission)に関する
性能計測を行い, 一般的なDPDKアプリケーションの開発時の性能チューニングの助けとなる
ことが本レポートの目標である.

前言でも説明した通り, 本レポートでは, 以下の2つの技術に関しての調査と評価を行った.

- RSS (Receive Side Scaling)
- Tx Buffer (Bulk Transmission)

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


参考文献
--------

本レポートを記述するにあたり参考にした文献を示す.

- Hirochika Asai, Networking	Opera/ng	System	from	Scratch towards	High-Performance	COTS	Network	Facili/es, IIJ lab-seminor 2015/4/9, http://seminar-materials.iijlab.net/iijlab-seminar/iijlab-seminar-20150409-0.pdf
- DPDK Programmer's Guide, http://dpdk.org/doc/guides/prog_guide/index.html
- DPDK API Documentation, http://dpdk.org/doc/api/index.html
- DPDK L3fwd Sample Application Documentation, http://dpdk.org/doc/guides/sample_app_ug/l3_forward.html
- DPDK L3fwd Sample Application Source Code, http://dpdk.org/browse/dpdk/tree/examples/l3fwd?h=v17.11


性能計測評価に関して
--------------------

本レポートでの性能計測は, 原則として帯域と遅延に関して行った.
どちらもpktge-dpdkを用いて性能計測を行った. 性能計測時は :numref:`graph` に計算機を接続して
検証をおこなった.

.. todo:: あとで正しい画像に直す.

.. figure:: img/evaluation_network.png
	:name: graph

	実験環境の物理ネットワーク


今後の課題
==========

以下は未解決todoリストである.

.. todolist::
