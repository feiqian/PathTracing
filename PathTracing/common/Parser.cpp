#include "Parser.h"
#include "../primitive/Mesh.h"
#include "ObjParser.h"

bool Parser::parse(std::string fileName,Scene* scene)
{
	Camera* camera = new Camera;
	scene->camera = camera;

	if(fileName.find(".obj")!=-1)
	{
		Mesh* mesh = NULL;
		if(ObjParser::parse(fileName,mesh)) scene->primitives.push_back(mesh);
		else return false;
	}
	else
	{
		//自己建模的场景
		PointLight* pointLight = new PointLight(Vec3(0,5,-5),Vec3::WHITE,1);
		scene->primitives.push_back(pointLight);

		IPrimitive* sphere = new Sphere(Point3(2,0,-5),1);
		Material attr;
		attr.ka = Vec3(1,0,0);
		attr.kd = Vec3(0.70,0.27,0.08);
		attr.ks = Vec3(0.26,0.18,0.06);
		attr.shiness = 12.8;
		sphere->attr = attr;
		scene->primitives.push_back(sphere);

		IPrimitive* cube = new Cube();
		cube->rotate(-30,Vec3(0,1,0));
		cube->rotate(-20,Vec3(1,0,0));
		cube->translate(-1,0,-5);
		Material attr2;
		attr2.ka = Vec3(0,0,1);
		attr2.kd = Vec3(0.41,0.1,0.41);
		attr2.ks = Vec3(0.41,0.41,0.41);
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
		attr3.ka = Vec3(1, 1, 1);
		attr3.kd = Vec3(0.5, 0.5, 0.5);
		attr3.ks = Vec3(0,0,0);
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