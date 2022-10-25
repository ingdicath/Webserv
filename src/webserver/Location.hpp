/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dsalaman <dsalaman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 11:43:30 by dsalaman      #+#    #+#                 */
/*   Updated: 2022/10/25 11:43:30 by dsalaman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <set>
#include <map>

class Location {
public:
	Location();
	explicit Location(std::string path);
	Location(Location const &obj);
	Location &operator=(Location const &obj);
	virtual ~Location();

	//Setters
	void setAutoindex(bool autoindex);
	void setMethods(const std::set<std::string> &acceptedMethods);
	void setRoot(const std::string &root);
	void setCGI(const std::pair<std::string, std::string> &cgi);
	void setRedirection(const std::pair<int, std::string> &redirection);
	void setUpload(const std::string &upload);
	void setIndex(const std::string &index);

	//Getters
	bool isAutoindex() const;
	const std::string &getPathLocation() const;
	const std::string &getUpload() const;
	const std::string &getRoot() const;
	const std::string &getIndex() const;
	const std::set<std::string> &getAcceptedMethods() const;
	const std::pair<int, std::string> getRedirection() const;
	const std::pair<std::string, std::string> &getCgi() const;

private:
	std::string _pathLocation;
	std::string _upload;
	std::string _root;
	std::string _index;
	bool _autoindex;
	std::set<std::string> _acceptedMethods;
	std::pair<int, std::string> _redirection;
	std::pair<std::string, std::string> _cgi;

	void	_setDefaultRedirectionPages();
	void	_setDefaultAllowedMethods();
	void	_setDefaultRoot();
};
