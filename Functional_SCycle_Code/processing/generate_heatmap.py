'''
Generates a series of heatmaps to show the distribution of nitrogen content in a
given reservoir (x-axis) as a function of redox parameter (y-axis). The color
scale is a histogram of the distribution of random runs for a given redox value.
'''

from glob import glob
import os
import argparse
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

COLUMNS = ["Atmosphere0", "CCrust2", "Oceans0", "Oceans1", "Oceans2", "LMantle2", "UMantle2", "OCrust2"]

UNITS = {"Atmosphere0": "PAL",
         "CCrust2": "PAL",
         "Oceans0": "mM",
         "Oceans1": "mM",
         "Oceans2": "mM",
         "LMantle2": "ppm",
         "UMantle2": "ppm",
         "OCrust2": "ppm"}

NORMS = {"Atmosphere0": 4e18,  # PAL
         "CCrust2": 4e18,      # PAL
         "Oceans0": 3.8e16,    # mM ; molar = m*1e3 g/(28g/mol)/1.35e21 L
         "Oceans1": 5.1e16,    # mM ; average of 30 and 46 g/mol
         "Oceans2": 2.4e16,    # mM ; average of 17 and 18 g/mol
         "LMantle2": 2.5e18,   # ppm ; v(lmantle) = 6.3e20 m3, rho = 4000 kg/m3
         "UMantle2": 9.6e17,   # ppm ; v(umantle) = 2.4e20 m3, rho = 4000 kg/m3
         "OCrust2": 1e15}      # ppm ; msed = 1.04e21 kg

PARSER = argparse.ArgumentParser()
PARSER.add_argument('-f', '--folder', help='data directory')

ARGS = PARSER.parse_args()

DATA_DIRECTORY = ARGS.folder

def get_full_data(field_, norm, time):
    '''Get data (time and field vectors) for a given time. Fetches output from
       all folders in DATA_DIRECTORY.'''


    folder = DATA_DIRECTORY
    ndata = len(glob(folder+'/*.txt'))

    output = []
    for data_file in sorted(glob(folder+'/*.txt')):
        dataframe = pd.read_csv(data_file)

        split = data_file.split("_")
        xlab = split[1]
        ylab = split[3]
        xx = float(split[2])
        yy = float(split[4])

        reservoir = dataframe[field_].values[time]/norm
        simtime = dataframe["time"].values[time]

        output.append({'x':xx, 'y':yy, 'z':reservoir})

    a = sorted(output, key=lambda v:(v['x'],v['y']))
    xval = [x['x'] for x in a]
    yval = [x['y'] for x in a]
    zval = [x['z'] for x in a]
    return xval, yval, zval, simtime, ndata, xlab, ylab

def make_time_plot(field_, norm, time):
    '''Makes the heatmap for a given time and field.'''

    xval, yval, zval, stime, ndata, xlab, ylab = get_full_data(field_, norm, time)

    x, y = np.meshgrid(np.unique(xval), np.unique(yval))
    z = np.reshape(zval, x.shape).T

    plt.figure()

    plt.pcolormesh(x, y, z)
    plt.colorbar()
    plt.yscale('log')

    plt.title("%s - %04d Ma [%s]" % (field_, stime, UNITS[field_]))
    plt.xlabel(xlab)
    plt.ylabel(ylab)
    plt.savefig("%s-%03d.png" % (field_, time), format="png")
    plt.close()

def run_timeloop(field_):
    '''Makes all heatmaps for a given field'''
    print(field_)
    norm = NORMS[field_]
    make_time_plot(field_, norm, -1)

for field in COLUMNS:
    run_timeloop(field)
