/*
#include "Example.hpp"

#include "ConfigValidator.hpp"
#include <stdlib.h>
#include "../Utility/Utility.hpp"

namespace Config
{

	ConfigValidator::ConfigValidator(std::string file_path) :_file_path(file_path)
	{
	}

	ConfigValidator::~ConfigValidator()
	{
	}

	void ConfigValidator::_open_and_read_file(void)
	{
		std::ifstream file_stream;
		std::string line;

		file_stream.open(_file_path.c_str());
		if (!file_stream.is_open())
			throw std::runtime_error("Configuration file failed to open");
		while (std::getline(file_stream, line))
			_file_content.append(line + "\n");
		file_stream.close();
	}

	void ConfigValidator::_remove_comments_and_empty_lines(void)
	{
		std::istringstream stream(_file_content);
		std::string line;
		size_t hashtag_pos;
		std::string tmp;

		_file_content.clear();
		while (std::getline(stream, line))
		{
			hashtag_pos = line.find('#');
			if (hashtag_pos != std::string::npos)
				line.erase(hashtag_pos, std::string::npos);
			tmp = line;
			Utility::remove_white_space(tmp);
			if (!line.empty() && !tmp.empty())
				_file_content.append(line + "\n");
		}
	}

	void ConfigValidator::_are_brackets_balanced(void)
	{
		std::stack<char> brackets;

		for (size_t i = 0; i < _file_content.length(); i++)
		{
			if(_file_content[i] == '{')
			{
				brackets.push('{');
				continue;
			}
			if(brackets.empty() && _file_content[i] == '}')
				throw std::runtime_error("Invalid-Config: Unbalanced brackets");
			if(_file_content[i] == '}')
				brackets.pop();
		}
		if(!brackets.empty())
			throw std::runtime_error("Invalid-Config: Unbalanced brackets");
	}

	void ConfigValidator::_check_semi_colon(std::string line)
	{
		size_t length;
		std::string tmp;

		tmp = line;
		Utility::remove_white_space(tmp);
		size_t first = tmp.find_first_of(";");
		length = tmp.length();
		if (first != length - 1)
			throw std::runtime_error("missing semicolon or invalid use of semicolon in " + line);
	}

	void ConfigValidator::_check_closing_bracket_line(std::string line)
	{
		std::string temp = line;
		Utility::remove_white_space(temp);
		if (temp.compare("}") != 0)
			throw std::runtime_error("Invalid-Config: Closing bracket");
	}

	bool ConfigValidator::_validate_server_opening(std::string line)
	{
		std::string temp = line;
		Utility::remove_white_space(temp);
		if (temp.compare("server{") != 0)
			throw std::runtime_error("Server opening line is invalid in config");
		return true;
	}

	void ConfigValidator::_check_outside_of_server_block(std::string line)
	{
		std::string temp = line;
		Utility::remove_white_space(temp);
		if(!temp.empty())
			throw std::runtime_error("Invalid-Config: Information outside of server blocks");
	}

	void ConfigValidator::_validate_location_opening(std::string line)
	{
		std::vector<std::string> location_split;
		size_t size;

		location_split = Utility::split_string_by_white_space(line);
		size = location_split.size();
		if (size == 3)
		{
			if (location_split[0].compare("location") != 0)
				throw std::runtime_error("Invalid-Config: Location opening");
			if (location_split[2].compare("{") != 0)
				throw std::runtime_error("Invalid-Config: Location opening");
		}
		else if (size == 2)
		{
			if (location_split[0].compare("location") != 0)
				throw std::runtime_error("Invalid-Config: Location opening");
			if (location_split[1].find("{") != location_split[1].length() - 1)
				throw std::runtime_error("Invalid-Config: Location opening");
			if (location_split[1].compare("{") == 0)
				throw std::runtime_error("Invalid-Config: Location opening");
		}
		else
			throw std::runtime_error("Invalid-Config: Location opening");
	}

	void ConfigValidator::_validate_deny_directive(std::string line)
	{
		_check_semi_colon(line);
		Utility::remove_last_of(';', line);
		std::vector<std::string> deny_split = Utility::split_string_by_white_space(line);
		if (deny_split.size() != 2)
			throw std::runtime_error("invalid number of arguments in deny directive");
		if (deny_split[0].compare("deny") != 0)
			throw std::runtime_error("unknown directive " + deny_split[0]);
		if (deny_split[1].compare("all") != 0)
			throw std::runtime_error("invalid parameter " + deny_split[1]);
	}

	void ConfigValidator::_validate_limit_except(std::string line, std::istringstream &stream)
	{
		std::vector<std::string> limit_except_split = Utility::split_string_by_white_space(line);
		if (limit_except_split[0].compare("limit_except") != 0)
			throw std::runtime_error("unknown directive " + limit_except_split[0]);
		while (std::getline(stream, line))
		{
			if (line.find("deny") != std::string::npos)
				_validate_deny_directive(line);
			else if (line.find("}") != std::string::npos)
			{
				_check_closing_bracket_line(line);
				break;
			}
			else
				throw std::runtime_error("unknown directive " + line);
		}
	}

	void ConfigValidator::_validate_location_block(std::string line, std::istringstream &stream)
	{
		_validate_location_opening(line);
		while (std::getline(stream, line))
		{
			if (line.find("limit_except") != std::string::npos)
				_validate_limit_except(line, stream);
			else if (line.find("}") != std::string::npos)
			{
				_check_closing_bracket_line(line);
				break;
			}
			else
				_check_semi_colon(line);
		}
	}

	void ConfigValidator::_validate_server_blocks(void)
	{
		bool server_on;
		std::string line;
		std::istringstream stream(_file_content);

		server_on = false;
		while (std::getline(stream, line))
		{
			if (line.find("server") != std::string::npos && line.find("server_name") == std::string::npos)
				server_on = _validate_server_opening(line);
			else if(server_on == false)
				_check_outside_of_server_block(line);
			else if (line.find("location") != std::string::npos)
				_validate_location_block(line, stream);
			else if (line.find("}") != std::string::npos && server_on == true)
			{
				_check_closing_bracket_line(line);
				server_on = false;
			}
			else if (line.find("limit_except") != std::string::npos && server_on == true)
				throw std::runtime_error("limit_except directive is not allowed here");
			else
				_check_semi_colon(line);
		}
	}

	void ConfigValidator::validate(void)
	{
		_open_and_read_file();
		_remove_comments_and_empty_lines();
		_are_brackets_balanced();
		_validate_server_blocks();
	}

	const std::string& ConfigValidator::get_file_content() const
	{
		return _file_content;
	}

} // namespace Config


#include "Utility.hpp"
#include <algorithm>
#include <iostream>
#include <sys/time.h>
#include "../Constants.hpp"

namespace Utility
{

	bool contains_whitespace(std::string &str)
	{
		size_t whitespace_found = str.find(' ', 0);
		size_t tab_found = str.find('\t', 0);
		return whitespace_found != std::string::npos || tab_found != std::string::npos;
	}

	std::vector<std::string> _split_line(const std::string &line, const char delimiter)
	{
		size_t start = 0;
		std::vector<std::string> lines;

		while (true)
		{
			size_t match = line.find(delimiter, start);
			if (match == std::string::npos){
				break;
			}
			size_t len = match - start;
			lines.push_back(line.substr(start, len));
			start = match + 1;
		}
		lines.push_back(line.substr(start, line.size()));
		return lines;
	}

	std::vector<std::string> _split_line_in_two(const std::string &line, const char delimiter)
	{
		std::vector<std::string> parts;

		size_t delimiter_position = line.find_first_of(delimiter, 0);
		if (delimiter_position == std::string::npos) {
			return parts;
		}
		parts.push_back(line.substr(0, delimiter_position)); // second argument of substr equals the length of the first part
		parts.push_back(line.substr(delimiter_position + 1, line.size() - delimiter_position));
		return parts;
	}

	std::string _trim(const std::string &s)
	{
		std::string::const_iterator start = s.begin();
		while (start != s.end() && std::isspace(*start))
		{
			start++;
		}
		std::string::const_iterator end = s.end();
		do
		{
			end--;
		} while (std::distance(start, end) > 0 && std::isspace(*end));

		return std::string(start, end + 1);
	}

	bool check_first_keyword(std::string line, std::string keyword)
	{
		size_t first = line.find_first_not_of(" \t");
		if (first < 0)
			return false;
		size_t end = line.find_first_of(" \t;{", first + 1);
		if (line.substr(first, end - first).compare(keyword) == 0)
			return true;
		else
			return false;
	}

	void remove_white_space(std::string &temp)
	{
		temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
	}

	void remove_last_of(char to_find, std::string &line)
	{
		size_t pos = line.find_last_of(to_find);
		if (pos != std::string::npos)
			line.erase(pos, pos + 1);
	}

	bool check_after_keyword(size_t last_pos, std::string str)
	{
		std::string tmp = str.substr(last_pos, str.length());
		remove_white_space(tmp);
		if (tmp.compare(";") != 0)
			return false;
		return true;
	}

	std::vector<std::string> split_string_by_white_space(const std::string &str)
	{
		std::vector<std::string> result;
		std::istringstream iss(str);

		for (std::string s; iss >> s;)
			result.push_back(s);
		return result;
	}

	bool is_positive_integer(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it))
			++it;
		return !s.empty() && it == s.end();
	}

	std::string leading_trim(const std::string &s)
	{
		const std::string WHITESPACE = " \n\r\t\f\v";
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	std::string trailing_trim(const std::string &s)
	{
		const std::string WHITESPACE = " \n\r\t\f\v";
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	std::string trim_white_space(const std::string &s)
	{
		const std::string WHITESPACE = " \n\r\t\f\v";
		return trailing_trim(leading_trim(s));
	}

	bool is_hyphen(char c) {
		return c == '-';
	}

	const std::string to_string(const int code) {
		std::string stringified_code;
		std::stringstream sstream;
		sstream << code;
		sstream >> stringified_code;
		return stringified_code;
	}

	std::string get_formatted_date() {
		struct timeval tv;
		char buf[32];
		struct tm *time;

		gettimeofday(&tv, NULL);
		time = gmtime(&tv.tv_sec);
		strftime(buf, 32, "%a, %d %b %Y %T GMT", time);
		std::string ret_val(buf);
		return ret_val;
	}

	std::string get_number_in_string(std::string& line) {
		if (line == "") {
			return "";
		}
		size_t non_number_position = line.find_first_not_of("0123456789ABCDEF", 0);
		if (non_number_position != std::string::npos) {
			return line.substr(0, non_number_position);
		}
		else {
			return line.substr(0, line.size());
		}
	}

	void logger(std::string str, std::string color)
	{
		struct tm *tm;
		time_t rawtime;
		char buf[32];

		time(&rawtime);
		tm = localtime(&rawtime);
		int ret = strftime(buf, 32, "%T", tm);
		buf[ret] = '\0';
		(void)color;
		std::cout << GREEN << "[" << buf << "] " << RESET;
		std::cout << color << str << RESET << std::endl;
	}

	bool is_found(const std::string& haystack, const std::string& needle) {
		return haystack.find(needle) != std::string::npos;
	}

} // namespace Utility
*/