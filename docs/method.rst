
方法
=====

本研究は, 以下の項目を前提として研究を進めていく.

- NICの性能が向上するにつれ, 1ポートを複数のコアで処理する必要がある
- 複数プロセッサシステムでDPDKを動作させる場合,
  inter-processorではパケットをbulk-transmissionする必要がある
- RSSのスケーラビリティを正確に計測を行たい
- bulk-transmissionを正確に計測を行たい

多くのDPDKアプリケーションで高性能化のために必要不可欠と言われる,
RSSとTxBufferの構成を変化させ, 高性能通信を行う上での最適化
設定構成を明らかにしたい.

本研究では,これらの性能計測を円滑に行うために,
Xellicoというソフトウェアルータを新規に開発し, 計測を行なった.
Xellicoはオープンソースで開発を行なっており,DPDKのexample/l3fwd
をforkして開発開始を行なった. 本研究で調節を行たい, RSSと
TxBufferの構成パターンを実行時にjsonファイルから入力することができ,
性能評価に適している.

本研究の性能計測は,2台のXeonサーバを用いて行なった, 片方でDUTとして
Xellicoを動作させ, 他方では通信性能計測のためにpktgen-dpdkを動作させた.
性能は帯域と遅延に関して行う.  また, 性能計測結果の正確な考察を行うために,
TimeStampClockを用いてDPDKの関数のマイクロレイテンシを計測した.

.. toctree::
  :maxdepth: 1
  :caption: Contents:

  method_environment
  method_xellico
  method_delay
  method_rss
  method_txbuffer


