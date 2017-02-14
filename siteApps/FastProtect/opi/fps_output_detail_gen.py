#!/usr/bin/env python

# Date: 2016-12-12
# Author: Wayne Lewis
# Description:
# This file generates the RAON FPS output detail display from a set of input
# files. No arguments are required.

import sys

# System definition constants
num_columns = 8
num_links = 8
nodes_per_link = 5
inputs_per_node = 64
pvs_per_node = 2

# Geometry constants
x_start = 18
x_row_label_offset = -18
x_spacing = 133
x_inp_offset = 2
y_start = 96
y_title_offset = -11
y_row_label_offset = 4
y_spacing = 22
y_inp_offset = 3

# Geometry calculations
x_start_row_label = x_start + x_row_label_offset
y_start_title = y_start + y_title_offset
y_start_row_label = y_start + y_row_label_offset

def main():
    num_nodes = num_links * nodes_per_link

    input_files = {
            "head":"_fps_output_detail_start.xml", 
            "end":"_fps_output_detail_end.xml", 
            "input":"_fps_output_detail_input_led.xml", 
            "link_title":"_fps_output_detail_link_title.xml", 
            "row_label":"_fps_output_detail_row_label.xml", 
            "subarray":"_fps_output_detail_subarray.xml"}

    output_file = "fps_output_detail.opi"

    of = open(output_file, 'w')

    # Create the start of the OPI file
    f = open(input_files["head"], 'r')

    for line in f:
        of.write(line)

    f.close()

    # Create link titles
    f = open(input_files["link_title"], 'r')

    for link in range(0, num_links):
        f.seek(0)
        for line in f:
            if (not has_macro(line)):
                of.write(line)
            else:
                of.write(link_title_subst(line, link))

    f.close()

    # Create the labels
    f = open(input_files["row_label"], 'r')

    for row in range(0, num_nodes * pvs_per_node / num_columns):
        f.seek(0)
        for line in f:
            if (not has_macro(line)):
                of.write(line)
            else:
                of.write(row_label_subst(line, row))

    f.close()

    # Create the 'Important' and 'GoodState' subarray displays
    f = open(input_files["subarray"], 'r')

    for row in range(0, num_nodes * pvs_per_node / num_columns):
        for col in range(0, num_columns):
            f.seek(0)
            for line in f:
                if (not has_macro(line)):
                    of.write(line)
                else:
                    text = subst(
                            line, 
                            row,
                            col)
                    of.write(text)

    f.close()

    # Create each input widget
    f = open(input_files["input"], 'r')

    for link in range(0, num_links):
        for node in range(0, nodes_per_link):
            for input in range(0, inputs_per_node):
                f.seek(0)
                for line in f:
                    if (not has_macro(line)):
                        of.write(line)
                    else:
                        of.write(pv_subst(line, link, node, input))

    f.close()

    # Create the end of the OPI file
    f = open(input_files["end"], 'r')

    for line in f:
        of.write(line)

    f.close()
    of.close()

# Simple test for presence of macro in line of text
# Macro is defined as $macro_name
def has_macro(line):
    return '$' in line

# Do input PV substitution
def pv_subst(line, link, node, id):
    # Calculate the position for the widget
    x_start_inp = x_start + x_inp_offset
    y_start_inp = y_start + y_inp_offset

    # Define space between each input
    inp_x_spacing = 4

    # Calculate the position for the widget
    x = x_start_inp + link * x_spacing + (id % 32) * inp_x_spacing
    y = y_start_inp + (2 * node + (id / 32)) * y_spacing
    
    pv = "$(P)$(R)L%01dN%01dI%02d" % (link, node, id)

    vals = {"$pv": pv, "$x": str(x), "$y": str(y)}

    for macro in vals.keys():
        line = line.replace(str(macro), str(vals[macro]))

    return line

# Do link title macro substitution
def link_title_subst(line, link):
    # Calculate the position for the widget
    x = x_start + link * x_spacing 
    y = y_start_title
    
    L = link

    vals = {"$L": L, "$x": str(x), "$y": str(y)}

    for macro in vals.keys():
        line = line.replace(str(macro), str(vals[macro]))

    return line

# Do row label macro substitution
def row_label_subst(line, row_idx):
    # Calculate the position for the widget
    x = x_start_row_label 
    y = y_start_row_label + row_idx * y_spacing

    vals = {"$x": x, "$y": y }
    
    for macro in vals.keys():
        line = line.replace(str(macro), str(vals[macro]))

    return line

# Do main widget macro substitutionss
def subst(line, row_idx, col_idx):
    # Calculate the position for the widget
    x = x_start + col_idx * x_spacing
    y = y_start + row_idx * y_spacing

    # NOTE: integer division to work out element number
    el = (row_idx/pvs_per_node) * pvs_per_node * num_columns +  \
            pvs_per_node * col_idx + \
            row_idx % pvs_per_node
    
    vals = {"$El": str(el).zfill(2), "$x": x, "$y": y }
    
    for macro in vals.keys():
        line = line.replace(str(macro), str(vals[macro]))

    return line


if __name__ == '__main__':
    main()
