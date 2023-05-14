# Scripting language

This is a short, informal description of the scripting language in the Trestle engine.

## About

The Trestle language is a high level programming language designed for use in the Trestle engine as the main language for writing games. It mainly takes inspration from Lua but diverges in some ways.

## Core language

### Statements

```php
statement <newline>
```

Statements end with a newline.

### Code blocks

```php
{
    // this is a code block
}
```

### Function

```php
() {
    return 0;
}
```

Functions consist of arguments and a code block.

Returns are followed by a value.

### Expressions

```php
"test1"; // string value
2 + 3; // int value
2.4 * 6.0; // float value
```

Expressions are basically anything that returns a value.

Supported operators:

* `a + b`: Add
* `a - b`: Subtract
* `a * b`: Multiply
* `a / b`: Divide
* `a mod b`: Modulo
* `a ** b`: Exponentation
* `root a b`: Root
* `log a b`: Logarithm
* `-a`: Negate
* `++a`: IncReturn
* `a++`: ReturnInc
* `--b`: DecReturn
* `b--`: ReturnDec
* `a == b`: Equal
* `a != b`: NotEqual
* `a > b`: Greater
* `a < b`: Less
* `a >= b`: GreaterOrEqual
* `a <= b`: LessOrEqual
* `!a`, `not a`: Not
* `a or b`: Or
* `a and b`: And
* `a || b`, `a .. b`: Concatinate
* `a ^ b`: BitExclusiveOr
* `a | b`: BitOr
* `a & b`: BitAnd
* `~a`: BitNot
* `a << b`: BitShiftLeft
* `a >> b`: BitShiftRight
* `a <<< b`: BitRotLeft
* `a >>> b`: BitRotRight

### Variables

```php
test_var = "test1";

(() {
    test_var = "test2";
})()

(() {
    local test_var = "test3";
})()

print(test_var); // "test2"
```

Vars are global by default regardless of their scope, like lua.

### Control flow and conditionals

```php
if (cond) {
    // do something
}

while (cond) {
    // do something
}

for (int i = 0; cond; i++) {
    // do something
}

(cond) ? a : b;
```
