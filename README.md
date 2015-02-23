Revertris
=========

A Mini LD #57 production - by Ace

What is Revertris
-----------------

Revertris is a game based on the famous classic of Tetris, except done in reverse.
You start the game at the game over screen of Tetris with a board filled with failed rows of blocks, you then get to unplay the game while trying to score as many points as possible out of the colored pieces you have at your disposal.

Requirements
------------

You need a C++11 capable compiler to compile this game, it's been mainly tested on Visual Studio 2013 though it should work fine on 2012 and 2015 too.
On Linux, GCC 4.8.3 and Clang 3.5 are the only ones I've tested - though it should compile fine on earlier versions.

You'll also need a copy of OpenSceneGraph, I've only tested it with 3.3.4 but any 3.2.x version should hopefully work. The only parts of OSG that are completely necessary are osg, osgDB, osgGA, osgText, osgUtil, and osgViewer. You'll need the osgDB plugins for lua and tga as well as serializers for osg, osgGA, osgText, osgUtil, and osgViewer.

Compiling
---------

The CMake script should do all the hard work for you, so you only need to generate a suitable build environment and compile it. Necessary resources will be copied into the binary folder for you.
