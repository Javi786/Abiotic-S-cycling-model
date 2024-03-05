from glob import glob
import pandas as pd
import matplotlib.pyplot as plt
import argparse
import numpy as np
import os

parse = argparse.ArgumentParser()
parse.add_argument('-f', '--folder', default='output', type=str, 
                   help='Folder to look for out_* files')
args = parse.parse_args()

columns = ["Atmosphere0", "Oceans0", "Oceans1", "Oceans2", "LMantle2", "UMantle2", "OCrust2"]
norms = [4e18, 1.8e16, 1.8e16, 1.8e16, 2.2e18, 8.2e17, 1e15]

data = {}
for col in columns:
    data[col] = []

for data_file in glob(args.folder+'/*.txt'):
    print(data_file)
    df = pd.read_csv(data_file)
    total_mass = 0
    for col, norm in zip(columns, norms):
        reservoir = df[col].values[-1]
        data[col].append(reservoir/norm)
        total_mass += reservoir

if not os.path.isfile("summary.dat"):
    with open("summary.dat", "a") as out:
        for col in columns:
            out.write("%s-min," % col)
            out.write("%s-max," % col)
        out.write("\n")

f, axarr = plt.subplots(3, 3)
out = open("summary.dat", "a")
out.write("%s," % args.folder.split('_')[-1])


for i, col in enumerate(columns):
    print("%s, N = %d, min = %f, max = %f" % (col, len(data[columns[0]]), 
                                                   min(data[col]),
                                                   max(data[col])))

    axarr[i%3,i/3].hist(data[col])
    #axarr[i%3,i/3].set_title(col)
    axarr[i%3,i/3].plot(data[col][0], 10, 'ro')
    axarr[i%3,i/3].plot(data[col][2], 10, 'ko')
    axarr[i%3,i/3].plot(data[col][4], 10, 'go')
    hist, bins = np.histogram(data[col])
    valmin = bins[np.argmin(hist)]
    valmax = bins[np.argmax(hist)]
    
    out.write("%.4e," % valmin)
    out.write("%.4e," % valmax)

    ticks = np.arange(0, max(data[col]), 4)                                              
    axarr[i%3,i/3].set_xticks(ticks) 
    
out.write("\n")
out.close() 

plt.savefig("distributions.png", format="png")
