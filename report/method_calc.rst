


Packet/Secondの計算に関して
===========================

本研究では, 途中計算なしに, パケット処理能力の計算を行うことが多いため,
念のためにここでそれに関する方法を整理しておく.

単位に関して.

.. math::

  1 msec = 1 * 10^{-3} sec \\
  1 usec = 1 * 10^{-6} sec \\
  1 nsec = 1 * 10^{-9} sec \\
  1 GHz  = 1 * 10^{9} clocks/sec \\

64B,10Gbps に関して

.. math::

  10G bit/sec \\
  sizeof(Ethernet Frame) = 64 byte \\
  sizeof(preamble)       = 8  byte \\
  sizeof(FCS)            = 4  byte \\
  sizeof(InterFrameGap)  = 12 byte \\
  PacketSize = 88 byte = 704 bit \\

  10Gbit = 14.2 Mpacket \\
  64B, 10Gbps = 14Mpps \\

64B,10Gbps=14Mpps, 秒に関して

.. math::

  14Mpps = 14*10^6 packet/sec \\
  14*10^6 packet = 1 sec \\
  1 packet = frac {1} {14*10^6} sec \\
  1 packet = 71*10^-9 sec \\
  1 packet = 71 nsec \\

64B,10Gbps=14Mpps, 3GHz-CPUに関して

.. math::

  3GHz = 3*10^9 clock/sec \\
  14Mpps = 14M packet/sec \\

  14*10^6 packet = 3*10^9 clock \\
  1 packet = \frac {3*10^9} {14*10^6} clock \\
  1 packet = 214 clock

64B,10Gbps=14Mpps, 2GHz-CPUに関して

.. math::

  2GHz = 2*10^9 clock/sec \\
  14Mpps = 14M packet/sec \\

  14*10^6 packet = 2*10^9 clock \\
  1 packet = \frac {2*10^9} {14*10^6} clock \\
  1 packet = 142 clock

64B,100Gbps=142Mpps, 秒に関して

.. math::

  142Mpps = 142*10^6 packet/sec \\
  142*10^6 packet = 1 sec \\
  1 packet = frac {1} {142*10^6} sec \\
  1 packet = 7*10^-9 sec \\
  1 packet = 7 nsec !!!! \\

64B,100Gbps=142Mpps, 3GHz-CPUに関して

.. math::

  3GHz = 3*10^9 clock/sec \\
  142Mpps = 142 * 10^6 packet/sec \\

  142*10^6 packet = 3*10^9 clock \\
  1 packet = \frac {3*10^9} {142*10^6} clock \\
  1 packet = 21 clock !!!!


脇道にそれて少し考察
--------------------

**この部分は最後の考察に移動するかもしれない**

うーん, 実にすごい世界だ.... 100Gをするにはなかなかすごい計算を
しないと行けなさそうである. ちょっとだけ脇道にそれた計算をしよう...

2018年5月1日時点でもっともコア数が多く, 動作周波数もそこそこに
良いCPUである. Xeon SP Platinum 8180 [XEONSPplatinum8180]_
を使って100GNICを幾つかつけた
マシンを作ったと思って計算してみよう.

仮想のマシンスペックは以下のようになる.

.. csv-table::
  :header: 要素, 概要
  :widths: 5, 10

  NIC           , Mellanox Connect-X5 (100GbE) x4
  CPU           , Intel(R) Xeon(R) Platinum 8180 CPU @ 2.50GHz x4
  RAM           , DDR4 64GB
  HyperThreading, Disabled
  TurboBoost    , Enable 3.5GHz (default:2.5GHz)
  # of Cores    , 28 * 4 = 112

このマシンを使って, 100GbE x4の高速PCルータ (要するに400GbEルータ)
を開発する場合の計算速度を見積もってみよう.

まず前提計算としてPPSの計算をする.

64B,400GbpsのPPS計算

.. math::

  400Gbit/sec\\
  PacketSize = 88byte = 704bit \\
  400Gbit = 568Mpps \\

64B,400Gbps=568Mpps, 3.5GHz-CPUに関して

.. math::

  568Mpps = 568 * 10^6 packet / sec \\
  3.5GHz = 3.5 * 10^9 clock /sec \\

  568Mpps = 3.5Ghz \\
  568 * 10^6 packet = 3.5 * 10^9 clock \\
  1 packet = frac {3.5 * 10^9} {568 * 10^6} clock \\
  1 packet = 6 clock !! \\

コア数が112個あるので, RSSで完全並列にしたとして,

.. math::

  6 clock/packet, 1 cores \\
  672 clock/packet, 112 cores \\

うーん.. 帯域レベルを満足させるのなら, なんとかできそう..?
ちなみにこの構成でDEELのマシンを組んでみると以下のような見積もり値段である.
見積もり結果を :numref:`xeonplatinum` に示す.

.. figure:: img/xeonplatinum.png
  :name: xeonplatinum

  400GbE高速PCルータの計算機

900万円で400GルータのHWが揃うとおもうと, 安い.となるか. 高いとなるかは
みなさま次第だ.


