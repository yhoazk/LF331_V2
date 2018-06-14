# Handle signals

Signals are one of the first IPC. Signals are notifications sent by the kernel
to some process or one of its threads.
The "normal" signals g


Signals interrupt the programs normal flow, this means that in order to handle
a signal. The process of one of its threads stops and temporariliy switches to
the signal handler. Since kernel 2.6 most signals interrupt only one thread and
not the complete process. Moreover, the signal handler can be interrupted by 
other signals. Just like in embedded systems, some interrupts can be handled 
and others cannot.

The signals are grouped in "normal" signals and real-time signals. 

Real time signals are distinguished by the following:
1. The real time signals are queued, while if multiple instances of std signals
   are delivered while the signal is currenlty blocked, only one instance is 
   queued.
2. Real time signals are delivered in guaranteed order. They have priority though.
   the lower-numbered the signal the highest the priority.

To have a list of the regular signals, use the command
```
/bin/kill -L
```
The signals can be grouped as interrupts are:
-



Signals can be masked

TODO: show how to disable other signals when only one signal has to be handled.
TODO: 
