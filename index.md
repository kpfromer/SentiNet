# Sentinet

> *Sentinet* : A sentient, "living" "breathing" automation network, similar to the
> *"internet"* but for automation and robotics, allowing robots and large scale
> network connected systems the freedom of loading control systems (goals / objectives
> in the form of modular packages) and run them on their native hardware with
> native drivers

## *Summary*
Sentinet is an approach to modularize robotics and automation. It does this by providing a means of loading three different broad categories of automation pieces: control, message and peripheral modules, during runtime of a robot's lifetime. These modules are able to be hosted the same way we host plain text html pages. Each module is loaded via the system and attatched to one of the various interfaces implemented in sentient.

***

## *Three layer approach*
Sentinet divides its api into three main categories, Control, Message and Peripheral layers. The control layer is in charge of large scale goals or objectives. This could be a neural network, a state machine, or a simple bluetooth remote control interface. All the controller module needs to be able to do is communicate with the robot. This communication mechanism is abstracted, however, typically would rely on a message transport framework (zero mq and ros2 are our main ones).  The control module does **not** however need to provide the *exact* message type the robot requires. It can be any type of message a developer would like, as long as within that message, there is some sort of instruction that the robot can follow. This is where the message layer comes into play. At a low level, with tight latency requirements and hard time execution, message pipes do nothing. Although they exist to relay information to the low level code layer, a control module that is highly tuned to its low level counterpart simply sends its messages to the low level layer without interacting with message pipes. However, the idea of sentinet is to allow developers the ability to load whatever control modules they want into the same robot. So the fact that one control module sends messages in the time domain, while the robot accepts messages in the frequency domain, means that this control module would be incompatible for the given robot. This is not the case for senti net. Message pipes are simply message transformations. At a low level, they're just equations that convert an incoming message to individual motor frequencies. However, due to their abstract nature, message pipes can be as complex as language processesing trees that interpret incomming strings (commands) as instructions for each motor. This is also known as the interpretation layer. Finally, messages are relayed in a pleasant form for the low level, "peripheral layer". Peripherals are abstracted in a way that allows developers the ability to interact with hardware as well as abstract drivers. 

This three layer approach is meant to be as open to developers as possible. It is elegant in its simplicity as it only plugs in each layer to interface with the next layer. 

### *Control Layer*
***
The Control Layer does shiz
### *Interpretation Layer*
***
The Interpretation layer does other shiz
### *Peripheral Layer*
***
The peripheral layer does a lot of shiz
