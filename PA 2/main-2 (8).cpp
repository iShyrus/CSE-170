#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
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

// Other parameters
bool draw_wireframe = false;

/*=================================================================================================
	SHADERS & TRANSFORMATIONS
=================================================================================================*/

ShaderProgram PassthroughShader;
ShaderProgram PerspectiveShader;

glm::mat4 PerspProjectionMatrix(1.0f);
glm::mat4 PerspViewMatrix(1.0f);
glm::mat4 PerspModelMatrix(1.0f);

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;

/*=================================================================================================
	OBJECTS
=================================================================================================*/

GLuint axis_VAO;
GLuint axis_VBO[2];

float axis_vertices[100000] = {

};

float axis_colors[100000] = {
};

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
}

/*=================================================================================================
	BUFFERS
=================================================================================================*/

void CreateAxisBuffers(void)
{
	glGenVertexArrays(1, &axis_VAO);
	glBindVertexArray(axis_VAO);

	glGenBuffers(2, &axis_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_colors), axis_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);



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
	//uncomment below to repeatedly draw new frames
	glutPostRedisplay();
}

void reshape_func(int width, int height)
{
	WindowWidth = width;
	WindowHeight = height;

	glViewport(0, 0, width, height);
	glutPostRedisplay();
}





void increaseDecrease(float increment, float R, float r) {
	float vert[100000] = {};
	int count = 0;
	double theta = 0;
	double phi = 0;
	double twopi = 2 * 3.14159265359;

	//makes every value 0 so it resets
	std::fill_n(vert, 100000, 0);
	std::fill_n(axis_vertices, 100000, 0);


	//inputs values for colors, green red
	for (int i = 0; i <= 100000;i += 16) {
		axis_colors[i] = 1;
		axis_colors[i + 1] = 0;
		axis_colors[i + 2] = 0;
		axis_colors[i + 3] = 1;

		axis_colors[i + 4] = 0;
		axis_colors[i + 5] = 1;
		axis_colors[i + 6] = 0;
		axis_colors[i + 7] = 1;

		axis_colors[i + 8] = 0;
		axis_colors[i + 9] = 1;
		axis_colors[i + 10] = 0;
		axis_colors[i + 11] = 1;

		axis_colors[i + 12] = 0;
		axis_colors[i + 13] = 1;
		axis_colors[i + 14] = 0;
		axis_colors[i + 15] = 1;
	};

	
	twopi = 2 * 3.14159265359;

	//calculates shape of torus, +0.001 because doesnt reach to twopi by 0.001, add increment in 2nd to get 2nd sharing coord 
	for (float theta = twopi / increment; theta < twopi + 0.001; theta += twopi / increment) {
		for (float phi = twopi / increment; phi <= twopi + (twopi / increment) + 0.001; phi += twopi / increment) {


			vert[count++] = (R + r * cos(theta)) * cos(phi);
			vert[count++] = (R + r * cos(theta)) * sin(phi);
			vert[count++] = r * sin(theta);

			float add = (twopi / increment);


			vert[count++] = (R + r * cos(theta + add)) * cos(phi);
			vert[count++] = (R + r * cos(theta + add)) * sin(phi);
			vert[count++] = r * sin(theta + add);



		}
		//std::cout << "\n";
	//std::cout << theta<<"\n";
	}
	//std::cout << "_____________________________";
	//std::cout << testcheck <<"\n";
   //std::cout << increment << "\n";

	int k = 9;
	for (int i = 0;i <= count * 3;i += 9) {
		if (i == 0) {
			//starting triangle
			axis_vertices[i] = vert[i];
			axis_vertices[i + 1] = vert[i + 1];
			axis_vertices[i + 2] = vert[i + 2];
			axis_vertices[i + 3] = vert[i + 3];
			axis_vertices[i + 4] = vert[i + 4];
			axis_vertices[i + 5] = vert[i + 5];
			axis_vertices[i + 6] = vert[i + 6];
			axis_vertices[i + 7] = vert[i + 7];
			axis_vertices[i + 8] = vert[i + 8];

		}
		else {

			// gets previous vertices because triangles will share 2 points which needs 2 xyz coords
			axis_vertices[i] = axis_vertices[i - 6];
			axis_vertices[i + 1] = axis_vertices[i - 5];
			axis_vertices[i + 2] = axis_vertices[i - 4];
			axis_vertices[i + 3] = axis_vertices[i - 3];
			axis_vertices[i + 4] = axis_vertices[i - 2];
			axis_vertices[i + 5] = axis_vertices[i - 1];

			//pulls in new coords for point for new triangle
			axis_vertices[i + 6] = vert[k];
			axis_vertices[i + 7] = vert[k + 1];
			axis_vertices[i + 8] = vert[k + 2];

			k += 3;
		}
		//break so array doesnt extend more than needed
		if (k == count) {
			break;
		}
	}


	//for (int i = 0;i <= count * 3;i += 9) {
		//std::cout << axis_vertices[i] << ",\n";
		//std::cout << axis_vertices[i + 1] << ",\n";
		//std::cout << axis_vertices[i + 2] << ",\n";
		//std::cout << axis_vertices[i + 3] << ",\n";
		//std::cout << axis_vertices[i + 4] << ",\n";
		//std::cout << axis_vertices[i + 5] << ",\n";
		//std::cout << axis_vertices[i + 6] << ",\n";
		//std::cout << axis_vertices[i + 7] << ",\n";
		//std::cout << axis_vertices[i + 8] << ",\n";

	//}
	//std::cout << "\n\n\n\n\n";
	//std::cout << count << "\n";
	//for (int i = 0; i <= count; i += 3) {
		//std::cout << vert[i] << ",\n";
		//std::cout << vert[i + 1] << ",\n";
		//std::cout << vert[i + 2] << ",\n";
		//std::cout << vert[i + 3] << ",\n";
		//std::cout << vert[i + 4] << ",\n";
		//std::cout << vert[i + 5] << ",\n";
		//std::cout << vert[i + 6] << ",\n";
		//std::cout << vert[i + 7] << ",\n";
		//std::cout << vert[i + 8] << ",\n";
		//std::cout << "\n";


	//}
	glGenVertexArrays(1, &axis_VAO);
	glBindVertexArray(axis_VAO);

	glGenBuffers(2, &axis_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_colors), axis_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindVertexArray(axis_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 1000000);

	glBindVertexArray(0);
}


int n = 5;
float R = 2.0;
float r = 0.5;
float axis_vert[100000000] = {};



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
		increaseDecrease(n, R, r);
		break;
	}
	case's': {
		r -= 0.1;
		increaseDecrease(n, R, r);
		break;
	}
	case'e': {
		R += 0.1;
		increaseDecrease(n, R, r);
		break;
	}
	case'd': {
		R -= 0.1;
		increaseDecrease(n, R, r);
		break;
	}

	case 'q': {
		n++;
		increaseDecrease(n,R,r);
		break;
	}
	case 'a': {
		n--;
		increaseDecrease(n,R,r);
		break;
	}

	// Exit on escape key press
	case '\x1B':
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

	PerspectiveShader.Use();
	PerspectiveShader.SetUniform("projectionMatrix", glm::value_ptr(PerspProjectionMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("viewMatrix", glm::value_ptr(PerspViewMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("modelMatrix", glm::value_ptr(PerspModelMatrix), 4, GL_FALSE, 1);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(axis_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 1000000);

	glBindVertexArray(0);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
	//glEnable(GL_CULL_FACE); // enable back-face culling

	// Create shaders
	CreateShaders();

	// Create buffers
	CreateAxisBuffers();
	std::cout << "\n";
	std::cout << " q = increment the number of triangles (n)\n a = decrement the number of triangles (n)\n w = increment the r radius (by a small value)\n s = decrement the r radius (by a small value) \n e = increment the R radius (by a small value) \n d = decrement the R radius (by a small value)  ";
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
