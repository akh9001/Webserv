/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 14:33:28 by mokhames          #+#    #+#             */
/*   Updated: 2022/06/30 14:13:36 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP


#include "../webServer.hpp"
#include "Server.hpp"

// class Server;

class Location : public Server
{
        private :
                std::string location_match;
                std::string prefix_match;
                //std::string root1;
                std::vector<Location> _locations;
        public :
        
        // ! Constuctors and destructor
        Location();
        ~Location();
        Location(Location const& c);
        Location &operator=(Location const& c);
        Location(Server & c);
        // ! /////////////////////// Getters & Setters //////////////////////
        std::string getLocation_match() const;
        void setLocation_match(std::string c);
        void checkMissingFields() const;
};

#endif 