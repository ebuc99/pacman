Pacman
======

![in-game screenshot](https://libregamewiki.org/images/1/18/Pacman.png "in-game screenshot")

This is a clone of the original pacman by Namco, as I remember, that I played for the first time on an Atari 130 XL in the early 90s.

Also, Paul Neave's pacman clone has inspired me greatly.

One of the main goals of this implementation is an SDL application with a very low CPU usage.


## Install hint ##

You have to compile the Linux version on your own. For this, you'll need
* libsdl
* sdl-image
* sdl-ttf
* and sdl-mixer.

Then, download the zip file or clone the pacman repository. Inside the pacman directory,
1. prepare the compilation: `./configure`
2. compile the game: `make`
3. install the game: `make install`

You should now be able to start the game via command line: `pacman`


## License ##
Pacman is licensed under the terms of the GNU General Public License version 2 (or any later version).
