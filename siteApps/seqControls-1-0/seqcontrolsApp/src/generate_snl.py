from jinja2 import Environment, FileSystemLoader

def generate_single_snl_with_line_breaks(input_file, template_file, output_file):
    """입력 파일에서 PV 이름을 읽어 3개 단위로 끊어서 여러 줄의 문자열로 만들어 SNL 파일을 생성합니다."""
    env = Environment(loader=FileSystemLoader('.'))
    template = env.get_template(template_file)
    pv_names = []

    with open(input_file, 'r') as f:
        for line in f:
            pv_name = line.strip()
            if pv_name:
                pv_names.append(pv_name)

    pv_list_formatted = ""
    for i, pv_name in enumerate(pv_names):
        pv_list_formatted += pv_name
        if (i + 1) % 3 == 0 and (i + 1) < len(pv_names):
            pv_list_formatted += ",\n    "
        elif (i + 1) < len(pv_names):
            pv_list_formatted += ", "

    rendered_template = template.render(pv_names_formatted=pv_list_formatted)

    with open(output_file, 'w') as outfile:
        outfile.write(rendered_template)
    print(f"Generated: {output_file} with formatted PV list:\n{pv_list_formatted}")

def generate_single_snl_with_quoted_pv(input_file, template_file, output_file):
    """입력 파일에서 PV 이름을 읽어 quotation 문자로 구분하여 SNL 파일을 생성합니다."""
    env = Environment(loader=FileSystemLoader('.'))
    template = env.get_template(template_file)
    pv_names = []

    with open(input_file, 'r') as f:
        for line in f:
            pv_name = line.strip()
            if pv_name:
                pv_names.append(pv_name)

    pv_list_formatted = ', '.join(f'"{pv}"' for pv in pv_names)

    rendered_template = template.render(pv_names_formatted=pv_list_formatted)

    with open(output_file, 'w') as outfile:
        outfile.write(rendered_template)
    print(f"Generated: {output_file} with quoted PV list: {pv_list_formatted}")

if __name__ == "__main__":
    input_file = "pv_list.txt"  # PV 이름이 한 줄에 하나씩 있는 파일
    template_file = "template.snl.j2"
    output_file = "generated_pv_list.st"
    #generate_single_snl_with_line_breaks(input_file, template_file, output_file)
    generate_single_snl_with_quoted_pv(input_file, template_file, output_file)
