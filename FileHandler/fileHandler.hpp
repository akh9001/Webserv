/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 02:20:13 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/22 00:38:03 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_HANDLER_HPP__
# define __FILE_HANDLER_HPP__

# include <iostream>
# include <fstream>
# include <sys/stat.h>
# include <vector>

//TODO documentation
namespace ws {
	class fileHandler
	{
		private:
			int bufferSize;	
		private:
			fileHandler();
			static std::string generateTmpName();
			static int BUFFER_SIZE;
		public:
			static bool readingEnd;
		public:
			~fileHandler();
			static std::string create(std::string fileName, std::string path);
			static std::string createTmp(std::string fileName);
			static bool checkIfExist(std::string filePath);
			static void write(std::string filePath, std::string buffer);
			static void moveFile(std::string fileOldPath, std::string newPath);//maybe return value bool if removed moved
			static int removeFile(std::string filePath);//maybe return value bool to check if removed
			static std::string readFile(std::ifstream& ifile);//getnextline
			// bool checkIfFile(); //TODO if needed
			// bool checkIPermissions(); //TODO if needed
		struct FileExist : public std::exception
		{
			const char * what () const throw ()
			{
				return "This file exist";
			}
		};
	};
}

#endif