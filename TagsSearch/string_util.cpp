#include "string_util.h"
#include <windows.h>

std::vector<std::string> StringUtil::split(const std::string& s, const std::string& seperator)
{
	std::vector<std::string> result;
	std::string::size_type left = 0, right = s.find(seperator);
	while (std::string::npos != right)
	{
		result.push_back(s.substr(left, right - left));

		left = right + seperator.size();
		right = s.find(seperator, left);
	}
	if (left != s.length())
		result.push_back(s.substr(left));
	return result;
}

std::string& StringUtil::trim(std::string& s)
{
	if (s.empty())
		return s;

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

std::string StringUtil::UTF8ToGB(const std::string& str)
{
	std::string result;
	WCHAR *strSrc;
	LPSTR szRes;

	const char* c_str = str.c_str();

	// 获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, c_str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, c_str, -1, strSrc, i);

	// 获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}
