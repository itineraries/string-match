#include <stdio.h>
#include <wchar.h>
#include "matcher.h"
#define NUM_STRINGS 3

int main(){
	wprintf(
		L"Testing ComputeScore_CStrings...\n%d\n",
		ComputeScore_CStrings("Jupiter", "Jupiter")
	);
	wprintf(
		L"Testing ComputeScore_WCStrings...\n%d\n",
		ComputeScore_WCStrings(L"Jupiter", L"Jupiter")
	);
	wprintf(L"Testing Match_ArraysOfWCStrings...\n");
	const wchar_t* a[] = {
		L"Jupiter",
		L"14th Street",
		L"My name is David."
	};
	const wchar_t* b[] = {
		L"13th Street",
		L"My name is not David.",
		L"Saturn"
	};
	size_t results[NUM_STRINGS];
	score_t results_scores[NUM_STRINGS];
	Match_ArraysOfWCStrings(
		NUM_STRINGS, a,
		NUM_STRINGS, b,
		results,
		results_scores
	);
	wprintf(L"+-----------------------+-----------------------+-------+\n");
	wprintf(L"| String from List A    | String from List B    | Score |\n");
	wprintf(L"+-----------------------+-----------------------+-------+\n");
	{
		size_t i;
		for(i = 0; i < NUM_STRINGS; ++i){
			wprintf(
				L"| %-21ls | %-21ls | %5d |\n",
				a[i],
				b[results[i]],
				results_scores[i]
			);
		}
	}
	wprintf(L"+-----------------------+-----------------------+-------+\n");
	return 0;
}
