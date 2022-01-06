#!/usr/bin/env bash

log() {
    NAME=$1
    shift
    "$@" 2>&1 | sed $'s|^|\x1b[34m['"${NAME}"$']\x1b[39m |'
}

# Check that we have all the required dependencies installed on the system
log dependencies echo "make"
if ! command -v make >/dev/null; then
    log dependencies echo "Failed to locate 'make'."
    exit 1
fi

log dependencies echo "patch"
if ! command -v patch >/dev/null; then
    log dependencies echo "Failed to locate 'patch'."
    exit 1
fi

log dependencies echo "c compiler"
if ! ${CC:-cc} -o /dev/null -xc - >/dev/null <<'PROGRAM'
int main() {}
PROGRAM
then
    log dependencies echo "Failed to locate working c compiler."
    exit 1
fi

for lib in gmp mpc mpfr; do
    log dependencies echo "library $lib"
    if ! ${CC:-cc} -I /usr/local/include -L /usr/local/lib -l$lib -o /dev/null -xc - >/dev/null <<PROGRAM
#include <$lib.h>
int main() {}
PROGRAM
    then
      log dependencies "Failed to locate library $lib"
      exit 1
    fi
done
