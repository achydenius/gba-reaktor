# A Game Boy Advance 3D demo

![Screenshot](https://raw.githubusercontent.com/achydenius/gba-reaktor/main/gba-reaktor.png?token=GHSAT0AAAAAABHT7LULSISUPAM5XWAE63L4YS7X7FQ)

## How to build

1. Install [devkitPro](https://github.com/devkitPro/pacman/releases)
2. Install GBA toolchain with `sudo dkp-pacman -S gba-dev`
3. Build with `make`
4. Run _main.gba_ with e.g. [mGBA](https://mgba.io/)

## TODO

- Constant frame rate (looks a bit jerky on at least mGBA)
- Drawing the last pixel (?) of a horizontal span bugs a bit
- Divisions can be replaced with 1/z lookup tables
