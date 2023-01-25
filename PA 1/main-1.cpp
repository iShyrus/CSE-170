#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>


float color1 = 0;
float color2 = 0;
float color3 = 0;

//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	//glutPostRedisplay();
}

void reshape_func( int width, int height )
{
	glViewport( 0, 0, width, height );
	glutPostRedisplay();
}

void keyboard_func( unsigned char key, int x, int y )
{
	switch( key )
	{
		case 'w':
		{
			glTranslatef(0.0f, 0.10f, 0.0f);
			printf("Translated upwards\n");

			break;
		}

		case 'a':
		{
			glTranslatef(-0.10f, 0.0f, 0.0f);
			printf("Translated left\n");
			break;
		}

		case 's':
		{
			glTranslatef(0.0f, -0.10f, 0.0f);
			printf("Translated downwards\n");
			break;
		}

		case 'd':
		{
			glTranslatef(0.10f, 0.0f, 0.0f);
			printf("Translated right\n");
			break;
		}

		// Exit on escape key press
		case '\x1B':
		{
			exit( EXIT_SUCCESS );
			break;
		}
	}

	glutPostRedisplay();
}

void key_released( unsigned char key, int x, int y )
{
	printf("Key pressed is %c", key);
	printf("\n");

	switch (key) {
	case 'q':
	{
		glScalef(1.1, 1.0, 1.0);
		printf("Scaled up!\n");
		break;
	}
	case 'e':
	{
		glScalef(0.9, 1.0, 1.0);
		printf("Scaled down!\n");
		break;
	}

	case'1': {
		if (color2 != 1) {
			color2 += 0.25;
		}
		if (color3 != 0) {
			color3 -= 0.25;
			printf("Color Change! \n");
		}
		
		break;
	
	}
	case'2': {
		if (color2 != 0) {
			color2 -= 0.25;
			printf("Color Change!\n");
			}
		if (color3 != 1) {
			color3 += 0.25;
		}

		break;
	}
}

	
}

void key_special_pressed( int key, int x, int y )
{

}

void key_special_released( int key, int x, int y )
{
}

void mouse_func( int button, int state, int x, int y )
{
	glRotatef(10, 1.0f, 0.0, 0.0);
	printf("Clicked to rotate on x axis!\n");

}

void passive_motion_func( int x, int y )
{

}

void active_motion_func( int x, int y )
{

}

//=================================================================================================
// RENDERING
//=================================================================================================

void display_func( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
;
	glBegin( GL_TRIANGLES ); // C
		glColor3f(0.0f, color2, 1.0f);

		glVertex2f( -0.95f, 0.0f );
		glVertex2f( -0.85f, -0.5f );
		glVertex2f( -0.85f, 0.5f );

		glColor3f(1.0f, 0.0f, color3);

		glVertex2f(-0.85f, -0.5f);
		glVertex2f(-0.85f, -0.4f);
		glVertex2f(-0.6f, -0.5f);

		glVertex2f(-0.6f, 0.5f);
		glVertex2f(-0.85f, 0.5f);
		glVertex2f(-0.85f, 0.4f);

		glEnd();

		//
	

		glColor3f(1.0f, 1.0f, 1.0f); // Y
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, color3);

		glVertex2f(-0.6f, 0.5f);
		glVertex2f(-0.4f, 0.0f);
		glVertex2f(-0.6f, 0.23f);

		glVertex2f(-0.4f, 0.0f);
		glVertex2f(-0.2f, 0.5f);
		glVertex2f(-0.2f, 0.23f);
		glColor3f(0.0f, color2, 1.0f);

		glVertex2f(-0.45f, -0.5f);
		glVertex2f(-0.4f, 0.0f);
		glVertex2f(-0.35f, -0.5f);

		glEnd();

		//

		glColor3f(1.0f, 1.0f, 1.0f); //R
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, color3);

		glVertex2f(-0.1f, -0.50f);
		glVertex2f(-0.1f, 0.45f);
		glVertex2f(-0.175f, 0.0f);

		glVertex2f(-0.1f, 0.45f);
		glVertex2f(0.3f, 0.45f);
		glVertex2f(0.1f, 0.5f);

		glVertex2f(0.3f, 0.45f);
		glVertex2f(0.3f, 0.0f);
		glVertex2f(0.35f, 0.225f);
		glColor3f(0.0f, color2, 1.0f);

		glVertex2f(0.3f, 0.0f);
		glVertex2f(-0.1f, 0.0f);
		glVertex2f(0.1f, -0.05f);


		glVertex2f(-0.1f, 0.0f);
		glVertex2f(0.3f, -0.5f);
		glVertex2f(0.075f, -0.3f);

		glEnd();

		//
		glBegin(GL_TRIANGLES); // U
		glColor3f(1.0f, 0.0f, color3);


		glVertex2f(0.5f, 0.5f);
		glVertex2f(0.4f, 0.0);
		glVertex2f(0.5f, -0.5f);


		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.7f, -0.45);
		glVertex2f(0.9f, -0.5f);

		glColor3f(0.0f, color2, 1.0f);

		glVertex2f(0.9f, -0.5f);
		glVertex2f(1.0f, 0.0);
		glVertex2f(0.9f, 0.5f);

		glEnd();


	glutSwapBuffers();
}

//=================================================================================================
// INIT
//=================================================================================================

void init( void )
{
	// Print some info
	std::cout << "Vendor:         " << glGetString( GL_VENDOR   ) << "\n";
	std::cout << "Renderer:       " << glGetString( GL_RENDERER ) << "\n";
	std::cout << "OpenGL Version: " << glGetString( GL_VERSION  ) << "\n\n";
	printf("Controls:\n");
	printf("W - Translate up\n A - Translate Right\n S - Translate down\n D - Translate right\n 1 - Change Color\n 2 - Change Color \n Q - Scale Down\n E - Scale up\n Click - Rotate along X axis\n");

	// Set the background color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	std::cout << "Finished initializing...\n\n";
}

//=================================================================================================
// MAIN
//=================================================================================================

int main( int argc, char** argv )
{
	glutInit( &argc, argv );

	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( 800, 600 );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "CSE-170 Computer Graphics" );

	glutDisplayFunc( display_func );
	glutIdleFunc( idle_func );
	glutReshapeFunc( reshape_func );
	glutKeyboardFunc( keyboard_func );
	glutKeyboardUpFunc( key_released );
	glutSpecialFunc( key_special_pressed );
	glutSpecialUpFunc( key_special_released );
	glutMouseFunc( mouse_func );
	glutMotionFunc( active_motion_func );
	glutPassiveMotionFunc( passive_motion_func );

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
