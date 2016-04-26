#include "Utils.h"
#include "windows.h"

std::vector<std::string> Utils::GetSubFolders(const std::string& folder)
{
	std::string path = folder+"\\*.*";
	std::vector<std::string> files;
	struct _finddata_t file_info;

	intptr_t  file_handler = _findfirst(path.c_str(), &file_info);

	while (file_handler != -1)
	{
		if ((file_info.attrib & _A_SUBDIR)&&!(strcmp(file_info.name,".")==0||strcmp(file_info.name,"..")==0)) 
			files.push_back(file_info.name);
		if (_findnext(file_handler, &file_info) != 0) break;
	}
	_findclose(file_handler);

	return files;
}

void Utils::PrintProgress(double& progress,double increment)
{
	static CONSOLE_SCREEN_BUFFER_INFO info;

	if(!progress) GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
	double newProgress = progress+increment;

	if(int(newProgress+0.5)!=int(progress+0.5)) 
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),info.dwCursorPosition);
		std::cout<<"progress:"<<int(newProgress+0.5)<<"%"<<std::endl;
	}
	progress = newProgress;
}