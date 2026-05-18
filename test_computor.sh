#!/usr/bin/env bash

set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN="$ROOT_DIR/computor"

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[1;33m"
NC="\033[0m"

PASS_COUNT=0
FAIL_COUNT=0
STRICT_MODE=0

if [[ "${1:-}" == "--strict" ]]; then
	STRICT_MODE=1
fi

build_binary() {
	echo "==> Compilation..."
	if ! make -C "$ROOT_DIR" re >/dev/null 2>&1; then
		echo -e "${RED}Compilation KO${NC}"
		exit 1
	fi
	echo -e "${GREEN}Compilation OK${NC}"
}

assert_contains() {
	local haystack="$1"
	local needle="$2"
	if [[ "$haystack" == *"$needle"* ]]; then
		return 0
	fi
	return 1
}

run_test() {
	local name="$1"
	local equation="$2"
	shift 2
	local expectations=("$@")
	local output=""
	local all_ok=1

	output="$("$BIN" "$equation" 2>&1)"

	for expected in "${expectations[@]}"; do
		if ! assert_contains "$output" "$expected"; then
			all_ok=0
			break
		fi
	done

	if [[ $all_ok -eq 1 ]]; then
		echo -e "${GREEN}[OK]${NC} $name"
		PASS_COUNT=$((PASS_COUNT + 1))
	else
		echo -e "${RED}[KO]${NC} $name"
		echo "Equation: $equation"
		echo -e "${YELLOW}Sortie:${NC}"
		echo "$output"
		echo -e "${YELLOW}Attendus manquants:${NC}"
		for expected in "${expectations[@]}"; do
			if ! assert_contains "$output" "$expected"; then
				echo "  - $expected"
			fi
		done
		echo ""
		FAIL_COUNT=$((FAIL_COUNT + 1))
	fi
}

main() {
	build_binary

	echo "==> Lancement des tests..."
	if [[ $STRICT_MODE -eq 1 ]]; then
		echo "Mode: strict (attendus du sujet complet)"
	else
		echo "Mode: stage actuel (verifie parse + reduced form)"
	fi

	# Cas de la section demandee (sujet)
	run_test \
		"Quadratique discriminant positif" \
		"5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0" \
		"Equation:" \
		"Reduced form"

	run_test \
		"Lineaire" \
		"5 * X^0 + 4 * X^1 = 4 * X^0" \
		"Equation:" \
		"Reduced form"

	run_test \
		"Degre > 2" \
		"8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0" \
		"Equation:" \
		"Reduced form"

	run_test \
		"Infinite de solutions" \
		"6 * X^0 = 6 * X^0" \
		"Equation:" \
		"Reduced form"

	run_test \
		"Pas de solution" \
		"10 * X^0 = 15 * X^0" \
		"Equation:" \
		"Reduced form"

	run_test \
		"Quadratique discriminant negatif" \
		"1 * X^0 + 2 * X^1 + 5 * X^2 = 0" \
		"Equation:" \
		"Reduced form"

	# Cas supplementaires utiles
	run_test \
		"Coefficients decimaux simples" \
		"0.5 * X^0 + 1.5 * X^1 = 2 * X^0" \
		"Equation:"

	run_test \
		"Equation deja reduite" \
		"4 * X^2 - 3 * X^1 + 2 * X^0 = 0 * X^0" \
		"Equation:"

	run_test \
		"Termes dans ordre melange" \
		"-3 * X^2 + 7 * X^0 + 1 * X^1 = 2 * X^1 - 1 * X^0" \
		"Equation:"

	if [[ $STRICT_MODE -eq 1 ]]; then
		echo ""
		echo "==> Verifications strictes (sujet)"
		run_test \
			"[strict] Quadratique discriminant positif" \
			"5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0" \
			"Reduced form: 4 * X^0 + 4 * X^1 - 9.3 * X^2 = 0" \
			"Polynomial degree: 2" \
			"Discriminant is strictly positive"
		run_test \
			"[strict] Lineaire" \
			"5 * X^0 + 4 * X^1 = 4 * X^0" \
			"Reduced form: 1 * X^0 + 4 * X^1 = 0" \
			"Polynomial degree: 1" \
			"The solution is:"
		run_test \
			"[strict] Degre > 2" \
			"8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0" \
			"Reduced form: 5 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 0" \
			"Polynomial degree: 3" \
			"The polynomial degree is strictly greater than 2, I can't solve."
		run_test \
			"[strict] Infinite de solutions" \
			"6 * X^0 = 6 * X^0" \
			"Reduced form: 0 * X^0 = 0" \
			"Any real number is a solution."
		run_test \
			"[strict] Pas de solution" \
			"10 * X^0 = 15 * X^0" \
			"Reduced form: -5 * X^0 = 0" \
			"No solution."
		run_test \
			"[strict] Quadratique discriminant negatif" \
			"1 * X^0 + 2 * X^1 + 5 * X^2 = 0" \
			"Reduced form: 1 * X^0 + 2 * X^1 + 5 * X^2 = 0" \
			"Polynomial degree: 2" \
			"Discriminant is strictly negative"
		run_test \
			"[strict] Coefficients decimaux simples" \
			"0.5 * X^0 + 1.5 * X^1 = 2 * X^0" \
			"Reduced form: 1.5 * X^1 = 1.5 * X^0" \
			"Polynomial degree: 1" \
			"The solution is:"
		run_test \
			"[strict] Equation deja reduite" \
			"4 * X^2 - 3 * X^1 + 2 * X^0 = 0 * X^0" \
			"Reduced form: 4 * X^2 - 3 * X^1 + 2 * X^0 = 0" \
			"Polynomial degree: 2" \
			"Discriminant is strictly positive"
		run_test \
			"[strict] Termes dans ordre melange" \
			"-3 * X^2 + 7 * X^0 + 1 * X^1 = 2 * X^1 - 1 * X^0" \
			"Reduced form: -3 * X^2 + 8 * X^0 = 0" \
			"Polynomial degree: 2" \
			"Discriminant is strictly positive"
	fi

	echo "==> Resultats"
	echo -e "Passes : ${GREEN}$PASS_COUNT${NC}"
	echo -e "Echecs : ${RED}$FAIL_COUNT${NC}"

	if [[ $FAIL_COUNT -gt 0 ]]; then
		exit 1
	fi
}

main "$@"
