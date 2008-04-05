#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H


#include <iostream>

namespace pramodx  {

/**
  * The algorithm for this method goes like this :
  *
  *
   * walk throgh each of the arguments
   *
   * if it is an option then, parse the option and
   * add it to the list of options.
   *
   * if its not, add it to the list of filenames.
  *
  * The function first counts the number of options and filenames
  * allocates space for them.
  * Then it adds the options and filenames to the list.
  */

class option  {
	private:
		char option_name  [15];
		char option_value  [96];
	public:
		option();
		option(char* string);

		void parse_option(char* string);

		char* get_option();
		char* get_value();

		static bool is_option(char* string);
		// this one marks everything that has its first char
		// as a '-' or a '/' as a comment.

	friend std::ostream& operator<< (std::ostream&, option&);
};

class command_line_parser  {
	private:
		static const int FILENAME_SIZE = 255;

		option* options;
		unsigned n_options;

		char ** filenames;
		unsigned n_filenames;

		/* NOTE : argv is incremented and argc is decremented
		 * before these functions are called. This means that
		 * no useless params like the prog name are counted
		 */

		// This function also allocates the space required
		// for the entities, so that parse_entities can
		// just dump them in the array.
		void count_entities(int argc, char* argv[]);
		void parse_entities(int argc, char* argv[]);
	public:
		command_line_parser(int argc, char* argv[]);
		~command_line_parser();

		unsigned get_n_options();
		unsigned get_n_filenames();

		option& get_option(unsigned);
		char* get_filename(unsigned);

		option* get_options();
		char** get_filenames();

	friend std::ostream& operator<< (std::ostream&, command_line_parser&);
};

inline option::option()  {
}

inline option::option(char* string)  {
	parse_option(string);
}

inline bool option::is_option(char* string)  {
	return(string[0] == '/' || string[0] == '-');
}

inline char* option::get_option()  {
	return(option_name);
}

inline char* option::get_value()  {
	return(option_value);
}

inline std::ostream& operator<< (std::ostream& out, option& o)  {
	out << o.get_option() << ':' << o.get_value();
	return out;
}

inline command_line_parser::command_line_parser(int argc,
	char* argv[])  {
		argc--;
		argv++;

		n_filenames = 0;
		n_options = 0;

		count_entities(argc, argv);
		parse_entities(argc, argv);
	}

inline unsigned command_line_parser::get_n_options()  {
	return n_options;
}

inline unsigned command_line_parser::get_n_filenames()  {
	return n_filenames;
}

inline option& command_line_parser::get_option(unsigned i)  {
	return options[i];
}

inline char* command_line_parser::get_filename(unsigned i)  {
	return filenames[i];
}

inline option* command_line_parser::get_options()  {
	return(options);
}

inline char** command_line_parser::get_filenames()  {
	return(filenames);
}
} // namespace pramodx
#endif
