/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhalidy <akhalidy@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 02:22:26 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/05 22:48:17 by akhalidy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fileHandler.hpp"

int ws::fileHandler::BUFFER_SIZE = 1024;

bool ws::fileHandler::checkIfExist(std::string filePath)
{
	struct stat buffer;   
	return (stat (filePath.c_str(), &buffer) == 0 ); 
}


void ws::fileHandler::write(std::string filePath, std::string buffer)
{
	//TODO check if permission
	std::fstream file;
	file.open(filePath, std::fstream::app);
	file << buffer;
	file.close();
}

void ws::fileHandler::moveFile(std::string fileOldPath,std::string fileNewPath)
{
	//TODO check if already exist the same name throw error
	std::ofstream newFile(fileNewPath, std::fstream::app|std::fstream::out);
	std::ifstream file;
	file.open(fileOldPath, std::fstream::in);
	newFile << file.rdbuf();
	file.close();
	newFile.close();
	remove(fileOldPath.c_str());
}

std::string ws::fileHandler::create(std::string fileName, std::string path)
{
	std::string filePath = path+"/"+fileName;
	if(!checkIfExist(filePath))
	{	
		std::fstream file(filePath, std::fstream::in | std::fstream::out | std::fstream::app);
		file.close();
		return filePath;
	}
	else
		throw FileExist();
	
}

std::string ws::fileHandler::createTmp(std::string path)
{
	std::string filePath = path+"/"+ ws::fileHandler::generateTmpName();
	if(!checkIfExist(filePath))
	{	
		std::fstream file(filePath, std::fstream::in | std::fstream::out | std::fstream::app);
		file.close();
		return filePath;
	}
	else
		throw FileExist();
}

void ws::fileHandler::removeFile(std::string filePath)
{
	//TODO
	//check permission
	//check if exist
	//throw error
	remove(filePath.c_str());
}

std::string ws::fileHandler::generateTmpName()
{
	char filename[] = "tmpXXXXXX";
	char *tmp =  mktemp(filename);
	std::string tmpName(tmp);
	return tmpName;
}



/////////////// readFile example ///////////////////////////////////////////////////
//
// 		std::string path = "./files/hello.html";
// 		std::ifstream ifile(path, std::ifstream::in| std::ifstream::binary);
// 		while(ifile)
//  	   	std::cout << ws::fileHandler::readFile(ifile);
//
////////////////////////////////////////////////////////////////////////////////////

std::string ws::fileHandler::readFile(std::ifstream& ifile)
{
	//maybe i need to check some errors here and throw them !!
    std::vector<char> buffer (ws::fileHandler::BUFFER_SIZE + 1, 0);
    ifile.read(buffer.data(), ws::fileHandler::BUFFER_SIZE);
    std::streamsize s = ((ifile) ? ws::fileHandler::BUFFER_SIZE : ifile.gcount());
    buffer[s] = 0;
    if(!ifile)
        ifile.close();
    return std::string(buffer.data());
}