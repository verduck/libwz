# libwz
libwz is library for reading MapleStory WZ files.

## Install Required Dependencies

Before building the project, ensure that the following dependencies are installed:

- [CMake](https://cmake.org/download/)
- [vcpkg](https://github.com/microsoft/vcpkg)
- [Ninja](https://ninja-build.org/)

---

* macOS
```sh
brew install cmake ninja
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg
./bootstrap-vcpkg.sh
```

* Linux (Ubuntu/Debian)
```sh
sudo apt update
sudo apt install cmake ninja-build
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg
./bootstrap-vcpkg.sh
```

* Windows (PowerShell)
```powershell
winget install Kitware.CMake
winget install Ninja-build.Ninja
git clone https://github.com/microsoft/vcpkg.git $env:USERPROFILE/vcpkg
cd $env:USERPROFILE/vcpkg
.\bootstrap-vcpkg.bat
```

## Build

* macOS
```sh
cmake --preset default-mac
cmake --build build
```

* Linux (Ubuntu/Debian)
```sh
cmake --preset default-linux
cmake --build build
```

* Windows (PowerShell)
```powershell
cmake --preset default-windows
cmake --build build
```