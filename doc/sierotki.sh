#!/bin/bash
if [ "$1" ]; then
    cat "$1" | sed '
	/ [aeiouwzAEIOUWZ] *$/ {
	N
	s/  *\([aeiouwzAEIOUWZ]\)\n */ \1~/g
	}' | sed 's/^ *\([aeiouwzAEIOUWZ]\)  */ \1~/g'  | sed 's/  *\([aeiouwzAEIOUWZ]\)  */ \1~/g'
else
    echo "usage: ./sierotki.sh <filename>";
fi
