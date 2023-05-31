#include <getopt.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <string.h>

FILE * hexdump_file = fopen( "./hexdump.hex", "w" );

std::vector<uint8_t> process_line(const std::string& line)
{
    std::vector<uint8_t> bytes;
    std::string byteString;

    std::stringstream ss(line);
    while (ss >> byteString) {
        if (byteString.substr(0, 2) == "0x") {
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteString.substr(2), nullptr, 16));
            bytes.push_back(byte);
        }
    }
    return bytes;
}

uint32_t offset = 0x0;
static uint64_t diff = 0;

uint32_t write_line_to_file( std::vector<uint8_t> data )
{  
    time_t curtime;
    char buffer[30];
    memset(buffer, 0, 30);
    strftime(buffer, 30, "%m/%d/%y %T.", localtime( &curtime ) );
    fprintf( hexdump_file, "%s%ld\n", buffer, diff++);
    fprintf( hexdump_file, "%08x  ", offset);
    // Insert a linux psuedo header
    // +-------------------------+
    // |  Bus number+event flag  |
    // |        (1 Octet)        |
    // +-------------------------+
    // |          Flags          |
    // |       (4 Octets)        |
    // +-------------------------+
    // |    Hardware address     |
    // |        (1 Octet)        |
    // +-------------------------+
    // |       Payload           |
    fprintf( hexdump_file, "%02x %02x %02x %02x %02x ",
        0x00, 0x00, 0x00, 0x00, 0x00);
    offset += 6;
    std::vector<uint8_t>::iterator it;
    uint8_t l = 0;
    for( it = data.begin(); it != data.end(); it++) 
    {
        if (offset % 64 == 0)
        {
            //offset += 16;
            fprintf(hexdump_file, "\n%08x  ", offset);
        }
        fprintf(hexdump_file, "%02X ", *it);
        offset += 1;
    }
    fprintf(hexdump_file, "\n");
    offset = 0;
}

int main(int argc, char *argv[])
{
    char * filename = NULL;
    while(1)
    {
        const int opt = getopt(argc, argv, "f:h");
        if ( opt == -1)
        {
            break;
        }

        switch( opt )
        {
            case( 'f' ):
                filename = optarg;
                break;
            case( 'h' ):
                std::cout << "./saleae_to_hex -f <filename>" << std::endl;
                return(1);
        }
    }
    if ( filename == NULL )
    {
        std::cout << "Error - no input file was provided." << std::endl;
        return (1);
    }
    std::ifstream input_file_stream(filename);

    if( input_file_stream.is_open() )
    {
        std::string line;
        while( std::getline(input_file_stream, line) )
        {
            //std::cout << line << std::endl;
            std::vector<uint8_t> byte_vector = process_line( line );
            if ( byte_vector.size() > 1 )
            {
                std::cout << byte_vector.size() << std::endl;
                write_line_to_file( byte_vector );
            }
        }

    }
    fclose(hexdump_file);
    
}