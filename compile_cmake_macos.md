MacOS compilation instructions (error checking ONLY):

1. Install MacPorts (if not already)

2. Install clang and OpenMP through MacPorts:
``` bash
sudo port install g++-15
sudo port install libomp
```

3. Clone the repository in your folder of choice, e.g.:
``` bash
cd ~
git clone https://github.com/Tetris2102/Nuclear_Lego
```

4. Compile using CMake:
Intel MacOS:
``` bash
mkdir Nuclear_Lego/Particle_transport/build
cd Nuclear_Lego/Particle_transport/build
export DCMAKE_CXX_COMPILER=/usr/local/bin/g++-15
cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-15 ..
make
```

Apple Silicon MacOS:
``` bash
mkdir Nuclear_Lego/Particle_transport/build
cd Nuclear_Lego/Particle_transport/build
export DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-15
cmake -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-15 ..
make
```

Please note this is for debugging only and you will anyway receive an error:
.../pi5_main.cpp:8:10: fatal error: linux/i2c-dev.h: No such file or directory
    8 | #include <linux/i2c-dev.h>
      |          ^~~~~~~~~~~~~~~~~
