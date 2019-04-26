#!/usr/bin/env python

# Date: 2016-12-12
# Author: Wayne Lewis
# Description:
# This file generates the RAON FPS overview display from a set of input files.
# No arguments are required.

import sys

# System definition constants
links = 8
outputs = 16
nodes_per_link = 5

# Geometry constants
x_start = 18
x_spacing_out = 48
x_spacing_link = 96
y_start_out = 54
y_out_size = 70
y_mitigation_size = 30
y_gap = 10
y_node_size = 32
y_link_size = 32

# Geometry calculation
y_start_link = y_start_out + y_out_size + y_mitigation_size + 2*y_gap
y_nodes = nodes_per_link * y_node_size + (nodes_per_link + 1) * y_gap
y_start_engineering = y_start_link + y_link_size + y_nodes
x_start_engineering = x_start + 7 * x_spacing_link

def main():
    input_files = {
            "head":"_fps_overview_start.xml", 
            "end":"_fps_overview_end.xml", 
            "output":"_fps_overview_output.xml", 
            "engineering":"_fps_engineering.xml", 
            "node":"_fps_overview_node.xml", 
            "link":"_fps_overview_link.xml"}

    output_file = "fps_overview.opi"

    of = open(output_file, 'w')

# Create the output file header
    f = open(input_files["head"], 'r')

    for line in f:
        of.write(line)

    f.close()

# Create the links
    f = open(input_files["link"], 'r')

    for link in range(0, links):
        f.seek(0)
        for line in f:
            if (not has_macro(line)):
                of.write(line)
            else:
                of.write(link_subst(line, link))
    f.close()

# Create the nodes
    f = open(input_files["node"], 'r')

    for link in range(0, links):
        for node in range(0, nodes_per_link):
            f.seek(0)
            for line in f:
                if (not has_macro(line)):
                    of.write(line)
                else:
                    of.write(node_subst(line, link, node))
    f.close()

# Create the outputs
    f = open(input_files["output"], 'r')

    for output in range(0, outputs):
        f.seek(0)
        for line in f:
            if (not has_macro(line)):
                of.write(line)
            else:
                of.write(output_subst(line, output))

    f.close()

# Create the engineering link
    f = open(input_files["engineering"], 'r')

    for line in f:
        if (not has_macro(line)):
            of.write(line)
        else:
            of.write(engineering_subst(line, output))

    f.close()

# Create the end of the file
    f = open(input_files["end"], 'r')

    for line in f:
        of.write(line)

    f.close()
    of.close()

# Basic search for presence of macros
def has_macro(line):
    return '$' in line

# Do the substitutions for the links 
def link_subst(line, link):
    # Calculate the position for the widget
    x = x_start + link * x_spacing_link
    y = y_start_link
    
    L = "%01d" % (link)

    vals = {"$L": L, "$x": str(x), "$y": str(y)}

    for macro in vals.keys():
        line = line.replace(str(macro), str(vals[macro]))

    return line

# Do the substitutions for the nodes
def node_subst(line, link, node):
    stale_nodes = {0:"15_00", 1:"31_16", 2:"39_32"}

    # Calculate the position for the widget
    x = x_start + link * x_spacing_link
    y = y_start_link + y_link_size + node * y_node_size + (node + 1) * y_gap
    
    L = "%01d" % (link)
    NN = "%01d" % (node)
    # Define the record that has the stale data
    NR = stale_nodes[(node * links + link) / 16]
    # Define the bit in the record that has stale data for this node
    NB = (node * links + link) % 16

    vals = {"$L": L, 
            "$NN": NN, 
            "$NR": NR, 
            "$NB": NB, 
            "$x": str(x), 
            "$y": str(y)}

    for macro in vals.keys():
        line = line.replace(str(macro), str(vals[macro]))

    return line

# Do the substitutions for the outputs
def output_subst(line, output):
    # Calculate the position for the widget
    x = x_start + output * x_spacing_out
    y = y_start_out

    Od = "%02d" % (output)
    val = 1<<output

    vals = {"$Od": Od, "$Val": val, "$x": str(x), "$y": str(y)}
    
    for macro in vals.keys():
        line = line.replace(str(macro), str(vals[macro]))

    return line

# Do the substitutions for the engineering link button
def engineering_subst(line, output):
    x = x_start_engineering
    y = y_start_engineering

    vals = {"$x": str(x), "$y": str(y)}
    
    for macro in vals.keys():
        line = line.replace(str(macro), str(vals[macro]))

    return line


if __name__ == '__main__':
    main()
