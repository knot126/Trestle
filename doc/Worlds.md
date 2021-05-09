# Worlds

This document covers the new worlds design.

## Structures

The idea is fairly straightforward: for each entity, we store (cache) a list of component indices for each entity in the world, so we do not need to waste time finding the proper components and we don't need to allocate all components for all entities.

For example:

```c

struct C_SomeCompo {
	DgVec3 some_value;
};

struct EntityCache {
	int some_compo_count;
};

struct World {
	EntityCache abcedf[COUNT_ENTITIES];
	int mask[COUNT_ENTITIES];
	int entity_count;
	
	C_SomeCompo some_compos[/* count of entites that have this component */];
	int some_compos_count;
};

```

I belive this provides a good balance between using lots of CPU time (by not neededing to find each entity's other components and caching them) and lots of memory use (since we no longer need component base though we need to keep a virtual component base for each even when not used, but its not as bad as keeping the full unused component).

## Notes

 * The benefit of this over the current system (i.e. having a ComponentBase and using that to find the entity) is the reduced CPU time.
 * The benefit over switching to the alloc-for-all is that the memory-heavy components are not wasted, and instead only some light-er weight ints are wasted when an entity does not have a component.
 * Care will need to be taken when adding, removing and modifiying-components for entities in the world.
 * This is still not perfect; obviously, if there were a simple way without wasting memory and still being (mostly) cache effective, it would do that.
