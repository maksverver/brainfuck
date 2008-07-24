#!/bin/sh
# Brainfuck compiler depending on the system C-compiler

if [ $# = 0 ]; then
	echo "Usage: $0 sourcefile [compiler options]"
	exit
fi

PATH=/bin:/usr/bin:/usr/local/bin
( echo 'unsigned char get() { int c = getchar(); return c < 0 ? 0 : c; }'
  echo 'int main() { '
  echo 'static unsigned char buf[1000000]; '
  echo 'unsigned char *ptr = buf + 500000;'
  cat "$1" | \
  sed 's/[^]+-.,<>[]//g' | \
  sed 's/[+]/++*ptr;/g' | \
  sed 's/-/--*ptr;/g' | \
  sed 's/>/++ptr;/g' | \
  sed 's/</--ptr;/g' | \
  sed 's/[.]/putchar(*ptr);/g' | \
  sed 's/,/*ptr=get();/g' | \
  sed 's/[[]/while(*ptr){/g' | \
  sed 's/]/}/g'
  echo '}'
) |  cc -x c - -DFILE=$@
