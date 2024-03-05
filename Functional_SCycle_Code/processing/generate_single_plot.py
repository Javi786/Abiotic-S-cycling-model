'''
Should be able to generate different time evolution plots.
    - choose folder (with comparison capability)
    - choose data to output and axis
'''
import sys
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
PARSER.add_argument('-l', '--legend', action='append', default=[], help='legend labels')
PARSER.add_argument('-o', '--outname', default='output.png', help='output filename')
PARSER.add_argument('-e', '--earth', action='store_true', help='plot earth val')

ARGS = PARSER.parse_args()
PARAMS = Parameters().params
print("ARGS: ", ARGS)


COLORS = ['k', 'g', 'b', 'r', 'y', 'c']
STYLES = ['-', ':', '-.', '--']

FIG = plt.figure(figsize=(8, 8))
handles = []



def plot_time_evolution(ARGS):
    Final_Values = []
    islog = False
    print("ARGS.fnames: ", ARGS.fnames)
    if len(ARGS.fnames) < 5:
        for j, fname in enumerate(sorted(ARGS.fnames)):
            xaxis, yaxis = loadfile(fname, ARGS.columns, PARAMS)
            #print(xaxis, yaxis)
            if xaxis is None:
                continue
            print("Enumerated_ ARGS collumns = ", ARGS.columns)
            for i, col in enumerate(ARGS.columns):
                h, = plt.plot(xaxis, yaxis[i], COLORS[j], ls=STYLES[j], lw=2, label = str(col))
                print("HEREEE")
                print(col)
                handles.append(h)
                if ARGS.earth:
                    plt.axhline(y=PARAMS[col].earthscale, color='k', ls='--', lw=2)
    else: # fill_between plot
        islog = False
        print("Enumerated_ ARGS collumns = ", ARGS.legend)
        counts_reservoirs = 0
        for i, col in enumerate(ARGS.columns):
            min_vals = None
            max_vals = None
            for fname in ARGS.fnames:
                print("fname -> ", fname)
                print("col", col)
                print("HEREEE")
                xaxis, yaxis = loadfile(fname, col, PARAMS)
                print(xaxis, yaxis)
                if xaxis is None:
                    continue

                if min_vals is None:
                    min_vals = yaxis[0]
                    max_vals = yaxis[0]
                    continue
                    
                elif min_vals.size == 0:
                    min_vals = yaxis[0]
                    max_vals = yaxis[0]
    
                if len(yaxis[0]) > 0:
                    x_vals = xaxis
                    min_vals = np.minimum(min_vals, yaxis[0])
                    max_vals = np.maximum(max_vals, yaxis[0])
            Final_Values.append([str(col), min_vals[-1], max_vals[-1]])   
    	    
            print('min ----->', min_vals)  
            print('max ----->', max_vals)
            
            if max(min_vals) < 1000 and max(max_vals) < 1000:
                min_vals = [abs(x) for x in min_vals]
                max_vals = [abs(x) for x in max_vals]
            
            print("Counts Reservoirs: ", counts_reservoirs)
            
            reservoir = ARGS.legend[counts_reservoirs]
            h = plt.fill_between(x_vals, min_vals, max_vals,
                                 lw=None, edgecolor=COLORS[i], facecolor=COLORS[i], label = str(reservoir))
            handles.append(h)
            counts_reservoirs += 1
            
            print(np.max(max_vals[0]), np.min(min_vals[0]))
            if np.max(max_vals[0])/np.min(min_vals[0]) > 10 and np.min(min_vals[0]) > 0:
                islog = True
            
            #islog = True
            
	    
            if ARGS.earth:
                plt.axhline(y=PARAMS[col].earthscale, color=COLORS[i], ls='--', lw=2, label = str(reservoir)+str( " (PEL)"))
                print(reservoir)
        
    plt.ylabel(PARAMS[ARGS.columns[0]].ylabel)
    plt.xlabel('Time since formation [Ma]')
    plt.legend(loc='lower left' ,fontsize = 18)

    #plt.annotate('a)', xy = (0.90, 0.05), xycoords = 'axes fraction', fontsize = 24) #Mantle
    #plt.annotate('b)', xy = (0.90, 0.05), xycoords = 'axes fraction', fontsize = 24) #Oc Crust
    #plt.annotate('c)', xy = (0.90, 0.05), xycoords = 'axes fraction', fontsize = 24) #Cont Crust
    #plt.annotate('d)', xy = (0.90, 0.05), xycoords = 'axes fraction', fontsize = 24) #Marine Sediments
    #plt.ylim(1.5e11, 2e20)
    #plt.annotate('e)', xy = (0.90, 0.05), xycoords = 'axes fraction', fontsize = 24) #Oceans
    #plt.ylim(2e10, 2.5e18)
    plt.annotate('f)', xy = (0.90, 0.05), xycoords = 'axes fraction', fontsize = 24) #Atmosphere


    plt.xlim(-20, 4520)
    #plt.ylim(2e10, 2.5e18)
    plt.xticks(np.arange(500, 5500, 1000))
    
    print("Final Values = ", Final_Values)
    
    #if islog:
        #plt.yscale('log')
        
    plt.yscale('log')

    plt.grid(ls='dashed')
    plt.savefig(ARGS.outname, format='png', bbox_inches='tight')

if __name__ == "__main__":
    plot_time_evolution(ARGS)
