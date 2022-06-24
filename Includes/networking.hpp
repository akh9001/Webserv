/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   networking.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 19:48:05 by akhalidy          #+#    #+#             */
/*   Updated: 2022/06/24 16:58:56 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <exception>
#include <algorithm>
#include <fstream>
#include <string>
#include <algorithm>
#include <exception>
#include <ostream>
//* c headers
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/_types/_pid_t.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
