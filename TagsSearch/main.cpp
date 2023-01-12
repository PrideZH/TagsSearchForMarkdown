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
	// Read repo
	string buf;
	vector<util::File*> files = util::FileUtil::getFiles("./repo/", "*.md");
	for (util::File* file : files) {
		if (!file->open(ios::in)) continue;
		getline(*file->fileStream, buf);
		if (buf.substr(0, 5) != "tags:") {
			cout << "格式错误: " << file->filepath << endl;
			continue;
		}
		vector<string> tags = StringUtil::split(StringUtil::split(buf, ":")[1], ",");
		for (size_t i = 0; i < tags.size(); i++)
			tags[i] = StringUtil::trim(tags[i]);
		infos.push_back({ tags, StringUtil::trim(file->name) });
		file->close();
	}

	// Clean up temporary files
	vector<util::File*> tempFiles = util::FileUtil::getFiles("./temp/", "*.md");
	for (util::File* file : tempFiles)
		file->remove();
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
		util::File* outFile;
		util::File* inFile;
		string buf;
		string filePath = "./temp/temp-" + to_string(time(0)) + ".md";
		if ((outFile = util::FileUtil::open(filePath, ios::out)) == NULL)
		{
			printf("临时文件创建失败\n");
			return;
		}

		int count = 5;
		for (Info info : infos) {
			if (--count == 0)
				break;
			string path = "./repo/" + info.fileName + ".md";
			if ((inFile = util::FileUtil::open(path, ios::in)) == NULL)
			{
				cout << "找不到文件: " + path << endl;
				return;
			}

			*outFile->fileStream << endl << "## [" + StringUtil::toUTF8(info.fileName) + "](" + StringUtil::toUTF8(path) + ")" << endl;
			*outFile->fileStream << "<p>" << endl;
			for (string tag : info.tags)
				*outFile->fileStream << "<img alt = \"" + tag + "\" src = \"https://img.shields.io/badge/-" + tag + "-lightgrey\">" << endl;
			*outFile->fileStream << "</p>" << endl;

			while (!inFile->fileStream->eof()) {
				char szBuf;
				inFile->fileStream->read(&szBuf, sizeof(char));

				if (inFile->fileStream->eof())
					break;

				if (outFile->fileStream->bad())
					break;

				outFile->fileStream->write(&szBuf, sizeof(char));
			}
			inFile->close();
		}
		outFile->close();

		std::system(("start " + filePath).c_str());
	}
}

int main()
{
	init();
	run();
	return 0;
}
