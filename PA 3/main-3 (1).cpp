#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <math.h>
#include "shader.h"
#include "shaderprogram.h"

/*=================================================================================================
	DOMAIN
=================================================================================================*/

// Window dimensions
const int InitWindowWidth = 800;
const int InitWindowHeight = 800;
int WindowWidth = InitWindowWidth;
int WindowHeight = InitWindowHeight;

// Last mouse cursor position
int LastMousePosX = 0;
int LastMousePosY = 0;

// Arrays that track which keys are currently pressed
bool key_states[256];
bool key_special_states[256];
bool mouse_states[8];

double twopi = 2 * 3.14159265359;
bool draw_wireframe = false;


int n = 5;
float r = 0.5;
float R = 2;

float* torus_vertices(int increment, float r, float R) {
	//24 points, goes through 2 for loops of increment size
	float* torus_vert = new float[24 * (2 * increment * increment)];
	int count = 0;

	//calculates shape of torus, +0.001 because doesnt reach to twopi by 0.001, adds for other vertices
	for (float theta = twopi / increment; theta < twopi + 0.001; theta += twopi / increment) {
		for (float phi = twopi / increment; phi <= twopi + 0.001; phi += twopi / increment) {
			float add = (twopi / increment);

			//1st triangle
			torus_vert[count++] = ((R + (r * cos(theta))) * cos(phi));
			torus_vert[count++] = ((R + (r * cos(theta))) * sin(phi));
			torus_vert[count++] = (r * sin(theta));
			torus_vert[count++] = (1);

			torus_vert[count++] = ((R + (r * cos(theta))) * cos(phi + add));
			torus_vert[count++] = ((R + (r * cos(theta))) * sin(phi + add));
			torus_vert[count++] = (r * sin(theta));
			torus_vert[count++] = (1);

			torus_vert[count++] = ((R + (r * cos(theta + add))) * cos(phi));
			torus_vert[count++] = ((R + (r * cos(theta + add))) * sin(phi));
			torus_vert[count++] = (r * sin(theta + add));
			torus_vert[count++] = (1);


			//2nd triangle

			torus_vert[count++] = ((R + (r * cos(theta + add))) * cos(phi + add));
			torus_vert[count++] = ((R + (r * cos(theta + add))) * sin(phi + add));
			torus_vert[count++] = (r * sin(theta + add));
			torus_vert[count++] = (1);

			torus_vert[count++] = ((R + (r * cos(theta + add))) * cos(phi));
			torus_vert[count++] = ((R + (r * cos(theta + add))) * sin(phi));
			torus_vert[count++] = (r * sin(theta + add));
			torus_vert[count++] = (1);

			torus_vert[count++] = ((R + (r * cos(theta))) * cos(phi + add));
			torus_vert[count++] = ((R + (r * cos(theta))) * sin(phi + add));
			torus_vert[count++] = (r * sin(theta));
			torus_vert[count++] = (1);



		}
	}


	return torus_vert;
}


float* flat_normals(int increment, float r, float R) {
	float* flat_norm = new float[24 * (2 * increment * increment)];
	int count = 0;


	//goes through around whole torus to find flat normals
	for (float theta = twopi / increment; theta < twopi + 0.001; theta += twopi / increment) {
		for (float phi = twopi / increment; phi <= twopi + (twopi / increment) + 0.001; phi += twopi / increment) {
			float add = (twopi / increment);
			float firstX, secondX, thirdX, firstY, secondY, thirdY, firstZ, secondZ, thirdZ;

			//gets 3 points, 
			firstX = (R + r * cos(theta)) * cos(phi);
			firstY = (R + r * cos(theta)) * sin(phi);
			firstZ = r * sin(theta);

			secondX = (R + r * cos(theta)) * cos(phi + add);
			secondY = (R + r * cos(theta)) * sin(phi + add);
			secondZ = r * sin(theta);

			thirdX = (R + r * cos(theta + add)) * cos(phi);
			thirdY = (R + r * cos(theta + add)) * sin(phi);
			thirdZ = r * sin(theta + add);

			//subtracts points,helps calculate for flat shading
			glm::vec3 q = glm::vec3(secondX - firstX, secondY - firstY, secondZ - firstZ);
			glm::vec3 p = glm::vec3(thirdX - firstX, thirdY - firstY, thirdZ - firstZ);

			//gets cross product of u and v for flat shading
			glm::vec3 normal = glm::cross(q, p);


			//puts into array for buffer
			flat_norm[count++] = (normal.x);
			flat_norm[count++] = (normal.y);
			flat_norm[count++] = (normal.z);
			flat_norm[count++] = (1);

			flat_norm[count++] = (normal.x);
			flat_norm[count++] = (normal.y);
			flat_norm[count++] = (normal.z);
			flat_norm[count++] = (1);

			flat_norm[count++] = (normal.x);
			flat_norm[count++] = (normal.y);
			flat_norm[count++] = (normal.z);
			flat_norm[count++] = (1);



			//repeats for other neighbor vertices
			firstX = (R + r * cos(theta + add)) * cos(phi + add);
			firstY = (R + r * cos(theta + add)) * sin(phi + add);
			firstZ = r * sin(theta + add);

			secondX = (R + r * cos(theta + add)) * cos(phi);
			secondY = (R + r * cos(theta + add)) * sin(phi);
			secondZ = r * sin(theta + add);

			thirdX = (R + r * cos(theta)) * cos(phi + add);
			thirdY = (R + r * cos(theta)) * sin(phi + add);
			thirdZ = r * sin(theta);


			q = glm::vec3(secondX - firstX, secondY - firstY, secondZ - firstZ);
			p = glm::vec3(thirdX - firstX, thirdY - firstY, thirdZ - firstZ);

			normal = glm::cross(q, p);

			flat_norm[count++] = (normal.x);
			flat_norm[count++] = (normal.y);
			flat_norm[count++] = (normal.z);
			flat_norm[count++] = (1);

			flat_norm[count++] = (normal.x);
			flat_norm[count++] = (normal.y);
			flat_norm[count++] = (normal.z);
			flat_norm[count++] = (1);

			flat_norm[count++] = (normal.x);
			flat_norm[count++] = (normal.y);
			flat_norm[count++] = (normal.z);
			flat_norm[count++] = (1);
		}
	}



	return flat_norm;
}


float* draw_flat_normals(int increment, float r, float R) {
	float* draw_flat_norm = new float[24 * (2 * increment * increment)];
	int count = 0;

	for (float theta = twopi / increment; theta < twopi + 0.001; theta += twopi / increment) {
		for (float phi = twopi / increment; phi <= twopi + (twopi / increment) + 0.001; phi += twopi / increment) {
			float add = (twopi / increment);
			float firstX, secondX, thirdX, firstY, secondY, thirdY, firstZ, secondZ, thirdZ;

			//gets three points
			firstX = (R + r * cos(theta)) * cos(phi);
			firstY = (R + r * cos(theta)) * sin(phi);
			firstZ = r * sin(theta);

			secondX = (R + r * cos(theta)) * cos(phi + add);
			secondY = (R + r * cos(theta)) * sin(phi + add);
			secondZ = r * sin(theta);

			thirdX = (R + r * cos(theta + add)) * cos(phi);
			thirdY = (R + r * cos(theta + add)) * sin(phi);
			thirdZ = r * sin(theta + add);

			//calculates for flat normals by subtracting then finding the cross product
			glm::vec3 q = glm::vec3(secondX - firstX, secondY - firstY, secondZ - firstZ);
			glm::vec3 p = glm::vec3(thirdX - firstX, thirdY - firstY, thirdZ - firstZ);
			glm::vec3 normal = glm::cross(q, p);


			//divide by three, gets average
			draw_flat_norm[count++] = ((firstX + secondX + thirdX) / 3);
			draw_flat_norm[count++] = ((firstY + secondY + thirdY) / 3);
			draw_flat_norm[count++] = ((firstZ + secondZ + thirdZ) / 3);
			draw_flat_norm[count++] = (1);

			draw_flat_norm[count++] = ((firstX + secondX + thirdX) / 3 + normal.x);
			draw_flat_norm[count++] = ((firstY + secondY + thirdY) / 3 + normal.y);
			draw_flat_norm[count++] = ((firstZ + secondZ + thirdZ) / 3 + normal.z);
			draw_flat_norm[count++] = (1);



			//repeats for neighboring vertices
			firstX = (R + r * cos(theta + add)) * cos(phi + add);
			firstY = (R + r * cos(theta + add)) * sin(phi + add);
			firstZ = r * sin(theta + add);

			secondX = (R + r * cos(theta + add)) * cos(phi);
			secondY = (R + r * cos(theta + add)) * sin(phi);
			secondZ = r * sin(theta + add);

			thirdX = (R + r * cos(theta)) * cos(phi + add);
			thirdY = (R + r * cos(theta)) * sin(phi + add);
			thirdZ = r * sin(theta);

			q = glm::vec3(secondX - firstX, secondY - firstY, secondZ - firstZ);
			p = glm::vec3(thirdX - firstX, thirdY - firstY, thirdZ - firstZ);

			normal = glm::cross(q, p);

			draw_flat_norm[count++] = ((firstX + secondX + thirdX) / 3);
			draw_flat_norm[count++] = ((firstY + secondY + thirdY) / 3);
			draw_flat_norm[count++] = ((firstZ + secondZ + thirdZ) / 3);
			draw_flat_norm[count++] = (1);

			draw_flat_norm[count++] = ((firstX + secondX + thirdX) / 3 + normal.x);
			draw_flat_norm[count++] = ((firstY + secondY + thirdY) / 3 + normal.y);
			draw_flat_norm[count++] = ((firstZ + secondZ + thirdZ) / 3 + normal.z);
			draw_flat_norm[count++] = (1);
		}
	}



	return draw_flat_norm;
}


float* smooth_normals(int increment, float r, float R) {
	float* smooth_norm = new float[24 * (2 * increment * increment)];
	int count = 0;


	//goes through around the torus
	for (float theta = twopi / increment; theta < twopi + 0.001; theta += twopi / increment) {
		for (float phi = twopi / increment; phi <= twopi + (twopi / increment) + 0.001; phi += twopi / increment) {
			float add = (twopi / increment);
			float firstX, secondX, thirdX, firstY, secondY, thirdY, firstZ, secondZ, thirdZ;


			//gets 3 points, 
			firstX = (R + r * cos(theta)) * cos(phi);
			firstY = (R + r * cos(theta)) * sin(phi);
			firstZ = r * sin(theta);

			secondX = (R + r * cos(theta)) * cos(phi + add);
			secondY = (R + r * cos(theta)) * sin(phi + add);
			secondZ = r * sin(theta);

			thirdX = (R + r * cos(theta + add)) * cos(phi);
			thirdY = (R + r * cos(theta + add)) * sin(phi);
			thirdZ = r * sin(theta + add);


			//calculates for smooth shading v-c for 1 point
			smooth_norm[count++] = (firstX - R * cos(add));
			smooth_norm[count++] = (firstY - R * sin(add));
			smooth_norm[count++] = (firstZ);
			smooth_norm[count++] = (1);

			//calculates for smooth shading v-c for 2nd point

			smooth_norm[count++] = (secondX - R * cos(add));
			smooth_norm[count++] = (secondY - R * sin(add));
			smooth_norm[count++] = (secondZ);
			smooth_norm[count++] = (1);

			//calculates for smooth shading v-c for 3rd point
			smooth_norm[count++] = (thirdX - R * cos(add));
			smooth_norm[count++] = (thirdY - R * sin(add));
			smooth_norm[count++] = (thirdZ);
			smooth_norm[count++] = (1);



			//repeats for neighboring points
			firstX = (R + r * cos(theta + add)) * cos(phi + add);
			firstY = (R + r * cos(theta + add)) * sin(phi + add);
			firstZ = r * sin(theta + add);

			secondX = (R + r * cos(theta + add)) * cos(phi);
			secondY = (R + r * cos(theta + add)) * sin(phi);
			secondZ = r * sin(theta + add);

			thirdX = (R + r * cos(theta)) * cos(phi + add);
			thirdY = (R + r * cos(theta)) * sin(phi + add);
			thirdZ = r * sin(theta);

			smooth_norm[count++] = (firstX - R * cos(add));
			smooth_norm[count++] = (firstY - R * sin(add));
			smooth_norm[count++] = (firstZ);
			smooth_norm[count++] = (1);


			smooth_norm[count++] = (secondX - R * cos(add));
			smooth_norm[count++] = (secondY - R * sin(add));
			smooth_norm[count++] = (secondZ);
			smooth_norm[count++] = (1);



			smooth_norm[count++] = (thirdX - R * cos(add));
			smooth_norm[count++] = (thirdY - R * sin(add));
			smooth_norm[count++] = (thirdZ);
			smooth_norm[count++] = (1);
		}
	}


	return smooth_norm;
}


float* draw_smooth_normals(int increment, float r, float R) {
	float* draw_smooth_norm = new float[24 * (2 * increment * increment)];
	int count = 0;


	//goes through around torus
	for (float theta = twopi / increment; theta < twopi + 0.001; theta += twopi / increment) {
		for (float phi = twopi / increment; phi <= twopi + (twopi / increment) + 0.001; phi += twopi / increment) {
			float add = (twopi / increment);
			float firstX, secondX, thirdX, firstY, secondY, thirdY, firstZ, secondZ, thirdZ;


			//gets three points
			firstX = (R + r * cos(theta)) * cos(phi);
			firstY = (R + r * cos(theta)) * sin(phi);
			firstZ = r * sin(theta);

			secondX = (R + r * cos(theta)) * cos(phi + add);
			secondY = (R + r * cos(theta)) * sin(phi + add);
			secondZ = r * sin(theta);

			thirdX = (R + r * cos(theta + add)) * cos(phi);
			thirdY = (R + r * cos(theta + add)) * sin(phi);
			thirdZ = r * sin(theta + add);

			//gets coordinates to put into array for drawing
			draw_smooth_norm[count++] = (firstX);
			draw_smooth_norm[count++] = (firstY);
			draw_smooth_norm[count++] = (firstZ);
			draw_smooth_norm[count++] = (1);

			draw_smooth_norm[count++] = (firstX + (firstX - R * cos(add)));
			draw_smooth_norm[count++] = (firstY + (firstY - R * sin(add)));
			draw_smooth_norm[count++] = (firstZ + firstZ);
			draw_smooth_norm[count++] = (1);


			
			draw_smooth_norm[count++] = (secondX);
			draw_smooth_norm[count++] = (secondY);
			draw_smooth_norm[count++] = (secondZ);
			draw_smooth_norm[count++] = (1);

			draw_smooth_norm[count++] = (secondX + (secondX - R * cos(add)));
			draw_smooth_norm[count++] = (secondY + (secondY - R * sin(add)));
			draw_smooth_norm[count++] = (secondZ + secondZ);
			draw_smooth_norm[count++] = (1);


		}
	}


	return draw_smooth_norm;
}


/*=================================================================================================
	SHADERS & TRANSFORMATIONS
=================================================================================================*/

ShaderProgram PassthroughShader;
ShaderProgram PerspectiveShader;
ShaderProgram shader; //shader program

glm::mat4 PerspProjectionMatrix(1.0f);
glm::mat4 PerspViewMatrix(1.0f);
glm::mat4 PerspModelMatrix(1.0f);

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;

/*=================================================================================================
	OBJECTS
=================================================================================================*/


GLuint torus_VAO;
GLuint torus_VBO[3];
//torus vbos and vaos
GLuint normals_VAO;
GLuint normals_VBO[2];

/*=================================================================================================
	HELPER FUNCTIONS
=================================================================================================*/

void window_to_scene(int wx, int wy, float& sx, float& sy)
{
	sx = (2.0f * (float)wx / WindowWidth) - 1.0f;
	sy = 1.0f - (2.0f * (float)wy / WindowHeight);
}

/*=================================================================================================
	SHADERS
=================================================================================================*/

void CreateTransformationMatrices(void)
{
	// PROJECTION MATRIX
	PerspProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), (float)WindowWidth / (float)WindowHeight, 0.01f, 1000.0f);

	// VIEW MATRIX
	glm::vec3 eye(0.0, 0.0, 2.0);
	glm::vec3 center(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);

	PerspViewMatrix = glm::lookAt(eye, center, up);

	// MODEL MATRIX
	PerspModelMatrix = glm::mat4(1.0);
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationX), glm::vec3(1.0, 0.0, 0.0));
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationY), glm::vec3(0.0, 1.0, 0.0));
	PerspModelMatrix = glm::scale(PerspModelMatrix, glm::vec3(perspZoom));
}

void CreateShaders(void)
{
	// Renders without any transformations
	PassthroughShader.Create("./shaders/simple.vert", "./shaders/simple.frag");

	// Renders using perspective projection
	PerspectiveShader.Create("./shaders/persp.vert", "./shaders/persp.frag");

	// creates new shader
	shader.Create("./shaders/persplight.vert", "./shaders/persplight.frag");
}

/*=================================================================================================
	BUFFERS
=================================================================================================*/
bool check_flat = true;
bool check_smooth = false;
bool draw_normals = false;
//torus colors array
float* torus_colors = new float[100000];
float* normals_colors = new float[100000];

void remake_torus() {

	//creates red color for torus
	for (int i = 0; i <= 100000;i += 4) {
		torus_colors[i] = 1;
		torus_colors[i + 1] = 1;
		torus_colors[i + 2] = 0;
		torus_colors[i + 3] = 0;
	};


	//gets torus vertices
	float* torus = torus_vertices(n, r, R);
	float* norm;


	//checks for flat or smooth is true
	if (check_smooth == true) {
		norm = draw_smooth_normals(n, r, R);
	}
	else {
		norm = draw_flat_normals(n, r, R);
	}



	//for vertices
	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * (2 * n * n) * sizeof(float), torus, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//for colors
	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 24 * (2 * n * n) * sizeof(float), torus_colors -1, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//for shaders
	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 24 * (2 * n * n) * sizeof(float), norm, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	delete[] torus;
	delete[] norm;
}


void normals() {
	float* norm;

	//checks for flat or smooth
	if (check_smooth == true) {
		norm = draw_smooth_normals(n, r, R);
	}
	else {
		norm = draw_flat_normals(n, r, R);
	}

	//creates colors for normals' lines
	for (int i = 0; i <= 100000;i += 4) {
		normals_colors[i] = 0;
		normals_colors[i + 1] = 0;
		normals_colors[i + 2] = 1;
		normals_colors[i + 3] = 0;
	};
	//normals vert
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 16 * (2 * n * n) * sizeof(float), norm, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//normals color
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 16 * (2 * n * n) * sizeof(float), normals_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	delete[] norm;
}

//creates buffers for torus and normals
void torusBuffers(void)
{
	glGenVertexArrays(1, &torus_VAO);
	glBindVertexArray(torus_VAO);
	glGenBuffers(3, &torus_VBO[0]);
	remake_torus();
}
void normalBuffers(void)
{
	glGenVertexArrays(1, &normals_VAO);
	glBindVertexArray(normals_VAO);
	glGenBuffers(2, &normals_VBO[0]);

	normals();
}

/*=================================================================================================
	CALLBACKS
=================================================================================================*/

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	// uncomment below to repeatedly draw new frames
	glutPostRedisplay();
}

void reshape_func(int width, int height)
{
	WindowWidth = width;
	WindowHeight = height;

	glViewport(0, 0, width, height);
	glutPostRedisplay();
}






void keyboard_func(unsigned char key, int x, int y)
{
	key_states[key] = true;

	switch (key)
	{

	case '1':
	{
		draw_wireframe = !draw_wireframe;
		if (draw_wireframe == true)
			std::cout << "Wireframes on.\n";
		else
			std::cout << "Wireframes off.\n";
	}
	case'w': {
		r += 0.1;
		remake_torus();
		normals();
		break;
	}
	case's': {
		r -= 0.1;
		remake_torus();
		normals();
		break;
	}
	case'e': {
		R += 0.1;
		remake_torus();
		normals();
		break;
	}
	case'd': {
		R -= 0.1;
		remake_torus();
		normals();
		break;
	}

	case 'q': {
		n++;
		remake_torus();
		normals();
		break;
	}
	case 'a': {
		n--;
		remake_torus();
		normals();
		break;
	}
	case 'x':
	{
		check_smooth = true;
		check_flat = false;
		remake_torus();
		normals();
		break;
	}
	case 'z':
	{
		check_flat = true;
		check_smooth = false;
		remake_torus();
		normals();
		break;
	}
	case 'c':
	{
		draw_normals = !draw_normals;
		break;
	}

	case 'f':
	{
		draw_wireframe = !draw_wireframe;
		if (draw_wireframe == true)
			std::cout << "Wireframes on.\n";
		else
			std::cout << "Wireframes off.\n";
		break;
	}

	// Exit on escape key press
	case '\fx1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}
	}
}

void key_released(unsigned char key, int x, int y)
{
	key_states[key] = false;
}

void key_special_pressed(int key, int x, int y)
{
	key_special_states[key] = true;
}

void key_special_released(int key, int x, int y)
{
	key_special_states[key] = false;
}

void mouse_func(int button, int state, int x, int y)
{
	// Key 0: left button
	// Key 1: middle button
	// Key 2: right button
	// Key 3: scroll up
	// Key 4: scroll down

	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (button == 3)
	{
		perspZoom += 0.03f;
	}
	else if (button == 4)
	{
		if (perspZoom - 0.03f > 0.0f)
			perspZoom -= 0.03f;
	}

	mouse_states[button] = (state == GLUT_DOWN);

	LastMousePosX = x;
	LastMousePosY = y;
}

void passive_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	LastMousePosX = x;
	LastMousePosY = y;
}

void active_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (mouse_states[0] == true)
	{
		perspRotationY += (x - LastMousePosX) * perspSensitivity;
		perspRotationX += (y - LastMousePosY) * perspSensitivity;
	}

	LastMousePosX = x;
	LastMousePosY = y;
}

/*=================================================================================================
	RENDERING
=================================================================================================*/

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	CreateTransformationMatrices();

	shader.Use();
	shader.SetUniform("projectionMatrix", glm::value_ptr(PerspProjectionMatrix), 4, GL_FALSE, 1);
	shader.SetUniform("viewMatrix", glm::value_ptr(PerspViewMatrix), 4, GL_FALSE, 1);
	shader.SetUniform("modelMatrix", glm::value_ptr(PerspModelMatrix), 4, GL_FALSE, 1);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glBindVertexArray(torus_VAO);
	glDrawArrays(GL_TRIANGLES, 0, pow(n, 2) * 6);

	if (draw_normals== true) {
		glBindVertexArray(normals_VAO);
		glDrawArrays(GL_LINES, 0, pow(n, 2) * 4);
	}

	glBindVertexArray(0);

	if (draw_wireframe == true) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glutSwapBuffers();
}

/*=================================================================================================
	INIT
=================================================================================================*/

void init(void)
{
	// Print some info
	std::cout << "Vendor:         " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

	// Set OpenGL settings
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background color
	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_CULL_FACE); // enable back-face culling

	// Create shaders
	CreateShaders();

	// Create buffers
	torusBuffers();
	normalBuffers();

	std::cout << "Finished initializing...\n\n";
}

/*=================================================================================================
	MAIN
=================================================================================================*/

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(InitWindowWidth, InitWindowHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("CSE-170 Computer Graphics");

	// Initialize GLEW
	GLenum ret = glewInit();
	if (ret != GLEW_OK) {
		std::cerr << "GLEW initialization error." << std::endl;
		glewGetErrorString(ret);
		return -1;
	}

	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(key_released);
	glutSpecialFunc(key_special_pressed);
	glutSpecialUpFunc(key_special_released);
	glutMouseFunc(mouse_func);
	glutMotionFunc(active_motion_func);
	glutPassiveMotionFunc(passive_motion_func);

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}






