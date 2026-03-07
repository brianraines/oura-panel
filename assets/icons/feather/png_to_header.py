#!/usr/bin/env python3
"""
Converts a monochrome PNG icon to an Adafruit-GFX-compatible bitmap header.
Based on the approach used in ../Weather/esp32-weather-epd/icons/png_to_header.py.
"""

import getopt
import os.path
import sys
from PIL import Image

BITES_PER_LINE = 12
BITS_PER_BITE = 8
THRESHOLD = 127


def usage():
    print("png_to_header.py -i <inputfile> -o <outputfile>")


try:
    opts, _ = getopt.getopt(sys.argv[1:], "hi:o:", ["inputfile=", "outputfile="])
except getopt.GetoptError:
    usage()
    sys.exit(2)

inputfile = None
outputfile = None

for opt, arg in opts:
    if opt == "-h":
        usage()
        sys.exit(0)
    elif opt in ("-i", "--inputfile"):
        inputfile = arg
    elif opt in ("-o", "--outputfile"):
        outputfile = arg

if not inputfile:
    print("Error: inputfile is a required parameter. See usage -h.")
    sys.exit(1)
if not outputfile:
    print("Error: outputfile is a required parameter. See usage -h.")
    sys.exit(1)

src_image = Image.open(inputfile)
src_g = src_image.convert("L")
pixels = list(src_g.getdata())

with open(outputfile, "w", encoding="utf-8") as f:
    var = os.path.basename(outputfile).rsplit(".h", 1)[0]
    width, height = src_image.size
    f.write(f"// {width} x {height}\n")
    f.write(f"const unsigned char {var}[] PROGMEM = {{\n ")

    bit_cnt = 1
    transcribed_width = 1
    tmp_bite = 0
    n = len(pixels)
    line_width = 0

    for i in range(n):
        line_width += 1
        if pixels[i] <= THRESHOLD:
            tmp_bite |= 1
        if line_width == width:
            tmp_bite <<= BITS_PER_BITE - bit_cnt
            line_width = 0
            bit_cnt = 8
        if bit_cnt < BITS_PER_BITE and i != n - 1:
            tmp_bite <<= 1
            bit_cnt += 1
        elif i != n - 1:
            f.write(" " + "0x{:02x}".format(tmp_bite) + ",")
            if transcribed_width == BITES_PER_LINE:
                f.write("\n ")
                transcribed_width = 1
            else:
                transcribed_width += 1
            tmp_bite = 0
            bit_cnt = 1

    tmp_bite <<= BITS_PER_BITE - bit_cnt
    f.write(" " + "0x{:02x}".format(tmp_bite) + "\n};\n")
