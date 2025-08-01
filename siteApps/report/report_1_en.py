# 1_reportlab_example_en.py
from reportlab.lib.pagesizes import letter
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.lib import colors
from reportlab.lib.units import inch

def create_reportlab_pdf_en(filename="report_reportlab_en.pdf"):
    """
    Generates a simple sales report PDF using reportlab.
    """
    doc = SimpleDocTemplate(filename, pagesize=letter)
    styles = getSampleStyleSheet()
    story = []

    # 1. Title
    title_style = styles['h1']
    title_style.alignment = 1 # Center align
    story.append(Paragraph("Monthly Sales Report", title_style))
    story.append(Spacer(1, 0.2*inch))

    # 2. Report Summary
    story.append(Paragraph("This is a summary of sales performance for June 2025.", styles['BodyText']))
    story.append(Spacer(1, 0.2*inch))

    # 3. Sales Data Table
    data = [
        ['Product Name', 'Quantity Sold', 'Unit Price', 'Revenue'],
        ['Laptop', '30', '$1,200.00', '$36,000.00'],
        ['Monitor', '50', '$350.00', '$17,500.00'],
        ['Keyboard', '120', '$80.00', '$9,600.00'],
        ['Mouse', '150', '$30.00', '$4,500.00'],
        ['Total', '', '', '$67,600.00']
    ]

    # Define table style
    style = TableStyle([
        ('BACKGROUND', (0,0), (-1,0), colors.HexColor('#4c5f7a')),
        ('TEXTCOLOR', (0,0), (-1,0), colors.whitesmoke),
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('FONTNAME', (0,0), (-1,0), 'Helvetica-Bold'),
        ('BOTTOMPADDING', (0,0), (-1,0), 12),
        ('BACKGROUND', (0,1), (-1,-1), colors.beige),
        ('GRID', (0,0), (-1,-1), 1, colors.black)
    ])

    # Create table and apply style
    tbl = Table(data, colWidths=[2*inch, 1.5*inch, 1.5*inch, 1.5*inch])
    tbl.setStyle(style)
    story.append(tbl)
    
    # 4. Closing text
    story.append(Spacer(1, 0.4*inch))
    story.append(Paragraph("Report generated by: John Doe", styles['Normal']))

    # Build the PDF
    doc.build(story)
    print(f"Successfully created '{filename}'.")

if __name__ == '__main__':
    create_reportlab_pdf_en()
