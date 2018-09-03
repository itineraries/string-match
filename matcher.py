#!/usr/bin/env python3
import collections, ctypes, os.path
libmatcher = ctypes.cdll.LoadLibrary(os.path.join(os.path.dirname(__file__), "libmatcher.so"))
c_score_t = ctypes.c_uint
libmatcher.ComputeScore_CStrings.restype = c_score_t
libmatcher.ComputeScore_WCStrings.restype = c_score_t
libmatcher.Match_ArraysOfWCStrings.restype = ctypes.c_bool

def compute_score_bytes(bytes_a, bytes_b):
    return libmatcher.ComputeScore_CStrings(
        ctypes.c_char_p(bytes_a),
        ctypes.c_char_p(bytes_b)
    )
def compute_score_str(str_a, str_b):
    return libmatcher.ComputeScore_WCStrings(
        ctypes.c_wchar_p(str_a),
        ctypes.c_wchar_p(str_b)
    )
StringMatch = collections.namedtuple("StringMatch", "from_list_a from_list_b score")
def match_list_of_str(list_a, list_b):
    # Create the two output arrays.
    number_of_matches = min(len(list_a), len(list_b))
    results = (ctypes.c_size_t * number_of_matches)()
    results_scores = (ctypes.c_uint * number_of_matches)()
    # Convert the two lists to their respective C types.
    list_a = (ctypes.c_wchar_p * len(list_a))(*list_a)
    list_b = (ctypes.c_wchar_p * len(list_b))(*list_b)
    # Call the C function.
    if libmatcher.Match_ArraysOfWCStrings(
        ctypes.c_size_t(len(list_a)), list_a,
        ctypes.c_size_t(len(list_b)), list_b,
        results,
        results_scores
    ):
        return [
            StringMatch(list_a[index_a], list_b[index_b], score)
            for index_a, (index_b, score)
            in enumerate(zip(results, results_scores))
        ]
    raise ValueError(
        "Each list must have at least 1 item, "
        "and each string must not be empty."
    )
