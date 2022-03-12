# Create a service


Move the file `keep_me_alive.service` to `/etc/systemd/system/`
Enable and start the service with:

`systemctl enable keep_me_alive`
`systemctl start keep_me_alive`

Systemd gives up restarting if the service fails to start more than 5 times within a 10 seconds interval.
There are two `[Unit]` configuration options responsible for this:

```
StartLimitBurst=5
StartLimitInternalSec=10
```

The `RestartSec` directive also has impact on the outcome, if it's set to restart after 3 seconds then it
can never reach 5 failed retried in 10s.
The simple fix is to set `StartLimitIntervalSec=0`. This way, systemd will attempt to restart the service
forever.


## What does it mean the prepended `+`


## service fails to start due to selinux

in fedora there is selinux problems when trying to start the program.

```
Mar 06 20:09:14 host.ff systemd[1]: Started Keep me alive.
Mar 06 20:09:14 host.ff systemd[476085]: keep_me_alive.service: Failed to execute /usr/bin/keep_me_alive: Permission denied
Mar 06 20:09:14 host.ff systemd[476085]: keep_me_alive.service: Failed at step EXEC spawning /usr/bin/keep_me_alive: Permission denied
Mar 06 20:09:14 host.ff systemd[1]: keep_me_alive.service: Main process exited, code=exited, status=203/EXEC
Mar 06 20:09:14 host.ff systemd[1]: keep_me_alive.service: Failed with result 'exit-code'.
Mar 06 20:09:19 host.ff systemd[1]: keep_me_alive.service: Scheduled restart job, restart counter is at 4.
Mar 06 20:09:19 host.ff systemd[1]: Stopped Keep me alive.
```

fix:
```
restorecon -irv /usr/bin/keep_me_alive
Relabeled /usr/bin/keep_me_alive from unconfined_u:object_r:user_tmp_t:s0 to unconfined_u:object_r:bin_t:s0
```
restorecon restores default SELinux security contexts.
    - `-i`: ignore files that do not exist
    - `-r`: recursive changes
    - `-v`: show changes in files

After start the service `systemctl start keep_me_alive`

```
Mar 06 20:26:14 host.ff systemd[1]: Started Keep me alive.
Mar 06 20:26:14 host.ff keep_me_alive[477337]: Start keep alive sample app
Mar 06 20:26:14 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:19 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:24 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:29 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:34 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:39 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:44 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:49 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:54 host.ff keep_me_alive[477337]: staying alive...
Mar 06 20:26:59 host.ff keep_me_alive[477337]: Terminating application keep alive
Mar 06 20:26:59 host.ff systemd[1]: keep_me_alive.service: Deactivated successfully.
Mar 06 20:27:04 host.ff systemd[1]: keep_me_alive.service: Scheduled restart job, restart counter is at 1.
Mar 06 20:27:04 host.ff systemd[1]: Stopped Keep me alive.
Mar 06 20:27:04 host.ff systemd[1]: Started Keep me alive.
Mar 06 20:27:04 host.ff keep_me_alive[477378]: Start keep alive sample app
Mar 06 20:27:04 host.ff keep_me_alive[477378]: staying alive...
```


tools:
    - chcon
    - semanage
    - restorecon
