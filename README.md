You can use this Code to control the GPIO-pins on your NVIDIA-Jetson-Nano.
But remeber that gpio pins on the jetson-nano can only supply about 0,5mA of current. Thats not enough to light up a standard led.
To swicht an GPIO_Input from 0 to 1 to 0 you will n eed aproximately 5mA. I'm usung an OpAmp (LM324) to control gpio inputs via gpio outputs.   
This is a c++11 header-only repository.
This code depends on repository "Buerstenmacher/rom_header". you can remove this dependency by deleting those calls to ::rom::error(****), but you will loose this type of basic error handling.
and the objects of ::rom::autodelay ....  , but herby you will lose the timing functionality of rom_header repository

this code depends on a c++11 compiler. 

Usage:
------

create an object of wiringnano class:
--> wiringnano pin79{79};   //pin79 is now an object representing gpio79 == pin12 on J41 header

set a gpio to high voltage --> 3.3V:
pin79.pullhi();
or call:
pin79.write(1);

set a gpio to low voltage --> 0.0V :
pin79.pulllo();
or call:
pin79.write(0);

set a gpio to high input impendance without reading it's voltage:
pin79.flow();

read a gpio value and output it to user:
std::cout << uint16_t(pin79.read()) <<std::endl;    //for cout you will have to convert uint8_t to som other integer otherwise it           
                                                    //will be interpretet as char 
