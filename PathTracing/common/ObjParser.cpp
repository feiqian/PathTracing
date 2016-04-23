#include "ObjParser.h"
#include "../common/Material.h"
#include <fstream>
#include <string>
#include <map>

bool parseMaterial(string mtlPath,map<string,Material>& mp)
{
	ifstream file(mtlPath);
	if(!file.is_open()) {
		cout<<"mtl file:"+mtlPath+" not found!"<<endl;
		return false;
	}

	string type;
	string materialName;
	Material attr;
	bool flag = false;
	int illuminationModel;

	while(file>>type)
	{
		if(type=="newmtl")
		{				
			if(flag) 
			{
				mp.insert(make_pair(materialName,attr));
				attr = Material();
			}
			else flag = true;

			file>>materialName;
			attr.name = materialName;
		}
		else if(type=="Kd")
		{
			file>>attr.kd.x>>attr.kd.y>>attr.kd.z;
		}
		else if(type=="Ka")
		{
			file>>attr.ka.x>>attr.ka.y>>attr.ka.z;
		}
		else if(type=="Ks")
		{
			file>>attr.ks.x>>attr.ks.y>>attr.ks.z;
		}
		else if(type=="Ke")
		{
			file>>attr.emission.r>>attr.emission.g>>attr.emission.b;
		}
		else if(type=="Ns")
		{
			file>>attr.shiness;
		}
		else if(type=="Ni")
		{
			file>>attr.refractiveIndex;
		}
		else if(type=="Tf")
		{
			file>>attr.tf.x>>attr.tf.y>>attr.tf.z;
		}
		else if(type=="illum")
		{
			file>>illuminationModel;
			if(illuminationModel==5||illuminationModel==7) attr.bUseFresnel = true;
		}
	}

	if(flag) mp.insert(make_pair(materialName,attr));

	return true;
}



bool ObjParser::parse(string objPath,Mesh*& mesh)
{
	ifstream file(objPath);
	if(!file.is_open()) {
		cout<<"obj file:"+objPath+" not found!"<<endl;
		return false;
	}

	string type;
	Point3 pt;
	Point3 vt;
	Vec3 vn;
	int vertI[3];
	int normI[3];
	int texI[3];

	map<string,Material> materialMap;
	string materialName;
	Material attr;
	mesh = new Mesh;
	bool returnValue = true;

	while (file>>type)
	{
		if(type=="v")
		{
			file>>pt.x>>pt.y>>pt.z;
			mesh->vertices.push_back(pt);
		}
		else if(type=="f")
		{
			int index = 0;
			memset(normI,255,sizeof(normI));
			memset(texI,255,sizeof(texI));

			while (true)
			{
				char ch = file.get();
				if(ch==' ') continue;
				else if(ch == '\n' || ch == EOF) break;
				else file.putback(ch);

				file>>vertI[index];
				--vertI[index];

				char splitter = file.get();

				if(splitter=='/')
				{
					splitter = file.get();
					if(splitter=='/')
					{
						file>>normI[index];
						--normI[index];
					}
					else
					{
						file.putback(splitter);
						file>>texI[index];
						--texI[index];

						splitter = file.get();
						if(splitter=='/')
						{
							file>>normI[index];
							--normI[index];
						}
						else file.putback(splitter);
					}
				}
				else file.putback(splitter);

				++index;
				if(index>=3)
				{
					MeshTriangle* tri = new MeshTriangle(mesh,vertI,normI,texI);
					tri->attr = attr;
					mesh->triangleList.push_back(tri);

					vertI[1]=vertI[2];
					normI[1]=normI[2];
					texI[1]=texI[2];
					index = 2;
				}
			}
		}
		else if(type=="vn")
		{
			file>>vn.x>>vn.y>>vn.z;
			mesh->normals.push_back(vn);
		}
		else if(type=="vt")
		{
			file>>vt.x>>vt.z;
			mesh->textures.push_back(vt);
		}
		else if(type=="usemtl")
		{
			file>>materialName;
			map<string,Material>::const_iterator it = materialMap.find(materialName);
			if(it!=materialMap.end()) attr = it->second;
			else
			{
				cout<<"material information about "+materialName+" not found in mtl file!"<<endl;
				returnValue = false;
				break;
			}
		}
		else if(type=="mtllib")
		{
			string mtlPath;
			file>>mtlPath;
			
			int pos = objPath.find_last_of('/');
			string basePath = objPath.substr(0,pos+1);
			mtlPath = basePath+mtlPath;

			if(!parseMaterial(mtlPath,materialMap)) {
				returnValue  = false;
				break;
			}
		}
	}

	if(!materialMap.size())
	{
		returnValue = false;
		cout<<"material information  of this model not found!"<<endl;
	}

	if(!returnValue) delete mesh;

	file.close();
	return returnValue;
}