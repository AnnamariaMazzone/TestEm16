#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/annamaria/lavoro/xray_shielding/TestEm16
  make -f /Users/annamaria/lavoro/xray_shielding/TestEm16/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/annamaria/lavoro/xray_shielding/TestEm16
  make -f /Users/annamaria/lavoro/xray_shielding/TestEm16/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/annamaria/lavoro/xray_shielding/TestEm16
  make -f /Users/annamaria/lavoro/xray_shielding/TestEm16/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/annamaria/lavoro/xray_shielding/TestEm16
  make -f /Users/annamaria/lavoro/xray_shielding/TestEm16/CMakeScripts/ReRunCMake.make
fi

