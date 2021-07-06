# World Struct Deprecation

I have decided that perhaps the global world struct - and maybe the use of that form of ECS in Trestle - should be deprecated because it has many flaws and no design benefits over some other more maintainable and still probably cache-friendly architectures.

## The Issues

There are a LOT of issues with just having one big world.

  * **Everything depends on everything else**: Intertwining things and software architecture rarely mix, and when they do, they are probably a bad thing. One of the main issues with having one world struct with entities in them is that they make all of the systems depend on that world; that is, there is so little "encapsulation" (read: lightweight modularity, not protection from mistakes) that it actually does cause problems.
  * **It is harder to free resources properly**: This doesn't really seem like a big issue at first, because it seems like you can just take the entity mask and have a custom freeing function stored private to the world when you go to delete an entit(y|ies). This becomes harder when you consider that enties might have resources registered with things like OpenGL, so then you would need to include OpenGL management in the world header or wherever your entites will be stored. This really becomes a problem when you need to include things for a specific system, but the system also depends on the world, so compilation units get confusing.
  * **There is almost never a need to access other components**: In almost any system, the most you are probably going to access from a component at once is its physics state, graphics state and world graph information. Sure, it's going to be a pain to push three system states onto one update function, but it's not really that bad since everything is independent.
  * **Organisation becomes unclear**: Like anything with a lot of global state that anyone is allowed to access, organisation of where functions and structures should go becomes very unclear. You will probably end up with one big file where you need to keep all the structures that store information about a specific entity, or you will end up with the world becoming very intertwined with the systems once again and loosing any time put into trying to organise things. This just adds a lot of complexity to the architecture that could have been avoided.
  * **Composition is not the most common case**: One benefit of the world struct is that it allows composition at runtime and can be very flexible. Unfortulately, this is rarely useful in a highly game-specific engine because there are really few categories of things - that is, components - that actually make composition useful in the first place. The flexibility is nice for a big, general-purpose engine, but can be unneeded for a smaller, specialised one.

### Additional Points

These points are a bit more philisophical and not related to the main point.

#### Subtle pain in the ass

Over time, as the world structure grows and starts storing heaps of state, the benefits of organising it in that specific way become quite unclear. Why do all the enitites need to be stored next to each other, because isn't the benefit that the entites are sotred in lists, not that the pointers to those lists are stored together? There is a benefit to storing them togehter, no-doubt, but are those benefits worth having systems depend on each other so much? I think not.

#### Organisation is not a theroetical problem

Let me clear this: I am **NOT** concerned about organisation or modularity as some kind of what-if case where the architecture is successful. There will always be things that prevent that being meaningful and the argument of modularity is really BS. I am concerned about actually being able to reason about what a specific system is doing and not having to think about a bunch of other stuff when I am just trying to reason about the physics engine. In other words, I want only the state I need and access to no more because that prevents mistakes and *makes it easier to reason about what I am doing*.

## Solutions and Alternitives

This only lists a few of them.

### Main

  * **Let each system manage its own components**: Each system should be able to manage its own components, and provide interfraces to other systems so that they are able to read its state. This could be in the form of directly reading structs of those systems - here, interfaces doesn't just mean functions, it means any way of reading or writing data.
  * **The world should contain the systems**: This solves the issue where each system is needing to depend on the world, because now only the world depends on the systems: its a straight, clear and simple heiarchy where the systems do not need worlds, but the world needs the systems.

### Coordination

  * **Let the world contain entity info**: If composition is really needed, then the world could contain information that links entities in specific systems to those in the global world. OR...
  * **Use a common naming scheme**: To make sure that entites can be coordinated between independent systems, a common naming scheme could be used between the systems. For example, each system could be told to create an entity by a certian name, instead of the system deciding the name.

### As always...

  * **Still make sure to implement things cache-friendly**: It should still be possible to be quite cache friendly, even if the systems need to store their state differently from each other.

### Other Ideas

  * **Add another layer of indirection**: Perhaps, if global state is really needed that badly, then another layer of inderection could be added between the systems so they can coordinate their state. This could resolve the dependcy issue between systems and worlds while still keeping the global world.
  * **Let each component be owned by a system but accessed by all**: Extending the inderection argument, perhaps it would be a good idea to allow each system to make a structure in the world containing its owned components, but allow these components to be accssed by all. This would fix the main issue of lacking organisation.
  * **Maybe it's my fault?**: Perhaps I am writing this just to realise that I only need to split files up and everything will be okay (so long as cyclic dependncies do not present a problem).

## Examples

### Example 1: Current Setup

Traditional ECS with global world:

```c
struct Cmp1 {
	Vec3 pos;
	Quat rot;
};

struct Cmp2 {
	GLuint vao;
	GLuint vbo, ebo;
};

struct World {
	uint32_t count, *mask;
	
	Cmp1 *cmp1;
	Cmp2 *cmp2;
	
	size_t cmp1_count, cmp2_count;
};

uint32_t create_entity(World *world, uint32_t mask) {
	if ((mask & W_CMP1) == W_CMP1) {
		// add it...
	}
	
	// ...
}

void sys1_update(World *world) {
	// ...
}

int main(void) {
	World *world = world_init();
	
	while (true) {
		sys1_update(world);
		sys2_update(world);
	}
	
	return 0;
}
```

### Example 2: Proposed Solution 1

Our versions of an "ECS" (basically an ECS without the 'C'):

```c
struct Cmp1 {
	Vec3 pos;
	Quat rot;
};

struct Cmp2 {
	GLuint vao;
	GLuint vbo, ebo;
};

struct Sys1 {
	Cmp1 *cmp1;
	size_t cmp1_count;
};

struct Sys2 {
	Cmp2 *cmp2;
	size_t cmp2_count;
};

struct World {
	Sys1 sys1;
	Sys2 sys2;
	
	uint32_t next_name;
};

uint32_t sys1_create_entity(Sys1 *sys1, uint32_t name) {
	// ...
}

uint32_t sys2_create_entity(Sys2 *sys2, uint32_t name) {
	// ...
}

uint32_t create_entity(World *world, uint32_t mask) {
	if ((mask & W_SYS1) == W_SYS1) {
		sys1_create_entity(&world->sys1, world->next_name);
	}
	
	// ...
}

uint32_t sys1_update(Sys2 *sys2) {
	// ...
}

uint32_t sys2_update(Sys2 *sys2, Sys1 *sys1) {
	// ...
}

int main(void) {
	World *world = world_init();
	
	while (true) {
		sys1_update(&world->sys1);
		sys2_update(&world->sys2, &world->sys1);
	}
	
	return 0;
}
```

I would propose to rename the `World` to `Supervisor` and if feeling risky `System` to `Subsystem`, but the terms used here are much more fimilar. If I need to give this pattern a name please call it the "Supervisor-Subsystem" or "Factory" pattern. :)

## Losses

### Composition

The seperation between entites would now be hard-coded into the core of the engine itself, making true composition very hard or innefficent when comapred to doing it with the traditional global world ECS.

The flexibility lost could also make edge cases harder to deal with.

### Convience

There is going to be convinence lost by loosing composition as well as needing to pass in the system state for any systems that a specific system needs. In addition, the concept of components is completely lost in some respect, since each system basically has one component each.
