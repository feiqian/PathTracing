#ifndef __UTILS_H__
#define __UTILS_H__
#include <iostream>
#include <io.h>
#include <vector>
#include <string>
#include <list>

struct Utils
{
	static std::vector<std::string> GetFiles(const std::string& folder, const std::string& fileExtension = std::string("*"));
	static void PrintProgress(double& progress,double increment);
};

#endif