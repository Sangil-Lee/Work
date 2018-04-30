#!/bin/sh

E="FP_monTop.edl"
P="FP:"
R="1:"
HAVE_P=""
HAVE_R=""
export EDMDATAFILES="."

for i in "$@"
do
    case "$i" in
    *.edl)       E="$i"  ;;
    *)  if [ -z "$HAVE_P" ] 
        then
            HAVE_P="Y"
            P="$i"
        else
            if [ -z "$HAVE_R" ] 
            then
                HAVE_R="Y"
                R="$i"
            fi
        fi ;;
    esac
done

# Work around "System Integrity Protection" and bug in OpenMotif on later versions of OS X.
export DYLD_LIBRARY_PATH=/opt/X11/lib/flat_namespace

edm -eolc -x -m "P=${P},R=${R}" "$E" &
