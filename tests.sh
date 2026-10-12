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
    printf "\n\nPASS LEX\n"
else
    printf "\n\nFAIL LEX\n"
    diff <(printf '%s' "${actual%x}") <(printf '%s' "${expected%x}")
    exit 1
fi

PARSE_BIN="$BUILD/parse_debug"

"$PARSE_BIN" "$TEST/parse.ok.swift" #> /dev/null
if [[ $? -eq 0 ]]; then
    printf "\n\nPASS PARSE (valid input accepted)\n"
else
    printf "\n\nFAIL PARSE (valid input was rejected)\n"
    exit 1
fi

"$PARSE_BIN" "$TEST/parse.bad.swift" #> /dev/null 2>&1
if [[ $? -ne 0 ]]; then
    printf "\n\nPASS PARSE (invalid input rejected)\n"
else
    printf "\n\nFAIL PARSE (invalid input was accepted)\n"
    exit 1
fi
