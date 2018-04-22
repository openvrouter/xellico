
RSSの実験結果
-------------

- x540の場合, RSSはリニアにスケールする (dummy)
- xl710の場合, RSSはリニアにはスケールしない(dummy) (HW限界)

実験結果のグラフを
:numref:`rss_throughput_bps` ,
:numref:`rss_throughput_pps` ,
:numref:`rss_latency` に示す.

まずbpsについて

.. csv-table::
  :file: img/rss_throughput_bps.csv

.. figure:: img/rss_throughput_bps.png
  :name: rss_throughput_bps

  RSSの性能計測結果(帯域) bps

次にppsについて

.. csv-table::
  :file: img/rss_throughput_pps.csv

.. figure:: img/rss_throughput_pps.png
  :name: rss_throughput_pps

  RSSの性能計測結果(帯域) pps

そしてレイテンシ

.. figure:: img/rss_latency.png
  :name: rss_latency

  RSSの性能計測結果(遅延)


