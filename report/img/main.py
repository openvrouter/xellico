#!/usr/bin/env python3
import math, sys
import numpy as np
import matplotlib.ticker as tick
import matplotlib.pyplot as plt
plt.switch_backend('agg')
from pprint import pprint


def exp_delay():
    print('delay')
    def inside(infile, outfile):
        data = np.loadtxt(infile,
            delimiter=',', comments='#')
        idx = data[:,0]
        lat = data[:,2]
        plt.clf()
        plt.ylabel('latency [usec]')
        plt.xlabel('delay[usec]')
        plt.xticks(range(len(idx)), idx)
        plt.gca().xaxis.set_minor_locator(tick.MultipleLocator(1))
        plt.gca().yaxis.set_minor_locator(tick.MultipleLocator(1))
        plt.grid(which='minor')
        plt.plot(lat, label='actual')
        plt.plot(idx, label='ideal')
        plt.legend()
        plt.savefig(outfile)
    inside('delay_us_latency.csv', 'delay_us_latency.png')
    inside('delay_ns_latency.csv', 'delay_ns_latency.png')


def exp_dpdkfunc():
    print('dpdkfunc')
    def inside(infile, outfile):
        data = np.loadtxt(infile,
            delimiter=',', comments='#')
        idx = data[:,0]
        siz64 = data[:,1]
        siz128 = data[:,2]
        siz256 = data[:,3]
        siz512 = data[:,4]
        siz1024 = data[:,5]
        plt.clf()
        plt.ylabel('latency [clock]')
        plt.xlabel('# of burst')
        plt.xticks(range(len(idx)), idx)
        plt.plot(siz64, label='pktsize=64')
        plt.plot(siz128, label='pktsize=128')
        plt.plot(siz256, label='pktsize=256')
        plt.plot(siz512, label='pktsize=512')
        plt.plot(siz1024, label='pktsize=1024')
        plt.legend()
        plt.savefig(outfile)
    inside('rxburst_delay.csv', 'rxburst_delay.png')
    inside('txburst_delay.csv', 'txburst_delay.png')

    data = np.loadtxt('txbuffer_delay.csv',
            delimiter=",", comments="#")
    idx = data[:,0]
    delays = data[:,1]
    plt.clf()
    plt.ylabel('latency [clock]')
    plt.xlabel('pkt size [bytes]')
    plt.xticks(range(len(idx)), idx)
    plt.plot(delays)
    plt.legend()
    plt.savefig('txbuffer_delay.png')

def blackbox_inside(inname, outname):
    data = np.loadtxt(inname,
        delimiter=",", comments="#")
    idx = data[:,0]
    tp64 = data[:,1]
    tp128 = data[:,2]
    tp192 = data[:,3]
    tp256 = data[:,4]
    tp512 = data[:,5]
    tp1024 = data[:,6]
    tp1514 = data[:,7]
    plt.clf()
    plt.ylabel('performance [Mbps or nsec]')
    plt.xlabel('# of rx-queues')
    plt.xticks(range(len(idx)), idx)
    plt.plot(tp64, label='pktsize=64')
    plt.plot(tp128, label='pktsize=128')
    plt.plot(tp192, label='pktsize=192')
    plt.plot(tp256, label='pktsize=256')
    plt.plot(tp256, label='pktsize=512')
    plt.plot(tp1024, label='pktsize=1024')
    plt.plot(tp1514, label='pktsize=1514')
    plt.legend()
    plt.savefig(outname)


def exp_rss():
    print("rss")
    blackbox_inside('rss_throughput.csv', 'rss_throughput.png')
    blackbox_inside('rss_latency.csv', 'rss_latency.png')


def exp_txbuffer():
    print("txbuffer")
    blackbox_inside('bulktx_throughput.csv', 'bulktx_throughput.png')
    blackbox_inside('bulktx_latency.csv', 'bulktx_latency.png')


def main():
    exp_delay()
    exp_dpdkfunc()
    exp_rss()
    exp_txbuffer()
    return


if __name__ == '__main__':
    main()
