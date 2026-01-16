MacOS compilation instructions (error checking ONLY):

1. Install MacPorts (if not already)

2. Install clang and OpenMP through MacPorts:
``` bash
sudo port install cmake-18
sudo port install libomp
```

3. Clone the repository in your folder of choice, e.g.:
``` bash
cd ~
git clone https://github.com/Tetris2102/Nuclear_Lego
```

4. Compile using CMake:
``` bash
mkdir Nuclear_Lego/Particle_transport/build
cd Nuclear_Lego/Particle_transport/build
cmake ..
make
```
