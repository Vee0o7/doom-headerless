#!/bin/sh

cat $1 |
	sed 's/\.ascii\t//' |
	sed 's/\.string\t"\(.*\)"$/"\1\\0"/' |
	grep -Pv '\t\.local\t' |
	grep -Pv '\t\.global\t' |
	grep -Pv '\t\.type\t' |
	grep -Pv '\t\.size\t' |
	grep -Pv '\t\.file\t' |
	grep -Pv '\t\.ident\t' |
	grep -Pv '\t\.section\t' |
	sed 's/\.L/L/' |
  sed 's/^\([^"]*\)\.\([0-9]\)/\1\2/g' | # label.1 has weird behavior in Turing Complete
	cat
