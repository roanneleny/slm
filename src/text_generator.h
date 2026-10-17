#ifndef TEXT_GENERATOR_H
#define TEXT_GENERATOR_H

#include <random>
#include <string>

#include "language_model.h"

class TextGenerator {
public:
    TextGenerator(const LanguageModel& model, double alpha);

    // 生成指定长度的文本，包括起始 n-gram。
    // 如果生成器卡住，返回值可能短于 length。
    std::string generate(int length);

private:
    const LanguageModel& model_;
    double alpha_;
    std::mt19937 rng_;

    std::string chooseStartingNgram();
    char sampleNextChar(const std::string& ngram);
};

#endif