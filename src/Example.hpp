
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stack>

namespace Config
{

	class ConfigValidator
	{
	private:
		/* data */
		std::string _file_path;
		std::string _file_content;

		/* methods */
		void _open_and_read_file(void); //ok
		void _remove_comments_and_empty_lines(void); //ok
		void _validate_server_blocks(void);
		bool _validate_server_opening(std::string line);
		void _check_outside_of_server_block(std::string line); //ok
		void _validate_location_block(std::string line, std::istringstream &stream);
		void _validate_location_opening(std::string line);
		void _validate_limit_except(std::string line, std::istringstream &stream);
		void _validate_deny_directive(std::string line);
		void _are_brackets_balanced(void); //ok
		void _check_semi_colon(std::string line); //ok
		void _check_closing_bracket_line(std::string line);

	public:
		ConfigValidator(std::string file_path);
		~ConfigValidator();

		void validate(void);
		const std::string& get_file_content(void) const;
	};
} // namespace Config

#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace Utility {

	std::vector<std::string> _split_line(const std::string& line, const char delimiter);
	std::vector<std::string> _split_line_in_two(const std::string& line, const char delimiter);
	std::string _trim(const std::string& s);
	bool contains_whitespace(std::string &str);
	bool check_first_keyword(std::string line, std::string keyword);
	bool check_after_keyword(size_t last_pos, std::string str);
	void remove_white_space(std::string &line);
	void remove_last_of(char to_find, std::string &line);
	bool is_positive_integer(const std::string& s);
	std::string leading_trim(const std::string &s);
	std::string trailing_trim(const std::string &s);
	std::string trim_white_space(const std::string &s);
	std::vector<std::string> split_string_by_white_space(const std::string& str);
	bool is_hyphen(char c);
	const std::string to_string(const int code);
	std::string get_formatted_date();
	std::string get_number_in_string(std::string &line);
	void logger(std::string str, std::string color);
	bool is_found(const std::string& haystack, const std::string& needle);
}

