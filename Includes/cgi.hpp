/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:32 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/15 10:43:35 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "networking.hpp"
#include "../Parsing/Config/Server.hpp"
#include "../Parsing/Request/Request.hpp"

class CGI
{
	private:
		std::map<std::string, std::string>	_meta_variables;
	public:
		void	init_env(const Server &server, const Request &request);
};