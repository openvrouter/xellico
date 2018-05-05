
.. _method:

研究の方法
==========

ここでは, 本研究の進め方を説明していく.
今後, 根本的なアーキテクチャの変更がなく,
より高性能をサポートするハードウェアが現れた場合でも,
本研究の手法を同じく容易に適用できるように研究方法を設定した.

まず最初に, 以下に示す環境を前提として研究を進めていく.

- OnlyCOTS: 一般的なIAアーキテクチャを採用し,
  FPGA等の特別なハードウェアをパケット処理に使用しない.
  NICはPCIeにより接続され, DPDKから利用可能な状態である.
  HW支援機能としてRSSが利用可能状態である.
- NUMA Aware: 複数プロセッサマシンを利用する

.. toctree::
  :maxdepth: 1

  method_calc
  method_practice
  method_goal
  method_environment
  method_xellico
  method_eval


