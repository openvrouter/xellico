
# Benchmark of DPDK

- dpdk version is v17.11 (forked from dpdk.org)
- nic are x540 and xl710 for test

## exp1 RSS Evaluation

DPDKでRSSを有効にした時のベンチマークとスケーラビリティを示す.

```
pktsize\#queues   1    2    3    4    5    6    7    8
  64   [Mbps]
 128   [Mbps]
 256   [Mbps]
 512   [Mbps]
1024   [Mbps]
```

## exp2 Tx Buffer Evaluation

Tx bufferを有効かした時のベンチマークとスケーラビリティを示す.



## exp3 Bad-NUMA Evaluation



