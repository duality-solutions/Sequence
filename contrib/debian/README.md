
Debian
====================
This directory contains files used to package silkd/silk-qt
for Debian-based Linux systems. If you compile silkd/silk-qt yourself, there are some useful files here.

## Silk: URI support ##


silk-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install silk-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your silk-qt binary to `/usr/bin`
and the `../../share/pixmaps/silk128.png` to `/usr/share/pixmaps`

silk-qt.protocol (KDE)

