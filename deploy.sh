#!/usr/bin/bash

sudo apt install -y libsqlite3-dev
sudo apt install -y sqlite3-dev
sudo apt install -y libpinyin-dev

./autogen.sh
./configure  --prefix=/usr --libexecdir=/usr/lib/ibus
make
sudo make install