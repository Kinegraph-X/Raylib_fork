cmake -S .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="C:\Production Data\Git\projets_Cpp_Lib\RayLib" -DUSE_EXTERNAL_GLFW=OFF -DPLATFORM=Desktop
&& cmake --build . --config Debug 
&& cmake --install . --config Debug

cmake --build . --target clean