#include <stdio.h>
#include "raytracer/RayTracer.h"
#include "raytracer/Scene.h"
#include "common/Parser.h"
#include "common/GlutDisplay.h"

int main(int argc,char *argv[])
{
	char* inputFileName = "default.txt";
	if(argc>=2) inputFileName = argv[1];

	Scene* scene = new Scene;
	Parser::parse(inputFileName,scene);
	RayTracer* rayTracer = new RayTracer;
	rayTracer->setScene(scene);
	GlutDisplay::setRayTracer(rayTracer);
	GlutDisplay::render(argc,argv);

	return 0;
}