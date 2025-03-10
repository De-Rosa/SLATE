# SLATE
Simple operating system, currently directly copied from [Meaty Skeleton](https://wiki.osdev.org/Meaty_Skeleton).\
Subject to change.

## Compiling using Docker (Recommended)


| Dependency                     | Debian Command |
| ---------- | ------------------------------ |
| Docker              | *can be found on Docker website*                                   |
| Make              | `sudo apt install make`                                   |
| Qemu                 | `sudo apt install qemu-system`                               |

The i686-elf toolchain can be created using Docker, where it can then be ran to output an ISO file.\
In order to build the image:
```
make build
```
In order to compile SLATE into an ISO:
```
make run
```


## Prerequisites (Manual)

An i686-elf toolchain (the GCC cross-compiler) is required to build the SLATE ISO file. Instructions on how to create this toolchain is detailed in a later section.\
GRUB is required for runtime files and commands used for building. GRUB should be installed for Linux, but can otherwise be found online. \
Other dependencies required:

| Dependency                     | Debian Command |
| ---------- | ------------------------------ |
| Xorriso              | `sudo apt install xorriso`                                   |
| Qemu                 | `sudo apt install qemu-system`                               |




## Creating the GCC Cross-Compiler (Manual)
The dependencies for building the cross compiler can be found [here](https://wiki.osdev.org/GCC_Cross-Compiler#Installing_Dependencies).\
For Debian-based systems (Ubuntu, Mint, etc.):\
`sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev`\
\
You need to download the [latest release](https://ftp.gnu.org/gnu/binutils/?C=M;O=D) of Binutils (with-gold is fine) and the [latest release](https://ftp.gnu.org/gnu/gcc/?C=N;O=D) of GCC.\
Further instructions can be found [here](https://wiki.osdev.org/GCC_Cross-Compiler#macOS_Users) for MacOS or Windows users.\
\
It is recommended to extract the source files into `$HOME/src` which will be built into `$HOME/opt/cross`.
```
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

export BINUTILS_VER="with-gold-x.y.z"
export GCC_VER="x.y.z"
```
These commands are temporary to the current shell session and are used for building.\
For Binutils and GCC, replace the x.y.z with the version you downloaded (omitting 'with-gold-' if necessary).\
\
To compile Binutils:
```
cd $HOME/src 
mkdir build-binutils
cd build-binutils
../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
```

To build GCC:\
We can verify that the `$PREFIX/bin` directory is in the PATH using\
`which -- $TARGET-as || echo $TARGET-as is not in the PATH`.

```
cd $HOME/src 
mkdir build-gcc
cd build-gcc
../gcc-$GCC_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
make all-gcc
make all-target-libgcc
make all-target-libstdc++-v3
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
```

We can then add our new compiler to PATH:
```
export PATH="$HOME/opt/cross/bin:$PATH"
```
This will only work for the current shell session, and therefore if wanted to use permanently must be added to your shell configuration script (e.g. `.zshrc` for zsh).\
Furthermore, we can keep `$TARGET` by also adding
```
export TARGET=i686-elf
```

Our compiler can now be ran by invoking `$TARGET-gcc`!

## Building and Running (Manual)
```
./clean.sh 
./headers.sh 
./iso.sh 
./qemu.sh
```
You may need to make the shell scripts executable. On unix-like operating systems:\
`chmod +x build.sh clean.sh config.sh default-host.sh headers.sh iso.sh qemu.sh target-triplet-to-arch.sh`

## Sources Used
https://wiki.osdev.org
