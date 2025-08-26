# clover
Lightweight C implementation of [Plover](https://github.com/openstenoproject/plover)'s simplest functionality 

## Building
### 1. Install system dependencies
Clover uses libevdev for sending keyboard events.
#### Ubuntu/Debian
```bash
sudo apt install libevdev-dev
```
#### Arch Linux
```bash
sudo pacman -S libevdev
```
### 2. Clone with submodules
```bash
git clone --recursive https://github.com/goopletdev/clover.git
cd clover
```
If you cloned without --recursive:
```bash
git submodule update --init --recursive
```
### 3. Build
```bash
make
```
### 4. Run tests
```bash
make test
```

## Dictionaries:
In `dictionaries/`:
```
https://raw.githubusercontent.com/aerickt/steno-dictionaries/refs/heads/main/lapwing-base.json
```
Also, `assets/test-dictionary.json` is currently taken from the first several entries of the above linked `lapwing-base.json` file from the [Lapwing theory](https://github.com/aerickt/steno-dictionaries) for plover. 

### notes to self
Document code with Doxygen tags/Javadoc-style doc tags

Consider incorporating https://zserge.com/jsmn/ for json parsing

For steno history, many translations will also result in backspaces. Undoing will therefore also have to keep track of the chars deleted by said backspaces

### todo:
assign unique id to each root steno dictionary: 0 for highest-priority, incrementing from there
replace "translations" field with object containing function pointers, raw text, source steno dictionary id
add "collisions" to handle same entry in multiple dictionaries
