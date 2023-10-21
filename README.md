# cham
A cli image editing tool producing GIFs with color reduction and dithering.

## Build
```
make MODE=release
```
Builds the release version of the binary, depends on the included [stb headers](https://github.com/nothings/stb)
and [gifenc](https://github.com/lecram/gifenc).

## Usage

```
cham <input file> [OPTS] -o <output file>
```

### Options

```
--output -o
    Produces output with the argument given. If the option is not used, the
    output produced will be the input prefixed with 'cham_'.

--color-palette -p
    If given, will use predef palette instead of generated one. Defaults to
    256 palette.
    Avaliable palettes:
        - blue_mono
        - bw
        - 256 (six_eight_five)

--bit-depth -b
    The bit color depth of the generated palette, if not used will default to
    3. If the k arg is used, it takes priority.

--k-color -k
    The number of colors present in the generated palette, if not used defaults
    to bit-depth value of 2^depth.

--dither -d
    Enables dither with the chosen dithering algorithm as the argument,
    defaults to floyd.
    Avaliable Algorithms:

--width -w
    Resize image to the given width. If height is not set it preserves the
    original ratio.

--height -h
    Resize image to the given height. If width is not set it preserves the
    original ratio.
```

## TODO:

- generate random color palettes
- add color cache (cache 16 colors when we're looking for the closest color)
