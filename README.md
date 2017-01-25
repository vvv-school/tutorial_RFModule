Tutorial on RFModule and ResourceFinder
=======================================

This tutorial will guide you to better coding using YARP RFModule and ResourceFinder. 
After doing this tutorial you will be expected to know how to write a proper YARP module, manage the module parameters and other resources. 


will guide you to the use 
- [yarp::os::RFModule] (http://www.yarp.it/classyarp_1_1os_1_1RFModule.html)
- [yarp::os::ResourceFinder] (http://www.yarp.it/classyarp_1_1os_1_1ResourceFinder.html)
- [module for yarpmanager] (http://www.yarp.it/yarpmanager.html)
- Better Cmake file 



# Tutorial
In this tutorial we will develop e YARP module called `tutorial_RFModule` to encode and decode string messages in the simplest way! The module can work as coder or encoder depending on its parameters : 
```
$ tutorial_RFModule --mode coder
$ tutorial_RFModule --mode decoder
```

It has an input port to receive text messages, an output port to steam out the encoded/decoded messages and an rpc port for the remote and runtime configuration: 

![module](/misc/tutotial_RFModule.png)

# Build and Install the code
Follow these steps to build and properly install your module: 
```
$ cd tutotial_RFModule
$ mkdir build; cd build
$ cmake ../
$ make
$ make install
```
the `make install` will install your module (binary, xml files, etc) in the icub contrib folder which is already setup on your machine. 

# Running the tutorial application
- Simply run the `yarpmanager` and open/run the `Tutorial_RFModule` application for the GUI then connect the ports: 
```
$ yarpmanager
```

# Testing the tutorial application
- In a terminal run `yarp read` 
```
$ yarp read ... /decoder/Codec/out
```
- In another terminal run `yarp write` and type something 
```
$ yarp write ... /coder/Codec/in
>> Hello Tutorial
```
You should be able to see the `Hello Tutorial` message in the `yarp read` terminal. 

- In a terminal run `yarp rpc` and type `quit`
```
$ yarp rpc /coder/Codec/rpc
>> quit
```
this will stop the the `tutotial_RFModule` which was responsible for encoding our message. Check this by pressing the **Refresh** button of the `yarpmanager` GUI. 


# Scenario 
As it is shown in the following diagram, there are two instances of the `tutotial_RFModule` is running. 
- First `tutotial_RFModule` lunched with `--mode coder` to encode the input string 
- Second `tutotial_RFModule` lunched with `--mode decoder`; it receives the input encoded message, decode it and send it out to the `yarp read` 

![application](/misc/application.png)

