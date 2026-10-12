# slm - Small Language Model

A C++ command-line tool that trains an n-gram language model on a text file and generates new text.

## Build

```bash
make
```

## Usage

```bash
slm [-a alpha] n file length
```

- `n`: n-gram length, a positive integer.
- `file`: training text file.
- `length`: number of characters to output, must be at least `n`.
- `-a alpha`: optional pseudocount, default 0.
- 这一段注释可以考虑删掉？

