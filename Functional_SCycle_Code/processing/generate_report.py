'''
Generate fluxes and evolution plots for a given run. If with_graph is set, will
also include the network graph in the output pdf.
'''

from collections import OrderedDict
from glob import glob
import argparse
from Report import Report

PARSER = argparse.ArgumentParser()
PARSER.add_argument('-i', '--id', default=False, type=str,
                    help='Case ID, useful if there is more than one in folder')
PARSER.add_argument('-g', '--graph', default=False, action='store_true',
                    help='Add graph representation in the report')
PARSER.add_argument('-f', '--folder', default='output/', type=str,
                    help='folder where out_* are stored')

ARGS = PARSER.parse_args()

if ARGS.id:
    r = Report(ARGS.id, sim_dir=ARGS.folder)
    r.process()
    r.to_file(with_graph=ARGS.graph)

else:
    OUTPUT = glob(ARGS.folder+"/*.txt")
    for out in OUTPUT:
        file_id = out.split("out_")[-1]
        file_id = file_id.split(".txt")[0]

        r = Report(file_id, sim_dir=ARGS.folder)
        r.process()
        r.to_file(with_graph=ARGS.graph)
