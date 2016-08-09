# Mylib JSON

This is a parser for (a reasonable subset of) JSON.

## Benchmark

They do the same thing: read in the whole file (10000 entries), parse it, and 
exit.

### Mylib

```
real	0m0.035s
user	0m0.029s
sys	0m0.006s
```

### Python + json

```
real	0m0.046s
user	0m0.034s
sys	0m0.011s
```

## Memory

I believe this lib doesn't leak. And Valgrind confirms so.

```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
  total heap usage: 34 allocs, 34 frees, 76,232 bytes allocated

All heap blocks were freed -- no leaks are possible

For counts of detected and suppressed errors, rerun with: -v
ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Implementation
This is a pretty naive implementation of JSON.

Limitations:

1. Bad debug support (no line number, no clear message)
2. No comment
3. Only integers
4. No \uXXXX support

But it supports major features of JSON (thus a subset).
It mainly follows the specification on http://www.json.org/.

The library might need to be completely refactored to solve limitation
1 and 2, but limitation 3 and 4 seem easy to fix (don't ask me, I'm lazy).

## Run

```
make
./test    # you are not running the same code as is used in benchmark!
```

