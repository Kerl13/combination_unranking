#!/bin/sh

set -euC

# Build directory
readonly BUILD=build/

if [ "$#" = 0 ]; then
  echo "usage: $0 ALGO1 ALGO2 ..."
  exit 1
fi >&2

extension=""
if [ "$(basename "$0")" = "k_varies.sh" ]; then
  extension=.k.dat
elif [ "$(basename "$0")" = "n_varies.sh" ]; then
  extension=.n.dat
else
  exit 2
fi

get_color () {
  if [ "${1#*recursive}" != "$1" ]; then
    echo "red"
  elif [ "${1#*combinadics2}" != "$1" ]; then
    echo "#7b4000"
  elif [ "${1#*combinadics}" != "$1" ]; then
    echo "#6494ee"
  elif [ "${1#*factoradics}" != "$1" ]; then
    echo "#004521"
  else
    echo I don\'t know this algorithm >&2
    exit 1
  fi
}

for name in "$@"; do
  make "${BUILD}bench/${name}${extension}"
done >&2

get_cmd_line_args () {
  # Set the 'files' variable
  printf '%s\n' "-e"
  printf '%sfiles = %s' "'" '"'
  for name in "$@"; do
    printf ' %sbench/%s%s' "$BUILD" "$name" "$extension"
  done
  printf '%s%s\n' '"' "'"

  # Set the colors
  POS=0
  for name in "$@"; do
    POS="$((POS + 1))"
    printf '%s\n' "-e"
    printf '%sset style line %d lt rgb "%s"%s\n' "'" "$POS" "$(get_color "$name")" "'"
  done

  # Specify the script to be called
  printf '%s\n' -d
  printf 'plot.gpi\n'
}

get_cmd_line_args "$@" | xargs gnuplot
