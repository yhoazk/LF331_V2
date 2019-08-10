# Rpi I2C

## Enable the driver

```
sudo rasp-config
```
Then `pheripherals > `


## Discovering connected devices:

For old rpi the bus number is `1`

```
i2cdiscover -y <bus_num>
```

For 9 DOF stick:

```
i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- 1e -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- 53 -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- 68 -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --
```

### Reading a device:

```
i2cdump -i <bus_num> <dev_addr_dec>
```

For the 9 DOF the addresses are:

The stick contains 3 ICs:

i2c 100 kHz and 400 kHz supported

* `ADXL345`:
    - 3 axis accelerometer
    - 13 bit resolution
    - Measures +-16g
    - Digital output is formatted as 16-bit two complement
    - Resolution 3.9 mg/LSB
    - Able to measure inclinations of less than 1.0°
    - `0x53`: Accelerometer:
        - According to i2c the W address is `0xA6` and for read `0xA7`
* `HMC5883LSMD`:
    - 3 axis compass
    - 12 bit resolution
    - 160 Hz output rate
    - 1-2° degree accurracy
    - `0x1E`:
        - 0x3D read and 0x3C write
* `ITG-3200`:
    - 3 axis gyro
    - 16 bit 
    - internal digitaly programmable low-pass filter
    - 400 kHz I<sup>2</sup>C 
    - Sesivity of 14.375 LSB °/sec
    - Range of +-2000°/sec
    - `0x68`:


## Compass interaction

This  section  describes  the  process  of  reading  from  and  writing  to  
this  device.    The  devices  uses  an  address  pointer  to indicate which
register  location is to be readfrom or written to.  These pointer locations
are sent from the master to this slave device and succeed the 7-bit 
address (0x1E) plus 1 bit read/write identifier,i.e. 0x3D for read and 0x3C for
write. 
To minimize  the  communication  between  the  master  and this  device,
the  address  pointer  updated  automatically  without master  intervention.

The  register  pointer  will  be  incremented  by  1  automatically  after  the
current  register  has  been  read successfully.The address pointer value
itself cannot be read via theI2C bus. Any attempt to read an invalid address
location returns 0’s, and any write to an invalid address location or an
undefined bitwithin a valid address location is ignored by this device.

To move the address pointer toarandomregister location, first issue a “write”
to that register location with no data byte following the commend.
For example, to move the address pointer to register 10, send 0x3C 0x0A.