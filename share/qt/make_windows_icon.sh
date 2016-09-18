#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/silk.png
ICON_DST=../../src/qt/res/icons/silk.ico
convert ${ICON_SRC} -resize 16x16 silk-16.png
convert ${ICON_SRC} -resize 32x32 silk-32.png
convert ${ICON_SRC} -resize 48x48 silk-48.png
convert silk-16.png silk-32.png silk-48.png ${ICON_DST}

