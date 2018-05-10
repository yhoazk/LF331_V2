# Linux HPET support

https://blog.fpmurphy.com/2009/07/linux-hpet-support.html

IA-PC HPET (high precision event timer) is a specification which was jointly 
developed by intel and Microsoft. They defined a set of timers that can be 
used by the OS. A timer block is a combination of a single counter and up to 32
comparators and match registers. The comparator compares the content of the 
match register and a free running monotonic up-counter. When the values are 
equal a interruption is generated. Each comparator can output an interrupt.
A max of 8 timer blocks are 



The BIOS uses ACPI (advanced configuration and power interfaces) functionality
to inform the oprating system of the location of the HPET memory mapped
register  space. 

Example:
```
sudo cat /sys/firmware/acpi/tables/HPET > /tmp/hpet.out
iasl -d /tmp/hpet.out
cat /tmp/hpet.dsl
/*
 * Intel ACPI Component Architecture
 * AML Disassembler version 20090123
 *
 * Disassembly of /var/tmp/hpet.out, Sun Jul  5 19:34:47 2009
 *
 * ACPI Data Table [HPET]
 *
 * Format: [HexOffset DecimalOffset ByteLength]  FieldName : FieldValue
 */

[000h 000  4]                    Signature : "HPET"    /* High Precision Event Timer table */
[004h 004  4]                 Table Length : 00000038
[008h 008  1]                     Revision : 01
[009h 009  1]                     Checksum : CE
[00Ah 010  6]                       Oem ID : "INTEL "
[010h 016  8]                 Oem Table ID : "DX48BT2 "
[018h 024  4]                 Oem Revision : 0000076E
[01Ch 028  4]              Asl Compiler ID : "MSFT"
[020h 032  4]        Asl Compiler Revision : 01000013

[024h 036  4]            Hardware Block ID : 8086A301

[028h 040 12]         Timer Block Register : 
[028h 040  1]                     Space ID : 00 (SystemMemory)
[029h 041  1]                    Bit Width : 00
[02Ah 042  1]                   Bit Offset : 00
[02Bh 043  1]                 Access Width : 00
[02Ch 044  8]                      Address : 00000000FED00000

[034h 052  1]              Sequence Number : 00
[035h 053  2]          Minimum Clock Ticks : 0001
[037h 055  1]        Flags (decoded below) : 00
                              Page Protect : 0
                           4K Page Protect : 0
                          64K Page Protect : 0
Raw Table Data

  0000: 48 50 45 54 38 00 00 00 01 CE 49 4E 54 45 4C 20  HPET8.....INTEL
  0010: 44 58 34 38 42 54 32 20 6E 07 00 00 4D 53 46 54  DX48BT2 n...MSFT
  0020: 13 00 00 01 01 A3 86 80 00 00 00 00 00 00 D0 FE  ................
  0030: 00 00 00 00 00 01 00 00 
```

The description of the table is in the page 30 of the specification manual.
Note that only one event timer block need to be described in the HPET table
in order to bootstrap an OS. In that case the HPET replaces the 8245/RTC 
periodic interrupt.

Other event time blocks are described in the ACPI namespace. 
Check the DMESG output to discover how the timer is being configured by the 
OS.

```
dmesg | grep -i HPET
[    0.000000] ACPI: HPET 0x000000009CFF7000 000038 (r01 LENOVO DX48BT5    00000001 ACPI 00040000)
[    0.000000] ACPI: HPET id: 0x8086a201 base: 0xfed00000
[    0.000000] clocksource: hpet: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 133484882848 ns
[    0.000000] hpet clockevent registered
[    0.031081] DMAR-IR: HPET id 0 under DRHD base 0xfed9f000
[    0.700459] hpet0: at MMIO 0xfed00000, IRQs 2, 8, 0, 0, 0, 0, 0, 0
[    0.700463] hpet0: 8 comparators, 64-bit 14.318180 MHz counter
[    0.702492] clocksource: Switched to clocksource hpet
[    3.636765] rtc_cmos 00:02: alarms up to one month, 242 bytes nvram, hpet irqs
```

The files generating this output are: 
* `.../arch/x86/kernel/acpi/boot.c` When the ACPI HPET table is mapped.
* The second line is when the legacy interrupts are activated and HPET is registered
as a global tick source.
* `.../drivers/char/hpet.c` shows allocations to the timers


There is more information about hpet in the `/proc/time_list` file:

```
Tick Device: mode:     1
Broadcast device
Clock Event Device: hpet
 max_delta_ns:   149983013277
 min_delta_ns:   13410
 mult:           61496111
 shift:          32
 mode:           1
 next_event:     9223372036854775807 nsecs
 set_next_event: hpet_legacy_next_event
 shutdown: hpet_legacy_shutdown
 periodic: hpet_legacy_set_periodic
 oneshot:  hpet_legacy_set_oneshot
 resume:   hpet_legacy_resume
 event_handler:  tick_handle_oneshot_broadcast
 retries:        0

tick_broadcast_mask: 00
tick_broadcast_oneshot_mask: 00
```

Even more information can be found in the output of `/proc/sys/dev/hpet` and 
`/proc/driver/rtc`.

```
cat /proc/sys/dev/hpet/max-user-freq 
64
at /proc/driver/rtc 
rtc_time	: 15:20:16
rtc_date	: 2018-05-10
alrm_time	: 05:00:13
alrm_date	: 2018-05-11
alarm_IRQ	: no
alrm_pending	: no
update IRQ enabled	: no
periodic IRQ enabled	: no
periodic IRQ frequency	: 1024
max user IRQ frequency	: 64
24hr		: yes
periodic_IRQ	: no
update_IRQ	: no
HPET_emulated	: yes
BCD		: yes
DST_enable	: no
periodic_freq	: 1024
batt_status	: okay
```



The HPET driver `/dev/hpet`  has a similar API to the real time clock driver. It is 
a simple character device which can support any number of HPET devices. The 
Kernel API has three interfaces exported from the driver.

```
hpet_register( struct hpet_task *tp, int periodic )
hpet_unregister( struct hpet_task *tp )
hpet_control( struct hpet_task *tp, unsigned int cmd, unsigned long arg )
```

The user space interface to HPET is defined in the header `/usr/include/linux/hpet.h`


```c
#define	HPET_IE_ON	_IO('h', 0x01)	/* interrupt on */
#define	HPET_IE_OFF	_IO('h', 0x02)	/* interrupt off */
#define	HPET_INFO	_IOR('h', 0x03, struct hpet_info)
#define	HPET_EPI	_IO('h', 0x04)	/* enable periodic */
#define	HPET_DPI	_IO('h', 0x05)	/* disable periodic */
#define	HPET_IRQFREQ	_IOW('h', 0x6, unsigned long)	/* IRQFREQ usec */
```

In the example code a function handler is called periodically using the HPET 
interruption and prints the time delay between each call to the function.
