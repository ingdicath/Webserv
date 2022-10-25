/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   FileUtils.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: diana <diana@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 08:35:51 by diana         #+#    #+#                 */
/*   Updated: 2022/10/25 08:35:51 by diana         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class FileUtils {
public:
	FileUtils();
	virtual ~FileUtils();

	bool isEmptyFile(std::ifstream &inputFile);
	void openFile(std::ifstream &file, const std::string &filePath);
	void closeFile(std::ifstream &file);
};

