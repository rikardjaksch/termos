#!/usr/bin/env bash

log() {
    NAME=$1
    shift
    "$@" 2>&1 | sed $'s|^|\x1b[34m['"${NAME}"$']\x1b[39m |'
}

pushds() {
    command pushd "$@" > /dev/null
}

popds() {
    command popd "$@" > /dev/null
}
    

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BINUTILS_VERSION="2.37"
BINUTILS_MD5SUM="1e55743d73c100b7a0d67ffb32398cdb"
BINUTILS_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_PKG="${BINUTILS_NAME}.tar.gz"
BINUTILS_BASE_URL="https://ftp.gnu.org/gnu/binutils"

GCC_VERSION="11.2.0"
GCC_MD5SUM="dc6886bd44bb49e2d3d662aed9729278"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="${GCC_NAME}.tar.gz"
GCC_BASE_URL="https://ftp.gnu.org/gnu/gcc"

mkdir -p "$DIR/tarballs"

pushds "$DIR/tarballs"
    md5=""
    binutils_dirty=False
    gcc_dirty=False

    if [ -e "$BINUTILS_PKG" ]; then
        md5="$(md5sum $BINUTILS_PKG | cut -f1 -d' ')"
    fi

    if [ "$md5" != ${BINUTILS_MD5SUM} ] ; then
        rm -f $BINUTILS_PKG
        echo "Downloading binutils tarball"
        curl -LO "$BINUTILS_BASE_URL/$BINUTILS_PKG"
        binutils_dirty=True
    else
        echo "Skipped downloading binutils"
    fi
    
    md5=""
    if [ -e "$GCC_PKG" ]; then
        md5="$(md5sum $GCC_PKG | cut -f1 -d' ')"
    fi

    if [ "$md5" != ${GCC_MD5SUM} ] ; then
        rm -f $GCC_PKG
        echo "Downloading gcc tarball"
        curl -LO "$GCC_BASE_URL/$GCC_NAME/$GCC_PKG"
        gcc_dirty=True
    else
        echo "Skipped downloading gcc"
    fi

    if [ $binutils_dirty = True ] || [ ! -d $BINUTILS_NAME ]; then
        echo "Extracting binutils"
        tar -xzf $BINUTILS_PKG

        #echo "Patching binutils"
        #pushds $BINUTILS_NAME
        #    patch -p0 -Ni "$DIR/patches/binutils.patch" -r /dev/null
        #popds
    fi

    if [ $gcc_dirty = True ] || [ ! -d $GCC_NAME ]; then
        echo "Extracting gcc"
        tar -xzf $GCC_PKG

        #echo "Patching gcc"
        #pushds $GCC_NAME
        #    patch -p0 -Ni "$DIR/patches/gcc.patch" -r /dev/null
        #popds
    fi

    unset binutils_dirty
    unset gcc_dirty
    unset md5
popds


SYSROOT="$DIR/../build/sysroot"

# Intermediate directory
mkdir -p "$DIR/build"
# Install directory for the cross compiler binaries
mkdir -p "$DIR/bin"
# Folder which acts as the sysroot
mkdir -p "$SYSROOT"

pushds "$DIR/build"
    # Ignore debug symbols in our cross compiler
    export CFLAGS="-g0 -O2 -mtune=native"
    export CXXFLAGS="-g0 -O2 -mtune=native"

    rm -rf binutils
    mkdir -p binutils
    pushds binutils
        echo "Configuring binutils"
        "$DIR"/tarballs/$BINUTILS_NAME/configure --prefix="$DIR/bin" \
            --target="i686-elf" \
            --with-sysroot \
            --disable-werror \
            --disable-nls || exit 1

        echo "Building binutils"
        make -j $(nproc) || exit 1
        make install || exit 1
    popds

    rm -rf gcc
    mkdir -p gcc
    pushds gcc
        echo "Configuring gcc"
        "$DIR/tarballs/gcc-$GCC_VERSION/configure" --prefix="$DIR/bin" \
            --target="i686-elf" \
            --disable-nls \
            --without-headers \
            --enable-languages=c,c++ || exit 1

        echo "Building gcc"
        make -j $(nproc) all-gcc || exit 1
        make -j $(nproc) all-target-libgcc || exit 1
        make install-gcc || exit 1
        make install-target-libgcc || exit 1

        #echo "Building libstdc++"
        #make -j $(nproc) all-target-libstdc++-v3 || exit 1
    popds
popds
