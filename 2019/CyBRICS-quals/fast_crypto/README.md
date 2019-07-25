# Challenge description

> Here you have some modern encryption software. Actually, it’s even too modern for your hardware.
>
> Try to find out how to decode the WAV file with a secret message:
>
> [./fastcrypto.zip](fastcrypto.zip)

# Analysis

The zip file contains three files. An encrypted wav file, a public key (consisting of `N` and `O`) and the python code used for encryption.

The python script asks for a seed (`0 <= seed <= 2^16`) and a power a power (`2 <= power <= 16`).

These values are used in conjuction with the public key to generate a stream of bytes.

At each iteration the 8 least significant bits of the state are the next byte of the stream. The entered seed is the initial state, and the state transition is defined as `state = state ** power % N`. The first `O` bytes of the stream are discarded, and the rest was used to XOR encrypt the WAV file.

# Solution

The used seed and power are unknown, but luckily the search space for this is only of size `2^16 * 2^4 = 2^20`, which is bruteforceable. The task now becomes searching for a combination (seed, power) which generated the correct bytestream to decrypt the first 4 bytes of the encrypted WAV file to `0x52 0x49 0x46 0x46` which are the magic bytes of WAV files.

However, the testing of each combination is very slow (2-3s each in C) and should be sped up a little to finish in a reasonable time.

The first thing to notice is that the state transition can be more efficiently expressed as `state = pow(state, power, N)`, which makes sure the intermediate results are considered modulo N as well. This is obviously better than calculating the actual power, and then reducing it with `%N`.

The second observation to make is that discarding the first `O` iterations is doing the same as transforming the state like `state = state ** (power ** O) % N`.
The factor `power ** O` is independant of the seed, and only needs to be calculated for every value of `power`.

Doing this speeds up the calculations to 0.5-1s per combination.

The last optimisation is possible because `N` is easily factorable (sage solves this near instantly), so Euler's Theorem can be applied: `state = state ** (power ** O) % N = state ** (power ** O % phi(N)) % N`. Using the python power notation, this becomes: `state = pow(state, pow(power, O, phi(N)), N)`.

Doing this speeds the process up to <10ms per iteration.

C code can be found in [fastcrypto.c](fastcrypto.c).
