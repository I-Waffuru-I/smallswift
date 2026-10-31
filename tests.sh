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
#   - the output must contain "Parse error" (see yyerror in parser.y), so an unreadable/missing file
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
    elif [[ "$out" != *"Parse error"* ]]; then
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

# ---------------------------------------------------------------------------
# TYPECHECK
#   test/tcheck_ok/*.swift   must typecheck: exit status 0
#   test/tcheck_bad/*.swift  must be rejected *by the typechecker*:
#     - exit status exactly 1 (a crash, status >= 128, is not a rejection)
#     - stderr must contain the expected text. An empty expected text means
#       "any message", which still requires a clean exit 1.
# ---------------------------------------------------------------------------
COMP_BIN="$BUILD/comp"

printf "\n\nTYPECHECK (valid inputs)\n"
ok_failed=0
ok_total=0
for f in "$TEST"/tcheck_ok/*.swift; do
    [[ -e "$f" ]] || { printf "  FAIL  %s (file not found)\n" "$f"; ok_failed=$((ok_failed + 1)); continue; }
    ok_total=$((ok_total + 1))

    out="$("$COMP_BIN" "$f" 2>&1 >/dev/null)"
    status=$?

    if [[ $status -eq 0 ]]; then
        printf "  ok    %s\n" "$f"
    else
        printf "  FAIL  %s (valid input rejected, exit status %d)\n" "$f" "$status"
        printf '%s\n' "$out" | sed 's/^/        | /'
        ok_failed=$((ok_failed + 1))
    fi
done

if [[ $ok_failed -eq 0 ]]; then
    printf "\n\nPASS TYPECHECK (all %d valid inputs accepted)\n" "$ok_total"
else
    printf "\n\nFAIL TYPECHECK (%d of %d valid inputs rejected)\n" "$ok_failed" "$ok_total"
fi

# name (without .swift)   text expected on stderr
TCHECK_BAD=(
    "int_plus_bool|Mismatched types"
    "arith_on_bools|"
    "compare_bools|"
    "use_undeclared|"
    "self_reference|"
    "redeclare|"
    "assign_to_let|not defined as mutable"
    "assign_wrong_type|can't assign"
    "assign_unknown|not declared"
    "reassign_newtype|can't assign"
)

printf "\n\nTYPECHECK (invalid inputs)\n"
tbad_failed=0
tbad_total=0
for entry in "${TCHECK_BAD[@]}"; do
    name="${entry%%|*}"
    want="${entry#*|}"
    f="$TEST/tcheck_bad/$name.swift"
    tbad_total=$((tbad_total + 1))

    [[ -e "$f" ]] || { printf "  FAIL  %s (file not found)\n" "$f"; tbad_failed=$((tbad_failed + 1)); continue; }

    out="$("$COMP_BIN" "$f" 2>&1 >/dev/null)"
    status=$?

    if [[ $status -eq 0 ]]; then
        printf "  FAIL  %s (invalid input was accepted)\n" "$f"
        tbad_failed=$((tbad_failed + 1))
    elif [[ $status -ge 128 ]]; then
        printf "  FAIL  %s (crashed with signal %d)\n" "$f" $((status - 128))
        tbad_failed=$((tbad_failed + 1))
    elif [[ $status -ne 1 ]]; then
        printf "  FAIL  %s (unexpected exit status %d)\n" "$f" "$status"
        tbad_failed=$((tbad_failed + 1))
    elif [[ "$out" != *"$want"* ]]; then
        printf "  FAIL  %s (exit 1 but message lacks \"%s\")\n" "$f" "$want"
        printf '%s\n' "$out" | sed 's/^/        | /'
        tbad_failed=$((tbad_failed + 1))
    else
        printf "  ok    %s\n" "$f"
    fi
done

if [[ $tbad_failed -eq 0 ]]; then
    printf "\n\nPASS TYPECHECK (all %d invalid inputs rejected)\n" "$tbad_total"
else
    printf "\n\nFAIL TYPECHECK (%d of %d invalid inputs not rejected properly)\n" "$tbad_failed" "$tbad_total"
fi

if [[ $ok_failed -ne 0 || $tbad_failed -ne 0 ]]; then
    exit 1
fi
