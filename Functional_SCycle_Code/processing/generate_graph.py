import argparse
import glob

from graphviz import render

PARSER = argparse.ArgumentParser()
PARSER.add_argument(dest='fnames', nargs='*', help='file to use for plotting')

ARGS = PARSER.parse_args()

def plot_graph(ARGS):
    for dotfile in ARGS.fnames:
        if not dotfile.endswith('dot'):
            print("Likely not a graphviz file, skipping", dotfile)
            continue
            
        print("Generating png file for", dotfile)
        render('dot', 'png', dotfile)

if __name__ == "__main__":
    plot_graph(ARGS)
