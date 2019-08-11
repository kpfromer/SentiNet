# Build instructions


Dependencies (most of these you can install with apt or pacman or yum ...)

  - make
  - cmake
  - libcurl (curl - I think you need to install seperate headers if you want lib curl)
  - zmq (Build this from source - [How to build lib zmq](http://zeromq.org/build:_start))
  - No Ros yet, but in the future, you'll need ros / ros2
  - protobuf (Build this from source [Protobuf master branch](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md))
  - yaml-cpp (Should be installable via apt, on arch, it's on the AUR I haven't checked, otherwise this is really easy to build from source [here] (https://github.com/jbeder/yaml-cpp))
  
**Some of the above, I'll add as vendor libraries so that they're build on make of this project, but I haven't done that yet**


## To build
``` bash 
$ make
# that's it!!
```
 
Artifacts are placed in ./build/x86_64/bin
``` bash
$ ./build/x86_64/bin/<example name>
```

Later, I'm going to add a rule to cmake to put examples in a different directory, but for now, bin is just a conglomeration of all the tests / executables I wrote (all found in ./cpp/src/impl/)

#  Directory structure

## cpp
This is the main directory, you can pretty much ignore everything else for now
***
*include*:
All the header files and framework stuff
Most important things

./cpp/include/framework/control
./cpp/include/networking/zmq
./cpp/include/framework/utils
./cpp/include/networking/curl

Everything else in include is just scratch mostly

*src*:
All the implimentation files
./cpp/src/libs - implimentation of the libraries
./cpp/src/impl - the executables that are compiled

*models*:
In the future, these are going to house all proto files (https://developers.google.com/protocol-buffers/docs/proto)
There's one debug module here right now
