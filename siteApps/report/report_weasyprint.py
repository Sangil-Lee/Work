# 3_weasyprint_example_en.py
import weasyprint
import datetime

def create_weasyprint_pdf_en(filename="report_weasyprint_en.pdf"):
    """
    Generates an invoice PDF from HTML and CSS using WeasyPrint.
    """
    # 1. Create HTML content
    issue_date = datetime.date.today()
    due_date = issue_date + datetime.timedelta(days=30)
    
    html_content = f"""
    <html>
    <head>
        <meta charset="UTF-8">
        <style>
            @page {{ 
                size: A4; 
                margin: 1cm;
                @bottom-center {{
                    content: "Page " counter(page) " of " counter(pages);
                    font-size: 10px;
                }}
            }}
            body {{ font-family: sans-serif; color: #333; }}
            h1 {{ text-align: center; color: #4A90E2; }}
            .invoice-box {{ max-width: 800px; margin: auto; padding: 30px; border: 1px solid #eee; box-shadow: 0 0 10px rgba(0, 0, 0, 0.15); }}
            .header {{ display: flex; justify-content: space-between; margin-bottom: 20px; }}
            .header div {{ text-align: right; }}
            .info table {{ width: 100%; line-height: inherit; text-align: left; }}
            .info table td {{ padding: 5px; vertical-align: top; }}
            .item-table {{ width: 100%; border-collapse: collapse; text-align: left; margin-top: 20px; }}
            .item-table th, .item-table td {{ border: 1px solid #ddd; padding: 8px; }}
            .item-table th {{ background-color: #4A90E2; color: white; }}
            .total {{ text-align: right; margin-top: 20px; font-size: 1.2em; font-weight: bold; }}
        </style>
    </head>
    <body>
        <div class="invoice-box">
            <h1>Invoice</h1>
            <div class="header">
                <div>
                    <strong>Invoice #:</strong> INV-123<br>
                    <strong>Issued:</strong> {issue_date.strftime('%Y-%m-%d')}<br>
                    <strong>Due:</strong> {due_date.strftime('%Y-%m-%d')}
                </div>
            </div>
            <div class="info">
                <table>
                    <tr>
                        <td>
                            <strong>Bill To:</strong><br>
                            ABC Company<br>
                            123 Main Street, New York, NY 10001
                        </td>
                        <td>
                            <strong>From:</strong><br>
                            Python Solutions Inc.<br>
                            456 Tech Avenue, Silicon Valley, CA 94000
                        </td>
                    </tr>
                </table>
            </div>

            <table class="item-table">
                <thead>
                    <tr>
                        <th>Item</th>
                        <th>Quantity</th>
                        <th>Unit Price</th>
                        <th>Amount</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>Web Consulting</td>
                        <td>1</td>
                        <td>$1,500.00</td>
                        <td>$1,500.00</td>
                    </tr>
                    <tr>
                        <td>Server Maintenance (Monthly)</td>
                        <td>1</td>
                        <td>$500.00</td>
                        <td>$500.00</td>
                    </tr>
                </tbody>
            </table>
            <div class="total">
                <strong>Total Amount: $2,000.00</strong>
            </div>
        </div>
    </body>
    </html>
    """

    # 2. Convert HTML to PDF
    weasyprint.HTML(string=html_content).write_pdf(filename)
    print(f"Successfully created '{filename}'.")

if __name__ == '__main__':
    create_weasyprint_pdf_en()
