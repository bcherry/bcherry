-module(fact).
-export([fac/1,fib/1]).

fac(0) -> 1;
fac(N) -> N * fac(N-1).

fib(1) -> 0;
fib(2) -> 1;
fib(N) -> fib(N-1) + fib(N-2).
