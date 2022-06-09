#include "Location.hpp"
#include "Server.hpp"

 Location::Location() {}
        Location::~Location() {}
        Location::Location(Location const& c) {*this = c;}
        
        Location::Location(Server const& c) { 
             this->root = c.getRoot();
             this->_allowed_methods = c.getAllowedMethods();
             this->_index = c.getIndex();
             this->_clinet_max_body_size = c.getClientMaxBodySize();
             this->_autoindex = c.getAutoIndex();
             this->redirect_uri = c.getRedirectUri();
             this->errorPages = c.getErrorPages();
             this->cgiPath = c.getCgiPath();


        }
Location &Location::operator=(Location const& c) { 
            location_match = c.location_match;
             prefix_match = c.prefix_match;
             this->root = c.root;
             this->_allowed_methods = c._allowed_methods;
             this->_index = c._index;
             this->_clinet_max_body_size = c._clinet_max_body_size;
             this->_autoindex = c._autoindex;
             this->redirect_uri = c.redirect_uri;
             this->errorPages = c.errorPages;
             this->cgiPath = c.cgiPath;
            return *this; 
            }

        // ! /////////////////////// Getters & Setters //////////////////////
        std::string Location::getLocation_match() const { return location_match;}
        void Location::setLocation_match(std::string c) { location_match = c;}