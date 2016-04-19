#ifndef __OBJ_PARSER_H__
#define __OBJ_PARSER_H__
#include <iostream>
#include <vector>
#include "VMath.h"
#include "Parser.h"
#include "../primitive/Mesh.h"
using namespace std;

struct ObjParser
{
	static bool parse(string objPath,Mesh*& mesh);
};


#endif