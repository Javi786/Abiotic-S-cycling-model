# -*- coding: utf-8 -*-
"""
@author: mlaneuville
"""
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from matplotlib.backends.backend_pdf import PdfPages
from collections import OrderedDict
from PyPDF2 import PdfFileReader, PdfFileMerger
import graphviz
import os
import sys

from tools import Parameters

PARAMS = Parameters().params

class Report:
    def __init__(self, name, sim_dir="../output"):
        self.graph_fname = sim_dir + "/out_" + name + ".dot"
        self.fname = sim_dir + "/out_" + name + ".txt"
        self.data = self.load_data()
        self.output_file = "report_"+self.fname.replace("/","-")[3:]+".pdf"
        self.timeseries = OrderedDict()
        self.subplots = []


    def load_data(self):
        return pd.read_csv(self.fname)


    def get_label(self, name):
        species = {"0": "N2", "1": "NOx", "2": "NH4", "3":"NA"}
        label = name
        if isinstance(name, list):
            label = []
            for value in name:
                if value[-1].isdigit():
                    label.append(value[:-1] + " -- " + species[value[-1]])
                else:
                    label.append(value[:17])
        return label


    def add_timeseries(self, name, colnames=[], ylabel="", isLog=False, norm=[]):
        if len(colnames) == 0:
            colnames = [name]
        if len(colnames) != len(norm):
            val = 1 if len(norm) == 0 else norm[0]
            norm = [val for x in colnames]
    
        fig, ax = plt.subplots(1, 1)
        self.timeseries[name] = {'fig':fig, 'ax':ax, 'ylabel':ylabel,
                                 'isLog':isLog, 'colnames':colnames, 'norm':norm} 


    def add_subplot(self, plots):
        if len(plots) == 0 or len(plots) > 4:
            print("Wrong number of plots: "+str(len(plots)))
            sys.exit()

        fig, axarr = plt.subplots(2, 2, sharex=True)

        cols = []
        ylabels = []
        norms = []
        islog = []
        earths = []
        for k, plot in plots.items():
            cols.append(plot['cols'])
            ylabels.append(plot['ylabel'])

            norm = [1]*len(plot['cols']) if 'norm' not in plot.keys() else plot['norm']
            norms.append(norm)

            log = True if 'isLog' in plot.keys() else False
            islog.append(log)

            earth = [-1] if 'earth' not in plot.keys() else plot['earth']
            earths.append(earth)

        self.subplots.append({'fig':fig, 'ax':axarr, 'cols':cols, 'ylabels':ylabels, 
                              'norms':norms, 'isLog':islog, 'earth':earths}) 

    def process(self):

        for col in self.data.columns:
            if col in ['iter', 'time']:
                continue
            if len(np.unique(self.data[col].iloc[1:])) == 1:
                continue

            print(col)
            p = PARAMS.get(col, PARAMS['default'])
            fig, ax = plt.subplots(1, 1)
            self.timeseries[col] = {'fig':fig, 'ax':ax, 'ylabel':p.ylabel,
                                    'colnames':[col], 'norm':[p.norm]}

        for k, v in self.timeseries.items():
            print("Plotting "+k)
            for column, norm in zip(v['colnames'], v['norm']):
                label = self.get_label(column)
                data = self.data[column][1:]/norm
                v['ax'].plot(self.data['time'][1:], data, lw=2, label=label)

            v['ax'].legend(loc='best')
            v['ax'].set_ylabel(v['ylabel'])
            v['ax'].set_xlabel("Time since formation [Ma]")
            v['ax'].set_title(k)
            v['ax'].set_xlim(0, 4500)
            v['ax'].grid()

            if np.max(data)/np.min(data) > 10 and np.min(data) > 0:
                v['ax'].set_yscale('log')

        color = ['b', 'g', 'r']
        for sub in self.subplots:
            for i, (col, norm, log, earth) in enumerate(zip(sub['cols'], sub['norms'], sub['isLog'], sub['earth'])):
                idx = (i/2, i%2)
                data = self.data[col][1:]/norm
                if col[0][:3] == "Oce":
                    data["Oceans0"] /= self.data["Henry1"][1:].values
                    data["Oceans1"] /= self.data["Henry1"][1:].values
                    data["Oceans2"] /= self.data["Henry1"][1:].values
                data = np.ma.masked_where(data < 0, data)
                label = self.get_label(col)
                sub['ax'][idx].plot(self.data['time'][1:], data, lw=2)
                if earth[0] > -1:
                    for c, line in zip(color,earth):
                        sub['ax'][idx].axhline(y=line, color=c, ls='--', lw=2)
                sub['ax'][idx].set_xticks(np.arange(0, 4500, 1000))
                sub['ax'][idx].set_ylabel(sub['ylabels'][i])
                leg = sub['ax'][idx].legend(labels=label, loc='best')
                leg.draw_frame(False)
                if log:
                    sub['ax'][idx].set_yscale('log')
                sub['ax'][idx].grid()
                sub['fig'].tight_layout()
                if i >= 2:
                    sub['ax'][idx].set_xlabel("Time since formation [Ma]")


    def to_file(self, with_graph=True):

        graph_src = open(self.graph_fname, "r")
        dot = graphviz.Source(graph_src.read(), engine="dot", format="pdf")
        dot.render()

        with PdfPages(self.output_file) as pdf:
            for k, v in self.timeseries.items():
                pdf.savefig(v['fig'])
                plt.close()

            for v in self.subplots:
                pdf.savefig(v['fig'])
                plt.close()


        graph = PdfFileReader("Source.gv.pdf")
        report = PdfFileReader(self.output_file)

        merger = PdfFileMerger()
        if with_graph:
            merger.append(graph)
        merger.append(report)
        merger.write(self.output_file)
        os.remove("Source.gv.pdf")
        os.remove("Source.gv")
