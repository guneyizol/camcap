**requires opencv, zeromq, and cmake**

## build & run instructions
```
$ cd videoserver
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ./CamCap
```

```
$ cd videoclient
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ./PrintToScreen
```

## after running videoclient
- press 'q' to quit
- press 'g' for grayscale video
- press 'c' for colored video

## after running videoserver
- CTRL + C for quitting videoserver
