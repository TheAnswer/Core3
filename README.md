# SWGepub6
### forked from SWGEmu Core3

## What is SWGpub6?

Ditto SWGEmu but pub 6

## How to Build

### Dependencies

  * CMake 3.1.0 or higher
  * BerkeleyDB 5.3
  * MySQL Client and Server
  * OpenSSL libraries
  * pthreads
  * Lua 5.3 libraries
  * Zlib libraries
  * g++ 5.4+ or compatible
  * engine3
  * java jre 1.7+

### Build

  * Install dependencies (Debian 9+ or Ubuntu 16.04+)

        sudo apt install build-essential libmysqlclient-dev liblua5.3-dev libdb5.3-dev libssl-dev cmake git default-jre

  * Install dependencies (RHEL/CentOS 8+ or Fedora 28+)

        sudo dnf install automake cmake git gcc gcc-c++ java-1.8.0-openjdk-headless libatomic libdb-devel lua-devel make mariadb-devel openssl-devel

  * Clone core3 repository somewhere  (~/git)

        mkdir -p ~/git
        cd ~/git
        git clone https://github.com/TheArchitect108/swgepub6/
  * Build Core3 with 8 threads

        cd MMOCoreORB
        make -j8
  * Import sql database

        mysql -h<MYSQLHOST> -u<MYSQLUSER> -p<MYSQLPASSWORD> < sql/swgemu.sql

## How to Run

    cd ~/git/swgepub6/MMOCoreORB/bin
    ./core3

## License

    Copyright (C) 2020 SWGEmu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License along with this program.
    If not, see <http://www.gnu.org/licenses/>.
