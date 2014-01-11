rain
====

A simple utility to view stdin as "digital rain" à la The Matrix (cos that's useful, right?)

I originally started it as an excuse to explore the ncurses library a bit, at some point back in my uni days. It was left in a rather untidy state, but it works. I may clean it up, or even expand it at some point.

Currently it is very simple to build:
```
> make
```
(assuming you have basic build packages installed - I’ll leave it to you to find the dependencies if not)

You now have a `rain` executable; congratulations! Place this somewhere on your path, and pipe any text into it. For example, the source code itself:
```
> cat rain.c | rain
```
Each line will be placed in a random terminal column and "rain" down.

Note that currently an EOF does not terminate _rain_, so you will need to `ctrl+c` when you're done.

Have fun!
