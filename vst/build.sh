#!/usr/bin/env bash
set -euo pipefail
: "${MPC_VST:?MPC_VST must point to a checkout of sd88me/mpc-vst-plugins}"
"$MPC_VST/tools/build_port.sh" "$PWD/vst/vst.json" armv7
