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

parserslt=$("$PARSE_BIN" "$TEST/parse.ok.swift") #> /dev/null
if [[ $? -eq 0 ]]; then
    printf "\n\nPASS PARSE (valid input accepted)\n"
else
    printf "\n\nFAIL PARSE (valid input was rejected)\n"
    printf "\n$parserslt\n"
fi

# Every invalid file must be rejected *by the parser*:
#   - exit status exactly 1 (what yyerror() uses). Anything >= 128 means the
#     binary was killed by a signal (e.g. 139 = SIGSEGV), which is a crash,
#     not a rejection, even though it is "non-zero".
#   - the output must contain "parse error:", so an unreadable/missing file
#     (which also exits 1) can't masquerade as a pass.
printf "\n\nPARSE (invalid inputs)\n"
bad_failed=0
bad_total=0
for f in "$TEST"/parse_bad/*.swift; do
    [[ -e "$f" ]] || { printf "  FAIL  %s (file not found)\n" "$f"; bad_failed=$((bad_failed + 1)); continue; }
    bad_total=$((bad_total + 1))

    out="$("$PARSE_BIN" "$f" 2>&1)"
    status=$?

    if [[ $status -eq 0 ]]; then
        printf "  FAIL  %s (invalid input was accepted)\n" "$f"
        bad_failed=$((bad_failed + 1))
    elif [[ $status -ge 128 ]]; then
        printf "  FAIL  %s (crashed with signal %d)\n" "$f" $((status - 128))
        bad_failed=$((bad_failed + 1))
    elif [[ $status -ne 1 ]]; then
        printf "  FAIL  %s (unexpected exit status %d)\n" "$f" "$status"
        bad_failed=$((bad_failed + 1))
    elif [[ "$out" != *"parse error:"* ]]; then
        printf "  FAIL  %s (exit 1 but no parse error reported)\n" "$f"
        printf '%s\n' "$out" | sed 's/^/        | /'
        bad_failed=$((bad_failed + 1))
    else
        printf "  ok    %s\n" "$f"
    fi
done

if [[ $bad_failed -eq 0 ]]; then
    printf "\n\nPASS PARSE (all %d invalid inputs rejected)\n" "$bad_total"
else
    printf "\n\nFAIL PARSE (%d of %d invalid inputs not rejected properly)\n" "$bad_failed" "$bad_total"
    exit 1
fi
