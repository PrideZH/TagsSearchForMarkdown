#include "string_util.h"
#include <windows.h>
#include <iostream>
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
		vector<string> nameAndTags = StringUtil::split(buf, ":");
		vector<string> tags;
		for (string tag : StringUtil::split(nameAndTags[1], ","))
			tags.push_back(StringUtil::UTF8ToGB(StringUtil::trim(tag)));
		infos.push_back({ tags, StringUtil::trim(nameAndTags[0]) });
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
		getline(cin, tagsStr);
		vector<string> tags = StringUtil::split(tagsStr, ",");

		// Initialize the weight value
		for (size_t i = 0; i < infos.size(); i++)
			infos[i].weight = 0;

		for (string tag : tags) {
			// Calculated weight value
			for (size_t i = 0; i < infos.size(); i++) {
				for (string infoTag : infos[i].tags) {
					if (tag == infoTag) {
						infos[i].weight++;
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
			string path = StringUtil::UTF8ToGB("./data/" + info.fileName + ".md");
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
