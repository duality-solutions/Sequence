#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/sequence.png
ICON_DST=../../src/qt/res/icons/sequence.ico
convert ${ICON_SRC} -resize 16x16 sequence-16.png
convert ${ICON_SRC} -resize 32x32 sequence-32.png
convert ${ICON_SRC} -resize 48x48 sequence-48.png
convert sequence-16.png sequence-32.png sequence-48.png ${ICON_DST}

