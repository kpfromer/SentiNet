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



# Tutorial

The pub sub framework for sentinet is very simple. Here is an example publisher subscriber (in the same process)

For more examples, see cpp/src/impl/examples

Of course, this could be in two seperate processes. One for a publisher and one for a subscriber, this is just demonstrating the strength of a single Control Client instance

This example uses publisher subscriber, but this can also be serve request (will add an example in the future, somewhat more complex here)

```cpp
// Include Control Client (ZMQ Is just 1 implimentation of CC)
#include "networking/zmq/ZMQControlClient.hpp"

int main() {

  // Create a new ZCC instance
  auto a = std::make_unique<ZMQControlClient>();

  // Values to publish need to be ~global or else they are created everytime a publisher wants to publish
  std::string value_to_publish("Hi there");

  // parameters are (Address (soon won't be required), Topic, Callback function, period)
  /*
    a->publish("Topic name", value_to_publish); // In the future it will be this easy. This is like a 5 minute addition
   */
  a->publish("tcp://*:5555", "Topic name",
      [&]() -> std::string& {
        return value_to_publish;
        }, std::chrono::seconds(1));

  // parameters are (address to connect to (soon won't be required), Topic, callback)
  a->subscribe("tcp://localhost:5555", "Topic name", 
      [&](const std::string& value) -> void {
        std::cout << "Recieved "<<value<< " on topic Topic name"<<std::endl;
        });

  // Since this is inherently multithreaded, you need to have something else going on, a simple while(1) works too
  sleep(10);

  // ALWAYS quit. I'm thinking about adding this to the desctructor, but for now, quit, this terminates threads and stops callbacks
  a->quit();

  return 0;

}
```







