A City builder simulation game made with sfml/c++.

![Alt Text](https://github.com/msearles25/city-builder/blob/main/citybuilder.gif)

# How to compile

### Visual Studio 

1. Download SFML 2.5.1 32-bit from: https://www.sfml-dev.org/download/sfml/2.5.1/
2. Go to project propereties.
3. C/C++ -> General -> Additional Include Directories, set this to your SFML-2.5.1\include folder.
4. Linker -> General -> Additional Library Directories, Set this to your SFML-2.5.1\lib folder.
5. Linker -> Input -> Additional Depenedcies, add all of these:  
sfml-graphics-d.lib;
sfml-window-d.lib;
sfml-system-d.lib;
sfml-network-d.lib;
sfml-audio-d.lib;
6. Grab the .dll's from your SFML-2.5.1\bin folder, put them into the CityBuilder folder.