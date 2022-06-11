/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 00:20:06 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/12 00:54:50 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILITY_HPP__
# define __UTILITY_HPP__
# include <map>
# include <utility>
# include <iostream>

	static std::map<std::string,std::string> statusCodeMessages;
	void init_statusCodeMessages();


#endif