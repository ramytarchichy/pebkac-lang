# PEBKAC-Lang

*Dollar store knockoff of Haskell. Not made in China, surprisingly.*

This is a simple compiler/interpreter for a VERY basic functional programming language I created for a university assignment. It is not meant to be actually useful, I only spent a few days on this. If you want a real functional programming language, ignore this garbage. I do NOT want this to turn into the next JavaScript. JavaScript was a mistake.

## Features

- Purely functional programming
- Higher-order functions
- Lambda functions
- Strong typing

I know functional programming usually involves tail recursion optimization and other things... I'm not going to bother implementing any of that because I have a deadline to meet, and no one will ever use this thing afterwards anyway (I hope).

## Data Types

- `integer` (long long)
- `boolean`
- `void`
- functions

## Usage

	pebkacc <source> <output> [options]

### Options

	- `--tokens` Outputs tokens in JSON format.
	- `--ast` Outputs abstract syntax tree in JSON format.