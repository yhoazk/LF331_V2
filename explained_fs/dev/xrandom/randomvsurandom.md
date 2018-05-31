# diff /dev/random /dev/urandom

* `/dev/random/`: blocking
* `/dev/urandom/`: non-blocking


## `/dev/random`
Returns random bytes only when sufficient amount of entropy has been collected.
If there is no entropy to produce the requested number of bytes, the device
blocks until more entropy can be obtained. This interfaces is more secure
according to some, more insecure according to others. What is clear is that 
it will block if someone is consuming all the entropy.

If `/dev/random` is ised instead of `urandom` the program will unpredictably
(or, of you are an attacker, very predictably) hang when there's no more 
of that entropy. Then `random` will make the program less stable, but not 
more cryptographically safe.

## `/dev/urandom`

Returns bytes regardless of the amount of entropy available. It does not block
a read request due to lack of entropy


## From where comes this entropy?

## Why should we care?

`/dev/urandom` is used for TCP sequence numbers to avoid spoofed packets.
The entropy is finite in the random devices, both come from the same entropy
source, then if `random` gets starved `urandom` also starves.

A remote attack:
  - Generate as may TCP connections and empty the primary pool.
    - each new TCP connection calls `random` to start the packet sequence.
      this sequence is unique for each connection and its supposed to be 
      consecutive from packet to packet.

