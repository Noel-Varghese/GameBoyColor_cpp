#!/bin/bash
set -e

OUT="gbc"
FLAGS="-std=c++17 -Wall -Wextra"
MODE="${1:-debug}"

case "$MODE" in
  debug)
    FLAGS="$FLAGS -g -O0 -fsanitize=address,undefined"
    ;;
  release)
    FLAGS="$FLAGS -O2 -DLOG_DISABLE_DEBUG"
    ;;
  clean)
    rm -f "$OUT"
    echo "removed ./$OUT"
    exit 0
    ;;
  *)
    echo "usage: ./build.sh [debug|release|clean]"
    exit 1
    ;;
esac

if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists sdl3; then
  SDL_FLAGS=$(pkg-config --cflags --libs sdl3)
else
  echo "warning: pkg-config could not find sdl3, falling back to -lSDL3"
  echo "         (Fedora: sudo dnf install SDL3-devel)"
  SDL_FLAGS="-lSDL3"
fi

mapfile -t SOURCES < <(find . -name '*.cpp' -not -path './build/*')

if [ ${#SOURCES[@]} -eq 0 ]; then
  echo "no .cpp files found"
  exit 1
fi

echo "building ($MODE): ${#SOURCES[@]} source files"
g++ $FLAGS "${SOURCES[@]}" $SDL_FLAGS -o "$OUT"
echo "built ./$OUT"
