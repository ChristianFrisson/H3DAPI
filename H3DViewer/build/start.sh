#!/bin/sh

H3DVIEWER_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/H3DViewer//'`"

H3DVIEWER_RESOURCES="$H3DVIEWER_BUNDLE/Contents/Resources"

echo "running $0"
echo "H3DVIEWER_BUNDLE: $H3DVIEWER_BUNDLE"

#export "DYLD_LIBRARY_PATH=$H3DVIEWER_RESOURCES/lib"
#export "FONTCONFIG_PATH=$H3DVIEWER_RESOURCES/etc/fonts"

#export
exec "$H3DVIEWER_RESOURCES/bin/H3DViewer"