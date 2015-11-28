#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/silkcoin.ico

convert ../../src/qt/res/icons/silkcoin-16.png ../../src/qt/res/icons/silkcoin-32.png ../../src/qt/res/icons/silkcoin-48.png ${ICON_DST}
