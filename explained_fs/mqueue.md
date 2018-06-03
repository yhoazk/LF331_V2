# mqueue

mqueue are POSIX message queues, they allow process to exchange data in the
form of messages. 

Messages queues are crested an opened using `mq_open()`, this function returns
a message queue descriptor, which is used to refer to the open message queue in
later calls.

Each message queue is identified by a name of the form `/somename`, that is a 
null terminated string of up-tp `NAME_MAX` (255) chaaracters, consisting of an
initial slash followed by one or more chars, none of them a slash.

Two processes can operate on the same queue by passing the same name to mq_open.
Messages are

