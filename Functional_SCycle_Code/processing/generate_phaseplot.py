import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams

from tools import Parameters, loadfile

rcParams.update({'font.size': 20, 'axes.axisbelow': True})

PARSER = argparse.ArgumentParser()
PARSER.add_argument(dest='fnames', nargs='*', help='file to use for plotting')
PARSER.add_argument('-c', '--columns', action='append', default=[], help='column(s) to plot')
PARSER.add_argument('-o', '--outname', default='output.png', help='output filename')
PARSER.add_argument('-e', '--earth', action='store_true', help='plot earth val')

ARGS = PARSER.parse_args()
PARAMS = Parameters().params

def plot_phase_space(ARGS):
    for fname in ARGS.fnames:

        xaxis, yaxis = loadfile(fname, ARGS.columns[1], PARAMS, xaxis=ARGS.columns[0])
        if xaxis is None:
            continue

        snapshots = [0, -1]
        colors = ['ko', 'k*']
        msize = [10, 20]

        plt.plot(xaxis, yaxis[0], 'k')
        for idx, col, size in zip(snapshots, colors, msize):
            plt.plot(xaxis[idx], yaxis[0][idx], col, markersize=size)

    plt.xlabel(PARAMS[ARGS.columns[0]].ylabel)
    plt.ylabel(PARAMS[ARGS.columns[1]].ylabel)
    plt.grid()
    plt.savefig(ARGS.outname, format='png', bbox_inches='tight')

if __name__ == "__main__":
    if len(ARGS.columns) != 2:
        print("Please specify two columns for phase space.")
        sys.exit()
    plot_phase_space(ARGS)
