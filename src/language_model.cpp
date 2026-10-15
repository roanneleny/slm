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

}

double LanguageModel::ngramFrequency(const std::string& ngram) const {
    auto it = ngramCounts_.find(ngram); //if ngram is not found it will become ngramCounts_.end()
    if (it == ngramCounts_.end()) {
        return 0.0;
    }
    return static_cast<double>(it->second) / totalNgrams_;
}