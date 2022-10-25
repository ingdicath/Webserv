/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: diana <diana@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 08:58:23 by diana         #+#    #+#                 */
/*   Updated: 2022/10/25 08:58:23 by diana         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>

#define WHITESPACES " \n\t\r\f\v"

namespace utils {
	std::vector<std::string>	splitString(const std::string &str, char splitter);
	bool 						isPositiveNumber(const std::string &str);
	size_t 						stringToPositiveNum(const std::string &input);
	std::string 				&rTrim(std::string &str);
	std::string 				&lTrim(std::string &str);
	std::string 				&trim(std::string &str);
	std::string 				deleteLastOf(char toSearch, std::string str);
	std::string 				stringToUpper(std::string oString);
	std::string 				stringToLower(std::string oString);
	int 						stringToInt(const std::string &input);
	long 						stringToLong(const std::string &input);
	std::vector<std::string>	splitByWhiteSpaces(const std::string &string, const std::string &whiteSp);
}
