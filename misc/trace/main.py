#!/usr/bin/env python
# Copyright (c) PLUMgrid, Inc.
# Licensed under the Apache License, Version 2.0 (the "License")

# run in project examples directory with:
# sudo ./hello_world.py"
# see trace_fields.py for a longer example

from bcc import BPF

bpf_text="""
int slankdev(void *ctx) {
    bpf_trace_printk("Hello, World!\\n");
    return 0;
}
"""

b = BPF(text=bpf_text)
# path='/home/vagrant/git/xellico'
path='/home/vagrant/git/xellico/build/xellico'
b.attach_uprobe(name=path, sym='rte_eal_init', fn_name='slankdev')
b.trace_print()

