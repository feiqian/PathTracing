#include "GlutDisplay.h"

RayTracer* GlutDisplay::rayTracer = NULL;
Camera* GlutDisplay::camera = NULL;
bool GlutDisplay::bMouseDown = false;
int GlutDisplay::mouseX=-1;
int GlutDisplay::mouseY=-1;
Vec3** GlutDisplay::rgbArr;

void GlutDisplay::reshape(int w, int h)
{
	// prevents division by zero when minimizing window
	if (h == 0)	h = 1;
	glViewport(0, 0, w, h);
	rayTracer->getScene()->setSize(w,h);
}

void GlutDisplay::mouseCallBack(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			bMouseDown = true;
			mouseX=x;
			mouseY=y;
			return;
		}
	}
	bMouseDown = false;
}

void GlutDisplay::motionCallBack(int x, int y )
{
	if (bMouseDown)
	{
		camera->rotate(x - mouseX,Vec3(0,1,0));
		camera->rotate(y - mouseY,Vec3(1,0,0));
		mouseX = x;
		mouseY = y;
		glutPostRedisplay();
	}
}

void GlutDisplay::setRayTracer(RayTracer* rayTracer)
{
	GlutDisplay::rayTracer = rayTracer;
	GlutDisplay::camera = rayTracer->getScene()->camera;
	GlutDisplay::rgbArr = rayTracer->render();
}


void GlutDisplay::render(int argc, char**argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
	glutInitWindowSize(rayTracer->getScene()->getWidth(), rayTracer->getScene()->getHeight());
	glutCreateWindow("Ray Tracer");
	glutDisplayFunc(GlutDisplay::_render);
	//glutReshapeFunc(reshape);
	//glutMouseFunc(mouseCallBack);
	//glutMotionFunc(motionCallBack);
	
	glutMainLoop();
}

void GlutDisplay::_render()
{
	Scene* scene = rayTracer->getScene();
	int width = scene->getWidth(),height = scene->getHeight();

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,0,height);
	
	glBegin(GL_POINTS);
	for (int y = 0; y < height; ++y)  
	{   
		for (int x = 0; x < width; ++x)  
		{  
			Color3& rgb = rgbArr[y][x];
			glColor3d(rgb.r,rgb.g,rgb.b);  
			glVertex2f(x,y);  
		}  
	}  
	glEnd();

	glutSwapBuffers();
}