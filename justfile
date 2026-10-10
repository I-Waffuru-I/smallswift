build_dir := "build"
target := "comp"

default:
    @just --list

build:
    cmake -S . -B {{build_dir}} -DCMAKE_BUILD_TYPE=Release
    cmake --build {{build_dir}} -j

run *args: build
    ./{{build_dir}}/bin/{{target}} {{args}}

lex file: build
    ./{{build_dir}}/bin/lex_debug {{file}}

parse file: build
    ./{{build_dir}}/bin/parse_debug {{file}}

clean:
    rm -rf {{build_dir}}


test: build 
    ./tests.sh

