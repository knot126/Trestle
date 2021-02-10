# Codebase Notes

 * You will see an inconsitsent naming scheme: `DgSomeFunction` vs `some_function`. This is on purpose to seperate the things that could be portable and may conflict with other libraries from the game-specific functions that will almost certianly not conflict and are part of the main game.
   * The `Dg` at the start indicates that this is part of the Decent Games reuseable function collection. In a future release, these should prefreably be organised away from the main game's source code.
 * This engine is written in C, but also avoids some of its advanced features and tries to avoid "too much OOP-likeness" in general.
