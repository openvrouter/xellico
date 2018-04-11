
Delayの実験結果
--------------------

実験のプログラムの実行結果を以下に示す.

.. code-block:: none

  jk6:~/git/xellico/misc/delay_eval:) make
    CC main.o
    LD delayeval
    INSTALL-APP delayeval
    INSTALL-MAP delayeval.map
  jk6:~/git/xellico/misc/delay_eval:) make run
  sudo ./build/delayeval
  freq: 1795792297
  rte_delay_us_block(1):  lat=1841 [clk] try=1000000, delay=1us
  rte_delay_us_block(2):  lat=3686 [clk] try=1000000, delay=2us
  rte_delay_us_block(3):  lat=5460 [clk] try=1000000, delay=3us
  rte_delay_us_block(4):  lat=7250 [clk] try=1000000, delay=4us
  dirty_looped_delay_10ns(1):  lat=20 [clk] try=100000000, delay=11ns
  dirty_looped_delay_10ns(2):  lat=33 [clk] try=100000000, delay=18ns
  dirty_looped_delay_10ns(3):  lat=59 [clk] try=100000000, delay=32ns
  dirty_looped_delay_10ns(4):  lat=86 [clk] try=100000000, delay=47ns

これによりわかる各Delay関数の評価結果を表として以下に示す.
rte_delay_us_block関数の評価結果を示す.

.. csv-table::
  :header: 設定delay[usec], 遅延結果[clock], 遅延結果[usec], accurecy[%]
  :widths: 5, 5, 5, 5

  1, 1841, 1, (未計測)
  2, 3686, 2, (未計測)
  3, 5460, 3, (未計測)
  4, 7250, 4, (未計測)

dirty_looped_delay_10ns関数の評価結果を示す.

.. csv-table::
  :header: 設定delay[nsec], 遅延結果[clock], 遅延結果[usec], accurecy[%]
  :widths: 5, 5, 5, 5

  10, 20, 11, (未計測)
  20, 33, 18, (未計測)
  30, 59, 32, (未計測)
  40, 86, 47, (未計測)



