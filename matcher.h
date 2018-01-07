#pragma once
#include <stddef.h>
#include <stdbool.h>
typedef unsigned int score_t;
#ifdef __cplusplus
extern "C" {
#endif
score_t ComputeScore_CStrings(const char* a, const char* b);
score_t ComputeScore_WCStrings(const wchar_t* a, const wchar_t* b);
bool Match_ArraysOfWCStrings(
	size_t len_a, const wchar_t** a,
	size_t len_b, const wchar_t** b,
	size_t* results,
	score_t* results_scores
);
#ifdef __cplusplus
}
#endif
