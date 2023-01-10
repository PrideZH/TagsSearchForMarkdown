#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>

#include <stdio.h>
#include <io.h>

using namespace std;

struct Info {
	vector<string> tags;
	string fileName;
	int weight;
};

vector<Info> infos;

vector<string> split(const string& s, const string& seperator)
{
	vector<string> result;
	string::size_type left = 0, right = s.find(seperator);
	while (string::npos != right)
	{
		result.push_back(s.substr(left, right - left));

		left = right + seperator.size();
		right = s.find(seperator, left);
	}
	if (left != s.length())
		result.push_back(s.substr(left));
	return result;
}

string& trim(std::string& s)
{
	if (s.empty())
		return s;

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

string UTF8ToGB(const string& str)
{
	string result;
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

void init() {
	// Read configuration file
	fstream file;
	string buf;
	file.open("./data/tags.conf", ios::in);
	if (!file.is_open())
	{
		printf("找不到配置文件\n");
		return;
	}
	while (getline(file, buf)) {
		vector<string> nameAndTags = split(buf, ":");
		vector<string> tags;
		for (string tag : split(nameAndTags[1], ","))
			tags.push_back(trim(tag));
		infos.push_back({ tags, trim(nameAndTags[0]) });
	}
	file.close();
	// Clean up temporary files
	_finddata_t dir_info;
	_finddata_t file_info;
	intptr_t f_handle;
	char tmp_path[1024];
	if ((f_handle = _findfirst("./temp/*.md", &dir_info)) != -1)
	{
		while (_findnext(f_handle, &file_info) == 0) {
			strcpy_s(tmp_path, "./temp/\0");
			strcat_s(tmp_path, file_info.name);
			if (remove(tmp_path) - 1) {
				errno_t err;
				_get_errno(&err);
				if (EACCES == err)
				{
					printf("%s had been opend by some application, can't delete.\n", tmp_path);
					break;
				}
			}
		}
	}
}

void run() {
	string tagsStr;
	while (true)
	{
		cin.get();
		getline(cin, tagsStr);
		vector<string> tags = split(tagsStr, ",");

		// Initialize the weight value
		for (Info info : infos)
			info.weight = 0;

		for (string tag : tags) {
			// Calculated weight value
			for (Info info : infos) {
				for (string infoTag : info.tags) {
					if (tag == infoTag) {
						info.weight++;
						break;
					}
				}
			}
		}

		sort(infos.begin(), infos.end(), [](Info a, Info b) { return a.weight > b.weight; });

		// Construct the file and open it
		fstream file;
		string buf;
		string filePath = "./temp/temp-" + to_string(time(0)) + ".md";
		file.open(filePath, ios::out | ios::binary);
		if (!file.is_open())
		{
			printf("临时文件创建失败\n");
			return;
		}

		int count = 5;
		for (Info info : infos) {
			if (--count == 0)
				break;
			file << endl << "## " << info.fileName << endl;
			fstream infoFile;
			string path = UTF8ToGB("./data/" + info.fileName + ".md");
			infoFile.open(path, ios::in | ios::binary);
			if (!infoFile.is_open())
			{
				cout << "找不到文件: " + path << endl;
				return;
			}
			while (!infoFile.eof()) {
				char szBuf;
				infoFile.read(&szBuf, sizeof(char));

				if (infoFile.eof())
					break;

				if (file.bad())
					break;

				file.write(&szBuf, sizeof(char));
			}
			infoFile.close();
		}
		file.close();

		std::system(("start " + filePath).c_str());
	}
}

int main()
{
	init();
	run();
	return 0;
}
