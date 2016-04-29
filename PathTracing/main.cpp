#include <iostream>
#include <string>
#include "raytracer/RayTracer.h"
#include "common/Utils.h"
using namespace std;

int main(int argc,char *argv[])
{
	string objDirectory="../data";	
	vector<string> files = Utils::GetSubFolders(objDirectory);

	if(files.size())
	{
		for(int i=0,len=files.size();i<len;++i)
		{
			cout<<i<<":"<<files[i]<<endl;
		}

		int id;
		while(true)
		{
			cout<<"please choose one scene to render: ";
			cin>>id;
			if(id>=0&&id<files.size()) break;
			else cout<<"valid input"<<endl;
		}

		string objFile =  objDirectory+"/"+files[id]+"/"+files[id]+".scene";
		RayTracer* rayTracer = new RayTracer;
		rayTracer->run(objFile);
	}
	return 0;
}