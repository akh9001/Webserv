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
        Location(Server const& c);
        // ! /////////////////////// Getters & Setters //////////////////////
        std::string getLocation_match() const;
        void setLocation_match(std::string c);
        // std::string getRoot() const { return root;}
        // void setRoot(const std::string &c) {root = c;}
        void checkMissingFields() const;
};

#endif 