#!/bin/bash
# build.sh - For Linux builds

set -ex

mkdir -p $PREFIX/bin

cp bin/libplugify-plugin-s2sdk.so $PREFIX/bin/
cp plugify-plugin-s2sdk.pplugin $PREFIX/
cp gamedata.jsonc $PREFIX/
cp settings.jsonc $PREFIX/
cp translations.jsonc $PREFIX/

chmod 755 $PREFIX/bin/libplugify-plugin-s2sdk.so
chmod 644 $PREFIX/plugify-plugin-s2sdk.pplugin
chmod 644 $PREFIX/gamedata.jsonc
chmod 644 $PREFIX/settings.jsonc
chmod 644 $PREFIX/translations.jsonc
