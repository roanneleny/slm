#include "text_generator.h"

#include <stdexcept>
#include <vector>

TextGenerator::TextGenerator(const LanguageModel& model, double alpha)
    : model_(model), alpha_(alpha), rng_(std::random_device{}()) {
}

std::string TextGenerator::chooseStartingNgram() {
    std::vector<std::string> ngrams;
    std::vector<double> weights;

    for (const auto& pair : model_.ngramCounts()) {
        ngrams.push_back(pair.first);
        weights.push_back(static_cast<double>(pair.second));
    }

    if (ngrams.empty()) {
        throw std::runtime_error("no n-grams available to start generation");
    }

    std::discrete_distribution<std::size_t> dist(weights.begin(), weights.end());
    return ngrams[dist(rng_)];
}

char TextGenerator::sampleNextChar(const std::string& ngram) {
    std::vector<char> chars;
    std::vector<double> probs;

    for (char c : model_.alphabet()) {
        double p = model_.nextCharProbability(ngram, c, alpha_);
        if (p > 0.0) {
            chars.push_back(c);
            probs.push_back(p);
        }
    }

    if (chars.empty()) {
        throw std::runtime_error(
            "cannot sample next character: no positive-probability character");
    }

    std::discrete_distribution<std::size_t> dist(probs.begin(), probs.end());
    return chars[dist(rng_)];
}

std::string TextGenerator::generate(int length) {
    if (length < model_.n()) {
        throw std::invalid_argument("length must be at least n");
    }

    std::string output = chooseStartingNgram();
    std::string current = output;

    while (static_cast<int>(output.size()) < length) {
        // alpha = 0 且当前 n-gram 没有后继：停止
        if (alpha_ == 0.0 && !model_.hasNextDistribution(current)) {
            break;
        }

        char next = sampleNextChar(current);
        output += next;

        // 更新当前 n-gram：去掉第一个字符，追加 next
        current.erase(0, 1);
        current += next;
    }

    return output;
}