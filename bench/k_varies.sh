#!/bin/sh

set -euC

# Build directory
readonly BUILD=build/

if [ "$#" = 0 ]; then
  echo "usage: $0 ALGO1 ALGO2 ..."
  exit 1
fi >&2

for name in "$@"; do
  make "${BUILD}bench/$name.dat"
done >&2

{ for name in "$@"; do
    printf '%sbench/%s.dat ' "$BUILD" "$name"
  done } | gnuplot -d plot.gpi
