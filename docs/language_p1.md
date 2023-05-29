# Trestle Language

**Trestle Language** or **Trestle Script** is the scripting and configuration language in the Trestle game engine. They generally have the `.tr` (for general scripting) or `.res` (for procedureally generated resources, objects and stratigies) file extensions.

## Functions

In Trestle, one of the most used parts of the language is the function &mdash; they are used (and abused) very often thoughout the design of the engine.

A function defition looks like this:

```lua
[<type> | function] [<name>?](arg1, arg2, ..., argN)
    ...
end
```

Here is a full example that uses functions to make a small toy menu!

```lua
function load_save()
    return reg.set("player.savedata", io.read_file("crd://"))
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
    h.pos(vector3 {5.0 * randfloat(), -2.0 * randfloat(), 3.0 * randfloat()})
    h.rot(0.125, 0.125, 0.0)
    
    b = h.box()
    b.apply_texture(t, "basic") -- apply to each of the six sides of the box
    
    return h
end

font kumbh_sans()
    return fonts.load_ttf("assets://fonts/KumbhSans-variable.ttf")
end

function init()
    i = load_image("assets://images/menu.res", "play_button") -- name is optional
    s = load_shape(funny_box) -- can also just pass in a local name
    
    bg = lay.layer(3, "within") -- 3d layer, mix within other layers
    ui = lay.layer(2, "ontop") -- 2d layer, mix ontop of other layers
    
    bg.place(s)
    ui.place(i)
end

function tick()
end

function free()
end
```
