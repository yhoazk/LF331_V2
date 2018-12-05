# GDB Checkpoints

Useful to save the state of the program when we are close to the failure point.
This way is not needed to restart the stepping process nor the possible state
of the system.

To create a checkpoint use the command: `checkpoint`<br>
This command does not take any argument. Returns an id so we can return to that
point by invoking `restart checkpoint-id`. 

- List all the checkpoints: `info checkpoints`
- Deleta a checkpoint: `delete checkpoint checkpoint-id`

The changes stored include OS status, File pointers are reseted to their values
then older values are overwritten.

A checkpoint is an identical copy of the process, this means that the address
randomization is the same in this checkpoint. This is only valid for GNU/Linux.