#include "Parser.h"
#include "../primitive/Mesh.h"
#include "../light/AreaLight.h"
#include "ObjParser.h"
#include <fstream>
#include <string>

bool Parser::parse(std::string fileName,Scene* scene)
{
	if(fileName.find(".scene")!=-1)
	{
		//解析场景
		ifstream file(fileName);
		if(!file.is_open()) {
			cout<<"scene file:"+fileName+" not found!"<<endl;
			return false;
		}

		int pos = fileName.find_last_of('/');
		string basePath = fileName.substr(0,pos+1);

		string type;
		bool bError = false;

		while(file>>type)
		{
			if(type=="Obj")
			{
				string objFileName;
				file>>objFileName;
				Mesh* mesh = NULL;
				if(!ObjParser::parse(basePath+objFileName,mesh)) 
				{
					bError = true;
					break;
				}
				else
				{
					for(int i=0,len=mesh->triangleList.size();i<len;++i)
					{
						MeshTriangle* tri = ((MeshTriangle*)mesh->triangleList[i]);
						tri->init();
						Material& attr = tri->attr;
						if(attr.emission!=Color3::BLACK) 
							scene->lights.push_back(new AreaLight(tri->origin,tri->dx,tri->dy,attr.emission));
					}
					mesh->init();
					scene->primitives.push_back(mesh);
				}
			}
			else if(type=="Camera")
			{
				Camera* camera = new Camera;
				int width,height;
				double fov;

				while(file>>type)
				{
					if(type=="EndCamera")
					{
						camera->calcViewPort(fov,width,height);
						scene->camera = camera;
						break;
					}
					else if(type=="lookat")
					{
						Vec3 position,target,up;
						file>>position.x>>position.y>>position.z>>target.x>>target.y>>target.z>>up.x>>up.y>>up.z;
						camera->lookAt(position.x,position.y,position.z,target.x,target.y,target.z,up.x,up.y,up.z);
					}
					else if(type=="res")
					{
						file>>width>>height;
					}
					else if(type=="fov")
					{
						file>>fov;
					}
				}
			}
			else if(type=="AreaLight")
			{
				Vec3 origin,dx,dy,emission;
				while (file>>type)
				{
					if(type=="EndAreaLight")
					{
						//默认加入矩形光源
						scene->lights.push_back(new AreaLight(origin,dx,dy,emission));
						scene->lights.push_back(new AreaLight(origin+dx+dy,-dx,-dy,emission));
						break;
					}
					else if(type=="position")
					{
						file>>origin.x>>origin.y>>origin.z;
					}
					else if(type=="dx")
					{
						file>>dx.x>>dx.y>>dx.z;
					}
					else if(type=="dy")
					{
						file>>dy.x>>dy.y>>dy.z;
					}
					else if(type=="intensity")
					{
						file>>emission.x>>emission.y>>emission.z;
					}
				}
			}
		}

		file.close();

		return !bError;

	}
	else
	{
		//自己建模的场景,现在渲染可能会有些问题

		Camera* camera = new Camera;
		scene->camera = camera;

		PointLight* pointLight = new PointLight(Vec3(0,5,-5),Vec3::WHITE);
		scene->lights.push_back(pointLight);

		IPrimitive* sphere = new Sphere(Point3(2,0,-5),1);
		Material attr;
		attr.ref.ka = Vec3(1,0,0);
		attr.ref.kd = Vec3(0.70,0.27,0.08);
		attr.ref.ks = Vec3(0.26,0.18,0.06);
		attr.shiness = 12.8;
		sphere->attr = attr;
		scene->primitives.push_back(sphere);

		IPrimitive* cube = new Cube();
		cube->rotate(-30,Vec3(0,1,0));
		cube->rotate(-20,Vec3(1,0,0));
		cube->translate(-1,0,-5);
		Material attr2;
		attr2.ref.ka = Vec3(0,0,1);
		attr2.ref.kd = Vec3(0.41,0.1,0.41);
		attr2.ref.ks = Vec3(0.41,0.41,0.41);
		attr2.shiness = 51.2;
		cube->attr = attr2;
		scene->primitives.push_back(cube);

		Plane* plane1 = new Plane(Point3(-5,-5,10),Vec3(1000,0,0),Vec3(0,0,-1000));//floor
		Plane* plane2 = new Plane(Point3(-5,-5,10),Vec3(0,0,-1000),Vec3(0,1000,0));//left
		Plane* plane3 = new Plane(Point3(-5,-5,-10),Vec3(1000,0,0),Vec3(0,1000,0));//back
		Plane* plane4 = new Plane(Point3(-5,5,10),Vec3(0,0,-1000),Vec3(1000,0,0));//ceil
		Plane* plane5 = new Plane(Point3(5,-5,10),Vec3(0,1000,0),Vec3(0,0,-1000));//right
		Plane* plane6 = new Plane(Point3(-5,-5,10),Vec3(0,1000,0),Vec3(1000,0,0));//front

		Material attr3;
		attr3.ref.ka = Vec3(1, 1, 1);
		attr3.ref.kd = Vec3(0.5, 0.5, 0.5);
		attr3.ref.ks = Vec3(0,0,0);
		attr3.shiness = 89.6;
		plane1->attr = attr3;
		plane2->attr = attr3;
		plane3->attr = attr3;
		plane4->attr = attr3;
		plane5->attr = attr3;
		plane6->attr = attr3;

		scene->primitives.push_back(plane1);
		scene->primitives.push_back(plane2);
		scene->primitives.push_back(plane3);
		scene->primitives.push_back(plane4);
		scene->primitives.push_back(plane5);
		scene->primitives.push_back(plane6);
	}

	return true;
}