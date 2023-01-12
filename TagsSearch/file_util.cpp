#include "file_util.h"
#include <io.h>
#include <iostream>

using namespace util;

File* FileUtil::getFile(const std::string filePath)
{
}

std::vector<File*> FileUtil::getFiles(std::string path, std::string match)
{
	std::vector<File*> result;
	_finddata_t file;
	intptr_t f_handle;

	f_handle = _findfirst((path + match).c_str(), &file);
	if (f_handle == -1L)
		return result;

	do {
		// 判断是否有子目录
		if (file.attrib & _A_SUBDIR)
		{
			if ((strcmp(file.name, ".") == 0) || (strcmp(file.name, "..") == 0))
				continue;
			std::vector<File*> fileNames = getFiles(path + "\\" + file.name, match);
			result.insert(result.end(), fileNames.begin(), fileNames.end());
		}
		else
		{
			std::string filename = file.name;
			result.push_back(new File(filename, path));
		}
	} while (_findnext(f_handle, &file) == 0);

	_findclose(f_handle);
	return result;
}

File* FileUtil::open(std::string filepath, std::ios_base::openmode mode)
{
	std::fstream* fileStream = new std::fstream();
	fileStream->open(filepath, mode);
	if (!fileStream->is_open()) {
		delete fileStream;
		return NULL;
	}
	File* file = new File(filepath);
	file->fileStream = fileStream;
	return file;
}

File::File(std::string filepath) {
	this->filepath = path + fileneme;
}

File::File(std::string fileneme, std::string path)
{
	this->fileneme = fileneme;
	this->name = fileneme.substr(0, fileneme.rfind("."));
	this->filepath = path + fileneme;
	this->path = path;
}

File::~File()
{
	if (this->fileStream != NULL) {
		this->fileStream->close();
		free(this->fileStream);
	}
}

bool File::remove() {
	return std::remove((this->filepath).c_str()) - 1;
}

bool File::open(std::ios_base::openmode mode)
{
	this->fileStream = new std::fstream();
	fileStream->open(this->filepath, mode);
	return fileStream->is_open();
}

void File::close()
{
	delete this;
}