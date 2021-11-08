#pragma once
#include <fstream>
#include <iostream>
#include <io.h>
#include <string>


static std::vector<std::string> get_files_inDirectory(const std::string _path, const std::string _fillter)
{
	std::vector<std::string> return_;

	struct _finddata_t fd;

	intptr_t handle;

	if ((handle = _findfirst((_path + _fillter).c_str(), &fd)) == -1L)
		return return_;
	do
	{
		return_.push_back(fd.name);

	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);

	return return_;
}

class StringSaver
{
	std::string file_path;
	std::ofstream writeFile;
public:
	StringSaver(std::string file_path) : 
		file_path(file_path),
		writeFile(file_path)
	{
	}
	~StringSaver()
	{
		writeFile.close();
	}
	void set_line(std::string line)
	{
		writeFile << line + '\n';
	}
};

class StringReader
{
	std::string file_path;
	std::fstream openFile;
public:
	StringReader(std::string file_path) :
		file_path(file_path),
		openFile(file_path)
	{
	}
	~StringReader()
	{
		openFile.close();
	}
	std::string get_line()
	{
		char line[1000]{};
		openFile.getline(line, 1000);
		return line;
	}
	bool get_is_opened() { return openFile.is_open(); }
};