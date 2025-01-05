conan install . -sbuild_type=Release -of=out/build/deps/conan/ --build=missing
cd out/build
cmake ../.. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="deps\conan\conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --verbose
cd Release
NES.exe