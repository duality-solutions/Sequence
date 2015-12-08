#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/silk.ico

convert ../../src/qt/res/icons/silk-16.png ../../src/qt/res/icons/silk-32.png ../../src/qt/res/icons/silk-48.png ${ICON_DST}
