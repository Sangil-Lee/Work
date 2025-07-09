# 2_fpdf2_example_en.py
from fpdf import FPDF
import datetime

class MyPDF(FPDF):
    def header(self):
        # Logo (replace with your actual image file)
        # self.image('logo.png', 10, 8, 33) 
        self.set_font('helvetica', 'B', 15)
        self.cell(0, 10, 'Project Status Report', border=0, ln=1, align='C')
        self.ln(10)

    def footer(self):
        self.set_y(-15)
        self.set_font('helvetica', 'I', 8)
        # Page number
        self.cell(0, 10, f'Page {self.page_no()}', border=0, align='C')

def create_fpdf_pdf_en(filename="report_fpdf_en.pdf"):
    """
    Generates a project status report PDF using fpdf2.
    """
    pdf = MyPDF()
    pdf.add_page()
    pdf.set_font('helvetica', '', 12)
    
    # 1. Report Info
    today = datetime.date.today().strftime("%Y-%m-%d")
    pdf.cell(0, 10, f'Report Date: {today}', ln=1)
    pdf.cell(0, 10, 'Project Name: New System Development', ln=1)
    pdf.ln(10)

    # 2. Progress Section
    pdf.set_font('helvetica', 'B', 14)
    pdf.cell(0, 10, '1. Key Progress', ln=1, border='B')
    pdf.ln(5)
    
    pdf.set_font('helvetica', '', 12)
    progress_text = (
        "- User login and registration feature development complete (95%)\n"
        "- Main dashboard UI prototype finished (80%)\n"
        "- Database schema design and implementation complete (100%)"
    )
    pdf.multi_cell(0, 8, progress_text)
    pdf.ln(10)

    # 3. Issues and Action Items Section
    pdf.set_font('helvetica', 'B', 14)
    pdf.cell(0, 10, '2. Issues and Action Items', ln=1, border='B')
    pdf.ln(5)

    pdf.set_font('helvetica', '', 12)
    issue_text = (
        "1. Issue: Intermittent timeouts when integrating with a third-party API.\n"
        "   - Action Item: Add retry logic and logging to the API request function.\n\n"
        "2. Issue: Minor CSS breaking points in mobile responsive view.\n"
        "   - Action Item: Collaborate with QA team to resolve by the end of this week."
    )
    pdf.multi_cell(0, 8, issue_text)

    # Save the PDF
    pdf.output(filename)
    print(f"Successfully created '{filename}'.")

if __name__ == '__main__':
    create_fpdf_pdf_en()
