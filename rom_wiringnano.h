/*Jetson Nano J41 Header
source:	https://www.jetsonhacks.com/nvidia-jetson-nano-j41-header-pinout/

 +-----+-----+---------+------+---+---Nano---+---+------+---------+-----+-----+
 |     |     |   Name  | Mode | V | Physical | V | Mode | Name    |     |     |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3V |      |   |  1 || 2  |   |      |    5.0V |     |     |
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

/*--- 	SYSFS EXAMPLE:
source:	https://www.jetsonhacks.com/2019/06/07/jetson-nano-gpio/

//Map GPIO Pin	//gpio79 is pin 12 on the Jetson Nano
$ echo 79 > /sys/class/gpio/export

// Set Direction
$ echo out > /sys/class/gpio/gpio79/direction

// Bit Bangin'!
$ echo 1 > /sys/class/gpio/gpio79/value
$ echo 0 > /sys/class/gpio/gpio79/value

// Unmap GPIO Pin
$ echo 79 > /sys/class/gpio/unexport

// Query Status
$ cat /sys/kernel/debug/gpio

In the above code, the 79 refers to a translation of the Linux sysfs GPIO named gpio79.
If we look at the Jetson Nano J41 Header Pinout, we can see that gpio79 is physically
pin 12 of the header.		*/

#ifndef rom_wiringnano_h
#define rom_wiringnano_h

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

// this code depends on Repository: "Buerstenmacher/rom_header"
#include "rom_error.h"
#include "rom_time.h"

namespace rom {

// **********************************************************************************
// **********************************************************************************
class file {			// create or open files; buffer them in ram; an use them if they where std::vector<uint8_t>
private:                	// This Class is good handling Files that are smaler then 10MB
std::fstream 	fst;       	// It reads the whole file into ram and writes it out only
std::string 	filename;   	// in case of change
uint8_t 	is_unsync;      // 0 if sync, 1 if buffer is newer, 2 if file is newer
std::vector<uint8_t> data;	// user data that should be writen to file before closing it

void open(const std::string mode = "in") {
fst.open(filename.c_str(),std::ios::out|std::ios::app); //This should create the file if it does not exist
fst.close();
if (mode == "out")      {fst.open(filename.c_str(),std::ios::out);}
if (mode == "in")       {fst.open(filename.c_str(),std::ios::in);}
if (!fst.is_open()) {rom::error("A file with name "+filename+" failed to open in Mode: "+mode);}
}

void close(void)        {fst.close();}

void flush(void) {
if (!is_unsync) {return;}	//flush is fast if data is already syncronized
else if (is_unsync == 1) {      //write buffer to file
        open("out");
        fst.seekp(0,std::ios::beg);
        for(auto item:data) {fst << item;}
        is_unsync = 0;
        }
else /*(is_unsync == 2)*/ {     //read from file to buffer
        open("in");
        fst.seekg (0, std::ios::end);
        auto  size{fst.tellg()};
        char* mem{new char [size]};
	if (!mem) {error("A file with name: "+filename+" does not fit into ram");}
        fst.seekg (0, std::ios::beg);
        fst.read (mem, size);
        data.clear();        	//delete the whole buffer
        data.reserve(size);	//reserve memory for filecontent
	for (int64_t i{0}; i<size;++i) {data.push_back(uint8_t(mem[i]));}
        delete[] mem;
        is_unsync = 0;
        }
close();
}

public:
void reread(void) {
is_unsync = 2;
flush();
}

void rewrite(void) {
is_unsync = 1;
flush();
}

file(const std::string& name = "",uint8_t writeonly = 0):fst{},
	filename(name),is_unsync(writeonly?1:2),data{} {
if (filename.size()==0) {filename = rom::humantime{}().substr(4,20)+".log";}//defaultname is "time.log"
flush();
}

~file(void) {flush();}

void push_back(uint8_t datain) {
data.push_back(datain);
is_unsync = 1;
}

//delegating push_back() for container
void push_back(const std::vector<uint8_t>& datain) {for (auto a:datain) {push_back(a);}}

//delegating push_back() for container
void push_back(const std::string& stin) {for (auto ch:stin) {push_back(uint8_t(ch));}}

uint8_t& at(size_t pos) {
is_unsync = 1;
if (pos>=data.size())  {data.resize(pos+1);}
return data.at(pos);
}

uint8_t at(size_t pos) const {	//const overload
if (pos>=data.size()) {error("Trying to get item from file: "+filename+" that does not exist right now.");}
return data.at(pos);
}

void resize(size_t sz) {
data.resize(sz);
is_unsync = 1;
}

};//class rom_file
// **********************************************************************************
// **********************************************************************************


// **********************************************************************************
// **********************************************************************************
class wiringnano {// this class creates a rpresentation for one GPIO Pin on Jeton Nano
private:
uint8_t wp;	//pinnumber for gpio;
uint8_t val;	//last output value: 0->low  1->high  3->input
rom::file* exp;  //the files are pointers to get the best control of construction an destruction timing
rom::file* unexp;
rom::file* dirfile;
rom::file* valuefile;


static const std::vector<uint8_t>& available_gpio(void) {
static std::vector<uint8_t> available_gpio_int{216,50,79,14,194,232,15,16,17,13,18,
						19,20,149,200,168,38,76,51,12,77,78};
return available_gpio_int;
}

static const std::vector<std::string>& available_directions(void) {
static std::vector<std::string> available_directions_int {"high","low","in","out"};
return available_directions_int;
}

static const std::string& export_path(void) {
static std::string export_path_int{"/sys/class/gpio/export"};
return export_path_int;
}

static const std::string& unexport_path(void) {
static std::string unexport_path_int{"/sys/class/gpio/unexport"};
return unexport_path_int;
}

std::string pinstr(void) 		{return std::to_string(wp);}
std::string direction_path(void) 	{return std::string{"/sys/class/gpio/gpio"+pinstr()+"/direction"};}
std::string value_path(void) 		{return std::string{"/sys/class/gpio/gpio"+pinstr()+"/value"};}

void export_(void) {
exp->resize(0);
exp->push_back(pinstr());
exp->rewrite();
}

void unexport_(void) {
unexp->resize(0);
unexp->push_back(pinstr());
unexp->rewrite();
}

void set_direction(const std::string& dir) {
for (auto& s:available_directions()) {
	if (dir == s)	{
		dirfile->resize(0);
		dirfile->push_back(dir);
		dirfile->rewrite();
		return;
		}
	}
error("Direction: "+dir+" is not available for gpio"+pinstr()+".");
}

uint8_t read_value(void) {
valuefile->reread();	//reread from file on disk
return (valuefile->at(0)=='0')?0:1;
}

public:
wiringnano(const wiringnano&) = delete;
wiringnano operator=(const wiringnano&) = delete;

wiringnano(uint8_t nr):wp(nr),val{3},exp{nullptr},unexp{nullptr},dirfile{nullptr},valuefile{nullptr} {
if (std::find(available_gpio().begin(),available_gpio().end(),nr)==available_gpio().end())
	{::rom::error("Gpio: "+pinstr()+" is not available.");}
exp = new rom::file{export_path(),1};
unexp = new rom::file{unexport_path(),1};
export_();
dirfile = new rom::file(direction_path());
valuefile = new rom::file(value_path());
set_direction("in");
}

~wiringnano(void) {
set_direction("in");
delete valuefile;
delete dirfile;
unexport_();
delete unexp;
delete exp;
}

void pullhi() {//set digital output to high voltage
if (val != 1)	{set_direction("high");}
val = 1;
}

void pulllo() {//set digital output to high voltage
if (val != 0)	{set_direction("low");}
val = 0;
}

void flow() {
if (val !=3) {set_direction("in");}
val =3;
}

void write(uint8_t bit) {
if (val != bit) {
	if (bit == 1) 		{pullhi();}
	else if (bit == 0)	{pulllo();}
}
}

uint8_t read() {
flow();
return read_value();
}


};
// **********************************************************************************
// **********************************************************************************



void wiringnano_t(void) {	//simple demo of gpio output on jetson nano
::rom::autodelay delay{};	//create a delay object from rom_time.h
wiringnano pin79{79};		//create a wiringnano gpio interface for gpio 79 == pin 12
wiringnano pin232{232};		//create a wiringnano gpio interface for gpio 232 == pin 16
				// (see table at begin of file)

for (uint16_t i{0};i<10;++i){	//loop 10 times
	pin79.pulllo();		//send 0.0 Volt to pin 12
	delay(0.5);		//wait 0.5 sec.
	pin79.pullhi();		//send 3.3 Volt to pin 12
	delay(0.5);		//wait 0.5 sec.
}

std::cout << "Pin 16 is: " << (pin232.read()?"high":"low ") << std::endl; //read and output status of pin 16

}

}//namespace rom
#endif  //rom_wiringnano.h

