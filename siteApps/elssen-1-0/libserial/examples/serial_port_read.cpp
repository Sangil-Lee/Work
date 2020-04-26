/**
 *  @example serial_port_read.cpp
 */

#include <libserial/SerialPort.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <ctime>
#include <unistd.h>

constexpr const char* const SERIAL_PORT_1 = "/dev/ttyUSB0" ;

/**
 * @brief This example demonstrates configuring a serial port and 
 *        reading serial data.
 */
int main()
{
    using namespace LibSerial ;
    using namespace std;

    // Instantiate a SerialPort object.
    SerialPort serial_port ;

    try
    {
        // Open the Serial Port at the desired hardware port.
        serial_port.Open(SERIAL_PORT_1) ;
    }
    catch (const OpenFailed&)
    {
        std::cerr << "The serial port did not open correctly." << std::endl ;
        return EXIT_FAILURE ;
    }

    // Set the baud rate of the serial port.
    serial_port.SetBaudRate(BaudRate::BAUD_115200) ;

    // Set the number of data bits.
    serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;

    // Turn off hardware flow control.
    serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;

    // Disable parity.
    serial_port.SetParity(Parity::PARITY_NONE) ;
    
    // Set the number of stop bits.
    serial_port.SetStopBits(StopBits::STOP_BITS_1) ;
    
    // Wait for data to be available at the serial port.
    while(!serial_port.IsDataAvailable()) 
    {
        usleep(1000) ;
    }

    // Specify a timeout value (in milliseconds).
    size_t ms_timeout = 250 ;

    // Char variable to store data coming from the serial port.
    char data_byte ;

    // Read one byte from the serial port and print it to the terminal.
    try
    {
        // Read a single byte of data from the serial port.
        serial_port.ReadByte(data_byte, ms_timeout) ;

        // Show the user what is being read from the serial port.
        std::cout << data_byte << std::flush ;
    }
    catch (const ReadTimeout&)
    {
        std::cerr << "\nThe ReadByte() call has timed out." << std::endl ;
    }

    // Wait a brief period for more data to arrive.
    usleep(1000) ;

    DataBuffer read_buffer ;

	while(true)
	{
		try {
			// Read as many bytes as are available during the timeout period.
			serial_port.Read(read_buffer, 0, ms_timeout) ;

			for(auto iter : read_buffer)
				printf("%x", iter);
		}
		catch (const ReadTimeout&)
		{
			//std::string s1 = std::to_string(read_buffer.at(3)) + std::to_string(read_buffer.at(4));
			std::stringstream sstream;
			for (size_t i = 0 ; i < read_buffer.size() ; i++) {
				//sstream << std::hex << static_cast<uint32_t> (read_buffer.at(i)) << std::flush;
				sstream << std::setfill ('0') << std::setw(sizeof(uint8_t)*2) << std::hex << static_cast<int> (read_buffer.at(i)) << std::flush;
			};

			std::string strstring = sstream.str();
			if(strstring.empty() == true || strstring.size() != 28|| strstring.find("eace") == std::string::npos) continue;

			cout << strstring << flush;
			
			//serial index: 8,9,10,11
			int midnum = atoi(strstring.substr(9,1).c_str()) + atoi(strstring.substr(10,1).c_str());
			stringstream strmid;
			strmid << std::hex << midnum << flush;
			string strserial = strstring.substr(8,1) + strmid.str() + strstring.substr(11,1);
			int nserial = stoi(strserial, 0, 16);

			float temp_a = 0.0f, temp_b = 0.0f, temp_c = 0.0f, temp_d = 0.0f;
			temp_a = stoi(strstring.substr(12,4), 0, 16)/100.0;
			temp_b = stoi(strstring.substr(16,4), 0, 16)/100.0;
			temp_c = stoi(strstring.substr(20,4), 0, 16)/100.0;
			temp_d = stoi(strstring.substr(24,4), 0, 16)/100.0;

			//random number
			srand(time(nullptr));
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.01, 0.02);
			float fRvar = dis(gen);

			std::time_t datetime = std::time(nullptr);



			//printf("Random num: %f\n", fRvar);
			printf("\n Serial: %d, TempA: %f, TempB: %f, TempC: %f, Tempd: %f, Time:%s\n", nserial, temp_a+fRvar, temp_b+fRvar, temp_c, temp_d, 
									std::asctime(std::localtime(&datetime)));

#if 0
			for (size_t i = 0 ; i < read_buffer.size() ; i++)
			{
				std::cout<< "index:(" << i << "): " << std::hex << static_cast<unsigned short> (read_buffer.at(i)) << ", "<< std::flush ;
			}

			for(auto iter: read_buffer)
				std::cout << iter << std::endl;
#endif

			//std::cerr << "The Read() call timed out waiting for additional data." << std::endl ;
		}
	};

    // Successful program completion.
    std::cout << "The example program successfully completed!" << std::endl ;
    return EXIT_SUCCESS ;
}
