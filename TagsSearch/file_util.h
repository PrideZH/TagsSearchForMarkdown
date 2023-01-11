#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <string>
#include <fstream>
#include <vector>

class FileUtil
{
public:
	/**
	 * 打开文件，返回是否成功打开
	 */
	static std::fstream* open(const std::string filePath, std::ios_base::openmode mode);

	static void close(std::fstream* file);

	/**
	 * 获取目录以及子目录中的所有文件路径
	 * @param mode: *.txt, *.*, ...
	 */
	static std::vector<std::string> listFilePaths(std::string path, std::string mode);
};

#endif // _FILE_UTIL_H_