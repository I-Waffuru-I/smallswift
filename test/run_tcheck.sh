#!/usr/bin/env bash
# Typechecker tests.
#   test/tcheck_ok/*.swift   must typecheck: exit code 0
#   test/tcheck_bad/*.swift  must be rejected CLEANLY: exit code 1 and the
#                            expected text on stderr (a crash is not a pass)
# usage: test/run_tcheck.sh [path/to/comp]
cd "$(dirname "$0")/.." || exit 1
BIN=${1:-./build/bin/comp}
pass=0; fail=0

for f in test/tcheck_ok/*.swift; do
    "$BIN" "$f" >/dev/null 2>/tmp/tcheck.err; rc=$?
    if [ $rc -eq 0 ]; then printf "PASS ok   %s\n" "$f"; pass=$((pass+1))
    else printf "FAIL ok   %s (exit %d)\n" "$f" $rc; sed 's/^/       /' /tmp/tcheck.err; fail=$((fail+1)); fi
done

# name  expected-substring-on-stderr
expect_bad() {
    local f="test/tcheck_bad/$1.swift" want="$2"
    "$BIN" "$f" >/dev/null 2>/tmp/tcheck.err; local rc=$?
    if [ $rc -eq 1 ] && grep -q "$want" /tmp/tcheck.err; then
        printf "PASS bad  %s\n" "$f"; pass=$((pass+1))
    else
        printf "FAIL bad  %s (exit %d, wanted exit 1 with \"%s\")\n" "$f" $rc "$want"; fail=$((fail+1))
    fi
}
# An empty expected text means "any message, but it must be a clean exit 1".
expect_bad int_plus_bool     "Mismatched types"
expect_bad arith_on_bools    ""
expect_bad compare_bools     ""
expect_bad use_undeclared    ""
expect_bad self_reference    ""
expect_bad assign_to_let     "not defined as mutable"
expect_bad assign_wrong_type "can't assign"
expect_bad redeclare         ""
# keep these two files from your earlier tests too
[ -f test/tcheck_bad/assign_unknown.swift ] && expect_bad assign_unknown "not declared"
[ -f test/tcheck_bad/reassign_newtype.swift ] && expect_bad reassign_newtype "can't assign"

printf "\n%d passed, %d failed\n" $pass $fail
[ $fail -eq 0 ]
