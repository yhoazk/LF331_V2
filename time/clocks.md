# Clocks in linux



## System time

This time is handled by the OS, when a process queries for the time this is the
returned time. It's being stored in RAM which makes a fast read.

## Hardware time

This is normally a RTC hardware mantained by a battery, which makes persist after
reboot. As the HW needs to be queried when the time is asked, the operation takes
longer and uses more system resources than reading the system time.

The HW clock can be set to local time or UTC, while the system clock is always
set to UTC.

It's recommended to keep the HW clock in UTC if the system is linux only, when
is a multiboot system use local time as windows  systems assumes the HW clock
runs in local time by default this is also true for virtual HW. Both virtual and
physical HW need to use the same either local or UTC.

### Syncing system and hardware time

The HW and system clock can be synced in both directions.
Either set the HW clock to the system time:

```
hwclock --systohc
```

This is not needed if the system is using ntp, which syncs every 11 min, this
command is reasonable initial system time.
To set the system time from the HW clock:

```
hwclock --hctosys
```

To choose to use the local or UCT time for the HW clock add the option `--utc`
or the option `--localtime`. This is recorded in `/etc/adjtime`


### When are they used?

The HW clock is read at boot then is synced by the OS/systemd

### How linux keeps track of the time




### `/etc/adjtime`

This file contains the information about HW clock and drift factor.
Its read by `hwclock` and other programs like `rtcwake` to get RTC time mode.

HW clock are not accurate, but their inaccuracy is predictable, the predictable
drift is also called systematic drift. This drift is modeled and the parametrs
for the model are stored in `/etc/adjtime` 

#### Example:

```
cat /etc/adjtime
0.000000 1589051314 0.000000
1589051314
UTC
```

The first line:

```
drift_factor last_adjust adjust_status
```
Second line is the last calibration time, which counts the seconds since epoch
of the most recent calibration. Zero if it has not been calibrated yet or the
the HW clock failed since the last calibration.

Third line is the clock mode which is a string either `UTC` or `LOCAL` for
UTC is for the coordinated Universal time or local time. UTC never changes
regardless of the day time savings therefore is the preferred.

UTC is the successor to Greenwich Mean Time (GMT). GMT is based on astronomical
observations while UTC is based on atomic clocks.

The mean solar time at the royal observatory in Greenwich on Easter london.
When the sun is at its highest point exactly above Greenwich, it is 12 noon GMT
except: The earth spins unevenly, so 12 noon is defined as the anual average, 
mean of when the sun is at its highest. In GMT there can never be any leap second
because rotation of earth does not leap.

In UTC a second always has the same length, Leap seconds are inserted in UTC
to keep UTC and GMT to difting apart. By contrast, in GMT the seconds are
streched as necessary, so in theory they do not have the same length.




- - -

1 [UTC](Coordinated Universal Time)
2 [SyncHwSys](https://docs.fedoraproject.org/en-US/Fedora/23/html/System_Administrators_Guide/sect4-synchronizing-date-time-hwclock.html)
* 