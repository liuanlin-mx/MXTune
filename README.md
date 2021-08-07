# MXTune
## hot key
alt+mouse wheel: x zoom
ctrl+mouse wheel: y zoom
(mouse wheel) or (W, S): y move
(shift+mouse wheel) or (A, D): x move


(pitch_detector_talent.cpp,pitch_shifter_talent.cpp,auto_tune.cpp)The core algorithm comes from Tom Baran's Autotalent(http://web.mit.edu/tbaran/www/autotalent.html) and  TalentedHack(http://code.google.com/p/talentledhack/)
JUCE is licensed under the GPL v3 or a commercial license. See juce.com for more details.

# build

## windows(msys2)

### msys2
```
pacman -S mingw-w64-x86_64-toolchain
pacman -S make cmake autoconf automake-wrapper libtool mingw-w64-x86_64-python3 mingw-w64-x86_64-waf mingw-w64-x86_64-fftw

```

### JUCE
Download Juce (https://github.com/juce-framework/JUCE  5.4.7)
modify: JUCE/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp:2442
```
/*
    extern "C" BOOL WINAPI DllMain (HINSTANCE instance, DWORD reason, LPVOID)
    {
        if (reason == DLL_PROCESS_ATTACH)
            Process::setCurrentModuleInstanceHandle (instance);

        return true;
    }
*/
```

run Projucer.exe
File->Open MXTune/JUCE/mx_tune.jucer
File->Global Paths   modify "Path to JUCE" and "JUCE Modules"
File->Save All


### VST SDK
Download the VST SDK (http://www.steinberg.net/en/company/developers.html)
copy vstsdk2.4/pluginterfaces to VST_SDK/VST3_SDK/
copy VST_SDK/VST3_SDK to MXTune/

modify:
```
VST3_SDK\base\source\fstring.cpp:226
#define vsnprintf _vsnprintf
->
//#define vsnprintf _vsnprintf
```

### build SoundTouch

```
./bootstrap 
./configure --prefix=/mingw64 --enable-static --disable-shared
make CXXFLAGS="-DSOUNDTOUCH_PREVENT_CLICK_AT_RATE_CROSSOVER=1"
make install
```

### build aubio
```
python3 /mingw64/bin/waf configure --enable-fftw3f --disable-tests --disable-examples --disable-wavread --disable-wavwrite --prefix=/mingw64
python /mingw64/bin/waf install -j4
``

### build MXTune

mkdir build-cmake
cd build-cmake
cmake .. -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -G "Unix Makefiles"
make -j4