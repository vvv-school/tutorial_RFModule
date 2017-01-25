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

