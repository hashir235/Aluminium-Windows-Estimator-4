#!/usr/bin/env python3
"""
CuttingPDF_fixed.py - FINAL VERSION (Fake Section Fix + Center Section + Footer)
Design 100% unchanged — only:
✔ Section heading centered
✔ Footer added
"""

import sys, os, re, datetime
from pathlib import Path
from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Paragraph, Spacer
from reportlab.platypus import PageBreak
from reportlab.lib.pagesizes import A4
from reportlab.lib.units import cm, mm
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib import colors

# =============================================================
# FIXED PATHS
# =============================================================

BASE_DIR = Path(__file__).resolve().parent.parent
DATA_DIR = BASE_DIR / "data"
OUTPUT_DIR = BASE_DIR / "Downloads" / "CuttingReport"
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

# =============================================================
# STYLES (unchanged)
# =============================================================

styles = getSampleStyleSheet()

style_section_center = ParagraphStyle(
    "section_center",
    parent=styles["Heading2"],
    fontName="Helvetica-Bold",
    fontSize=14,
    alignment=1,            # CENTER
    spaceAfter=6
)

style_header = ParagraphStyle(
    "header",
    parent=styles["Normal"],
    fontName="Helvetica-Bold",
    fontSize=10,
    alignment=1
)

style_cell = ParagraphStyle(
    "cell",
    parent=styles["Normal"],
    fontName="Helvetica",
    fontSize=9
)

# Column widths SAME as original
COL_WIDTHS = [
    2.0*cm, 3.5*cm, 1.5*cm, 4.0*cm, 3.0*cm, 2.0*cm
]

# =============================================================
# HEADER PARSING REGEX
# =============================================================

SECTION_RE = re.compile(r"^=+\s*(.+?)\s*=+$")
LENGTH_RE  = re.compile(r"^Length\s*:\s*([\d.]+)\s*ft", re.I)
WASTAGE_RE = re.compile(r"^WASTAGE\s*:\s*([\d.]+)\s*ft", re.I)


# =============================================================
# PARSER (Fake section fix included)
# =============================================================

def parse_txt(path):

    lines = Path(path).read_text(encoding="utf-8").splitlines()
    lines = [ln.strip() for ln in lines if ln.strip()]

    # Extract heading from top
    heading = "Cutting Sizes Report"
    if lines and lines[0].startswith("="):
        heading = lines[0].replace("=", "").strip()

    fake_heading = heading

    sections = {}
    current_sec = None

    i = 0
    while i < len(lines):
        ln = lines[i]

        # SECTION HEADER
        mh = SECTION_RE.match(ln)
        if mh:
            sec_name = mh.group(1).strip()

            # Skip fake section
            if sec_name == fake_heading:
                i += 1
                continue

            sections.setdefault(sec_name, [])
            current_sec = sec_name
            i += 1
            continue

        # LENGTH BLOCK
        ml = LENGTH_RE.match(ln)
        if ml and current_sec:
            length = ml.group(1) + " ft"
            rows = []
            wastage = ""

            i += 1
            # skip header lines
            while i < len(lines) and (
                lines[i].startswith("-") or
                "window" in lines[i].lower() or
                not lines[i]
            ):
                i += 1

            # read rows
            while i < len(lines):
                row = lines[i]

                # wastage
                mw = WASTAGE_RE.match(row)
                if mw:
                    wastage = mw.group(1) + " ft"
                    i += 1
                    while i < len(lines) and lines[i].startswith("-"):
                        i += 1
                    break

                if SECTION_RE.match(row):
                    break

                if "|" in row:
                    cols = [p.strip() for p in row.split("|")]
                    if len(cols) >= 4:
                        rows.append((cols[0], cols[1], cols[2], cols[3]))

                i += 1

            sections[current_sec].append({
                "length": length,
                "wastage": wastage,
                "rows": rows
            })

            continue

        i += 1

    return heading, sections


# =============================================================
# FOOTER (new)
# =============================================================

def draw_footer(canvas, doc):
    canvas.setFont("Helvetica", 8)
    canvas.setFillColor(colors.gray)
    canvas.drawCentredString(
        A4[0] / 2,
        10,   # bottom margin
        "Thank you for using AL Window Solver — Aluminium Estimation Made Easy."
    )


# =============================================================
# BUILD PDF (design unchanged except center headings + footer)
# =============================================================

def build_pdf(heading, sections, output_path):

    doc = SimpleDocTemplate(
        str(output_path),
        pagesize=A4,
        leftMargin=12*mm,
        rightMargin=12*mm,
        topMargin=12*mm,
        bottomMargin=20*mm   # space for footer
    )

    story = []

    # TOP HEADING ONLY ONCE
    story.append(Paragraph(heading, styles["Title"]))
    story.append(Spacer(1, 10))

    # All sections
    for sec_name, blocks in sections.items():

        # SECTION NAME — NOW CENTERED
        story.append(Paragraph(sec_name, style_section_center))

        table_data = []

        # TABLE HEADER
        table_data.append([
            Paragraph("<b>Length</b>", style_header),
            Paragraph("<b>Window</b>", style_header),
            Paragraph("<b>No#</b>", style_header),
            Paragraph("<b>Dimension</b>", style_header),
            Paragraph("<b>Size</b>", style_header),
            Paragraph("<b>Wastage</b>", style_header),
        ])

        # Stack all blocks
        for blk in blocks:

            length = blk["length"]
            wastage = blk["wastage"]
            rows = blk["rows"]

            if not rows:
                continue

            for r in rows:
                table_data.append([
                    length, r[0], r[1], r[2], r[3], wastage
                ])
                length = ""
                wastage = ""

        tbl = Table(table_data, colWidths=COL_WIDTHS, repeatRows=1)

        style = [
            ("GRID", (0,0), (-1,-1), 0.4, colors.black),
            ("VALIGN", (0,0), (-1,-1), "MIDDLE"),
            ("BACKGROUND", (0,0), (-1,0), colors.whitesmoke),
            ("FONTSIZE", (0,0), (-1,-1), 9),
        ]

        # Merging logic unchanged
        row_ptr = 1
        for blk in blocks:
            rows = blk["rows"]
            if not rows:
                continue

            start = row_ptr
            end = row_ptr + len(rows) - 1

            style.append(("SPAN", (0, start), (0, end)))
            style.append(("ALIGN", (0, start), (0, end), "CENTER"))

            style.append(("SPAN", (5, start), (5, end)))
            style.append(("ALIGN", (5, start), (5, end), "CENTER"))

            style.append(("BACKGROUND", (0, start), (-1, end), colors.HexColor("#E0EEFF")))
            style.append(("LINEABOVE", (0, start), (-1, start), 2.0, colors.black))
            style.append(("LINEBELOW", (0, end), (-1, end), 2.0, colors.black))

            row_ptr = end + 1

        tbl.setStyle(TableStyle(style))

        story.append(tbl)
        story.append(Spacer(1, 12))

    # build with footer on all pages
    doc.build(story, onFirstPage=draw_footer, onLaterPages=draw_footer)


# =============================================================
# MAIN
# =============================================================

def main():

    if len(sys.argv) < 2:
        print("Usage: python CuttingPDF_fixed.py data/CuttingSizeReport.txt")
        return

    input_file = DATA_DIR / Path(sys.argv[1]).name

    if not input_file.exists():
        print("❌ ERROR: File not found:", input_file)
        return

    heading, sections = parse_txt(input_file)

    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    output_pdf = OUTPUT_DIR / f"CuttingReport_{timestamp}.pdf"

    build_pdf(heading, sections, output_pdf)

    print("✅ PDF generated:", output_pdf)


if __name__ == "__main__":
    main()
