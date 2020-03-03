# Hethkerp

*Dollar store knockoff of Haskell. Not made in China, surprisingly.*

This is a simple compiler/interpreter for a VERY basic functional programming language I created for a university assignment. It is not meant to be actually useful, I only spent a few days on this. If you want a real functional programming language, ignore this garbage. I do NOT want this to turn into the next JavaScript. JavaScript was a mistake.

## Features

- Purely functional programming
- Higher-order functions
- Lambda functions
- Strong typing
- Type inference
- Function overloading

I know functional programming usually involves tail recursion and other things... I'm not going to bother implementing any of that because I have a deadline to meet, and no one will ever use this thing afterwards anyway.

## Syntax

	//Function that returns the factorial of a number:
	fun factorial(n: integer): integer
	{
		if (n == 0)
		{
			return 1;
		}
		else
		{
			return n*factorial(n-1);
		}
	}



	fun is_even(n: integer): boolean
	{
		return n % 2 == 0;
	}

	fun is_odd(n: integer): boolean
	{
		return !is_even(n);
	}

	//Higher order functions!
	fun validate(n: integer, f: (integer)->boolean): boolean
	{
		return f(n);
	}



	// Immutable values
	let X: integer = 1234;

	// Main function
	io fun main(): void
	{
		let result = factorial(X);
		print(result);

		return;
	}

## Data Types

- `integer` (long long)
- `boolean`
- `void`
- functions

## Usage

	`hethkerp <file name>`
