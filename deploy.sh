#!/usr/bin/bash

sudo apt install -y build-essential autoconf automake libtool
sudo apt install -y libsqlite3-dev sqlite3 libibus-1.0-dev libpinyin-dev gnome-common libspeechd-dev ibus-libpinyin
sudo apt install -y gobject-introspection

./autogen.sh
./configure  --prefix=/usr --libexecdir=/usr/lib/ibus
make
sudo make install
im-config -n ibus
ibus-daemon -drx
