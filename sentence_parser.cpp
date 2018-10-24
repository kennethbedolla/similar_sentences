#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <fstream>
#include <numeric>
#include <sstream>
#include <streambuf>
#include <string>
#include <tuple>
#include <vector>

#include "sentence_parser.h"

using namespace std;

static std::array<std::string, 16> skip_words = {
        "0th",
        "1st",
        "2nd",
        "3rd",
        "4th",
        "5th",
        "6th",
        "7th",
        "8th",
        "9th",
        "dr",
        "eg",
        "etc",
        "mr",
        "mrs",
        "st"
};

const std::string::size_type skip_words_max_length = 3;

sentence_parser::sentence_parser(const std::string &file_location) {
    load_document_(file_location);
}

void sentence_parser::load_document_(const std::string &file_location) {
    ifstream f(file_location);
    if (!f.is_open()) {
        return;
    }

    std::stringstream buffer;
    buffer << f.rdbuf();

    auto raw_data = buffer.str();
    auto leading_meta_sentinel = string("***\r\n");
    auto trailing_meta_sentinel = string("\r\n***");

    auto stripped_header = raw_data.substr(raw_data.find(leading_meta_sentinel) + leading_meta_sentinel.size());
    data = stripped_header.substr(0, stripped_header.find(trailing_meta_sentinel));
}


void sentence_parser::find_paragraph_breaks_(vector <string::size_type> &breaks) const {
    // Find all of the paragraphs. They are separated by a blank line.
    string line_end("\r\n\r\n");
    string::size_type start_pos = 0;
    while (true) {
        auto next_pos = data.find(line_end, start_pos);
        breaks.emplace_back(next_pos);
        if (next_pos == string::npos) {
            return;
        }
        start_pos = next_pos + line_end.size();
    }
}

string word_to_lower(const std::string &word) {
    string lower_word;
    lower_word.reserve(word.size());
    transform(begin(word), end(word), back_inserter(lower_word), [](string::value_type c) {
        return tolower(c);
    });

    return lower_word;
}

bool sentence_parser::is_skip_word_(string::size_type pos) const {
    // If the period is in the first word of the first line of data...
    if (pos <= skip_words_max_length && pos > 0) {
        if (find(begin(skip_words), end(skip_words), data.substr(0, pos - 1)) != end(skip_words)) {
            return true;
        }
    }

    // If the period is at the end of a word followed by a space, and that word is in our skip list...
    auto word_start_pos = data.rfind(' ', pos);
    auto word_length = pos - word_start_pos - 1;
    if (word_start_pos != string::npos && word_length <= skip_words_max_length) {
        if (find(begin(skip_words), end(skip_words), word_to_lower(data.substr(word_start_pos + 1, word_length))) !=
            end(skip_words)) {
            return true;
        }
    }

    // If the period is at the end of a word followed by a newline...
    word_start_pos = data.rfind('\n', pos);
    word_length = pos - word_start_pos - 1;
    if (word_start_pos != string::npos && word_length <= skip_words_max_length) {
        if (find(begin(skip_words), end(skip_words), word_to_lower(data.substr(word_start_pos + 1, word_length))) !=
            end(skip_words)) {
            return true;
        }
    }

    // All other cases mean that it is not a skip word.
    return false;
}

void sentence_parser::find_sentence_breaks_(vector <string::size_type> &breaks) const {
    // Find all question marks and exclamation points. They are always unambiguous.
    string sent_end("?!");
    string::size_type start_pos = 0;
    while (true) {
        auto next_pos = data.find_first_of(sent_end, start_pos);
        breaks.emplace_back(next_pos);
        if (next_pos == string::npos) {
            return;
        }
        start_pos = next_pos + sent_end.size();
    }
}

void sentence_parser::find_period_breaks_(vector <string::size_type> &breaks) const {
    typedef tuple<string::size_type, bool> item_type;

    // Find periods may end a sentence, or not. Use heuristics to determine when
    // the period is actually a sentence end.
    vector <item_type> maybe_breaks;
    string::size_type start_pos = 0;
    while (true) {
        auto next_pos = data.find('.', start_pos);
        if (next_pos == string::npos) {
            break;
        }
        maybe_breaks.emplace_back(next_pos, true);
        start_pos = next_pos + 1;
    }

    // When the period is preceded by a single character which is preceded by whitespace, it cannot be a sentence end
    transform(begin(maybe_breaks), end(maybe_breaks), begin(maybe_breaks), [this](const item_type &item) -> item_type {
        auto pos = std::get<0>(item);
        if (pos > 1 && isspace(data[pos - 2])) {
            return make_tuple(pos, false);
        }
        return make_tuple(pos, true);
    });

    // If the word that the period follows is in our skip words list, it is not a sentence end.
    transform(begin(maybe_breaks), end(maybe_breaks), begin(maybe_breaks), [this](const item_type &item) -> item_type {
        auto pos = get<0>(item);;
        auto kept = get<1>(item);
        // Don't process entries already excluded from earlier steps.
        if (!kept) {
            return make_tuple(pos, false);
        }
        return make_tuple(pos, !is_skip_word_(pos));
    });

    // Remove breaks that won't be kept.
    maybe_breaks.erase(remove_if(begin(maybe_breaks), end(maybe_breaks), [](const item_type &item) -> bool {
        return !std::get<1>(item);
    }), end(maybe_breaks));

    transform(begin(maybe_breaks), end(maybe_breaks), back_inserter(breaks),
              [](const item_type &item) -> string::size_type {
                  return std::get<0>(item);
              });
}

std::vector <std::string> sentence_parser::operator()() const {
    vector <string> sentences;
    if (data.empty()) {
        return sentences;
    }

    vector <string::size_type> breaks;

    find_paragraph_breaks_(breaks);
    find_sentence_breaks_(breaks);
    find_period_breaks_(breaks);

    sort(begin(breaks), end(breaks));
    accumulate(begin(breaks), end(breaks), 0,
               [&sentences, this](string::size_type first, string::size_type last) -> string::size_type {
                   if (first == string::npos) {
                       return first;
                   }

                   if (last != string::npos) {
                       sentences.emplace_back(data.substr(first, (last - first) + 1));
                   } else {
                       sentences.emplace_back(data.substr(first));
                   }
                   return last + 1;
               });



    // Clear out all characters that are undesirable
    transform(begin(sentences), end(sentences), begin(sentences), [](string &s) {
        string out;
        // Turn linefeeds into spaces.
        transform(begin(s), end(s), begin(s), [](string::value_type c) {
            return c == '\n' || c == '-' ? ' ' : c;
        });

        // Only keep characters we want.
        copy_if(begin(s), end(s), back_inserter(out), [](string::value_type c) {
            if (c > 127 || c < 32) {
                return false;
            }

            switch (c) {
                default:
                    return !ispunct(c);
                case '\r':
                case '\t':
                    return false;
            }
        });

        // Strip leading whitespace
        out.erase(0, out.find_first_not_of(" \r\n"));
        // Strip trailing whitespace
        auto new_end = out.find_last_not_of(" \r\n");
        if (new_end != string::npos) {
            out.erase(new_end + 1);
        }

        return out;
    });

    // Drop all empty strings
    sentences.erase(remove_if(begin(sentences), end(sentences), [](const std::string &s) {
        if (s.empty()) { return true; }
        return std::all_of(begin(s), end(s), [](string::value_type c) {
            return isspace(c);
        });
    }), end(sentences));

    return sentences;
}

