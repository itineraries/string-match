/*
String Matcher by David Tsai
*/
#include "matcher.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::fill_n;
using std::greater;
using std::pair;
using std::priority_queue;
using std::sort;
using std::string;
using std::vector;
using std::wstring;
typedef pair<size_t, size_t> Location;
struct ScoreAndLocation {
	ScoreAndLocation(
		score_t score,
		size_t location_first,
		size_t location_second
	) : score(score), location(location_first, location_second) {}
	score_t score;
	Location location;
};
bool operator<(const ScoreAndLocation& lhs, const ScoreAndLocation& rhs){
	return lhs.score < rhs.score;
}
bool operator>(const ScoreAndLocation& lhs, const ScoreAndLocation& rhs){
	return lhs.score > rhs.score;
}
/*
This function compares two containers and prints a numerical value
based on how different the containers are. The higher the value,
the more different the containers are. The minimum score is 1.
A return value of 0 means that there was an error.

The two containers can be of different types, but the == operator
must be defined for the items in the first container and the items
in the second container.

Also, both containers must use numeric indices.
*/
template <class T1, class T2>
score_t ComputeScore_Containers(const T1& first, const T2& second){
	if(!(first.size() && second.size())){
		return 0;
	}
	// This is a textbook example of the uniform cost search algorithm. The
	// two strings form the axes of a rectangular grid. The grid becomes a
	// directed graph with three edges from each point on the grid:
	//   1. To the next letters in both words
	//   2. To the next letter in the first word
	//   3. To the next letter in the second word
	priority_queue<
		ScoreAndLocation,
		vector<ScoreAndLocation>,
		greater<ScoreAndLocation>
	> q;
	// In the beginning, nothing has been visited yet.
	vector<vector<bool>> visited(first.size(), vector<bool>(second.size()));
	// Start at the zero offsets in both strings. Initialize the score to 1.
	q.emplace(1, 0, 0);
	// Do Dijkstra.
	do{
		if(!visited[q.top().location.first][q.top().location.second]){
			// Mark this node as visited.
			visited[q.top().location.first][q.top().location.second] = true;
			// Find the edges.
			size_t next_first = q.top().location.first + 1;
			size_t next_second = q.top().location.second + 1;
			bool next_second_valid = next_second < second.size();
			if(next_first < first.size()){
				if(next_second_valid){
					// Follow the edge to the next items in both containers.
					q.emplace(
						q.top().score +
							(first[next_first] == second[next_second] ? 0 : 1),
						next_first,
						next_second
					);
				}
				// Follow the edge to the next item in the first container
				// and the same item in the second container.
				q.emplace(
					q.top().score +
						(
							first[next_first] ==
								second[q.top().location.second]
							? 0 : 1
						),
					next_first,
					q.top().location.second
				);
			}else if(!next_second_valid){
				// This is the last node.
				// We have reached the ends of both containers.
				return q.top().score;
			}
			if(next_second_valid){
				// Follow the edge to the same item in the first container
				// and the next item in the second container.
				q.emplace(
					q.top().score +
						(
							first[q.top().location.first] ==
								second[next_second]
							? 0 : 1
						),
					q.top().location.first,
					next_second
				);
			}
		}
		q.pop();
	}while(!q.empty());
	return 0;
}
/*
This function takes two containers of items. If both containers
have the same number of items, then each item in the first
container is matched to one item in the second container. If one
container has fewer items, then a match is made for every
item in the container with fewer items.

Both containers must use numeric indices.

Every item must be a valid argument to ComputeScore_Containers.

The results are written to results (the argument), which must be
an array with a length equal to the size of the first container.
Every index corresponds to an index in the first container, and
the value at every index corresponds to an index in the second
container. The association represents a match. For example, if
results[3] == 6, that means that firsts[3] matches with
seconds[6]. If the value is seconds.size(), that means that
first.size() > seconds.size() and that no match was found.
The score of each match is stored in results_scores.

If either of the containers is empty, results is null, or
results_scores is null, then the function will immediately
return false. If ComputeScore_Containers fails, this function
will return false. If this function returns true, then there
was no error.
*/
template <class T1, class T2>
bool Match_ContainersOfContainers(
	const T1& firsts, const T2& seconds,
	size_t* results, score_t* results_scores
){
	if(!(firsts.size() && seconds.size() && results && results_scores)){
		return false;
	}
	// Score every item in the first container with every item in the
	// second container.
	vector<ScoreAndLocation> scores;
	scores.reserve(firsts.size() * seconds.size());
	for(size_t i = 0; i < firsts.size(); ++i){
		for(size_t j = 0; j < seconds.size(); ++j){
			score_t score = ComputeScore_Containers(firsts[i], seconds[j]);
			if(score == 0){
				return false;
			}
			scores.emplace_back(score, i, j);
		}
	}
	// Sort the scores from lowest to highest. We are not using a
	// selection algorithm because we don't actually know how many
	// scores we will need. Although we will be returning k results,
	// where k is the number of items in the container with fewer
	// items, each item cannot be matched with more than one item.
	// Well, actually, we could probably do something like only
	// sorting the lower partition in each step and only sorting
	// higher partitions when needed. I wonder whether there is a
	// way to sort only a part of a heap.
	sort(scores.begin(), scores.end());
	// Each item can only be matched with one item in the other
	// container. Keep track of which items have been matched.
	vector<bool> matched_firsts(firsts.size());
	vector<bool> matched_seconds(seconds.size());
	// This also means that if one container has more items than the
	// other, not every item in it will be matched. There will only
	// be as many matches as there are items in the container with
	// fewer items.
	size_t number_of_matches;
	if(firsts.size() > seconds.size()){
		number_of_matches = seconds.size();
		// Some items in the first container won't be matched to
		// items in the second container. In results, each should
		// be set to seconds.size() to indicate that there was no
		// match. We'll just set all of them to non-matches now.
		fill_n(results, number_of_matches, seconds.size());
	}else{
		number_of_matches = firsts.size();
	}
	// Now, find matches so that the scores are the lowest and that
	// no item matches with more than one item.
	for(const ScoreAndLocation& s : scores){
		if(
			!matched_firsts[s.location.first] &&
			!matched_seconds[s.location.second]
		){
			matched_firsts[s.location.first] = true;
			matched_seconds[s.location.second] = true;
			results[s.location.first] = s.location.second;
			results_scores[s.location.first] = s.score;
			// Stop after the expected number of matches have been made.
			if(number_of_matches == 0){
				break;
			}
			--number_of_matches;
		}
	}
	return true;
}
// Driver functions for the templates
score_t ComputeScore_CStrings(const char* a, const char* b){
	string first(a);
	string second(b);
	return ComputeScore_Containers(first, second);
}
score_t ComputeScore_WCStrings(const wchar_t* a, const wchar_t* b){
	wstring first(a);
	wstring second(b);
	return ComputeScore_Containers(first, second);
}
bool Match_ArraysOfWCStrings(
	size_t len_a, const wchar_t** a,
	size_t len_b, const wchar_t** b,
	size_t* results,
	score_t* results_scores
){
	vector<wstring> firsts(a, a + len_a);
	vector<wstring> seconds(b, b + len_b);
	return Match_ContainersOfContainers(
		firsts,
		seconds,
		results,
		results_scores
	);
}
