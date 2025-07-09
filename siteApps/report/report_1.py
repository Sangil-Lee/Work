# 1_reportlab_example.py
from reportlab.lib.pagesizes import letter
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.lib import colors
from reportlab.lib.units import inch

def create_reportlab_pdf(filename="report_reportlab.pdf"):
    """
    reportlab을 사용하여 간단한 판매 보고서 PDF를 생성합니다.
    """
    doc = SimpleDocTemplate(filename, pagesize=letter)
    styles = getSampleStyleSheet()
    story = []

    # 1. 제목
    title_style = styles['h1']
    title_style.alignment = 1 # 중앙 정렬
    story.append(Paragraph("월간 판매 보고서", title_style))
    story.append(Spacer(1, 0.2*inch))

    # 2. 보고서 요약
    story.append(Paragraph("2025년 6월 판매 실적 요약입니다.", styles['BodyText']))
    story.append(Spacer(1, 0.2*inch))

    # 3. 판매 데이터 테이블
    data = [
        ['제품명', '판매 수량', '단가', '매출'],
        ['노트북', '30', '1,200,000원', '36,000,000원'],
        ['모니터', '50', '350,000원', '17,500,000원'],
        ['키보드', '120', '80,000원', '9,600,000원'],
        ['마우스', '150', '30,000원', '4,500,000원'],
        ['총계', '', '', '67,600,000원']
    ]

    # 테이블 스타일 정의
    style = TableStyle([
        ('BACKGROUND', (0,0), (-1,0), colors.HexColor('#4c5f7a')), # 헤더 배경색
        ('TEXTCOLOR', (0,0), (-1,0), colors.whitesmoke),          # 헤더 텍스트 색상
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),                      # 모든 셀 중앙 정렬
        ('FONTNAME', (0,0), (-1,0), 'Helvetica-Bold'),             # 헤더 폰트 굵게
        ('BOTTOMPADDING', (0,0), (-1,0), 12),                     # 헤더 하단 패딩
        ('BACKGROUND', (0,1), (-1,-1), colors.beige),            # 데이터 영역 배경색
        ('GRID', (0,0), (-1,-1), 1, colors.black)                 # 전체 표에 검은색 격자
    ])

    # 테이블 생성 및 스타일 적용
    tbl = Table(data, colWidths=[2*inch, 1*inch, 1.5*inch, 1.5*inch])
    tbl.setStyle(style)
    story.append(tbl)
    
    # 4. 마무리 문구
    story.append(Spacer(1, 0.4*inch))
    story.append(Paragraph("보고서 작성자: 홍길동", styles['Normal']))

    # PDF 빌드
    doc.build(story)
    print(f"'{filename}' 파일이 성공적으로 생성되었습니다.")

if __name__ == '__main__':
    create_reportlab_pdf()
