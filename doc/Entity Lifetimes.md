# Entity Lifetimes

Let us say we want to keep track of what entities we could have - or rather, do have - on some levels in the game.

## Refrence Counting

One way we could do this is to keep track of what entities belong to what levels, and store that with each entity. At every frame or so, we could then check this and see if the entity still needs to be loaded. Essentially, we are doing a sort of refrence counting, *but not quite*.

There are some advantages and disadvantages to this:

 * It allows everything to be controlled independently of one another, so we can free and allocate as 
needed. 
 * We can't free the actual memory used by them, since the active entity names are derived from their index. Moving the entities downstream would cause the wrong IDs to be used.
   * *NOTE*: Maybe this could be fixed easily?
   * *NOTE*: Actually, this would only affect the masks of the entities. The actual entity storage is already using something like this that would allow it to be done easily.
     * One thing to try is to swap the entity masks for index lists, like I once tried to do but stopped doing it because it took more memory and made little performance difference.

## Offsets

I tried coming up with a method that used offsets for the masks to solve the problem with the first method and was simple, but I think you have to store a list of offsets and some other data - something I don't want to do.

## Trim-only (Grouping Stack)

Another possibility would be to store the index of the first element that was loaded with the current level. Then, when it comes time to load in the second level, we unload all of those things by trimming out the allocated memory (no complex moving memory) and load the next level.

This might work fairly well if it weren't for the fact that we usually want to load the next level while the current level is still loaded in the game.
