# umgmt
C library for user management on linux systems.

# Build
Use the following set of commands to build the project:
```sh
git submodule init
git submodule update
mkdir build; cd build
cmake .. # setup CMAKE_INSTALL_PREFIX if needed
make -j
```

# Documentation
The files are documented using doxygen comments - use doxygen to build documentation:
```sh
mkdir docs
doxygen Doxyfile
```
Use doxygen or browse ```.h``` files for an API description.