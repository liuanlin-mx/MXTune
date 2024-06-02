set -e
mkdir -p third_party
pushd third_party
mkdir -p build
rm build/* -rf


#wget -c https://waf.io/waf-2.0.5 -O waf && chmod +x ./waf

#wget -c https://github.com/libsndfile/libsamplerate/releases/download/0.1.9/libsamplerate-0.1.9.tar.gz
tar xvf libsamplerate-0.1.9.tar.gz -C build
pushd build/libsamplerate-0.1.9
./configure CFLAGS=-fPIC LDFLAGS=-fPIC
make -j4
sudo make install
popd


#wget -c https://aubio.org/pub/aubio-0.4.9.tar.bz2
tar xvf aubio-0.4.9.tar.bz2 -C build
pushd build/aubio-0.4.9/
./waf configure --enable-fftw3f --disable-tests --notests --disable-examples --disable-wavread --disable-wavwrite CFLAGS='-fvisibility=hidden -fdata-sections -ffunction-sections'
./waf build --disable-tests --notests --disable-examples
sudo ./waf install --disable-tests --notests --disable-examples
popd


#wget -c https://www.surina.net/soundtouch/soundtouch-2.3.3.tar.gz
tar xvf soundtouch-2.3.3.tar.gz -C build
pushd build/soundtouch
cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-DSOUNDTOUCH_PREVENT_CLICK_AT_RATE_CROSSOVER=1 -fvisibility=hidden -fdata-sections -ffunction-sections -fPIC"
sudo make install
popd


#wget -c https://breakfastquay.com/files/releases/rubberband-3.3.0.tar.bz2
tar xvf rubberband-3.3.0.tar.bz2 -C build
pushd build/rubberband-3.3.0
sed -i 's/-O3 -ftree-vectorize/-O3 -ftree-vectorize -fPIC/g' otherbuilds/Makefile.linux
sed 's/\%PREFIX\%/\/usr\/local/g' rubberband.pc.in > rubberband.pc
make -f otherbuilds/Makefile.linux
sudo cp -rf rubberband /usr/local/include/ 
sudo cp -rf lib/* /usr/local/lib/
sudo cp rubberband.pc /usr/local/lib/pkgconfig/rubberband.pc
popd

#wget -c https://github.com/juce-framework/JUCE/releases/download/7.0.5/juce-7.0.5-linux.zip
unzip juce-7.0.5-linux.zip -d build/
./build/JUCE/Projucer --set-global-search-path linux defaultJuceModulePath $(pwd)/build/JUCE/modules 
./build/JUCE/Projucer --resave ../JUCE/mx_tune.jucer 

popd

set +e
#git clone --recursive https://github.com/steinbergmedia/vst3sdk.git VST3_SDK
set -e
pushd VST3_SDK
git checkout v3.7.1_build_50
git submodule update --recursive
popd


mkdir -p build-cmake
pushd build-cmake
cmake .. -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -G "Unix Makefiles"
make -j4
popd