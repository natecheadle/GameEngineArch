# Ignosi Game Engine
As a learning excercise I am building a simple game engine. This is built in C++ using CMake and all the code should compile and run on Windows/Mac/Linux, but I only personally build and run on Windows/MSVC and Apple Silicon/Clang.

The inspiration for this project is Serenity O/S in that I strive to build a lot of the pieces of the game engine myself even if libraries exist that could do it better. This will mostly be focused on things I find interesting. So you will likely see libraries for things I find boring. It will be sort of random in this way and mostly based on my personal preferences.

The general architecture will try to emulate some of the concepts of Linux and Boost Developement. The core of the engine will be a set of libraries like in boost with each doing a specific task and ideally having minimal dependencies. For QOL utilities instead of building a full IDE integration like Unity/Unreal/Godot I will instead build many small utilities that do one task well. As an example I might build a utility that only previews models given a specific set of textures and shaders. 

It is called Ignosi Game Engine because I as someone with minimal game development experience am using this a learning opportunity. (i gnósi is greek for knowledge). I would generally not recommend at this time using the code in other projects because you can likely find better implementations elsewhere. I also don't plan on accepting pull requests for changes at this time. If there is a simple bug fix you want added I am open to it, but for now I am not looking to run an open source project.

From a licensing perspective it is licensed under the MIT license, but I wouldn't put much stock into this choice. Basically if you want to use the code you can, but I wouldn't recommend it. Also I might use more restrictively licensed libraries which might negate this license. Essentially again use at your own risk.
