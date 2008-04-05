#include "command_line_parser.h"

#include <iostream>
#include <cstring>

using namespace std;

namespace pramodx  {


void option::parse_option(char* string)  {
	// move past the first char, i.e. the '-' or the '/'
	string++;
	char* colon_pos = strchr(string, ':'); // find ':'
	if(colon_pos /* not null */)  {
		string[colon_pos - string] = char(NULL);
		colon_pos++;

		strcpy(option_name, string);
		strcpy(option_value, colon_pos);
	}
	else  {
		strcpy(option_name, string);
		option_value[0] = char(NULL);
	}
}

// ----------------------------------------------------
// Code for class command_line_parser
//


void command_line_parser::count_entities(int argc, char* argv[])  {
	for(int i = 0; i < argc; i++)  {
		if(option::is_option(argv[i]))  {
			n_options++;
		}
		else  {
			n_filenames++;
		}
	}
	options = new option[n_options];
	filenames = new char*[n_filenames];
	for(unsigned i = 0; i < n_filenames; i++)  {
		filenames[i] = new char[FILENAME_SIZE];
	}
}

command_line_parser::~command_line_parser()  {
	delete [] options;

	for(unsigned i = 0; i < n_filenames; i++)  {
		delete [] filenames[i];
	}
	delete [] filenames;
}

void command_line_parser::parse_entities(int argc, char* argv[])  {
	unsigned c_option = 0, c_filename = 0;

	for(int i = 0; i < argc; i++)  {
		if(option::is_option(argv[i]))  {
			options[c_option++].parse_option(argv[i]);
		}
		else  {
			strcpy(filenames[c_filename++], argv[i]);
		}
	}
}

ostream& operator<< (ostream& out, command_line_parser& c)  {
	out << "Options : " << endl;

	for(unsigned i = 0; i < c.get_n_options(); i++)  {
		out << c.get_option(i) << endl;
	}

	out << "Filenames : " << endl;
	for(unsigned i = 0; i < c.get_n_filenames(); i++)  {
		out << c.get_filename(i) << endl;
	}
       
	return out;
}

	
} // end namespace pramodx
