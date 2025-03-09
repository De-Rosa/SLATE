FROM ubuntu:24.10 AS ubuntu 

ARG HOME=/home/ubuntu
ARG PREFIX=$HOME/opt/cross 
ARG TARGET=i686-elf
ARG PATH="$PREFIX/bin:$PATH"

ARG BINUTILS_VER="with-gold-2.44"
ARG GCC_VER="14.2.0"

# getting dependencies
RUN apt update && apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev xorriso wget

# building toolchain 
WORKDIR /$HOME/src
RUN wget -qO- https://ftp.gnu.org/gnu/binutils/binutils-with-gold-2.44.tar.gz | tar xzvf - && wget -qO- https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz | tar xzvf -


WORKDIR ./build-binutils
RUN ../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot &&\ 
make && make install

WORKDIR ../
RUN mkdir build-gcc

WORKDIR ./build-gcc
RUN ../gcc-$GCC_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx && make all-gcc && make all-target-libgcc &&\ 
make all-target-libstdc++-v3 &&\ 
make install-target-libgcc && make install-target-libstdc++-v3

ENTRYPOINT ["/bin/bash"]
