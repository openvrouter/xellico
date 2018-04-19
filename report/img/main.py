#!/usr/bin/env python3
import math, sys
import numpy as np
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
        plt.plot(lat)
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




def exp_rss():
    print("rss")

def exp_txbuffer():
    print("txbuffer")

def main():
    exp_delay()
    exp_dpdkfunc()
    exp_rss()
    exp_txbuffer()
    return

if __name__ == '__main__':
    main()

