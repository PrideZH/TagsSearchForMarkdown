#include "string_util.h"
#include "file_util.h"
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
	fstream* file;
	if ((file = FileUtil::open("./data/tags.conf", ios::in)) == NULL)
	{
		printf("找不到配置文件\n");
		return;
	}

	string buf;
	while (getline(*file, buf)) {
		vector<string> nameAndTags = StringUtil::split(buf, ":");
		vector<string> tags;
		for (string tag : StringUtil::split(nameAndTags[1], ","))
			tags.push_back(StringUtil::UTF8ToGB(StringUtil::trim(tag)));
		infos.push_back({ tags, StringUtil::trim(nameAndTags[0]) });
	}
	FileUtil::close(file);

	// Clean up temporary files
	vector<string> tempFilePaths = FileUtil::listFilePaths("./temp/", "*.md");
	for (string path : tempFilePaths) {
		if (remove(path.c_str()) - 1) {
			errno_t err;
			_get_errno(&err);
			if (EACCES == err)
			{
				printf("%s had been opend by some application, can't delete.\n", path.c_str());
				break;
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
		fstream* outFile;
		fstream* inFile;
		string buf;
		string filePath = "./temp/temp-" + to_string(time(0)) + ".md";
		if ((outFile = FileUtil::open(filePath, ios::out | ios::binary)) == NULL)
		{
			printf("临时文件创建失败\n");
			return;
		}

		int count = 5;
		for (Info info : infos) {
			if (--count == 0)
				break;
			*outFile << endl << "## " << info.fileName << endl;
			string path = StringUtil::UTF8ToGB("./data/" + info.fileName + ".md");
			if ((inFile = FileUtil::open(path, ios::in | ios::binary)) == NULL)
			{
				cout << "找不到文件: " + path << endl;
				return;
			}
			while (!inFile->eof()) {
				char szBuf;
				inFile->read(&szBuf, sizeof(char));

				if (inFile->eof())
					break;

				if (outFile->bad())
					break;

				outFile->write(&szBuf, sizeof(char));
			}
			FileUtil::close(inFile);
		}
		FileUtil::close(outFile);

		std::system(("start " + filePath).c_str());
	}
}

int main()
{
	init();
	run();
	return 0;
}
