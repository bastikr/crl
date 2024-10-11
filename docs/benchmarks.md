# Benchmarks

What are interesting questions we would like to answer?

## Build example program
Measure:
* Source code size:
    * Header
    * Source
    * Total
* Binary size:
    * libassets
    * libcrl
    * executable
* Time to build:
    * Each step separately? (compile crl, compile assets, compile main, linking separate?)

Dependend on:
* Build status:
    * Clean
    * Nothing changed
    * One file added
    * All files changed
* cmake/ninja
* testsets
* features

## Build benchmark program
* all features enabled

Measure:
* Time to run different get functions

Dependend on:
* testsets (file size should not be relevant)
* search file:
    * exists/does not exist
    * order in sorted file list

## Testset characteristics
Testset characteristics:
* 1kb - 10MB
* 1 file - 10'000 files
* What about path length/directory depth?

## Features
* str comparison
* directory tree based
* simple hashing
* gperf perfect hashing
* static access
