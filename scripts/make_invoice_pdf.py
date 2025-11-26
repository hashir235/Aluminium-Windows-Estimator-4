import sys
from pathlib import Path
from reportlab.lib.pagesizes import A4
from reportlab.lib import colors
from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Paragraph, Spacer, Flowable
from reportlab.lib.styles import getSampleStyleSheet
from datetime import datetime
from reportlab.lib.utils import ImageReader
from PIL import Image as PILImage, ImageDraw

# ------------------ FIXED BASE PATH SYSTEM ------------------
BASE_DIR = Path(__file__).resolve().parent.parent          # WE Folder 16/
DATA_DIR = BASE_DIR / "data"                               # WE Folder 16/data/
ASSETS_DIR = BASE_DIR / "assets"                           # WE Folder 16/assets/
OUTPUT_DIR = BASE_DIR / "Downloads" / "Invoice"            # WE Folder 16/Downloads/Invoice/

OUTPUT_DIR.mkdir(parents=True, exist_ok=True)              # Ensure folder exists


# ------------------ WATERMARK FLOWABLE ------------------
class Watermark(Flowable):
    def __init__(self, text, max_width=None, max_height=None):
        super().__init__()
        self.text = text
        self.max_width = max_width
        self.max_height = max_height

    def draw(self):
        self.canv.saveState()
        self.canv.setFont("Helvetica-Bold", 50)
        self.canv.setFillColorRGB(0.6, 0.6, 0.8, alpha=0.25)

        page_width, page_height = A4
        center_x = min(self.max_width or page_width, page_width / 2)
        center_y = min(self.max_height or page_height, page_height / 2)

        self.canv.translate(center_x, center_y)
        self.canv.rotate(30)
        self.canv.drawCentredString(-150, -220, self.text)
        self.canv.restoreState()


# ------------------ MAIN FUNCTION ------------------
def generate_invoice(input_txt, output_pdf):

    # FIXED CORRECT PATHS
    input_path = DATA_DIR / Path(input_txt).name
    output_path = OUTPUT_DIR / Path(output_pdf).name

    # ---------- READ FILE ----------
    with open(input_path, 'r', encoding='utf-8') as f:
        lines = [line.strip() for line in f if line.strip()]

    pdf = SimpleDocTemplate(
        str(output_path), pagesize=A4,
        rightMargin=40, leftMargin=40, topMargin=150, bottomMargin=50
    )
    styles = getSampleStyleSheet()
    elements = []

    # ------------------------------------
    # 🌟 EVERYTHING BELOW IS **UNCHANGED**
    # ------------------------------------

    # ---------- TABLE 1 : CUSTOMER INFO ----------
    info_fields = [
        ("Project", "Customer"),
        ("Location", "Location"),
        ("Customer Ph.no", "Phone"),
        ("Gage Number", "Gage Number"),
        ("Glass Color", "Glass Color"),
        ("Aluminium Color", "Aluminium Color"),
        ("Workshop/Contractor", "Workshop/Contractor"),
        ("Contect no", "Contact No")
    ]

    info_data = []
    info_data.append(["Date", datetime.now().strftime("%d-%b-%Y")])
    
    for key, label in info_fields:
        for line in lines:
            if line.startswith(key):
                parts = line.split(":", 1)
                if len(parts) == 2:
                    info_data.append([label, parts[1].strip()])
                    break

    info_table = Table(info_data, hAlign='LEFT', colWidths=[150, 300])
    table_style = TableStyle([
        ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor("#004080")),
        ('TEXTCOLOR', (0, 0), (-1, 0), colors.white),
        ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
        ('FONTNAME', (0, 0), (-1, -1), 'Helvetica'),
        ('FONTSIZE', (0, 0), (-1, -1), 10),
        ('GRID', (0, 0), (-1, -1), 0.25, colors.gray),
    ])
    for row_index in range(1, len(info_data)):
        bg = colors.whitesmoke if row_index % 2 == 0 else colors.lightblue
        table_style.add('BACKGROUND', (0, row_index), (-1, row_index), bg)

    info_table.setStyle(table_style)
    elements.append(info_table)
    elements.append(Spacer(1, 20))

    # ---------- TABLE 2 : SECTION DETAILS ----------
    section_lines = []
    start_idx = None
    end_idx = None
    for i, line in enumerate(lines):
        if "Win_type" in line:
            start_idx = i
        elif start_idx and line.startswith("Total Windows/Doors"):
            end_idx = i
            section_lines = lines[start_idx:end_idx]
            break

    section_data = []
    for line in section_lines:
        if "|" in line:
            cols = [c.strip() for c in line.split("|")]
            if len(cols) == 3:
                cols[2] = cols[2].replace("sq", "").replace("Rs", "").strip()
            section_data.append(cols)

    if not section_data or "Area" not in section_data[0][-1]:
        section_data.insert(0, ["Win_type", "Qty", "Area (sq.ft)"])
    section_data[0] = ["Win_type", "Qty", "Area (sq.ft)"]

    for i in range(1, len(section_data)):
        bg = colors.whitesmoke if i % 2 == 0 else colors.lightblue
        section_data[i].append(bg)

    section_table = Table(
        [r[:-1] if len(r) == 4 else r for r in section_data],
        hAlign='LEFT', colWidths=[150, 100, 150]
    )
    style = TableStyle([
        ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor("#004080")),
        ('TEXTCOLOR', (0, 0), (-1, 0), colors.white),
        ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
        ('FONTNAME', (0, 0), (-1, -1), 'Helvetica'),
        ('FONTSIZE', (0, 0), (-1, -1), 10),
        ('GRID', (0, 0), (-1, -1), 0.25, colors.gray),
    ])
    for i in range(1, len(section_data)):
        bg = section_data[i][-1] if len(section_data[i]) == 4 else colors.white
        style.add('BACKGROUND', (0, i), (-1, i), bg)

    section_table.setStyle(style)
    elements.append(section_table)

    # TOTAL ROW
    total_windows = None
    total_area = None
    for line in lines:
        if "Total Windows/Doors/Arch" in line:
            parts = line.split("=")
            total_windows = parts[1].strip().replace("No:", "").strip()
        elif "Total Area" in line:
            parts = line.split("=")
            total_area = parts[1].strip().replace("sq", "").strip()

    if total_windows and total_area:
        total_row_data = [["Total Windows/Doors/Arch", f"{total_windows}", f"{total_area}"]]
        total_row = Table(total_row_data, hAlign='LEFT', colWidths=[150, 100, 150])
        total_row.setStyle(TableStyle([
            ('BACKGROUND', (0, 0), (-1, -1), colors.white),
            ('FONTNAME', (0, 0), (-1, -1), 'Helvetica-Bold'),
            ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
            ('GRID', (0, 0), (-1, -1), 0.25, colors.gray)
        ]))
        elements.append(total_row)

    elements.append(Spacer(1, 25))

    # ---------- TABLE 3 : COST SUMMARY ----------
    cost_lines = []
    for i, line in enumerate(lines):
        if line.startswith("Net Rate (per sq.ft)"):
            cost_lines = lines[i:]
            break

    cost_data = []
    for line in cost_lines:
        if "=" in line:
            parts = line.split("=")
            cost_data.append([parts[0].strip(), parts[1].strip()])

    if cost_data:
        final_data = [["Description", "Amounts (PKR)"]] + cost_data
        for i in range(1, len(final_data)):
            bg = colors.whitesmoke if i % 2 == 0 else colors.lightblue
            final_data[i].append(bg)

        cost_table = Table(
            [r[:-1] if len(r) == 3 else r for r in final_data],
            hAlign='LEFT', colWidths=[250, 200]
        )
        style = TableStyle([
            ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor("#004080")),
            ('TEXTCOLOR', (0, 0), (-1, 0), colors.white),
            ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
            ('FONTNAME', (0, 0), (-1, -1), 'Helvetica'),
            ('GRID', (0, 0), (-1, -1), 0.25, colors.gray),
        ])
        for i in range(1, len(final_data)):
            bg = final_data[i][-1] if len(final_data[i]) == 3 else colors.white
            style.add('BACKGROUND', (0, i), (-1, i), bg)

        cost_table.setStyle(style)
        elements.append(cost_table)
        elements.append(Spacer(1, 30))

    # ---------- LOGO + TITLE ----------
    def draw_header(canvas_obj, doc):
        page_width, page_height = A4

        logo_path = ASSETS_DIR / "AL Windows.jpg"     # <-- FIXED
        try:
            pil_img = PILImage.open(logo_path).convert("RGBA")
            size = min(pil_img.size)
            mask = PILImage.new("L", (size, size), 0)
            draw = ImageDraw.Draw(mask)
            draw.ellipse((0, 0, size, size), fill=255)
            pil_img = pil_img.crop((0, 0, size, size))
            pil_img.putalpha(mask)
            pil_img = pil_img.resize((60, 60), PILImage.LANCZOS)
            x_center = (page_width - 60) / 2
            y_top = page_height - 80
            canvas_obj.drawImage(ImageReader(pil_img), x_center, y_top, width=60, height=60, mask='auto')
        except Exception as e:
            print("⚠ Logo error:", e)

        canvas_obj.setFont("Helvetica-Bold", 25)
        canvas_obj.setFillColor(colors.HexColor("#004080"))
        canvas_obj.drawCentredString(page_width / 2, y_top - 45, "FINAL ESTIMATION INVOICE")

    # ---------- FOOTER ----------
    footer = Paragraph(
        "<font size=10 color='gray'>Thank you for using <b>AL Window Solver</b> — Aluminium Estimation Made Easy.</font>",
        styles['Normal']
    )
    elements.append(footer)

    # ---------- WATERMARK ----------
    elements.append(Watermark("AL Windows Solver"))

    # ---------- BUILD PDF ----------
    pdf.build(elements, onFirstPage=draw_header, onLaterPages=draw_header)
    print(f"✅ PDF Generated Successfully: {output_path}")


# ------------------ ENTRY POINT ------------------
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python make_invoice_pdf.py input.txt output.pdf")
    else:
        generate_invoice(sys.argv[1], sys.argv[2])
