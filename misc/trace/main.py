#!/usr/bin/env python2
from bcc import BPF

bpf_text="""
BPF_HASH(start, u64, u64);

int fcal(void *ctx) {
    u64 zero = 1;
    u64 tsp = bpf_ktime_get_ns();
    start.update(&tsp, &zero);
    return 0;
}

int fret(void *ctx) {
    u64 zero = 1;
    u64 tsp = bpf_ktime_get_ns();
    u64* before = start.lookup(&zero);
    if (before) {
        u64 delta = tsp - *before;
        bpf_trace_printk("delta: %lu nsec \\n", delta);
    } else {
        bpf_trace_printk("miss \\n");
    }
    start.delete(&zero);
    return 0;
}
"""

b = BPF(text=bpf_text)
path='/home/slank/git/xellico/misc/trace/a.out'
b.attach_uprobe(name=path, sym='yukaribonk', fn_name='fcal')
b.attach_uretprobe(name=path, sym='yukaribonk', fn_name='fret')
b.trace_print();

