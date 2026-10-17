#include "language_model.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

void LanguageModel::trainFromFile(const std::string& filename, int n) {
    n_ = n;

    // 1. Open the file
    std::ifstream file(filename, std::ios::binary);// open the file in binary mode
    if (!file) {
        throw std::runtime_error("cannot open file '" + filename + "'");
    }

    // 2. Read the file contents into text_
    std::ostringstream ss;
    ss << file.rdbuf();
    text_ = ss.str();   // store the file contents into a string


    // 3.check if the content is long enough?
    if (text_.empty()){
        throw std::runtime_error("training text is empty");
    }

    if (static_cast<int>(text_.size()) < n + 1){
        throw std::runtime_error(
            "training text is too short to contain any n-gram followed by a character"
        );
    }

    // 4. construct alphabet
    alphabet_.clear();   //clear is a member function of std::set that removes all elements

    for(char c : text_){ 
        alphabet_.insert(c);
    }

    // 5. construct n-gram frequency map
    ngramCounts_.clear();
    totalNgrams_ = static_cast<int>(text_.size()) - n_ + 1;

    for (int i = 0; i < totalNgrams_; ++i) {
        std::string ngram = text_.substr(i, n_);
        ngramCounts_[ngram] += 1; 
        //perhaps ngram was not in nCounts, but it will add automatically with default value 0.

    }

    // 6. count the following character for each n-gram
    nextCharCounts_.clear();
    totalNextCounts_.clear();

    for (int i = 0; i < totalNgrams_; ++i) {
        std::string ngram = text_.substr(i, n_); //new cycle need to be recalculated for each position

        // since the position of the next character is i + n_
        int nextPos = i + n_;
        if (nextPos < static_cast<int>(text_.size())) {
            char nextChar = text_[nextPos]; 
            nextCharCounts_[ngram][nextChar] += 1;
            totalNextCounts_[ngram] += 1;
        }
        // if nextPos is not less than text_.size(), 
        // it means there is no following character for this n-gram
    }

}

double LanguageModel::ngramFrequency(const std::string& ngram) const {
    auto it = ngramCounts_.find(ngram); //if ngram is not found it will become ngramCounts_.end()
    if (it == ngramCounts_.end()) {
        return 0.0;
    }
    return static_cast<double>(it->second) / totalNgrams_;
}


int LanguageModel::totalNextCount(const std::string& ngram) const {
    auto it = totalNextCounts_.find(ngram);
    if (it == totalNextCounts_.end()) {
        return 0;
    }
    return it->second;
}


bool LanguageModel::hasNextDistribution(const std::string& ngram) const {
    return totalNextCount(ngram) > 0;//totalNextCount(ngram) > 0 could be true or false.
}


double LanguageModel::nextCharProbability(const std::string& ngram,
                                          char c,
                                          double alpha) const {
    int total = totalNextCount(ngram);
    int count = 0;

    auto it = nextCharCounts_.find(ngram);
    if (it != nextCharCounts_.end()) {
        auto charIt = it->second.find(c); //find the matched pair of c(c = a or b) of {ab:{a:2, b:1}}
        if (charIt != it->second.end()) { // if c is found in the nextCharCounts_
            count = charIt->second;
        }
    }

    double alphabetSize = static_cast<double>(alphabet_.size());

    if (alpha == 0.0) {
        if (total == 0) {
            // No following character for this n-gram, return 0 (caller should check hasNextDistribution)
            return 0.0;
        }
        return static_cast<double>(count) / total;
    }

    // pseudocount
    return (count + alpha) / (total + alpha * alphabetSize);
}
