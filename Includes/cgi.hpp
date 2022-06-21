/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 14:45:32 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/21 14:18:06 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "networking.hpp"
#include "../Parsing/Request/Request.hpp"

extern char **environ;
int	cgi(const Request &request, const char *script_path);