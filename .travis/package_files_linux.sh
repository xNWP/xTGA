rm -rf build
mkdir packages && cd packages
mkdir buildDebug && mkdir buildRelease

echo Building 64-bit Targets...
cd buildDebug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
cmake --build . --target xTGA --config Debug
cmake --build . --target xTGAs --config Debug
cp libxTGA_d.so libxTGAs_d.a ..

cd ../buildRelease
cmake -DCMAKE_BUILD_TYPE=Release ../..
cmake --build . --target xTGA --config Release
cmake --build . --target xTGAs --config Release
cp libxTGA.so libxTGAs.a ..
echo Done.

echo Packaging 64-bit Targets...
cd ..
cp -ar ../xTGA/include ./include
cp ../LICENSE .
rm -f ./include/xTGA/.gitignore
rm -rf buildDebug buildRelease
tar -czf xtga-$1-linux-x64.tar.gz *
rm -f libxTGA.so libxTGA_d.so libxTGAs.a libxTGAs_d.a
echo Done.

echo Builing 32-bit Targets...
mkdir buildDebug && mkdir buildRelease
cd buildDebug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=x86-toolchain.cmake ../..
cmake --build . --target xTGA --config Debug
cmake --build . --target xTGAs --config Debug
cp libxTGA_d.so libxTGAs_d.a ..

cd ../buildRelease
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=x86-toolchain.cmake ../..
cmake --build . --target xTGA --config Release
cmake --build . --target xTGAs --config Release
cp libxTGA.so libxTGAs.a ..
echo Done.

echo Packaging 32-bit Targets...
cd ..
rm -rf buildDebug buildRelease
tar -czf xtga-$1-linux-x86.tar.gz -- libxTGA.so libxTGA_d.so libxTGAs.a libxTGAs_d.a include LICENSE
rm -rf libxTGA.so libxTGA_d.so libxTGAs.a libxTGAs_d.a include LICENSE
echo Done.
