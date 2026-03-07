#!/usr/bin/env python3
from __future__ import annotations

from pathlib import Path

import freetype


FIRST = 0x20
LAST = 0x7E


def rasterize_glyph(face: freetype.Face, ch: int):
    face.load_char(chr(ch), freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_MONO)
    g = face.glyph
    bmp = g.bitmap
    width = bmp.width
    rows = bmp.rows
    pitch = abs(bmp.pitch)
    buf = bmp.buffer

    packed = bytearray()
    bit_acc = 0
    bit_count = 0

    for y in range(rows):
        row = buf[y * pitch : y * pitch + pitch]
        for x in range(width):
            src_byte = row[x // 8] if pitch > 0 else 0
            on = 1 if (src_byte & (0x80 >> (x % 8))) else 0
            bit_acc = (bit_acc << 1) | on
            bit_count += 1
            if bit_count == 8:
                packed.append(bit_acc)
                bit_acc = 0
                bit_count = 0

    if bit_count:
        packed.append(bit_acc << (8 - bit_count))

    return {
        "bitmap": packed,
        "width": width,
        "height": rows,
        "xAdvance": g.advance.x >> 6,
        "xOffset": g.bitmap_left,
        "yOffset": -g.bitmap_top,
    }


def gen_font(ttf_path: Path, size: int, out_path: Path):
    face = freetype.Face(str(ttf_path))
    face.set_pixel_sizes(0, size)

    y_advance = face.size.height >> 6
    if y_advance <= 0:
        y_advance = size

    bitmaps = bytearray()
    glyph_lines = []

    for ch in range(FIRST, LAST + 1):
        g = rasterize_glyph(face, ch)
        offset = len(bitmaps)
        bitmaps.extend(g["bitmap"])
        glyph_lines.append(
            f"    {{{offset}, {g['width']}, {g['height']}, {g['xAdvance']}, {g['xOffset']}, {g['yOffset']}}}, // 0x{ch:02X}"
        )

    symbol = f"PoppinsRegular{size}pt7b"
    bitmap_symbol = f"{symbol}Bitmaps"
    glyph_symbol = f"{symbol}Glyphs"

    hex_bytes = ", ".join(f"0x{b:02X}" for b in bitmaps)
    if not hex_bytes:
        hex_bytes = "0x00"

    content = f"""#pragma once
#include <Adafruit_GFX.h>

const uint8_t {bitmap_symbol}[] PROGMEM = {{
    {hex_bytes}
}};

const GFXglyph {glyph_symbol}[] PROGMEM = {{
{chr(10).join(glyph_lines)}
}};

const GFXfont {symbol} PROGMEM = {{
    (uint8_t*){bitmap_symbol},
    (GFXglyph*){glyph_symbol},
    0x{FIRST:02X}, 0x{LAST:02X}, {y_advance}
}};
"""
    out_path.write_text(content, encoding="utf-8")
    print(f"Wrote {out_path}")


def main():
    root = Path(__file__).resolve().parents[1]
    ttf = root / "assets" / "fonts" / "ttf" / "Poppins-Regular.ttf"
    out_dir = root / "include" / "fonts"
    out_dir.mkdir(parents=True, exist_ok=True)

    for size in (9, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 42, 46, 48, 52, 64):
        out = out_dir / f"PoppinsRegular{size}pt7b.h"
        gen_font(ttf, size, out)


if __name__ == "__main__":
    main()
