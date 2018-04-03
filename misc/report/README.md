
# Benchmark of DPDK

- 目的
	- RSSのスケーラビリティを調べる
	- tx-bufferの有用性に関して調べる

- dpdk
	- version is v17.11
	- forked from dpdk.org
- NIC
	- x540 (10GbE)
	- xl710 (40GbE)
- forwarding
	- 32rx/1tx w/ buffer
	- 32rx/1tx w/o buffer
	- 32rx/32tx w/ buffer
	- 32rx/32tx w/o buffer
	- 1rx/1tx w/ buffer
	- 1rx/1tx w/o buffer
- rss conf
	- 1-8 queues
- pkt size
	- 64,128,196,256,512,1024,1518


