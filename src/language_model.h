#ifndef LANGUAGE_MODEL_H
#define LANGUAGE_MODEL_H

#include <map>
#include <set>
#include <string>

class LanguageModel {
public:
    // read training text from a file (only declaration)
    void trainFromFile(const std::string& filename, int n);

    // accessors for the training text, alphabet, and n-gram length
    // (example usage: const std::string& txt = lm.text(); this will get text_)
    const std::string& text() const { return text_; }
    const std::set<char>& alphabet() const { return alphabet_; }
    // since [set] will automatically remove duplicates
    std::size_t alphabetSize() const { return alphabet_.size(); }
    int n() const { return n_; }

    const std::map<std::string, int>& ngramCounts() const { return ngramCounts_;}
    int totalNgrams() const { return totalNgrams_; }
    double ngramFrequency(const std::string& ngram) const;

private:
    std::string text_;          // training text content
    std::set<char> alphabet_;   // all unique characters that have appeared
    int n_ = 0;                 // n-gram length

    std::map<std::string, int> ngramCounts_; // map of n-grams to their counts
    int totalNgrams_ = 0;              // total number of n-grams
};

#endif