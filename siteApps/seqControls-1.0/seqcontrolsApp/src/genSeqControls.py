import argparse
import re
import os
import sys
from jinja2 import Environment, FileSystemLoader

def get_file_list(directory_path):
    file_list = []
    try:
        if os.path.isdir(directory_path):
            for filename in os.listdir(directory_path):
                file_path = os.path.join(directory_path, filename)
                if os.path.isfile(file_path):
                    file_list.append(filename)
        else:
            print(f"Error: '{directory_path}' invalid path")
    except FileNotFoundError:
        print(f"Error: Can't find {directory_path}")
    except Exception as e:
        print(f"Error {e}")
    return file_list

def generate_snl2(input_file, template_file, seq_file, dbd_file):
    env = Environment(loader=FileSystemLoader('./'))
    template = env.get_template(template_file)

    print(input_file)

    seqprog = ""
    pv_dict = {}
    group_name = None
    with open(input_file, 'r') as f:
        for line in f:
            if not line or line[0]== '#' :
                continue
            line = re.split(r'[\s=]+', line)
            if line[0].lower() == "seqprog":
                seqprog=line[1]
                continue
            elif line[0].lower() == "groupname" :
                group_name = line[1]
                pv_dict[group_name] = []
                continue
            if len(line[0]) != 0:
                pv_name = f"\"{line[0]}\""
                #print(pv_name)
                if pv_name:
                    pv_dict[group_name].append(pv_name)

    for groupname, pv_names in pv_dict.items():
        pv_list_formatted = ""
        pv_count = 0
        for i, pv_name in enumerate(pv_names):
            pv_list_formatted += f"\"{pv_name}\""
            if (i + 1) % 3 == 0 and (i + 1) < len(pv_names):
                pv_list_formatted += ",\n"
            elif (i + 1) < len(pv_names):
                pv_list_formatted += ", "

        pv_count = i
        #print(groupname, "==>>")
        #print(pv_list_formatted)


    print(pv_dict[next(iter(pv_dict))])
    rendered_template = template.render(groupPVs=pv_dict, j_file_name=seqprog)
    try:
        with open(seq_file, 'w') as outfile:
            outfile.write(rendered_template)
    except FileNotFoundError:
        print(f"Error: Makefile '{seq_file}' Not found.")
    except Exception as e:
        print(f"Error: {e}")

    generate_dbd(dbd_file, seqprog)
        #print(f"Generated: {seq_file} with quoted PV list: {pv_list_formatted}")

def generate_snl(input_file, template_file, seq_file, dbd_file):
    env = Environment(loader=FileSystemLoader('./'))
    template = env.get_template(template_file)
    pv_names = []

    print(input_file)

    seqprog = ""
    with open(input_file, 'r') as f:
        for line in f:
            if len(line.strip()) == 0 or line[0]== '#' :
                continue
            line = re.split(r'[\s=]+', line)
            if line[0].lower() == "seqprog":
                seqprog=line[1]
                continue
            pv_name = line[0]
            #print(pv_name)
            if pv_name:
                pv_names.append(pv_name)

    pv_list_formatted = ""
    pv_count = 0
    for i, pv_name in enumerate(pv_names):
        pv_list_formatted += f"\"{pv_name}\""
        if (i + 1) % 3 == 0 and (i + 1) < len(pv_names):
            pv_list_formatted += ",\n"
        elif (i + 1) < len(pv_names):
            pv_list_formatted += ", "
    pv_count = i

    #rendered_template = template.render(j_pv_list=pv_list_formatted, j_pv_count=pv_count+1, j_file_name=program)
    rendered_template = template.render(j_pv_list=pv_list_formatted, j_pv_count=pv_count+1, j_file_name=seqprog)

    try:
        with open(seq_file, 'w') as outfile:
            outfile.write(rendered_template)
    except FileNotFoundError:
        print(f"Error: Makefile '{seq_file}' Not found.")
    except Exception as e:
        print(f"Error: {e}")

    generate_dbd(dbd_file, seqprog)
    #print(f"Generated: {seq_file} with quoted PV list: {pv_list_formatted}")

def generate_dbd(dbd_file, program):
    with open(dbd_file, 'w') as outfile:
        registrar = f"registrar({program}Registrar)"
        outfile.write(registrar)

def append_to_makefile(makefile_path, target_param, value_to_add):
    try:
        lines = []
        found = False
        bStart = False
        with open(makefile_path, 'r') as f:
            for line in f:
                line = re.sub(r'^\s+', '', line)
                #print(line)
                if "SNCSEQ" in line:
                    #print("Found SNCSEQ====>")
                    bStart = True

                #match = re.match(rf"({re.escape(target_param)}\s*\+=\s*)(.*)", line)
                match = re.match(rf"({re.escape(target_param)}\s*\+=*)(.*)", line)
                #print(f"bStart({bStart}): {match}")

                if value_to_add in line:
                    lines.append(line)
                    found = True
                    continue
                elif match and bStart == True:
                    lines.append(f"{match.group(1)}{match.group(2)} {value_to_add}\n")
                    found = True
                else:
                    lines.append(line)

        if found:
            with open(makefile_path, 'w') as f:
                f.writelines(lines)
            print(f"'{target_param}' '{value_to_add}' Complete Adding)")
        else:
            print(f"Warning!: Makefile '{target_param} += ...' Can't find pattern.")

    except FileNotFoundError:
        print(f"Error: Makefile '{makefile_path}' Not found.")
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="EPICS Sequence Generation with jinja2 Template")
    parser.add_argument('-i',  help='Input PV list file')
    parser.add_argument('-t',  help='Sequence Jinja Template File')
    #parser.add_argument('-p',  help='Sequence Program Name')
    parser.add_argument('-a', type=bool, help='All Sequence Files Generation ')

    args = parser.parse_args()

    if args.a is not None and args.t is not None:
        pvfile_path = "./pv.list/"
        pvfiles = get_file_list(pvfile_path)
        makefile = "Makefile"
        if pvfiles:
            for file in pvfiles:
                pvlist_file=pvfile_path+file
                file_name, ext = os.path.splitext(file)
                filename = "gen_"+file_name
                seq_file = filename+".stt"
                dbd_file = filename+".dbd"
                generate_snl2(pvlist_file, args.t, seq_file, dbd_file)
                #Insert sequence files in Makefile
                target   = "seqcontrols_DBD"
                append_to_makefile(makefile, target, dbd_file)

                target  = "seqcontrols_SRCS"
                append_to_makefile(makefile, target, seq_file)
        sys.exit(1)
    #elif args.i is None or args.t is None or args.p is None:
    elif args.i is None or args.t is None :
        print(f"Usage:./getSeqControls.py -i input_pvlist.txt -t jinja.snl.j2")
        sys.exit(1)

    #input_file = "SeqControls.txt"  # PV 이름이 한 줄에 하나씩 있는 파일
    #template_file = "seqControls.snl.j2"
    input_file = args.i
    template_file =args.t

    #template_file = os.path.basename(template_file)
    base_file = os.path.basename(input_file)
    file_name, ext = os.path.splitext(base_file)

    filename = "gen_"+file_name
    seq_file = filename+".stt"
    dbd_file = filename+".dbd"

    generate_snl2(input_file, template_file, seq_file, dbd_file)

    #Insert sequence files in Makefile
    makefile = "Makefile"
    target   = "seqcontrols_DBD"
    add_dbd  = dbd_file 
    append_to_makefile(makefile, target, add_dbd)

    target  = "seqcontrols_SRCS"
    add_seq = seq_file
    append_to_makefile(makefile, target, add_seq)

