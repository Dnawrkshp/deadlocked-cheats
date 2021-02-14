# deadlocked-cheats

Sample project showing how to use libdl to create cheat codes for Ratchet: Deadlocked PS2.

## Building

You'll need a local copy of the [PS2SDK](https://github.com/ps2dev/ps2sdk).

1. Install [libdl](https://github.com/Dnawrkshp/libdl).

2. Install [bin2code](https://github.com/Dnawrkshp/bin2code).

3. Clone the repo and enter it:

```sh
git clone https://github.com/Dnawrkshp/deadlocked-cheats.git
cd deadlocked-cheats
```

4. Then you can build using:

```sh
make
```

## Building with Docker (recommended)

1. Install [docker](https://docker.com).

2. Clone the repo and enter it:

```sh
git clone https://github.com/Dnawrkshp/deadlocked-cheats.git
cd deadlocked-cheats
```

3. Grab the ps2dev image with bin2code and launch an instance with the current directory mounted:

```sh
docker pull dnawrkshp/ps2dev-bin2code
docker run -it --rm -v "$PWD\:/src" dnawrkshp/ps2dev-bin2code
cd src
apk add bash && ./docker-init.sh
```

4. Build with:

```sh
make
```
