#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <string>
#include <fstream>
#include <vector>

namespace util {
	class File {
	public:
		std::fstream* fileStream;

		std::string name; // 文件名
		std::string fileneme; // 文件名 (带后缀)
		std::string path;
		std::string filepath;

		File(std::string filepath);

		File(std::string fileneme, std::string path);

		~File();

		bool remove();

		bool open(std::ios_base::openmode mode);

		void close();
	};

	class FileUtil
	{
	public:
		static File* getFile(const std::string filePath);

		static std::vector<File*> getFiles(std::string path, std::string match = "*");

		static File* open(std::string filepath, std::ios_base::openmode mode);
	};
}

#endif // _FILE_UTIL_H_