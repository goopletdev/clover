# clover
Lightweight C implementation of [Plover](https://github.com/openstenoproject/plover)'s simplest functionality 

## dependencies
in `test` directory:
```
git clone https://github.com/ThrowTheSwitch/Unity.git
git pull
```

in `assets` directory:
```
https://raw.githubusercontent.com/aerickt/steno-dictionaries/refs/heads/main/lapwing-base.json
```
Also, `assets/test-dictionary.json` is currently taken from the first several entries of the above linked `lapwing-base.json` file from the [Lapwing theory](https://github.com/aerickt/steno-dictionaries) for plover. 

### notes to self
Document code with Doxygen tags/Javadoc-style doc tags

Consider incorporating https://zserge.com/jsmn/ for json parsing