# Strings

This document talks about strings in the engine.

## Abstractions???

**NO!** All strings in the engine should just be `(char *)` or `(const char *)` and be null terminated. The encoding we tipically use (either ASCII or UTF-8) still allows normal C strings with null termination to work somewhat correctly (AFAIK). 

## Encoding

Try to at least have some compatibility with UTF-8 and Unicode. Do not force me to use UTF-16; that is a waste of space since 99% of the strings are in latin languages (though not nessicarially english). 

And if you even sugguest UTF-32...

Admittedly, I have not really tried too hard to support UTF-8 compatible strings. I will try to get better at that, but string encoding isn't really a priority right now, at least not on the developer end of things where most text will be in one language.
