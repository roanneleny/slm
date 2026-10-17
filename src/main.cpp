#include <iostream>
#include <stdexcept>
#include <string>

#include "language_model.h"
#include "text_generator.h"

using namespace std;

// command line arguments structure
struct Arguments {
    double alpha = 0.0;
    int n = 0;            // n-gram length
    string filename;
    int length = 0;       // output length
};

// parse command line arguments and return an Arguments struct;
// throws invalid_argument on error
Arguments parseArguments(int argc, char* argv[]) {
    Arguments args;       // create an Arguments struct with default values
                          // and call it args  (like an empty box)
    int i = 1;

    // 1. check if optional -a argument exists
    if (i < argc && string(argv[i]) == "-a") {
        ++i;
        if (i >= argc) {
            // if no value is provided after -a，raise an error
            throw invalid_argument("option -a requires a value"); 
        }

        try {
            size_t pos = 0;
            args.alpha = stod(argv[i], &pos);
            if (pos != string(argv[i]).size()) {
                throw invalid_argument("");
            }
        } catch (const exception&) {
            throw invalid_argument("option -a requires a non-negative number");
        }
        if (args.alpha < 0.0) {
            throw invalid_argument("option -a requires a non-negative number");
        }
        ++i;
    }

    // 2. if the first positional argument starts with '-', it's an unknown option
    if (i < argc && argv[i][0] == '-') {
        throw invalid_argument("unknown option: " + string(argv[i]));
    }

    // 3. the remaining arguments must be three positional parameters
    if (argc - i != 3) {
        throw invalid_argument("usage: slm [-a alpha] n file length");
    }

    // 4. parse n
    try {
        size_t pos = 0;
        args.n = stoi(argv[i], &pos);
        if (pos != string(argv[i]).size()) {
            throw invalid_argument("");
        }
    } catch (const exception&) {
        throw invalid_argument("n must be a positive integer");
    }
    if (args.n <= 0) {
        throw invalid_argument("n must be a positive integer");
    }
    ++i;

    // 5. filename
    args.filename = argv[i];
    ++i;

    // 6. parse length
    try {
        size_t pos = 0;
        args.length = stoi(argv[i], &pos);
        if (pos != string(argv[i]).size()) {
            throw invalid_argument("");
        }
    } catch (const exception&) {
        throw invalid_argument("length must be a positive integer");
    }
    if (args.length <= 0) {
        throw invalid_argument("length must be a positive integer");
    }
    if (args.length < args.n) {
        throw invalid_argument("length must be at least n");
    }

    return args;
}

int main(int argc, char* argv[]) {
    try {
        Arguments args = parseArguments(argc, argv);

        LanguageModel model;
        model.trainFromFile(args.filename, args.n);

        TextGenerator generator(model, args.alpha);
        string result = generator.generate(args.length);

        cout << result << "\n";

        /*单纯代码调试
        cout << "text length = " << model.text().size() << "\n";
        cout << "alphabet size = " << model.alphabetSize() << "\n";
        cout << "total n-grams = " << model.totalNgrams() << "\n";
        cout << "\n";

        // print n-gram frequencies
        for (const auto& pair : model.ngramCounts()) {
            const string& ngram = pair.first;
            //为什么这里不用指针啊！

            cout << "ngram";

            for (char c : ngram){
                if (c == ' ')       cout << "_"; 
                else if (c == '\n') cout << "\\n";
                else if (c == '\t') cout << "\\t";
                else                cout << c;
            }
            cout << ":\n";

            //????????
            bool hasDist = model.hasNextDistribution(ngram) || args.alpha > 0.0;
            if (!hasDist) {
                cout << "  (no next character distribution)\n";
                continue;
            }


            for (char c : model.alphabet()) {
                double p = model.nextCharProbability(ngram, c, args.alpha);

                if (p > 0.0) {
                    cout << " ->";
                    if (c == ' ')       cout << "_";
                    else if (c == '\n') cout << "\\n";
                    else if (c == '\t') cout << "\\t";
                    else                cout << c;
                    cout << ": " << p << "\n";
                }
            }
        
        }
        */

    } catch (const exception& e) {
        cerr << "slm: " << e.what() << "\n";
        return 1;
    }
    return 0;
}