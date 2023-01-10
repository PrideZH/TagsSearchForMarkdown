#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>
#include <vector>

class StringUtil
{
public:
	/**
	 * ·Ö¸î×Ö·û´®
	 */
	static std::vector<std::string> split(const std::string& s, const std::string& seperator);

	/**
	 * È¥³ý×Ö·û´®Ç°ºó¿Õ¸ñ
	 */
	static std::string& trim(std::string& s);

	static std::string UTF8ToGB(const std::string& str);
};

#endif // _STRING_UTIL_H_