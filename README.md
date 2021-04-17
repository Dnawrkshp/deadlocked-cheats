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

4. Then you can build and cheats by entering their directory using:

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

3. Grab the ps2dev image and launch an instance with the current directory mounted:

```sh
docker pull ps2dev/ps2dev
docker run -it --rm -v "$PWD\:/src" ps2dev/ps2dev
cd src
./docker-init.sh
```

NOTE: The docker-init script is known to fail due to inconsistent line endings when your git is configured to convert line endings (windows). If this is the case you can either replace all line endings in the file with LF or just run each command in the script manually.

4. Build with:

```sh
make
```
