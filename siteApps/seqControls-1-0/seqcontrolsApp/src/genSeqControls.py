from jinja2 import Environment, FileSystemLoader


def generate_snl(input_file, template_file, seq_file, program):
    """입력 파일에서 PV 이름을 읽어 quotation 문자로 구분하여 SNL 파일을 생성합니다."""
    env = Environment(loader=FileSystemLoader('.'))
    template = env.get_template(template_file)
    pv_names = []

    with open(input_file, 'r') as f:
        for line in f:
            pv_name = line.strip()
            if pv_name:
                pv_names.append(pv_name)

    #pv_list_formatted = ', '.join(f'"{pv}"' for pv in pv_names)
    #rendered_template = template.render(j_pv_list=quoted_pv_list_formatted, j_pv_count=10, j_file_name="pv_list" )

    pv_list_formatted = ""
    pv_count = 0
    for i, pv_name in enumerate(pv_names):
        pv_list_formatted += f"\"{pv_name}\""
        if (i + 1) % 3 == 0 and (i + 1) < len(pv_names):
            pv_list_formatted += ",\n"
        elif (i + 1) < len(pv_names):
            pv_list_formatted += ", "
    pv_count = i

    rendered_template = template.render(j_pv_list=pv_list_formatted, j_pv_count=pv_count+1, j_file_name=program)

    with open(seq_file, 'w') as outfile:
        outfile.write(rendered_template)
    print(f"Generated: {seq_file} with quoted PV list: {pv_list_formatted}")



def generate_dbd(dbd_file, program):
    with open(dbd_file, 'w') as outfile:
        registrar = f"registrar({program}Registrar)"
        outfile.write(registrar)


if __name__ == "__main__":
    input_file = "SeqControls.txt"  # PV 이름이 한 줄에 하나씩 있는 파일
    template_file = "seqControls.snl.j2"

    filename = "generated_SeqControls"
    seq_file = filename+".stt"
    dbd_file = filename+".dbd"
    program_name = "sncSeqControls"

    generate_snl(input_file, template_file, seq_file, program_name)
    generate_dbd(dbd_file, program_name)

