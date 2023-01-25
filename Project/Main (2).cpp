//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"
#include"Mesh.h"

const unsigned int width = 1920;
const unsigned int height = 1080;


float currentFrame=0;
float deltaTime = 0;
float lastFrame = 0;
int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Project", NULL, NULL);

	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);





	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	

	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath = "/Resources/test/models/test/scene.gltf";
	std::string modelPath1 = "/Resources/test/models/test1/scene.gltf";
	std::string modelPath2 = "/Resources/test/models/test2/scene.gltf";
	std::string modelPath3 = "/Resources/test/models/test3/scene.gltf";
	std::string modelPath4 = "/Resources/test/models/test4/scene.gltf";
	std::string modelPath5 = "/Resources/test/models/test4/scene.gltf";

	// Load in a model
	Model model((parentDir + modelPath).c_str());
	Model model1((parentDir + modelPath1).c_str());
	Model model2((parentDir + modelPath2).c_str());
	Model model3((parentDir + modelPath3).c_str());
	Model model4((parentDir + modelPath4).c_str());
	Model model5((parentDir + modelPath5).c_str());

	// Original code from the tutorial

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader

		// Draw a model
		model.Draw(shaderProgram, camera);
		model1.Draw1(shaderProgram, camera);
		model2.Draw2(shaderProgram, camera);
		model3.Draw3(shaderProgram, camera);
		model4.Draw4(shaderProgram, camera);
		model4.Draw5(shaderProgram, camera);
		model4.Draw6(shaderProgram, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();





		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		float lastFrame = currentFrame;
		std::cout << deltaTime << "\n";

		if (deltaTime <= 7) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, deltaTime *2.0 , 0.0f,"test",0.0f,"none");
		}
		else if (7<=deltaTime && deltaTime <= 8.0) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, deltaTime * 2.0, 0.2f, "Side3", 0.0f,"none");
		}
		else if (8.0 <= deltaTime && deltaTime <= 9.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, deltaTime * 2.0, 0.1f, "Side3", 0.0f,"none");
		}

		else if (8.0 <= deltaTime && deltaTime <= 9.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, deltaTime * 2.0, 0.1f, "Side3", 0.0f,"none");
		}
		else if (9.5 <= deltaTime && deltaTime <= 19.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f,9.5*2, 0.1f, "Up", 0.05f, "Left");
		}

		else if (19.5 <= deltaTime && deltaTime <= 22.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.2f, "Down", 0.00f, "none");
		}
		else if (22.5 <= deltaTime && deltaTime <= 23.0) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.2f, "Side4", 0.05f, "none");
		}


		else if (23.0 <= deltaTime && deltaTime <= 25.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.0f, "Side4", 0.05f, "Back");
		}
		else if (25.5 <= deltaTime && deltaTime <= 39.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.1f, "Up", 0.1f, "Left");
		}

		else if (39.5 <= deltaTime && deltaTime <= 42.0) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.1f, "Down", 0.1f, "Left");
		}
		else if (42.0 <= deltaTime && deltaTime <= 43.0) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.0f, "none", 0.1f, "Down");
		}
		else if (43.0 <= deltaTime && deltaTime <= 57) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.1f, "Up", 0.1f, "Left");
		}
		else if (57.0 <= deltaTime && deltaTime <= 58) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.1f, "Down", 0.1f, "none");
		}
		else if (58.0 <= deltaTime && deltaTime <= 58.75) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.9f, "none", 0.1f, "Left");
		}
		else if (58.75 <= deltaTime && deltaTime <= 59.25) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.9f, "none", 0.1f, "Down");
		}
		else if (59.25 <= deltaTime && deltaTime <= 61) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.9f, "none", 0.1f, "Back");
		}

		else if (61 <= deltaTime && deltaTime <= 65.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.1f, "Down", 0.1f, "Right");
		}
		else if (65.5 <= deltaTime && deltaTime <= 65.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.0f, "none", 0.1f, "Left");
		}
		else if (65.5 <= deltaTime && deltaTime <= 66.5) {
			std::cout << "test" << "\n";

			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.0f, "none", 0.1f, "Up");
			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.0f, "none", 0.1f, "Forward");
			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2, 0.0f, "none", 0.1f, "Right");

		}

		else {
			std::cout << "test1" << "\n";
			currentFrame = 0;
			camera.updateMatrix(45.0f, 0.1f, 100.0f, 9.5 * 2.0, 0.0f, "test", 0.0f,"none");

		}




	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}