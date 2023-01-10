#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>
#include <vector>

class StringUtil
{
public:
	/**
	 * �ָ��ַ���
	 */
	static std::vector<std::string> split(const std::string& s, const std::string& seperator);

	/**
	 * ȥ���ַ���ǰ��ո�
	 */
	static std::string& trim(std::string& s);

	static std::string UTF8ToGB(const std::string& str);
};

#endif // _STRING_UTIL_H_