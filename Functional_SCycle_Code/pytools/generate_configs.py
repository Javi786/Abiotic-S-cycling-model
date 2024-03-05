'''
This script should implement
- generate_reservoirs
- generate_parameters

E.g.,

from generate_configs import generate_reservoirs, generate_parameters

> r1 = {
>   'name': 'mantle',
>   'element': 'p',
>   'log': true,
>   'vmin': 1e17,
>   'vmax': 1e20
> }
>
> generate_reservoirs(r1, m=1e20)
Specify at least 2 reservoirs for random initial conditions.
>
> r2 = {
>   'name': 'Atmosphere',
>   'element': 'p',
>   'log': true,
>   'vmin': 1e17,
>   'vmax': 1e20
> }
> generate_reservoirs([r1, r2], m=1e20, n=20)
Generated 20 config files with 1e20 kg distributed between the Atmosphere (n2)
and the mantle (p).
>
> p1 = {
>   'process': 'AbioticFixation',
>   'parameter': 'F0',
>   'log': true,
>   'vmin': 1e5,
>   'vmax': 1e10
> }
> generate_parameters(p1, n=20)
Generated 20 config files on a grid for AbioticFixation (F0).
>
> p2 = {
>   'process': 'Volcanism',
>   'parameter': 'F0',
>   'log': true,
>   'vmin': 1e5,
>   'vmax': 1e10
> }
> generate_parameters([p1, p2], n=[10,20])
Generated 200 config files on a grid for AbioticFixation (F0, 10) and Volcanism
(F0, 20).
'''
import random
import secrets
import os

import numpy as np
import yaml

def generate_reservoirs(var, m=1e20, n=10, default='../config_default.yaml'):
    if type(var) != list or len(var) != 2:
        print('Specify at least two reservoirs for random initial conditions.')
        return

    for i in range(n):
        cfg = get_default_config(default)
        masstot = 0

        for v in var[:-1]:
            if type(v) != dict:
                print('Wrong format to specify parameters, please check doc.')
                return

            name, element, islog, vmin, vmax = get_values(v)
            if not (name and element and islog and vmin and vmax):
                print("Problem in the reservoir definition:")
                print(v)
                return

            if name not in cfg['Reservoirs'].keys():
                print(name + " not in possible reservoirs, please check spelling.")
                return
            if element not in cfg['Reservoirs'][name]['InitMasses'].keys():
                print(element + " not in possible elements, please check spelling.")
                print("Just add a dummy value in config file is element name is correct.")
                return

            if islog: # logU(a,b) ~ exp(U(log(a), log(b))
                r = np.exp(np.random.uniform(np.log(vmin), np.log(vmax)))
            else:
                r = np.random.uniform(vmin, vmax)

            cfg["Reservoirs"][name]['InitMasses'][element] = '%.2e' % r
            masstot += r

        if masstot > m:
            print("Error during initialization, sum(m)>m_budget")
            return

        name, element, islog, vmin, vmax = get_values(var[-1])
        cfg["Reservoirs"][name]['InitMasses'][element] = '%.2e' % (m-masstot)

        fname = os.path.dirname(default) + '/config_%s.yaml' % secrets.token_hex(5)
        print(fname)
        stream = open(fname, "w")
        yaml.dump(cfg, stream, default_flow_style=True)

    print("Be careful that the last reservoir is initialized with what remains from the budget")

def generate_parameters(var, n=10, default='../config_default.yaml'):
    if type(var) != list or len(var) != 2:
        print('Specify at least two reservoirs for random initial conditions.')
        return

    for i in range(n):
        cfg = get_default_config(default)

        for v in var:
            if type(v) != dict:
                print('Wrong format to specify parameters, please check doc.')
                return

            process, parameter, islog, vmin, vmax = get_values(v)
            if not (process and parameter and islog and vmin and vmax):
                print("Problem in the reservoir definition:")
                print(v)
                return

            if process not in cfg['Parameters'].keys():
                print(process + " not in possible parameters, please check spelling.")
                return
            if parameter not in cfg['Parameters'][process].keys():
                print(parameter + " not in possible elements, please check spelling.")
                print("Just add a dummy value in config file is element name is correct.")
                return

            if islog: # logU(a,b) ~ exp(U(log(a), log(b))
                r = np.exp(np.random.uniform(np.log(vmin), np.log(vmax)))
            else:
                r = np.random.uniform(vmin, vmax)

            cfg["Parameters"][process][parameter] = '%.2e' % r

        fname = os.path.dirname(default) + '/config_%s.yaml' % secrets.token_hex(5)
        print(fname)
        stream = open(fname, "w")
        yaml.dump(cfg, stream, default_flow_style=True)

    print("Be careful that the last reservoir is initialized with what remains from the budget")

    #pass

def get_values(v):
    name = v.get('name')
    element = v.get('element')
    islog = v.get('log')
    vmin = v.get('vmin')
    vmax = v.get('vmax')
    return name, element, islog, vmin, vmax

def get_default_config(fname):
    '''Read config.yaml from main directory to be used as default.'''
    stream = open(fname, "r")
    config = yaml.load(stream, Loader=yaml.SafeLoader)
    stream.close()
    return config
