#!/usr/bin/env bash

DOWNLOAD_PREFIX="download"
DOC_PREFIX="doc"

VERSION=$(grep -e '^version:' doc/project_info.txt | cut -d ' ' -f2)

DOWNLOAD_PATH="$DOWNLOAD_PREFIX/$VERSION"
DOC_PATH="$DOWNLOAD_PREFIX/$VERSION"

echo "preparing destination directories..."
if [ -d "$DOWNLOAD_PATH" ]; then
    rm -rf "$DOWNLOAD_PATH" || exit
fi

if [ -d "$DOC_PATH" ]; then
    rm -rf "$DOC_PATH" || exit
fi

echo "installing documentation..."
mv build-doc/generated_doc/html/* "$DOC_PATH" || exit

echo "installing packages..."
mkdir -p "$DOWNLOAD_PATH/windows" || exit
mv build/mapgen-$VERSION-win32.exe "$DOWNLOAD_PATH/windows/mapgen-$VERSION.exe" || exit

mkdir -p "$DOWNLOAD_PATH/linux" || exit
mv build/mapgen-$VERSION-Linux.sh "$DOWNLOAD_PATH/linux/mapgen-$VERSION.sh" || exit
mv build/mapgen-$VERSION-Linux.tar.Z "$DOWNLOAD_PATH/linux/mapgen-$VERSION.tar.Z" || exit
mv build/mapgen-$VERSION-Linux.tar.gz "$DOWNLOAD_PATH/linux/mapgen-$VERSION.tar.gz" || exit

mkdir -p "$DOWNLOAD_PATH/osx" || exit
mv build/mapgen-$VERSION-Darwin.sh "$DOWNLOAD_PATH/linux/mapgen-$VERSION.sh" || exit
mv build/mapgen-$VERSION-Darwin.pkg "$DOWNLOAD_PATH/linux/mapgen-$VERSION.pkg" || exit
mv build/mapgen-$VERSION-Darwin.tar.gz "$DOWNLOAD_PATH/linux/mapgen-$VERSION.tar.gz" || exit