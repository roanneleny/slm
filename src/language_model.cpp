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
}