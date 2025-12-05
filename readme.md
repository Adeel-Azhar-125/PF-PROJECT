Overview of the game:

This project is a Chess Game built using C++ and SFML.
It includes:

Full 8×8 chessboard.

Legal move validation.

Check, checkmate, stalemate detection.

Pawn promotion (interactive GUI).

Move highlighting.

Score saving into a file.

Chess piece textures and board rendering.

Sound effects.

Smooth GUI using SFML.


This game is designed for beginner-to-intermediate C++ learners who want to practice SFML-based game development.

Requirements

Before running this project, you must have:

 Required Software Visual Studio (2019/2022)

SFML 2.5+

C++17 or higher


 Required SFML Modules

Make sure the following modules are linked:

sfml-graphics

sfml-window

sfml-system

sfml-audio

 Project File Structure

YourProject

images/
white-pawn.png
white-rook.png
white-knight.png
white-bishop.png
white-queen.png
white-king.png
black-pawn.png
black-rook.png
black-knight.png
black-bishop.png
black-queen.png
black-king.png

sound effect/
music.mp3

font file/
text.otf

chess_scores.txt   (auto-generated)
main.cpp           (your code)
README.md

How to Build in Visual Studio

1️. Install SFML

Download from the official site:
https://www.sfml-dev.org/download.php

2️. Create New Visual Studio Project

File + New + Project

Choose empty project (C++).

Import your main.cpp.


3️. Configure SFML

Go to:

 Project Properties + C/C++ + General + Additional Include Directories

path_to_SFML/include

 Project Properties + Linker + General + Additional Library Directories

path_to_SFML/lib

 Project Properties + Linker + Input + Additional Dependencies Add these:

sfml-graphics.lib
sfml-window.lib
sfml-system.lib
sfml-audio.lib

4️. Copy SFML DLL Files

From SFML/bin + Copy all .dll files
Paste into your Visual Studio output directory:

YourProject/x64/Debug/

5️. Run the project

Press F5 to run.


 Game Features:

 Legal Move System

Every piece follows real chess movement:

Rook movement

Bishop movement

Knight (L-shape)

Queen (combined rook+bishop)

King (1 square)

Pawn movement + double step + captures


 Check & Checkmate Detection:

The engine checks:

Is king attacked?

Does the player have any legal moves?

Checkmate or stalemate automatically triggers.


 Highlights

Yellow: Selected piece

Red: Possible legal moves


 Pawn Promotion

A graphical menu appears with 4 choices:

Queen

Rook

Bishop

Knight


 Sound Effects

Piece movement plays a sound.

 Score Saving

Game results are saved to:

chess_scores.txt

Format:

White wins
Black wins
Draw


 How to Play the game:

Click a piece to select it

Red squares show legal moves

Click again to move

Game ends when:

Checkmate

Stalemate

Draw


 Developer of the Game

This chess game project is created by:

ADEEL AZHAR
25l-2023


 License

This project is free to use for learning and academic purposes.
