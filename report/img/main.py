#!/usr/bin/env python3
import math, sys
import numpy as np
import matplotlib.pyplot as plt
plt.switch_backend('agg')
from pprint import pprint


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
        plt.plot(siz64, label='pktsize=64')
        plt.plot(siz128, label='pktsize=128')
        plt.plot(siz256, label='pktsize=256')
        plt.plot(siz512, label='pktsize=512')
        plt.plot(siz1024, label='pktsize=1024')
        plt.xticks(range(len(idx)), idx)
        plt.legend()
        plt.savefig(outfile)

    # Txburst
    inside('rxburst_delay.csv', 'rxburst_delay.png')
    inside('txburst_delay.csv', 'txburst_delay.png')


def exp_rss():
    print("rss")

def exp_txbuffer():
    print("txbuffer")

def main():
    exp_dpdkfunc()
    exp_rss()
    exp_txbuffer()
    return

if __name__ == '__main__':
    main()

