# STAGE 1
# Get prerequisites and build cross-compiler.
FROM ubuntu:24.10 AS build-env

ARG PREFIX=/home/ubuntu/opt/cross 
ARG TARGET=i686-elf
ARG PATH="$PREFIX/bin:$PATH"

ARG BINUTILS_VER="with-gold-2.44"
ARG GCC_VER="14.2.0"

# Installing dependencies
RUN apt update && apt install -y \
    build-essential \
    bison \
    flex \
    libgmp3-dev \
    libmpc-dev \
    libmpfr-dev \
    texinfo \
    libisl-dev \
    xorriso \
    wget \
    gcc \
    make

# Build toolchain
WORKDIR /home/ubuntu/src
RUN wget -qO- https://ftp.gnu.org/gnu/binutils/binutils-with-gold-2.44.tar.gz | tar xzvf - && wget -qO- https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz | tar xzvf -

WORKDIR ./build-binutils
RUN ../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot &&\ 
make -j$(nproc) && make install


WORKDIR /home/ubuntu/src
RUN mkdir build-gcc

WORKDIR ./build-gcc

RUN ../gcc-$GCC_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx &&\ 
make -j$(nproc) all-gcc &&\ 
make -j$(nproc) all-target-libgcc &&\ 
make -j$(nproc) all-target-libstdc++-v3 &&\ 
make install-gcc &&\ 
make install-target-libgcc &&\ 
make install-target-libstdc++-v3


# STAGE 2: 
# Image used for compiling the OS.
FROM ubuntu:24.10 AS final-image

ARG PREFIX=/home/ubuntu/opt/cross
ARG TARGET=i686-elf
ENV PATH="${PREFIX}/bin:/usr/local/bin:/usr/bin:/bin:$PATH"

RUN apt update && apt install -y \
    build-essential \
    make \
    gcc \
    xorriso \
    grub-pc-bin

# Copy the toolchain from the build stage
COPY --from=build-env /home/ubuntu/opt/cross /home/ubuntu/opt/cross

RUN mkdir /build
WORKDIR /build

CMD ["/bin/bash"]
