# slm - Small Language Model

A C++ command-line tool that trains an n-gram language model on a text file
and generates new text by sampling characters according to the learned
conditional probabilities.

The program is intentionally simple: it learns which characters tend to follow
each n-gram in the training text, and then generates new text by repeatedly
sampling the next character.

## Build

Requirements:

- A C++17 compiler (e.g. `g++`)
- `make`

Compile from the project root:

```bash
make
```

This produces the executable `slm` in the project root.

To remove all generated files:

```bash
make clean
```

## Usage

```text
slm [-a alpha] n file length
```

Arguments:

- `n` — positive integer, the n-gram length.
- `file` — path to a plain text file used for training.
- `length` — positive integer, the number of characters to output,
  including the starting n-gram. Must be at least `n`.
- `-a alpha` — optional pseudocount, a non-negative number.
  If omitted, `alpha = 0`.

Examples:

```bash
# Bigram model, generate 5 characters
./slm 2 moby_dick.txt 5

# Trigram model with pseudocount 0.01, generate 40 characters
./slm -a 0.01 3 moby_dick.txt 40
```

Output is printed to standard output. If the generator gets stuck
(when `alpha = 0` and the current n-gram has no observed next character),
it stops early and the output may be shorter than `length`.

## How it works

1. Read the training text and collect the alphabet (all distinct characters).
2. Count how often each n-gram occurs.
3. For each n-gram, count how often each character follows it.
4. Estimate the conditional probability

   ```text
   P(c | w) = (count(w, c) + alpha) / (total(w) + alpha * |A|)
   ```

   where `w` is an n-gram, `c` is a character, `|A|` is the alphabet size,
   and `alpha` is the pseudocount.

5. Generate text:

   - Choose a starting n-gram according to its frequency.
   - Output it.
   - Repeatedly sample the next character from `P(c | w)`.
   - Slide the window by one character.
   - Stop when the requested length is reached, or when the generator
     gets stuck and `alpha = 0`.

## Error handling

User errors are detected and reported using C++ exceptions.
Errors are printed to standard error with a `slm:` prefix, and the program
exits with a non-zero status.

Examples of detected errors:

- wrong number of arguments
- `n` or `length` not a positive integer
- `length` smaller than `n`
- `-a` not followed by a non-negative number
- unknown option
- training file cannot be opened
- training text too short to contain any n-gram followed by a character

Example:

```text
$ ./slm 2 no_such_file.txt 100
slm: cannot open file 'no_such_file.txt'
```

## Project structure

```text
.
├── Makefile
├── README.md
├── .gitignore
└── src
    ├── main.cpp
    ├── language_model.h
    ├── language_model.cpp
    ├── text_generator.h
    └── text_generator.cpp
```

- `LanguageModel` reads the training file, builds the alphabet, counts
  n-grams and next characters, and computes conditional probabilities.
- `TextGenerator` chooses a starting n-gram and samples new characters.
- `main.cpp` parses command-line arguments, calls the model and generator,
  and handles exceptions.

## Version control

The project is managed with git. Generated files such as object files,
dependency files and the executable are ignored via `.gitignore`.