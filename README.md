
 ## Jetson Nano
 J41 Header Pinout



| Name | Physical | Physical | Nmae |
|:------------- |:-------------:|:-----:|-----:|
| 3.3VDC        | 1  | 2  | 5.0VDC |
| i2c_2_sda     | 3  | 4  | 5.0VDC |
| i2c_2_scl     | 5  | 6  | GND    |
| gpio 216      | 7  | 8  | uart_2_TX |
| GND           | 9  | 10 | uart_2_RX |
| gpio 50       | 11 | 12 | gpio 79 |
| gpio 14       | 13 | 14 | GND |
| gpio 194      | 15 | 16 | gpio 232 |
| 3.3VDC        | 17 | 18 | gpio 15 |
| gpio 16       | 19 | 20 | GND |
| gpio 17       | 21 | 22 | gpio 13 |
| gpio 18       | 23 | 24 | gpio 19 |
| GND           | 25 | 26 | gpio 20 |
| i2c_1_sda     | 27 | 28 | i2c_1_scl |
| gpio 149      | 29 | 30 | GND |
| gpio 200      | 31 | 32 | gpio 168 |
| gpio 38       | 33 | 34 | GND |
| gpio 76       | 35 | 36 | gpio 51 |
| gpio 12       | 37 | 38 | gpio 77 |
| GND           | 39 | 40 | gpio 78 |


You can use this code to control the gpio-pins on your NVIDIA-Jetson-Nano. :smile:

But remeber that gpio pins on the Jetson-Nano can only supply about 0,5mA of current. That's not enough to light up a standard led.  :disappointed:

To swicht an gpio_input from 0 to 1 and back to 0 you will need aproximately 5mA. I'm usiug an low voltage OpAmp (LM324) to control gpio inputs via gpio outputs.   :heavy_check_mark:

This is a c++11 header-only repository. Do not search for .cpp files!  :wink:

This code depends on repository "Buerstenmacher/rom_header". you can break this dependency by deleting the calls to ::rom::error() and the objects of ::rom::autodelay ....  , but hereby you will lose the precious timing functionality of rom_header repository and you will loose some type of basic error handling.
:stuck_out_tongue:

You will need a c++11 compiler. :computer:  if you use g++: the appropriate compile-flag is "-std=c++11"

### Usage:
----------

create an object of wiringnano class:

```cpp
wiringnano pin79{79};
//pin79 is now an object representing gpio79 == pin12 on J41 header
```


-------------------------------------------------------------------

set a gpio to high voltage --> 3.3V:

```cpp
pin79.pullhi();
```
or call:
```cpp
pin79.write(1);
```

---------------

set a gpio to low voltage --> 0.0V :

pin79.pulllo();

or call:

pin79.write(0);

---------------

set a gpio to high input impendance without reading it's voltage:

pin79.flow();

-------------

read a gpio value and output it to user:

std::cout << uint16_t(pin79.read()) <<std::endl;   

//for std::cout you will have to convert uint8_t to some other integer to prevent it from beeing interpretet as char 

---------------------------------------------------------------------
