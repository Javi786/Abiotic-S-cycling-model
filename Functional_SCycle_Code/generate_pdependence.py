'''
'''

from glob import glob
import os
import argparse
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import yaml
from matplotlib import rcParams
from math import degrees, atan2

rcParams.update({'font.size': 20})

PARAMS = {}
PARAMS['Atmosphere1'] = {'norm':3.6e9, 'ylabel':'Atmospheric Ox. S-content [PEL]', 'isLog':False, 'Earth':1} #changed
#PARAMS['Oceans1'] = {'norm':5.1e16, 'ylabel':'N-content [mM]', 'isLog':False, 'Earth':0.1}
#PARAMS['Oceans2'] = {'norm':2.4e16, 'ylabel':'N-content [mM]', 'isLog':True, 'Earth':3e-4}
#PARAMS['LMantle2'] = {'norm':2.5e16, 'ylabel':'N-content [ppm]', 'isLog':False, 'Earth':1}
#PARAMS['UMantle2'] = {'norm':9.6e17, 'ylabel':'N-content [ppm]', 'isLog':False, 'Earth':1}
PARAMS['OCrust0'] = {'norm':3.4e18, 'ylabel':'Ocean Crust Red. S-content [PEL]', 'isLog':True, 'Earth':1} #changed
PARAMS['OCrust1'] = {'norm':4.8e17, 'ylabel':'Ocean Crust Ox. S-content [PEL]', 'isLog':True, 'Earth':1} #changed
PARAMS['Sediments0'] = {'norm':8.8e17, 'ylabel':'Marine Sediments Red. S-content [PEL]', 'isLog':True, 'Earth':1} #changed
PARAMS['Sediments1'] = {'norm':2.2e17, 'ylabel':'Marine Sediments Ox. S-content [PEL]', 'isLog':True, 'Earth':1}
PARAMS['CCrust0'] = {'norm':1.8e19, 'ylabel':'Continental Crust Red. S-content [PEL]', 'isLog':True, 'Earth':1} #changed
PARAMS['CCrust1'] = {'norm':6.20e18, 'ylabel':'Continental Crust Ox. S-content [PEL]', 'isLog':True, 'Earth':1} #changed
#PARAMS['Volcanism0'] = {'norm':1e11, 'ylabel':'Degassing rate [10$^{11}$ kg yr$^{-1}$]', 'isLog':False}
#PARAMS['Volcanism1'] = {'norm':1e11, 'ylabel':'Degassing rate [10$^{11}$ kg yr$^{-1}$]', 'isLog':False}
#PARAMS['Volcanism2'] = {'norm':1e11, 'ylabel':'Degassing rate [10$^{11}$ kg yr$^{-1}$]', 'isLog':False}
#"""
PARAMS['Oceans0'] = {'norm':4.3e12, 'ylabel':'Oceans Red. S-content [PEL]', 'isLog':True, 'Earth':1}
PARAMS['Oceans1'] = {'norm':1.3e18, 'ylabel':'Oceans Ox. S-content [PEL]', 'isLog':True, 'Earth':1}
PARAMS['UMantle0'] = {'norm':5.5e20, 'ylabel':'Upper Mantle S-content [PEL]', 'isLog':True, 'Earth':1}
PARAMS['LMantle0'] = {'norm':15e20, 'ylabel':'Lower Mantle S-content [PEL]', 'isLog':True, 'Earth':1}
"""

PARAMS['Oceans0'] = {'norm':4.18e13, 'ylabel':'Oceans P concentration [\u03bcM]', 'isLog':True, 'Earth':2.2}
PARAMS['UMantle0'] = {'norm':1.1e18, 'ylabel':'Upper Mantle P concentration [ppm]', 'isLog':True, 'Earth':1}
PARAMS['LMantle0'] = {'norm':3e18, 'ylabel':'Lower Mantle P concentration [ppm]', 'isLog':True, 'Earth':1}
"""

PARAMS_DEFAULT = {'norm':1, 'ylabel':'[]', 'isLog':True, 'Earth':1}

PROPERTIES = {}
# 1) Erosion Rates for varying Subduction Rates
#PROPERTIES['subrate-050'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'} #10⁻⁵ to 10⁻² m/yr
#PROPERTIES['subrate-100'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'} #this one!!!
# #PROPERTIES['subrate-150'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'}
#PROPERTIES['subrate-200'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'}

# 2) Erosion Rates for Pre and Post GOE Earth
PROPERTIES['pre-goe'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'} #10⁻⁵ to 10⁻² m/yr
PROPERTIES['post-goe'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'} #this one!!!

# 3) Erosion Rates for several OWS-remaining fractions
#PROPERTIES['OWS_erosion_e_1e-6'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'} #10⁻⁵ to 10⁻² m/yr
#PROPERTIES['OWS_erosion_e_1e-4'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'} #this one!!!
#PROPERTIES['OWS_erosion_e_1e-2'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'}

# 4) Sulfate Solubility for several Erosion Rates:
#PROPERTIES['SulfSol_Erosion_rate-1e-2'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'} #10⁻⁶ to 10⁻¹ m/yr
#PROPERTIES['SulfSol_Erosion_rate-2e-4'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'} #this one!!!
#PROPERTIES['SulfSol_Erosion_rate-1e-5'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'}

# 5) Sulfate Solubility for several Subduction Rates:
#PROPERTIES['SulfSol_Subrate_050'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'} #10⁻⁶ to 10⁻¹ m/yr
#PROPERTIES['SulfSol_Subrate_100'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'} #this one!!!
#PROPERTIES['SulfSol_Subrate_200'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'}

# 6) Sulfate Solubility for several HT removal rates (done)
#PROPERTIES['SulfSol_HT_1e-9'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'} #10⁻⁶ to 10⁻¹ m/yr
#PROPERTIES['SulfSol_HT_1e-7'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'} #this one!!!
#PROPERTIES['SulfSol_HT_1e-5'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'}

# 7) Sulfate Solubility for several Baryte removal rates (done)
#PROPERTIES['SulfSol_Baryte_1e-1'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'} #10⁻⁶ to 10⁻¹ m/yr
#PROPERTIES['SulfSol_Baryte_1e-2'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'} #this one!!!
#PROPERTIES['SulfSol_Baryte_1e-3'] = {'xlabel':'Sulfate Max. Ocean Concentration [mol/L]'}

# 8) Atmospheric Timescales for several Subduction Rates:
#PROPERTIES['AtmTime_Subrate_050'] = {'xlabel':'Atmospheric Half-life of S [years]'} # 1e-3 years to 1e3 years (~ 9 hours to 1000 years)
#PROPERTIES['AtmTime_Subrate_100'] = {'xlabel':'Atmospheric Half-life of S [years]'} #this one!!!
#PROPERTIES['AtmTime_Subrate_200'] = {'xlabel':'Atmospheric Half-life of S [years]'}

#PROPERTIES['accretion-25'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'}#10⁻⁵ to 10⁻² m/yr
#PROPERTIES['accretion-50'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'}
#PROPERTIES['accretion-75'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'}

#PROPERTIES['subrate-050'] = {'xlabel':'Maximum Ocean Concentration of P [M]'} #1x10⁻⁷ to 1x10⁻⁴ M
#PROPERTIES['subrate-100'] = {'xlabel':'Maximum Ocean Concentration of P [M]'} #this one!!!
#PROPERTIES['subrate-150'] = {'xlabel':'Maximum Ocean Concentration of P [M]'}

#PROPERTIES['subrate-050'] = {'xlabel':'Continental Accretion Efficiency'} # 0 - 1
#PROPERTIES['subrate-100'] = {'xlabel':'Continental Accretion Efficiency'} #this one!!!
#PROPERTIES['subrate-150'] = {'xlabel':'Continental Accretion Efficiency'}

#PROPERTIES['factor-01'] = {'xlabel':'Mantle Mixing Rate'} # 10⁻¹⁰ to 10⁻⁶
#PROPERTIES['factor-05'] = {'xlabel':'Mantle Mixing Rate'} #this one!!!
#PROPERTIES['factor-10'] = {'xlabel':'Mantle Mixing Rate'}
#PROPERTIES['factor-100'] = {'xlabel':'Mantle Mixing Rate'}
#PROPERTIES['factor-1000'] = {'xlabel':'Mantle Mixing Rate'}

#PROPERTIES['factor-01'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'} #10⁻⁵ to 10⁻² m/yr
#PROPERTIES['factor-05'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'} #this one!!!
#PROPERTIES['factor-10'] = {'xlabel':'Erosion rate [m yr$^{-1}$]'}

#PROPERTIES['HT-2e-8'] = {'xlabel':'Maximum Ocean Concentration of S [M]'} #0.1x10⁻⁶ to 100x10⁻⁶ M
#PROPERTIES['HTe-7'] = {'xlabel':'Maximum Ocean Concentration of S [M]'} #this one!!!
#PROPERTIES['HT-5e-7'] = {'xlabel':'Maximum Ocean Concentration of S [M]'}
#PROPERTIES['HTe-7'] = {'xlabel':'Maximum Ocean Concentration of S [M]'}
#PROPERTIES['HTe-5'] = {'xlabel':'Maximum Ocean Concentration of S [M]'}

#PROPERTIES['Erosion-2e-5'] = {'xlabel':'Maximum Ocean Concentration of S [M]'} #3e-5 M = 0,03 mM to 3 M = 3000 mM, for an estimated 30mM
#PROPERTIES['Erosion-2e-4'] = {'xlabel':'Maximum Ocean Concentration of S [M]'} #this one!!!
#PROPERTIES['Erosion-2e-3'] = {'xlabel':'Maximum Ocean Concentration of S [M]'}

#PROPERTIES['subrate-050'] = {'xlabel':'Maximum Ocean Concentration of S [M]'} #3e-5 M = 0,03 mM to 3 M = 3000 mM, for an estimated 30mM
#PROPERTIES['subrate-100'] = {'xlabel':'Maximum Ocean Concentration of S [M]'} #this one!!!
#PROPERTIES['subrate-200'] = {'xlabel':'Maximum Ocean Concentration of S [M]'}

#PROPERTIES['factor-01'] = {'xlabel':'Volcanism P Enrichment Factor'} # 1 to 10, linear
#PROPERTIES['factor-03'] = {'xlabel':'Volcanism P Enrichment Factor'} #this one!!!
#PROPERTIES['factor-10'] = {'xlabel':'Volcanism P Enrichment Factor'}
#PROPERTIES['factor-30'] = {'xlabel':'Volcanism P Enrichment Factor'}

#PROPERTIES['subrate-050'] = {'xlabel':'Volcanism P Enrichment Factor'} # 1 to 10, linear
#PROPERTIES['subrate-100'] = {'xlabel':'Volcanism P Enrichment Factor'} #this one!!!
#PROPERTIES['subrate-150'] = {'xlabel':'Volcanism P Enrichment Factor'}

#PROPERTIES['subrate-050'] = {'xlabel':'Barite Sulfate Precipitation Fraction per 5000 years'} # 1 to 1e-12, log
#PROPERTIES['subrate-100'] = {'xlabel':'Barite Sulfate Precipitation Fraction per 5000 years'} #this one!!!
#PROPERTIES['subrate-150'] = {'xlabel':'Barite Sulfate Precipitation Fraction per 5000 years'}

"""
PROPERTIES['manmix-25'] = {'xlabel':'Mantle mixing rate [-]'}
PROPERTIES['manmix-50'] = {'xlabel':'Mantle mixing rate [-]'}
PROPERTIES['manmix-75'] = {'xlabel':'Mantle mixing rate [-]'}
PROPERTIES['ocemix-25'] = {'xlabel':'Ocean HT. circulation rate [-]'}
PROPERTIES['ocemix-50'] = {'xlabel':'Ocean HT. circulation rate [-]'}
PROPERTIES['ocemix-75'] = {'xlabel':'Ocean HT. circulation rate [-]'}
PROPERTIES['man-ox'] = {'xlabel':'Mantle redox state [-]'}
PROPERTIES['atm-ox'] = {'xlabel':'Atmosphere redox state [-]'}
PROPERTIES['out2in'] = {'xlabel':'Biotic direction ratio [-]'}
PROPERTIES['F0'] = {'xlabel':'Biotic cycling rate [-]'}
"""

PARSER = argparse.ArgumentParser()
PARSER.add_argument('-f', '--folder', action='append', default=[], help='data directory')
PARSER.add_argument('-o', '--outname', default='output.eps', help='output filename')
PARSER.add_argument('-l', '--legend', action='append', default=[], help='legend labels')
PARSER.add_argument('-c', '--column', action='append', default=[], help='column(s) to plot')
PARSER.add_argument('-r', '--relative', action='store_true', help='divide by value at t=0')
PARSER.add_argument('-n', '--notation', help='plot description, i.e., (a), (b)...')

ARGS = PARSER.parse_args()

DATA_DIRECTORY = ARGS.folder
COLORS = ['b', 'g', 'r', 'orange', 'brown']
STYLES = ['-', '-', '-', '-', '-']



FIG = plt.figure(figsize=(8, 8))

#for folder in sorted(os.listdir(DATA_DIRECTORY)):
#    if not os.path.isdir(DATA_DIRECTORY+folder):
#        continue
#
#    folder = DATA_DIRECTORY + folder

def takefirst(elem):
    return elem[0]

xaxis = [ [] for y in ARGS.folder]
yaxis = [[ [] for x in ARGS.column ] for y in ARGS.folder]
y_err = [[ [] for x in ARGS.column ] for y in ARGS.folder]
values = [[ [] for x in ARGS.column ] for y in ARGS.folder]

print(ARGS.folder)

for k, folder in enumerate(ARGS.folder):
    files = glob(folder + '*.txt')
    print("folder = ", folder)
    print("files = ", glob(folder + '*.txt'))
    
    for data_file in sorted(files, key=lambda x: str(x.split("/")[-1].split("_")[1])):
        dataframe = pd.read_csv(data_file)
        param = str(data_file.split("/")[-1].split("_")[1].split(".")[0])
        foldername = str(data_file.split("/")[0])
        print(data_file, param)
    
        print(ARGS.column)
        for i, col in enumerate(ARGS.column):
            norm = PARAMS.get(col, PARAMS_DEFAULT)['norm']
            values[k][i].append(dataframe[col].values[-1]/norm)
        
        yaml_filename = foldername + "/config_" + param + ".yaml"
        
        with open(yaml_filename, 'r') as stream:
            dict_yaml = yaml.load(stream, Loader = yaml.FullLoader)
        
        #param = float(dict_yaml['Precipitation']['sol_ox'])  ### CRUCIAL: TO CHANGE THE PARAMETER IN STUDY HERE!
        param = float(dict_yaml['Erosion']['alpha'])
        
        xaxis[k].append(param)
    
        for i in range(len(ARGS.column)):
            #yaxis[i].append(np.mean(values[i])) why was I taking the mean here?!
            yaxis[k][i].append(values[k][i][-1])
            y_err[k][i].append(np.std(values[k][i]))

    print("X axis ->>>", xaxis)
    print("Y axis ->>>", yaxis)
    
    ordered_xaxis = [ [] for y in ARGS.folder]
    ordered_yaxis = [[ [] for x in ARGS.column ] for y in ARGS.folder]
    
    
    for k in range(len(yaxis)):
        for i in range(len(yaxis[k])):
            datapoints = []
            for j in range(len(yaxis[k][i])):
            
                point = (xaxis[k][j], yaxis[k][i][j])
                datapoints.append(point)
    
            #print(datapoints)
            sorted_datapoints = sorted(datapoints, key = takefirst)
    
            #print(sorted_datapoints)
            
            for j in sorted_datapoints:
                
                ordered_xaxis[k].append(j[0])
                ordered_yaxis[k][i].append(j[1])
    
    
    #xaxis = []
    #yaxis = [[ [] for x in ARGS.column ] for y in ARGS.folder]
    
    #for i in sorted_datapoints:
        #xaxis.append(i[0])
        #yaxis[0][0].append(i[1])
    	
    print("X axis ORDENADO ->>>", ordered_xaxis)
    print("Y axis ORDENADO ->>>", ordered_yaxis)
    
    xaxis = ordered_xaxis
    yaxis = ordered_yaxis

for i in range(len(ARGS.column)):
    for k, folder in enumerate(ARGS.folder):
        yval = np.array(yaxis[k][i])
        if ARGS.relative:
            yval /= yaxis[i][0]
        yerr = np.array(y_err[k][i])

        if len(ARGS.folder) > 1:
            plt.plot(xaxis[k], yval, COLORS[k], lw=2, ls=STYLES[k], label=ARGS.column[i])
        else:
            plt.plot(xaxis[k], yval, COLORS[i], lw=2, ls=STYLES[i], label=ARGS.column[i])
#    plt.fill_between(xaxis, yval-yerr, yval+yerr, facecolor=COLORS[i], alpha=0.5)

#if PARAMS[ARGS.column[0]]['isLog']:
#    plt.yscale('log')
plt.xscale('log')
#plt.yscale('log')

#plt.ylim(0)
print(xaxis)

max_xaxis = []
min_xaxis = []

for j in xaxis:
    print(xaxis)
    print(j)
    min_xaxis.append(j[0])
    max_xaxis.append(j[-1])
    
plt.xlim(min(min_xaxis), max(max_xaxis))

print(PROPERTIES[ARGS.folder[0].split("/")[-2]]['xlabel'])
plt.xlabel(PROPERTIES[ARGS.folder[0].split("/")[-2]]['xlabel'])

ylabel = PARAMS.get(col, PARAMS_DEFAULT)['ylabel']
plt.ylabel(ylabel)

def labelLine(line,x,label=None,align=True,**kwargs):

    ax = line.axes
    xdata = line.get_xdata()
    ydata = line.get_ydata()

    if (x < xdata[0]) or (x > xdata[-1]):
        print('x label location is outside data range!')
        return

    #Find corresponding y co-ordinate and angle of the
    ip = 1
    for i in range(len(xdata)):
        if x < xdata[i]:
            ip = i
            break

    y = ydata[ip-1] + (ydata[ip]-ydata[ip-1])*(x-xdata[ip-1])/(xdata[ip]-xdata[ip-1])

    if not label:
        label = line.get_label()

    if align:
        #Compute the slope
        dx = xdata[ip] - xdata[ip-1]
        dy = ydata[ip] - ydata[ip-1]
        ang = degrees(atan2(dy,dx))

        #Transform to screen co-ordinates
        pt = np.array([x,y]).reshape((1,2))
        trans_angle = ax.transData.transform_angles(np.array((ang,)),pt)[0]

    else:
        trans_angle = 0

    #Set a bunch of keyword arguments
    if 'color' not in kwargs:
        kwargs['color'] = line.get_color()

    if ('horizontalalignment' not in kwargs) and ('ha' not in kwargs):
        kwargs['ha'] = 'center'

    if ('verticalalignment' not in kwargs) and ('va' not in kwargs):
        kwargs['va'] = 'center'

    if 'backgroundcolor' not in kwargs:
        kwargs['backgroundcolor'] = ax.get_fc()

    if 'clip_on' not in kwargs:
        kwargs['clip_on'] = True

    if 'zorder' not in kwargs:
        kwargs['zorder'] = 2.5

    ax.text(x,y,label,rotation=trans_angle,**kwargs)

def labelLines(lines,align=True,xvals=None,legend=None,**kwargs):

    ax = lines[0].axes
    labLines = []
    labels = []

    #Take only the lines which have labels other than the default ones
    for line in lines:
        label = line.get_label()
        if "_line" not in label:
            labLines.append(line)
            labels.append(label)
    if legend:
        labels = legend

    if xvals is None:
        xmin,xmax = ax.get_xlim()
        xvals = np.linspace(xmin,xmax,len(labLines)+2)[1:-1]

    for line,x,label in zip(labLines,xvals,labels):
        labelLine(line,x,label,align,**kwargs)

#labelLines(plt.gca().get_lines(), xvals=[0.000005, 0.0015, 0.015], legend=ARGS.legend, zorder=2.5)
labelLines(plt.gca().get_lines(), xvals=[0.00003, 0.0003, 0.003], legend=ARGS.legend, zorder=2.5) #for erosion rates sweeping


plt.axvline(2.2e-4, ls='--', lw=2, color='black') #for erosion rates sweeping
#plt.axvline(3e-2, ls='--', lw=2, color='black')
plt.axhline(1, ls ='--', lw=1, color= 'black')
plt.xlim([1e-5, 1.2e-2])  #for erosion rates sweeping
#plt.xlim([0.9e-6, 1.2e-1])
#plt.ylim([5e-6, 3e-2])

#plt.yscale("log")
if ARGS.notation:
    plt.title(ARGS.notation, x=0.9, y=0.6)

#if len(ARGS.legend) != 0:
#    plt.legend(ARGS.legend, loc='upper left')

plt.grid()
plt.savefig(ARGS.outname, format='png', bbox_inches='tight')
