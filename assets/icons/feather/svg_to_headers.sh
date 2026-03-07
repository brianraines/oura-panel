#!/bin/bash
# Feather SVG to Adafruit GFX header conversion script.
# Modeled after ../Weather/esp32-weather-epd/icons/svg_to_headers.sh

set -euo pipefail

if [ $# -lt 3 ]; then
  echo "Usage: $0 <size_px> <color_folder> <icon1> [icon2 ...]"
  echo "Example: $0 24 black moon zap activity heart"
  exit 1
fi

SIZE="$1"
COLOR="$2"
shift 2
ICONS=("$@")

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SVG_DIR="${SCRIPT_DIR}/${COLOR}"
PNG_PATH="${SCRIPT_DIR}/png/${COLOR}/${SIZE}x${SIZE}"
HEADER_PATH="${SCRIPT_DIR}/../../../include/icons/feather/${COLOR}/${SIZE}x${SIZE}"
MASTER_HEADER="${SCRIPT_DIR}/../../../include/icons/feather/${COLOR}/icons_${SIZE}x${SIZE}.h"
COLOR_UPPER="$(echo "${COLOR}" | tr '[:lower:]' '[:upper:]')"

mkdir -p "${PNG_PATH}"
mkdir -p "${HEADER_PATH}"
rm -f "${PNG_PATH}"/*.png
rm -f "${HEADER_PATH}"/*.h
rm -f "${MASTER_HEADER}"

PYTHON_BIN="${SCRIPT_DIR}/../../../.venv/bin/python"
if [ ! -x "${PYTHON_BIN}" ]; then
  PYTHON_BIN="python3"
fi

for icon in "${ICONS[@]}"; do
  svg="${SVG_DIR}/${icon}.svg"
  png="${PNG_PATH}/${icon}.png"
  out="${HEADER_PATH}/${icon}${SIZE}x${SIZE}.h"
  if [ ! -f "${svg}" ]; then
    echo "Missing SVG: ${svg}"
    exit 1
  fi

  echo "Converting ${svg} -> ${png}"
  "${PYTHON_BIN}" - <<PY
from cairosvg import svg2png
svg2png(url=r"${svg}", write_to=r"${png}", output_width=${SIZE}, output_height=${SIZE}, background_color="white")
PY

  echo "Generating header ${out}"
  "${PYTHON_BIN}" "${SCRIPT_DIR}/png_to_header.py" -i "${png}" -o "${out}"
done

echo "Generating include header ${MASTER_HEADER}"
{
  echo "#ifndef __FEATHER_${COLOR_UPPER}_ICONS_${SIZE}X${SIZE}_H__"
  echo "#define __FEATHER_${COLOR_UPPER}_ICONS_${SIZE}X${SIZE}_H__"
  for icon in "${ICONS[@]}"; do
    echo "#include \"${SIZE}x${SIZE}/${icon}${SIZE}x${SIZE}.h\""
  done
  echo "#endif"
} > "${MASTER_HEADER}"

echo "Done."
