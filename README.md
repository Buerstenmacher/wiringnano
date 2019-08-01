You can use this code to control the gpio-pins on your NVIDIA-Jetson-Nano. :simple_smile:

But remeber that gpio pins on the Jetson-Nano can only supply about 0,5mA of current. That's not enough to light up a standard led.  :disappointed:

To swicht an gpio_input from 0 to 1 and back to 0 you will need aproximately 5mA. I'm usiug an low voltage OpAmp (LM324) to control gpio inputs via gpio outputs.   :heavy_check_mark:

This is a c++11 header-only repository. Do not search for .cpp files!  :wink:

This code depends on repository "Buerstenmacher/rom_header". you can remove this dependency by deleting the calls to ::rom::error(), but you will loose some type of basic error handling, and the objects of ::rom::autodelay ....  , but herby you will lose the precious timing functionality of rom_header repository.
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
