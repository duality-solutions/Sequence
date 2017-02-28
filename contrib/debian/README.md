
Debian
====================
This directory contains files used to package sequenced/sequence-qt
for Debian-based Linux systems. If you compile sequenced/sequence-qt yourself, there are some useful files here.

## Sequence: URI support ##


sequence-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install sequence-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your sequence-qt binary to `/usr/bin`
and the `../../share/pixmaps/sequence128.png` to `/usr/share/pixmaps`

sequence-qt.protocol (KDE)

