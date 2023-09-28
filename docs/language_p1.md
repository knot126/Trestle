# Trestle Language

**Trestle Language** or **Trestle Script** is the scripting and configuration language in the Trestle game engine. It is very close in design to Lua, and doesn't deviate very much from it.

Trestle language files generally have the `.tr` (for general scripting) or `.res` (for procedureally generated resources, objects and stratigies) file extensions.

## Introduction: Hello, world!

Here is a small hello world application:

```lua
function init()
	log("Hello, world!")
	exit()
end
```

Put this inside a folder and name the file `main.ts`. After that, run `trestle --assets=<folder-path>`, making sure to repalce `<folder-path>` with your real project folder path.

It should print `Hello, world!` to the screen, then exit.

There are only a few things to note:

* We generally use `init` as the main function name. As the name suggests, this should take care of any initialisation in the script, but for a hello world it doesn't matter much.
* In Trestle, if you want to exit, you must *explicity* say so becuase Trestle is a game engine and is designed to run continously.
* The Weird Trick where you can write `log "Hello, world!"` (no brackets) does not work in Trestle Script as brackets are required for function calls.

## Functions

In Trestle, one of the most used parts of the language is the function &mdash; they are used (and abused) very often thoughout the design of the engine.

A function is formally defined as this:

```lua
[<type> | function] [<name>?](arg1, arg2, ..., argN)
    ...
end
```

Essentially, this means:

* A function starts with either a type name or the `function` keyword. (note: you cannot make your own types in trestle script)
* It is optionally followed by a function name (not having a name would make it a closure)
* Functions have a pretty normal arguments list, starting with an opening bracket, having a comma seperated list of argument names and ending with a closing bracket.
* Then there is the instructions in the function.
* Finally it finishes with an `end` keyword.

Example of a basic function:

```lua
function hello()
	log("UwU")
end
```

Example of a special function which uses typenames and is defined implicity as the main image for the file it is in:

```lua
image ()
	img = image(vector2 {786, 512})
	img.fill(vector3 {0.0, 0.0, 1.0})
	img.text(size = 50, pos = vector2 {30, 30}, colour = vector3 {1.0, 1.0, 0.0}, text = "Hewwo! :3")
	return img
end
```

## Big example

Here is a full example that uses various parts of trestle to make a small toy menu!

```lua
function load_save()
    return reg.set("player.savedata", table_from_json(io.read_file("crd://")))
end

-- default image for this script
image (bg, fg, r)
    img = image("default 1:1 medium") -- default 1:1 medium image size
    img.fill(bg)
    img.circle("centre", r, fg) -- make the cricle centred with radius r
                                -- and colour fg
    
    return img
end

-- named shape for this script
-- tip: trestle will know to cache if two objects share the same
--      parameters, so we only use pseudorandom values
shape funny_box(s)
    rand.seed(s)
    
    t = load_image(__self__)
    
    h = shape()
    h.pos(vector3 {5.0 * rand.float(), -2.0 * rand.float(), 3.0 * rand.float()})
    h.rot(vector3 {0.125, 0.125, 0.0})
    
    b = h.box()
    b.apply_texture(t, "basic") -- apply to each of the six sides of the box
    
    return h
end

font kumbh_sans()
    return formats.fonts.load_ttf("assets://fonts/KumbhSans-variable.ttf")
end

function init()
    play_button = load_image("assets://images/menu.res", "play_button") -- name is optional
    box1 = load_shape(funny_box) -- can also just pass in a local function
    
    bg = lay.layer(3, "within") -- 3d layer, mix within other layers
    ui = lay.layer(2, "ontop") -- 2d layer, mix ontop of other layers
    
    bg.place(box)
    ui.place(play_button)
end

function tick()
end

function free()
end
```
