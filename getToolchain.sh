#!/bin/bash
set -e

# see packages here: http://energia.nu/packages/package_energia_index.json
if [ "$(uname)" == "Darwin" ]; then
    CL_TOOLCHAIN_TARBALL=https://s3.amazonaws.com/energiaUS/tools/macosx/gcc-arm-none-eabi-6.3.1-20170620-mac.tar.bz2
else
    CL_TOOLCHAIN_TARBALL=https://s3.amazonaws.com/energiaUS/tools/linux64/gcc-arm-none-eabi-6.3.1-20170620-x86_64-pc-linux-gnu.tar.bz2
fi

mkdir -p toolchain/Connected_Launchpad
mkdir -p toolchain/downloads

if [ ! -f toolchain/downloads/cl_toolchain.tar.bz2 ]; then
    echo "Downloading Connected Launchpad Toolchain"
    curl $CL_TOOLCHAIN_TARBALL --output toolchain/downloads/cl_toolchain.tar.bz2
fi

if [ ! -d toolchain/Connected_Launchpad/bin ]; then
    echo "Extracting Connected Launchpad Toolchain"
    pushd toolchain/Connected_Launchpad
    tar --strip-components=1 -xjf ../downloads/cl_toolchain.tar.bz2
    popd
fi

echo "Done"