'''
Module to generate a series of initial conditions in a given folder. If
varchange is set (optional), oxidizing is set to varchange/100.
'''

import random
import yaml
import os
import sys
import argparse
import numpy as np
from copy import deepcopy

print("This will no longer work with the current config file structure.")
sys.exit()

TOTAL_BUDGET = 339 # 1e17 kg

DEFAULT_CONSTRAINTS = {0: 40,
                       1:  5,
                       2:  5,
                       3:  5,
                       4:  5,
                       5:120,
                       6:119,
                       7: 40}

MAP = {0: ["Atmosphere", 0],
       1: ["Oceans", 0],
       2: ["Oceans", 1],
       3: ["Oceans", 2],
       4: ["OCrust", 2],
       5: ["UMantle", 2],
       6: ["LMantle", 2],
       7: ["CCrust", 2]}

METHODS = {}
METHODS['man-ox'] = {'name':'man_ox', 
                     'values': np.linspace(0, 1, 20)}
METHODS['manmix'] = {'name':'Convection:F1',
                     'values': np.logspace(-10, -6, 20)}
METHODS['ocemix'] = {'name':'HydrothermalCirculation:F1',
                     'values': np.logspace(-10, -6, 20)}
METHODS['subrate'] = {'name': 'Subduction:tau',
                      'values': np.linspace(50e6, 150e6, 3)}
METHODS['erosion'] = {'name': 'Erosion:alpha',
                      'values': np.logspace(-5, -4, 3)}
METHODS['accretion'] = {'name': 'Subduction:accretion',
                        'values': np.linspace(0, 1, 3)}
METHODS['ocvol'] = {'name': 'Henry:change',
                    'values': [-0.4, 0, 0.4]}


def get_random_sequence():
    '''Generate a sequence of 8 integers which sum is TOTAL_BUDGET. The way to
       do this is to generate 7 random integers, add 0 and 20 to the list, order
       them and take the differences. The reason this works is that it
       corresponds to taking a rope and cutting it an random places and putting
       it back together.'''

    random.seed()
    reservoirs = []
    while len(reservoirs) < 7: # we want 7 numbers
        reservoirs.append(random.randint(1, TOTAL_BUDGET))

    reservoirs.append(0)
    reservoirs.append(TOTAL_BUDGET)
    reservoirs = sorted(reservoirs)

    return np.diff(reservoirs)

def get_constrained_sequence(d):
    '''Generate a sequence of 8 integers which sum is TOTAL_BUDGET, under constraints from
    d, a dict which is for instance {0:5, 3:4}.'''

    d_ = deepcopy(d)
    
    constrained = len(d_.keys())
    if constrained == 8:
        return list(d_.values())

    sum_constrained = sum(list(d_.values()))
    reservoirs = []
    while len(reservoirs) < 7 - constrained:
        reservoirs.append(random.randint(0, TOTAL_BUDGET-sum_constrained))

    reservoirs.append(0)
    reservoirs.append(TOTAL_BUDGET-sum_constrained)
    reservoirs = sorted(reservoirs)
    reservoirs = list(np.diff(reservoirs))

    for i in range(8):
        if i not in d_.keys():
            d_[i] = reservoirs.pop()

    return list(d_.values())

def get_default_config(fname):
    '''Read config.yaml from main directory to be used as default.'''
    stream = open(fname, "r")
    config = yaml.load(stream)
    stream.close()

    return config

def create_config(folder, fname, value='', constraints={}):
    '''Generate the new configuration based of the default config and the random
       sequence.'''
    cfg = get_default_config(fname)
    random_seq = get_random_sequence()

    if len(constraints.keys()) != 0:
        random_seq = get_constrained_sequence(constraints)

    for res, val in enumerate(random_seq):
        cfg["Reservoirs"][MAP[res][0]]["InitMasses"][MAP[res][1]] = "%.1e" % (val*1e17)
    cfg["OutFolder"] = folder

    fname = "config"
    for dim in value:
        parameter = next(iter(dim)).split(':')
        v = dim[next(iter(dim))]
        if len(parameter) == 1:
            cfg[parameter[0]] = '%.2e' % float(v)
        elif len(parameter) == 2:
            cfg[parameter[0]][parameter[1]] = '%.2e' % float(v)
        else:
            print("Unrecognized number of arguments")
            sys.exit(1)
        fname += "_" + parameter[0] + '_%.2e' % v

    for val in random_seq:
        fname += "_" + str(val)

    fullname = os.getcwd()+"/"+ARGS.folder+"/"+fname+".yaml"
    if os.path.isfile(fullname):
        print("%s already exists" % fullname)
        return

    stream = open(folder+'/'+fname+".yaml", "w")
    yaml.dump(cfg, stream, default_flow_style=True)

if __name__ == "__main__":
    PARSER = argparse.ArgumentParser()
    PARSER.add_argument('-n', '--num', default=1, type=int,
                        help='number of config to generate')
    PARSER.add_argument('-f', '--folder', default='output')
    PARSER.add_argument('-b', '--budget', default=339, type=int,
                        help='BSE nitrogen content in 1e17 kg unit')
    PARSER.add_argument('-d', '--default', action='store_true',
                        help='use default initial nitrogen distribution')
    PARSER.add_argument('-m', '--method', type=str, action='append')
    PARSER.add_argument('-c', '--config', default="config.yaml",
                        help="base config", type=str)

    ARGS = PARSER.parse_args()

    TOTAL_BUDGET = ARGS.budget

    if not os.path.isdir(ARGS.folder):
        os.mkdir(ARGS.folder)

    constraints = {}
    if ARGS.default:
        constraints = DEFAULT_CONSTRAINTS

    if not ARGS.method:
        for i in range(ARGS.num):
            create_config(ARGS.folder, ARGS.config, constraints=constraints)
    else:
        pathname = ARGS.folder + '/' + '-'.join(ARGS.method)
        if not os.path.isdir(pathname):
            os.mkdir(pathname)

        for n in range(ARGS.num):
            if not ARGS.default:
                constraints = {i: j for i, j in enumerate(get_random_sequence())}

            for x in METHODS[ARGS.method[0]]['values']:
                if len(ARGS.method) > 1:
                    for y in METHODS[ARGS.method[1]]['values']:
                        newval = [{METHODS[ARGS.method[0]]['name']: x},
                                  {METHODS[ARGS.method[1]]['name']: y}]
                        create_config(pathname, ARGS.config,
                                      value=newval, constraints=constraints)
                else:
                        newval = [{METHODS[ARGS.method[0]]['name']: x}]
                        create_config(pathname, ARGS.config,
                                      value=newval, constraints=constraints)
