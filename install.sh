#!/bin/sh

DIR=$1
if [[ "$DIR" == "" ]]; then
	echo missing install DIR
	exit 1
fi

cd "$(dirname "$(realpath $0)")"

if true; then
	sh asm_post.sh doomgeneric/everything.s
	echo "wad_file:"
	echo "include wad_file"
fi > exe.asm

cp exe.asm "$DIR/doom.asm"
cp encoder/doom1wad.enc "$DIR/wad_file.asm"
