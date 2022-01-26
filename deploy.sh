#!/usr/bin/bash

sudo apt install -y libsqlite3-dev
sudo apt install -y sqlite3
sudo apt install -y libsqlite3-dev
sudo apt install -y libibus-1.0-dev
sudo apt install -y libpinyin-dev
sudo apt install -y gnome-common
sudo apt install -y libspeechd-dev

./autogen.sh
./configure  --prefix=/usr --libexecdir=/usr/lib/ibus
make
sudo make install
ibus-daemon -drx
