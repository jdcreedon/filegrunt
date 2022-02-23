# FileGrunt

![e077e3b9ef8241e0be0901793a5b967b](https://user-images.githubusercontent.com/6379032/155383375-fbe860ee-1910-4f7a-992e-4572451cc676.png)

Utility to identify duplicate files/images and allow the user to move the duplicates to a location for backup or deletion.

This project is under development and is being used to help me learn C++, OpenCV and cross platform development.

[![GitHub issues](https://img.shields.io/github/issues/dcreedon/filegrunt)](https://github.com/dcreedon/filegrunt/issues)
[![GitHub](https://img.shields.io/github/license/dcreedon/filegrunt)](https://github.com/dcreedon/filegrunt/blob/main/LICENSE)

# Development
I'm using the CLION IDE on Debian Bullseye and Gnome Desktop as my Development Environment

I'm using QT Framework 6.2.2, OpenCV 4.5.5 , SQLite 3 and C++ 

sqlite Browser to view sqlite database https://github.com/sqlitebrowser/sqlitebrowser 

I'm aiming for a cross platform utility that will work on Linux, Mac & Windows.

# How it works Now

- Performs like a snail when lots of files being processed, expected as much as it's using local database rather than in memory. This helps to troubleshoot.

1. Select a source folder
2. List of media files added to sqlite database
3. Duplicates identified by comparing file name and file size
4. Duplicate image files are then compared to each other using OpenCV bitwise XOR which results in all zeros if images match
5. The duplicates are recorded in the sqlite database

# TODO
- Create a FileGrunt Logo - without this nothing will work!
- Allow visual inspection of original and duplicate
- Decide on which one should be classed as a duplicate e.g. earliest date?
- Move the Duplicates to a location for archival/deletion
- Remove the duplicates from the source
- Improve README.md file as I go.
- Setup Gitlab for CI/CD 
- Figure out how to create a cross platform executable/installation package.
- Create a Wiki/Issues list and tidy up this file :-)

# Working Prototype
![image](https://user-images.githubusercontent.com/6379032/155019123-48916c28-0a87-4d0c-a83e-b799b8989361.png)

![image](https://user-images.githubusercontent.com/6379032/155020061-055f2def-3031-4c95-b59c-eb53857136e0.png)

![image](https://user-images.githubusercontent.com/6379032/155021832-ac8f13e7-128e-48bc-ba37-b3ba9cb712d9.png)


# Does it Work
Initial inspection indicates duplicates are being identified correctly as expected.

# Useful Links
- Markdown Cheat Sheet for README.md https://www.markdownguide.org/cheat-sheet/
- Open Source Guide https://opensource.guide/
