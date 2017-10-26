# Audio mixing, audio synthesis and audio modulation


This project is the basis of my BSc. thesis at Eötvös Loránd University, Faculty of Informatics.

The project is written in C++ using the C++14 standard. The code itself is platform-independent and the project's dependencies can be found on most platforms, but the building environment has been made for and tested on only Linux.

The project uses CMAKE for building, and contains the following targets:

- **Szakdolgozat**, a dynamic library that provides the functinality described in the documentation
- **doc**, which will generate the documentation with Doxygen and Dot
- **Test**, which will compile a unit testing binary with GTEST
- **coverage**, which will make a code coverage report of the Test binary

Each target has dependencies, be they dynamic libraries or executables.

These dependencies are:

- **Szakdolgozat:** libogg, libvorbis, libvorbisenc, libportaudio
- **doc:** Doxygen, dot
- **Test:** libogg, libvorbis, libvorbisenc, libportaudio, libgtest
- **coverage:** lcov, gcov, gcovr, genhtml

To use the library made with this project, follow the standard CMAKE installation procedure, that is:

- make a `build/` folder in the project directory
- go into `build/`
- run `cmake ..`
- run `sudo make install`

This will install the header files and compiled dynamic library into your default folders.

After the installation is complete, test by compiling one of the example files from `example/`.

Example command for compiling an example: `g++ 08\ -\ simple\ note.cpp -lSzakdolgozat`.