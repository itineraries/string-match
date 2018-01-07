#!/usr/bin/env python3
import matcher
import tabulate, textwrap

wrapper_matches_string = textwrap.TextWrapper(width=32)
wrapper_matches_score = textwrap.TextWrapper(width=6)
def print_matches(list_a, list_b):
    print(
        tabulate.tabulate(
            (
                (
                    wrapper_matches_string.fill(match.from_list_a),
                    wrapper_matches_string.fill(match.from_list_b),
                    wrapper_matches_score.fill(str(match.score))
                ) for match in matcher.match_list_of_str(list_a, list_b)
            ),
            headers=("String from List A", "String from List B", "Score"),
            tablefmt="fancy_grid"
        )
    )
def main():
    print("Testing compute_score_bytes...")
    print(matcher.compute_score_bytes(b"Jupiter", b"Jupiter"))
    print("Testing compute_score_str...")
    print(matcher.compute_score_str("Jupiter", "Jupiter"))
    print("Testing match_list_of_str...")
    print_matches(
        [
            "Jupiter",
            "14th Street",
            "My name is David."
        ],
        [
            "13th Street",
            "My name is not David.",
            "Saturn"
        ]
    )
    print("Tests complete.")

if __name__ == "__main__":
    main()
