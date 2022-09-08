# General Notes

## Documentation

Most functions in both the `util` folder (Melon framework) and everywhere else have comments documenting the functions at the first part of their definition.

## Standardisation

Generally try to keep things within at least some standard. No standard is going to be great, but trying to keep things in a minimally spec standard will help with porting.

As an example, we try to implement our own string functions, like `strtok_r` (the only reasonable version of strtok), which is not part of the C standard.
