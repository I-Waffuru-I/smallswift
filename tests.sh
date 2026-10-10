#!/bin/bash

BUILD="./build/bin"
TEST="./test"

LEX_BIN="$BUILD/lex_debug"
LEX_EXPECTED="$TEST/lex.output.txt"

actual="$($LEX_BIN $TEST/lex.swift;)"
expected="$(cat "$LEX_EXPECTED";)"

echo "actual: [$actual]"
echo "expected: [$expected]"
if [[ "$actual" == "$expected" ]]; then
    echo "PASS LEX"
else
    echo "FAIL LEX"
    diff <(printf '%s' "${actual%x}") <(printf '%s' "${expected%x}")
    exit 1
fi

PARSE_BIN="$BUILD/parse_debug"

"$PARSE_BIN" "$TEST/parse.ok.swift" #> /dev/null
if [[ $? -eq 0 ]]; then
    echo "PASS PARSE (valid input accepted)"
else
    echo "FAIL PARSE (valid input was rejected)"
    exit 1
fi

"$PARSE_BIN" "$TEST/parse.bad.swift" #> /dev/null 2>&1
if [[ $? -ne 0 ]]; then
    echo "PASS PARSE (invalid input rejected)"
else
    echo "FAIL PARSE (invalid input was accepted)"
    exit 1
fi
