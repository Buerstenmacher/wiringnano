
//you should be on an nvidia-jetson-nano with ubuntu os installed and connection to internet

//install g++ :
//sudo apt-get install build-essential

//update your os by:
//sudo apt-get update
//sudo apt-get upgrade -y

//put all files of this repository into one writeable directory and compile it:
//sudo g++ rom_wiringnano.cpp -o rom_wiringnano -std=c++11 -Wno-psabi -Weffc++ -Wall -pedantic -pthread -Os

//run your new executeabel file:
//sudo ./rom_wiringnano

#include "rom_wiringnano.h"

int main(int argc, char *argv[]) {
rom::wiringnano_t();
return 0;
}





