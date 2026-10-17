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


    // count the following n-gram??? in the training text???????
    const std::map<std::string, std::map<char, int>>& nextCharCounts() const{
        return nextCharCounts_;
    }
    int totalNextCount(const std::string& ngram) const;//这都不设置函数内容吗？纯声明???

    // conditional probability of the next character given the n-gram
    double nextCharProbability(const std::string& ngram, char c, double alpha) const;

    // check if there is a next character distribution for the given n-gram
    bool hasNextDistribution(const std::string& ngram) const;//何意味



private:
    std::string text_;          // training text content
    std::set<char> alphabet_;   // all unique characters that have appeared
    int n_ = 0;                 // n-gram length

    std::map<std::string, int> ngramCounts_; // map of n-grams to their counts
    int totalNgrams_ = 0;              // total number of n-grams

    std::map<std::string, std::map<char, int>> nextCharCounts_;
    // map of n-grams to their following character counts
    std::map<std::string, int> totalNextCounts_;
};

#endif