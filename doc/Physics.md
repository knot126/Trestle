# Physics System

This document covers some good things to know about the physics system in Trestle.

## Components

Like other subsystems, the physics subsystem stores some *components* that can be accessed using the physics system's public API.

These components are basically the same as the ones that would be used in an ECS with one big world. Instead of storing them in one place, though, we sepreate them into each of their respective systems. The system now owns the components, not the world.

Using compoents in this way allows for some extra modularity and clarification of the code (since you don't need to consider unnessicary components) while still keeping the cache friendly properties (at least on a system level).

## Objects

First is the idea of a physics object. You probably already know what that is, but in case you do not, it's basically any object with a physics representation in the scene.

Take an example: There are a few trees and ball in a nice outside setting. The tress and ball would each be their own physics object.

*Anyways...* 

The main physics object structure (and its assocaited list of physics objects) stores the most crucial aspects of an object that all physics objects are likely to use. For example, it stores information related to the velocity of an object and its current acceleration - something that all physics objects will need to make use of at some point.

Here is how it looks at the time of writing:

```c
typedef struct PhysicsObject {
	DgVec3 lastPos;
	DgVec3 directForce;     // Applied via moving the object
	DgVec3 accel;           // Applied via proper integration
	uint64_t flags;
} PhysicsObject;
```

As you can see, there are no other properties than what the physics object aboslutely needs to store in order to even be considered a moving, non-static physics object.

*NOTE: Well, okay. Since not all physics objects will need to move, it's not strictly nessicary to have a physics object and movement data data in - for example - a statically positioned AABB. The engine does allow this: a shape does not have to have a physics object to go alongside it. I have only really said that a physics object contains data that is needed because usually you will want a physics object structure assocaited with anything in the physics system, even if the thing is not going to move or make good use of that data.*

### API for `PhysicsObject`

#### `Name physics_create_object(PhysicsSystem *this, Name name)`

Creates a physics object by the given name. Returns the name on success, `0` on failure.

#### `Name physics_clear_object(PhysicsSystem *this, Name name)`

Set all of an object's properties to zero. Zero is defined according to the table below:

|Type        |Value                   |
|------------|------------------------|
|`int64_t`   | `0`                    |
|`DgVec3`    | `{0.0f, 0.0f, 0.0f}`   |

#### `Name physics_set_accel(PhysicsSystem *this, Name name, DgVec3 accel)`

Set acceleration directly, overriding any previous sets of acceleration. Usually, you should use `physics_add_forces` instead.

#### `Name physics_set_flags(PhysicsSystem *this, Name name, uint64_t flags)`

Sets the flags parameter for a given physics object with the name. At time of writing, the following flags are valid for public API use:

  * `PHYSICS_STATIC`: Do not move the object, but allow collisions to occur with it.
  * `PHYSICS_NO_GRAVITY`: Do not use gravity on this object.
  * `PHYSICS_DRAG`: Enable some drag with the object. **(not implemented properly)**
  * `PHYSICS_MODE_PLAYER`: Allows the use of moving the object directly, bypassing the use of forces and a proper physics system. Try to avoid using this.

#### `Name physics_add_forces(PhysicsSystem *this, Name name, DgVec3 force)`

Add a force to an object. The force is expected to be in m/s^2 (metres per second per second).

#### `Name physics_move_object(PhysicsSystem *this, Name name, DgVec3 force)`

Move a physics object in space by some amount. Requires that the `PHYSICS_MODE_PLAYER` flag be set on the object.

## AABBs

AABBs are just another component. They will gain their position from their assocaited SceneGraph transform, so the physics object only stores the size of the AABB. Thier collision will be checked automatically, and they will respond to collision so long as they have an assocaited physics object and are not marked `QR_PHYSICS_STATIC` in their physics object flags.

*NOTE: The expected behaviour for a box without a physics object has not yet been implemented or at least tested to work; doing this right now may lead to unexpected behaviour or even a segfault.*

### API for `AABBShape`

Hopefully, these are self explainitory.

#### `Name physics_create_aabb(PhysicsSystem *this, Name name)`
#### `Name physics_set_aabb(PhysicsSystem *this, Name name, DgVec3 size)`

## PhysicsSystem

The PhysicsSystem structure represents the physics system's state as a whole: it contains all the components as well as some global state.

At time of writing, here is what it looks like:

```c
typedef struct PhysicsSystem {
	/// Physics Objects ///
	Name          *object_name;
	PhysicsObject *object;
	size_t         object_count;
	size_t         object_alloc;
	
	/// Axis Aligned Bounding Boxes ///
	Name      *aabb_name;
	AABBShape *aabb;
	size_t     aabb_count;
	size_t     aabb_alloc;
	
	/// Spheres ///
	Name        *sphere_name;
	SphereShape *sphere;
	size_t       sphere_count;
	size_t       sphere_alloc;
	
	/// Globals ///
	DgVec3 gravity;
	float delta_time; // NOTE: unused
	bool enabled;
} PhysicsSystem;
```

### API for `PhysicsSystem`

#### `void physics_enabled(PhysicsSystem *this, bool mode)`

Set weather the physics system will try to do any work or weather it will simpily return without doing anything. This is useful for when you want to pause the game, or swap between a menu and the game.

#### `physics_sync_graph(PhysicsSystem *this, SceneGraph *graph)`

Sync all of the object's last positions according to the graph.
