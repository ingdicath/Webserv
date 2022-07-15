/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: aheister <aheister@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 12:34:28 by aheister      #+#    #+#                 */
/*   Updated: 2022/06/30 12:34:33 by aheister      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H

class Request {
public:
	Request(void);
	Request(Request const & src);
	Request& operator=(Request const & rhs);
	virtual ~Request(void);

	//Configuration functions
	void	config_request(void);
	
private:
	

};



#endif //WEBSERV_REQUEST_H