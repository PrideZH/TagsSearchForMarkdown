#include "file_util.h"
#include <io.h>
#include <iostream>

std::fstream* FileUtil::open(const std::string filePath, std::ios_base::openmode mode)
{
	std::fstream* fileStream = new std::fstream();
	fileStream->open(filePath, mode);
	return fileStream->is_open() ? fileStream : NULL;
}

void FileUtil::close(std::fstream* file) {
	file->close();
	delete file;
}

std::vector<std::string> FileUtil::listFilePaths(std::string path, std::string mode) {
	std::vector<std::string> result;
	_finddata_t file;
	intptr_t f_handle;
	char tmp_path[1024];

	f_handle = _findfirst((path + mode).c_str(), &file);
	if (f_handle == -1L)
		return result;

	do {
		// 判断是否有子目录
		if (file.attrib & _A_SUBDIR)
		{
			if ((strcmp(file.name, ".") == 0) || (strcmp(file.name, "..") == 0))
				continue;
			std::vector<std::string> fileNames = listFilePaths(path + "\\" + file.name, mode);
			result.insert(result.end(), fileNames.begin(), fileNames.end());
		}
		else
		{
			result.push_back(path + file.name);
		}
	} while (_findnext(f_handle, &file) == 0);

	_findclose(f_handle);
	return result;
}