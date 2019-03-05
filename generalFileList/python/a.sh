#!/usr/bin/env sh

cat $1 | sed 's;^;"file://;g' | sed 's;$;",;'> mcLbToJPsiPcK$1_cfi.py
