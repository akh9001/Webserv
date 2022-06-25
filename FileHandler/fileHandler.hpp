/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laafilal <laafilal@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 02:20:13 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/25 15:27:44 by laafilal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_HANDLER_HPP__
# define __FILE_HANDLER_HPP__

# include <iostream>
# include <fstream>
# include <sys/stat.h>
# include <vector>

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
			static void moveFile(std::string fileOldPath, std::string newPath);
			static int removeFile(std::string filePath);
			static std::string readFile(std::ifstream& ifile);

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