#include "SceneCollision.h"
#include "MeshLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

SceneCollision::SceneCollision() : Scene()
{
	InitGL();
	InitLighting();
	InitObjects();

	std::cout << "Collision scene loaded." << std::endl;

	glutMainLoop();
}

SceneCollision::~SceneCollision(void)
{
	delete camera;
	camera = NULL;

	delete _lightPosition;
	_lightPosition = NULL;

	delete _lightData;
	_lightData = NULL;

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		delete objects[i];
		objects[i] = NULL;
	}
}

void SceneCollision::InitGL()
{
	GLUTCallbacks::Init(this);

	glutSpecialFunc(GLUTCallbacks::KeyboardSpecial);
	glutDisplayFunc(GLUTCallbacks::Display);
}

void SceneCollision::InitLighting()
{
	_lightPosition = new Vector4();

	_lightPosition->x = 0.0;
	_lightPosition->y = 0.0;
	_lightPosition->z = 1.0;
	_lightPosition->w = 0.0;

	_lightData = new Lighting();

	_lightData->Ambient.x = 0.2;
	_lightData->Ambient.y = 0.2;
	_lightData->Ambient.z = 0.2;
	_lightData->Ambient.w = 1.0;

	_lightData->Diffuse.x = 0.8;
	_lightData->Diffuse.y = 0.8;
	_lightData->Diffuse.z = 0.8;
	_lightData->Diffuse.w = 1.0;

	_lightData->Specular.x = 0.2;
	_lightData->Specular.y = 0.2;
	_lightData->Specular.z = 0.2;
	_lightData->Specular.w = 1.0;
}

void SceneCollision::InitObjects()
{
	camera = new Camera();

	camera->eye.x = 0.0f;
	camera->eye.y = 0.0f;
	camera->eye.z = 1.0f;

	camera->center.x = 0.0f;
	camera->center.y = 0.0f;
	camera->center.z = 0.0f;

	camera->up.x = 0.0f;
	camera->up.y = 1.0f;
	camera->up.z = 0.0f;

	cubeMesh = MeshLoader::Load((char*)"Objects/cube.txt");

	texturePenguins = new Texture2D();
	texturePenguins->Load((char*)"Textures/penguins.raw", 512, 512);

	textureStars = new Texture2D();
	textureStars->Load((char*)"Textures/stars.raw", 512, 512);

	objects[0] = new Cube(cubeMesh, texturePenguins, -2.5f, 0.0f, -5.0f);
	objects[1] = new Cube(cubeMesh, textureStars, 2.5f, 0.0f, -5.0f);
}

void SceneCollision::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		objects[i]->Draw();
	}

	Vector3 v = { -1.8f, 1.7f, -1.0f };
	Color c = { 0.0f, 1.0f, 0.0f };
	DrawString("Collision Example", &v, &c);

	glFlush();
	glutSwapBuffers();
}

void SceneCollision::Update()
{
	glLoadIdentity();

	gluLookAt(
		camera->eye.x,
		camera->eye.y,
		camera->eye.z,

		camera->center.x,
		camera->center.y,
		camera->center.z,

		camera->up.x,
		camera->up.y,
		camera->up.z
	);

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		objects[i]->Update();
		
		if (objects[i]->_position.x > 4.0f)
			objects[i]->_position.x -= 0.1f;
		if(objects[i]->_position.x < -4.0f)
			objects[i]->_position.x += 0.1f;

		if(objects[i]->_position.y > 4.0f)
			objects[i]->_position.y -= 0.1f;
		if (objects[i]->_position.y < -4.0f)
			objects[i]->_position.y += 0.1f;
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));

	glutPostRedisplay();
}

void SceneCollision::Keyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
		objects[0]->_position.y += 0.1f;
	if(key == 's')
		objects[0]->_position.y -= 0.1f;

	if (key == 'a')
		objects[0]->_position.x -= 0.1f;
	if (key == 'd')
		objects[0]->_position.x += 0.1f;

	for (int i = 0; i < OBJECTCOUNT; i++)
	{
		switch (key)
		{
		case 'r':
			objects[i]->red = 1.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 0.0f;
			break;
		case 'g':
			objects[i]->red = 0.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 0.0f;
			break;
		case 'b':
			objects[i]->red = 0.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 1.0f;
			break;
		case 'n':
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;
			break;
		case 'c':
			objects[i]->red = 0.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 1.0f;
			break;
		case 'm':
			objects[i]->red = 1.0f;
			objects[i]->green = 0.0f;
			objects[i]->blue = 1.0f;
			break;
		case 'y':
			objects[i]->red = 1.0f;
			objects[i]->green = 1.0f;
			objects[i]->blue = 0.0f;
			break;
		}
	}
}

void SceneCollision::KeyboardSpecial(int key, int x, int y)
{
	if(key == GLUT_KEY_UP)
		objects[1]->_position.y += 0.1f;
	if(key == GLUT_KEY_DOWN)
		objects[1]->_position.y -= 0.1f;

	if (key == GLUT_KEY_LEFT)
		objects[1]->_position.x -= 0.1f;
	if (key == GLUT_KEY_RIGHT)
		objects[1]->_position.x += 0.1f;
}

void SceneCollision::DrawString(const char* text, Vector3* position, Color* color)
{
	glPushMatrix();
		glTranslatef(position->x, position->y, position->z);
		glRasterPos2f(0.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);
	glPopMatrix();
}