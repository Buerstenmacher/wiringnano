
 ### Jetson Nano



| Tables        | Are           | Cool  ||
| ------------- |:-------------:| -----:||
| col 3 is      | right-aligned | $1600 ||
| col 2 is      | centered      |   $12 ||
| zebra stripes | are neat      |    $1 ||



 | Name | Physical |Physical | Name | 
 | 3.3V |        1 |     2   |  5.0V| 
   |     |     |i2c_2_SDA|      |   |  3 || 4  |   |      |    5.0V |     |     |
 |     |     |i2c_2_SCL|      |   |  5 || 6  |   |      |      0V |     |     |
 |     |     |GPIO 216 |      |   |  7 || 8  |   |      |UART_2_TX|     |     |
 |     |     |      0V |      |   |  9 || 10 |   |      |UART_2_RX|     |     |
 |     |     |GPIO  50 |      |   | 11 || 12 |   |      |GPIO  79 |     |     |
 |     |     |GPIO  14 |      |   | 13 || 14 |   |      |      0V |     |     |
 |     |     |GPIO 194 |      |   | 15 || 16 |   |      |GPIO 232 |     |     |
 |     |     |    3.3V |      |   | 17 || 18 |   |      |GPIO  15 |     |     |
 |     |     |GPIO  16 |      |   | 19 || 20 |   |      |      0V |     |     |
 |     |     |GPIO  17 |      |   | 21 || 22 |   |      |GPIO  13 |     |     |
 |     |     |GPIO  18 |      |   | 23 || 24 |   |      |GPIO  19 |     |     |
 |     |     |      0V |      |   | 25 || 26 |   |      |GPIO  20 |     |     |
 |     |     |i2c_1_sda|      |   | 27 || 28 |   |      |i2c_1_scl|     |     |
 |     |     |GPIO 149 |      |   | 29 || 30 |   |      |      0V |     |     |
 |     |     |GPIO 200 |      |   | 31 || 32 |   |      |GPIO 168 |     |     |
 |     |     |GPIO  38 |      |   | 33 || 34 |   |      |      0V |     |     |
 |     |     |GPIO  76 |      |   | 35 || 36 |   |      |GPIO  51 |     |     |
 |     |     |GPIO  12 |      |   | 37 || 38 |   |      |GPIO  77 |     |     |
 |     |     |      0V |      |   | 39 || 40 |   |      |GPIO  78 |     |     |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |   Name  | Mode | V | Physical | V | Mode | Name    |     |     |
 +-----+-----+---------+------+---+---Nano---+---+------+---------+-----+-----+ */



You can use this code to control the gpio-pins on your NVIDIA-Jetson-Nano. :smile:

But remeber that gpio pins on the Jetson-Nano can only supply about 0,5mA of current. That's not enough to light up a standard led.  :disappointed:

To swicht an gpio_input from 0 to 1 and back to 0 you will need aproximately 5mA. I'm usiug an low voltage OpAmp (LM324) to control gpio inputs via gpio outputs.   :heavy_check_mark:

This is a c++11 header-only repository. Do not search for .cpp files!  :wink:

This code depends on repository "Buerstenmacher/rom_header". you can break this dependency by deleting the calls to ::rom::error() and the objects of ::rom::autodelay ....  , but herby you will lose the precious timing functionality of rom_header repository and you will loose some type of basic error handling,
:stuck_out_tongue:

You will need a c++11 compiler. :computer:

Usage:
------

create an object of wiringnano class:

--> wiringnano pin79{79};

//pin79 is now an object representing gpio79 == pin12 on J41 header

-------------------------------------------------------------------

set a gpio to high voltage --> 3.3V:

pin79.pullhi();

or call:

pin79.write(1);

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
