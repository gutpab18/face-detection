# Prerequisites
1. Have Brew installed on Mac
2. C++ should be installed and you should be able to run the 'cmake' command-- many IDEs come with this installed automatically

# Setup Instructions

1. Install OpenCV locally for mac
```bash
brew install opencv
```
2. Set the correct paths for the training data inside of the main.cpp file in the main() method (line 101)

# Run the program
I recommend using an IDE like CLion or Visual Studio Code to automatically build and run your C++ code, it's much easier that way. You should be able to click
the 'Play' button on the top of your IDE to automatically build/execute the C++ code.

In case you need to run it manually, you can do something like this (not exactly like this, you will have to tweak for your environment):

```cpp
cmake --build /Path/to/project/face-tracking/cmake-build-debug --target FaceTracking -j 10
```
This will build an executable for the program called 'FaceTracking' inside of the cmake-build-debug/ folder. Then you can go into that folder and run the executable:

```bash
cd cmake-build-debug/
FaceTracking
```
Basically, you use cmake to build the project, then you can run the executable that is generated inside of the cmake-build-debug folder in your project