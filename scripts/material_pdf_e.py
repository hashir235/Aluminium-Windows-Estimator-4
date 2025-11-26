from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Spacer, Paragraph
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from datetime import datetime
from collections import Counter
from pathlib import Path
from PIL import Image as PILImage, ImageDraw
import re

# ===========================
# FIXED PROJECT PATH SYSTEM
# ===========================
BASE_DIR = Path(__file__).resolve().parent.parent               # WE Folder 16/
DATA_DIR = BASE_DIR / "data"                                    # data/
ASSET_DIR = BASE_DIR / "assets"                                 # assets/
OUTPUT_DIR = BASE_DIR / "Downloads" / "Material"                # Downloads/Material/

OUTPUT_DIR.mkdir(parents=True, exist_ok=True)


# ---------------- HEADER ----------------
def draw_header(canv, doc):
    page_width, page_height = A4
    try:
        logo_path = ASSET_DIR / "AL Windows.jpg"
        pil_img = PILImage.open(logo_path).convert("RGBA")
        size = min(pil_img.size)
        mask = PILImage.new("L", (size, size), 0)
        draw = ImageDraw.Draw(mask)
        draw.ellipse((0, 0, size, size), fill=255)
        pil_img = pil_img.crop((0, 0, size, size))
        pil_img.putalpha(mask)
        pil_img = pil_img.resize((60, 60), PILImage.LANCZOS)

        temp_path = BASE_DIR / "temp_logo_material.png"
        pil_img.save(temp_path)

        logo_x = (page_width - 60) / 2
        logo_y = page_height - 80
        canv.drawImage(str(temp_path), logo_x, logo_y, width=60, height=60, mask='auto')
    except Exception as e:
        print("⚠️ Logo failed:", e)


# ---------------- FOOTER ----------------
def draw_footer(canv, doc):
    canv.setFont("Helvetica", 9)
    canv.setFillColor(colors.gray)
    footer_text = "Thank you for using AL Window Solver — Aluminium Estimation Made Easy."
    canv.drawCentredString(A4[0] / 2, 20, footer_text)


# ---------------- WATERMARK ----------------
def draw_watermark(canv):
    canv.saveState()
    canv.setFont("Helvetica-Bold", 46)
    canv.setFillColorRGB(0.3, 0.4, 0.8, alpha=0.20)
    page_width, page_height = A4
    canv.translate(page_width / 2, page_height / 2)
    canv.rotate(30)
    canv.drawCentredString(0, 0, "AL WINDOWS SOLVER")
    canv.restoreState()


# ---------------- DECORATOR ----------------
def decorate_page(canv, doc):
    draw_header(canv, doc)
    draw_footer(canv, doc)
    draw_watermark(canv)


# ---------------- MAIN FUNCTION ----------------
def generate_material_pdf():

    # ===========================
    # UNIQUE FILE NAME HERE
    # ===========================
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    pdf_filename = OUTPUT_DIR / f"Material_Table_{timestamp}.pdf"

    # ===========================
    # ALWAYS USE TEMP_MATERIAL.TXT
    # ===========================
    txt_path = DATA_DIR / "Temp_material.txt"

    if not txt_path.exists():
        print("❌ ERROR: Temp_material.txt not found!")
        return

    # READ INPUT LINES
    with open(txt_path, "r", encoding="utf-8") as f:
        lines = [line.strip() for line in f if line.strip()]

    # ================================================
    # SAME LOGIC — SAME DESIGN — DO NOT TOUCH
    # ================================================
    sections_data = []
    for line in lines:
        if "|" in line and not line.startswith("-") and "Section" not in line:
            parts = [p.strip() for p in line.split("|")]
            if len(parts) >= 5:
                section, lengths_str, total_ft, rate, total_price = parts[:5]
                lengths = [x.strip() for x in re.split(r'[,]+', lengths_str) if x.strip().endswith("ft")]
                length_counts = Counter(lengths)
                section_rows = [[section, l, str(q), total_ft, rate, total_price] for l, q in length_counts.items()]
                sections_data.append(section_rows)

    grand_total = next((line.replace("Grand Total Aluminum Cost = ", "").strip()
                        for line in lines if "Grand Total" in line), "")

    # TABLE DATA
    table_data = [["Section", "Length", "Qty", "Total ft", "Rate", "Total Price"]]
    for rows in sections_data:
        table_data.extend(rows)

    # PDF SETUP
    doc = SimpleDocTemplate(str(pdf_filename), pagesize=A4,
                            topMargin=120, bottomMargin=40, leftMargin=40, rightMargin=40)

    styles = getSampleStyleSheet()
    title_style = ParagraphStyle(name="CustomTitle", parent=styles["Title"],
                                 textColor=colors.HexColor("#004080"), fontSize=21, spaceAfter=12)

    elements = []
    elements.append(Spacer(1, -30))
    elements.append(Paragraph("<b>Aluminum Section Cost Table</b>", title_style))
    elements.append(Spacer(1, 10))

    table = Table(table_data, colWidths=[80, 60, 40, 70, 70, 80])
    style = TableStyle([
        ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#004080")),
        ("TEXTCOLOR", (0, 0), (-1, 0), colors.white),
        ("ALIGN", (0, 0), (-1, -1), "CENTER"),
        ("FONTNAME", (0, 0), (-1, 0), "Helvetica-Bold"),
        ("FONTNAME", (0, 1), (-1, -1), "Helvetica"),
        ("FONTSIZE", (0, 0), (-1, -1), 9),
        ("GRID", (0, 0), (-1, -1), 0.3, colors.gray),
        ("VALIGN", (0, 0), (-1, -1), "MIDDLE"),
    ])

    # GROUP COLORING LOGIC — UNTOUCHED
    start_row = 1
    light_blue = colors.Color(0.88, 0.94, 1)
    for rows in sections_data:
        group_size = len(rows)
        if group_size > 1:
            end_row = start_row + group_size - 1
            style.add("SPAN", (0, start_row), (0, end_row))
            style.add("SPAN", (3, start_row), (3, end_row))
            style.add("SPAN", (4, start_row), (4, end_row))
            style.add("SPAN", (5, start_row), (5, end_row))
            style.add("BACKGROUND", (0, start_row), (-1, end_row), light_blue)
        else:
            if start_row % 2 == 0:
                style.add("BACKGROUND", (0, start_row), (-1, start_row), colors.whitesmoke)
        start_row += group_size

    table.setStyle(style)
    elements.append(table)
    elements.append(Spacer(1, 15))

    if grand_total:
        elements.append(Paragraph(f"Grand Total Aluminum Cost: <b>{grand_total}</b>", styles["Heading3"]))

    elements.append(Spacer(1, 8))
    elements.append(Paragraph(f"<b>Generated on:</b> {datetime.now().strftime('%d-%b-%Y %H:%M')}",
                              styles["Normal"]))

    # BUILD PDF
    doc.build(elements, onFirstPage=decorate_page, onLaterPages=decorate_page)

    print(f"✅ PDF created successfully: {pdf_filename}")


# ---------------- ENTRY ----------------
if __name__ == "__main__":
    generate_material_pdf()
