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
git clone --recursive https://github.com/goopletdev/clover.git && \
cd clover
```
If you cloned without --recursive:
```bash
git submodule update --init --recursive
```

### 3. Download dictionaries
You can manually download dictionaries using curl.
#### Lapwing dictionaries:
```bash
cd dictionaries && \
curl --remote-name-all https://raw.githubusercontent.com/aerickt/steno-dictionaries/main/{\
lapwing-commands.json,\
lapwing-numbers.json,\
lapwing-uk-additions.json,\
lapwing-proper-nouns.json,\
lapwing-base.json\
} && \
cd ..
```
#### Plover dictionaries:
```bash
cd dictionaries && \
curl --remote-name-all https://raw.githubusercontent.com/openstenoproject/plover/refs/heads/main/plover/assets/{\
user.json,\
commands.json,\
main.json\
} && \
cd ..
```
Also, `dictionaries/test-dictionary.json` is currently taken from the first several entries of the above linked `lapwing-base.json` file from the [Lapwing theory](https://github.com/aerickt/steno-dictionaries) for plover. 

### 4. Build
```bash
make
```

### 5. Run tests
```bash
make test
```
For further debugging and checking for memory leaks, consider installing Valgrind.
#### Ubuntu/Debian
```bash
sudo apt install valgrind
```
#### Arch Linux
```bash
sudo pacman -S valgrind
```

### 6. Find and set keyboard
If you know the path to your keyboard device event handler, set it in `default-config.toml` (or `config.toml` for persistence). If you don't, you can use evtest (you may have to install it first):
```bash
sudo evtest
```
Try selecting an event number; if the console starts logging event data from your keystrokes, that's the event number you want. Under `[keyboard]` in `config.toml`, set `path` to `"dev/input/eventYOUR_EVENT_NUMBER"`

### 7. Running Clover
Clover needs sudo priveleges to open device paths:
```bash
sudo make run
```
### notes to self
Consider incorporating https://zserge.com/jsmn/ for json parsing

For steno history, many translations will also result in backspaces. Undoing will therefore also have to keep track of the chars deleted by said backspaces

### todo:
- Document code with Doxygen tags/Javadoc-style doc tags
- implement steno-order sorted dictionary
- replace "translations" field with object containing function pointers, raw text, source steno dictionary id
- add keyboard state-checking functions
- make clover_pretty_chord and clover_paper_tape use buffers