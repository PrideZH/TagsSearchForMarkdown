#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <string>
#include <fstream>
#include <vector>

class FileUtil
{
public:
	/**
	 * ���ļ��������Ƿ�ɹ���
	 */
	static std::fstream* open(const std::string filePath, std::ios_base::openmode mode);

	static void close(std::fstream* file);

	/**
	 * ��ȡĿ¼�Լ���Ŀ¼�е������ļ�·��
	 * @param mode: *.txt, *.*, ...
	 */
	static std::vector<std::string> listFilePaths(std::string path, std::string mode);
};

#endif // _FILE_UTIL_H_