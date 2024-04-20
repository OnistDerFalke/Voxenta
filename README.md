# Voxenta

Voxenta is a basic image processing UI tool made with ImGui, OpenGL and OpenCV with simple interface and effects descriptions. It was created to make things easier with image processing to use ready-made UI environment and focus only on the effects creation in C++ using OpenCV (default) or other libraries.
Application was tested on Linux but it should also work after building it for Microsoft Windows environment. 

## Requirements

### Fedora Linux

```bash
glfw glfw-devel opencv opencv-devel dbus dbus-devel xdg-desktop-portal
```

### Arch Linux

```bash
glfw opencv vtk hdf dbus xdg-desktop-portal 
```

## Adding new effects
Voxenta already contains some class examples that you can study to understand how to add new effects in *source/voxenta_effects/src/effects* directory.

1. Prepare the effect class (need to inherit from *effect* class, study example effects).
2. Put it into *source/voxenta_effects/src/effects* directory.
3. Use **Effects -> Reload effects** button or **CTRL+R** shortcut to reload. It works in runtime.
4. If your effect was correctly prepared and has no errors, it should appear in dropdown options list.

![image](https://github.com/OnistDerFalke/Voxenta/assets/75864407/33c99892-860f-42ad-8311-7f40bbebdbbc)


## How does it work?

### Basic processing
You can load an image file that is supported by OpenCV with **File -> Load (CTRL+O)**. It will be shown in left image window. Then choose the effect from dropdown and set it's properties. It will be automatically applied to the image and shown in the right window. Then you can save it using **File->Save (CTRL+S)** option from menu. Use image file extension supported by OpenCV.

### Effects chaining
You can apply more than one effect. Just use **Effects -> Apply (CTRL+A)** shortcut to create a new chain element. Now processed image will appear on left window. You can undo this operation with **Effect -> Undo (CTRL+Z)**. The saved image is one that is visible on **right screen** so remember not to apply the last effect.

### Runtime effects modification
Now you can modify your effect file and use **Effects -> Reload effects (CTRL+R)** to reload changes in runtime. It grants fast and usefull tool to experiment with effects with no need to rebuild the project.


![image](https://github.com/OnistDerFalke/Voxenta/assets/75864407/291ce29c-0b6b-4e7e-ace1-4752c7ba1d7b)

## License
This application is provided under the terms of the **MIT License**, which means you have the right to copy, modify, and adapt our source code to your needs without obtaining my permission. 
However, please ensure to retain appropriate copyright notices and comply with the terms of the license.

## Contact
For any questions contact me on Discord: *onist*.
