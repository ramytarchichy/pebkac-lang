# PEBKAC-Lang

*Dollar store knockoff of Haskell. Not made in China, surprisingly.*

This is a simple compiler for a VERY basic functional programming language I created for a university assignment. It is not meant to be actually useful, I only spent a few days on this. If you want a real functional programming language, ignore this garbage. I do NOT want this to turn into the next JavaScript. JavaScript was a mistake.

Syntax is heavily inspired by Kotlin/Scala. Generates C++ code.

## Features

- Purely functional programming
- Higher-order functions
- Lambda functions
- Strong typing (No semantic analysis implemented yet, so it relies on the C++ compiler)
- Type inference (Again, relies on C++'s `auto` for now)

## Data Types

- `integer` (long long)
- `boolean`
- `void`
- functions

## Usage

	pebkacc <source> <output_type>

### Options

- `tokens` Outputs tokens in JSON format.
- `ast` Outputs abstract syntax tree in JSON format.
- `cpp` Outputs C++ source code.
