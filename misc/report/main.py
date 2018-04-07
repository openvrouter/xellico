#!/usr/bin/env python3

import math, sys
import numpy as np
import matplotlib.pyplot as plt
plt.switch_backend('agg')
from pprint import pprint

def main():
    infilename  = "data.csv"
    outfilename = "out.png"
    if (len(sys.argv) > 2):
        infilename  = sys.argv[1]
        outfilename = sys.argv[2]

    print("input : {}".format(infilename))
    print("output: {}".format(outfilename))
    data = np.loadtxt(infilename, delimiter=',', comments='#')
    idx = data[:,0]
    siz64 = data[:,1]
    siz128 = data[:,2]

    plt.ylabel('through put [Mbps]')
    plt.xlabel('# of RSS queues')

    plt.plot(siz64, label='slankdev')
    plt.plot(siz128, label='slankdev')
    plt.legend()
    plt.savefig(outfilename)


if __name__ == '__main__':
    main()
