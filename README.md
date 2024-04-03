# Voxenta

Voxenta is a basic image processing UI tool made with ImGui, OpenGL and OpenCV with simple interface and effects descriptions. It was created to make things easier with image processing to use ready-made UI environment and focus only on the effects creation in C++ using OpenCV (default) or other libraries.
Application was tested on Linux but it should also work after building it for Microsoft Windows environment. 

## Requirements
Commands for Fedora Linux

OpenCV:
**$** sudo dnf install opencv opencv-devel

OpenGL:
**$** sudo dnf install libglfw3-dev glfw-devel glew-devel SDL2-devel SDL2_image-devel glm-devel freetype-devel freeglut glibc-devel

## Ready to use
Voxenta already contains some examples that you can study to understand how to add new effects. It works as it is shown below:

![Screenshot_20240403_235244](https://github.com/OnistDerFalke/Voxenta/assets/75864407/28591582-4f19-4e11-b5c9-0dd82db66c33)

## How does it work?
You can load an image file that is supported by OpenCV. It is shown in left image window. Then choose the effect and it's numeric properties. It will be automatically applied to the image and shown in the right window. 
Then you can save it using *File->Save* option from menu. Use image file extension supported by OpenCV.

![image](https://github.com/OnistDerFalke/Voxenta/assets/75864407/3d039871-3b82-4005-9a10-a964bb409278)

## Adding new effects
To add an effect you have to:
1. Update the number of effects in *properties_window.h -> features*.
2. Add the feature name to the vector in properties_window.cpp constructor.
3. Add UI method to processing UI and add it's pointer to the functions vector in constructor.
4. Add processing method to processing_methods and add it's pointer to the functions vector in constructor.
5. Remember to save UI state and set your default values as it is done in examples.

## Licence
This application is provided under the terms of the **MIT Licence**, which means you have the right to copy, modify, and adapt our source code to your needs without obtaining my permission. 
However, please ensure to retain appropriate copyright notices and comply with the terms of the license.

## Contact
For any questions contact me on Discord: *onist*.
