# INI parser

This is an INI parser as a C++ exercise (interface design, performance, 
debugging, etc.).

## INI format

INI stands for initialization. An INI file is used in Windows as a simple way 
for configuration.

```
[Section1]
Key1 = 1
[Section2]
Key1 = This is a long string
```

## Benchmark

The same test.ini.

Sadly, mylib lacks many features and is much slower. Use GKeyFile instead!

### mylib

time

```
real	0m1.243s
user	0m1.187s
sys	0m0.056s
```

Valgrind output

```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
  total heap usage: 4,264,360 allocs, 4,264,360 frees, 175,223,183 bytes allocated

All heap blocks were freed -- no leaks are possible
```

### GKeyFile

time

```
real	0m0.528s
user	0m0.461s
sys	0m0.057s
```

Valgrind

```
HEAP SUMMARY:
    in use at exit: 21,264 bytes in 109 blocks
  total heap usage: 4,034,200 allocs, 4,034,091 frees, 148,196,857 bytes allocated

LEAK SUMMARY:
   definitely lost: 0 bytes in 0 blocks
   indirectly lost: 0 bytes in 0 blocks
     possibly lost: 0 bytes in 0 blocks
   still reachable: 21,264 bytes in 109 blocks
        suppressed: 0 bytes in 0 blocks
Rerun with --leak-check=full to see details of leaked memory

For counts of detected and suppressed errors, rerun with: -v
ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Implementation

Limitations:
* Data are kept in memory (at least one section)
* No invisible characters supported
* No write operations supported
* Thread-unsafe
* No async
* No comments

However, since the majority of INI files are small, so keep them in memory is 
not unbearable. The test uses a rather big one (~27 MiB), it should cover the
most common conditions. It's fast enough, so no need for async and thread.

