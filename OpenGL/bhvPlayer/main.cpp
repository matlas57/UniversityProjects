//Matan Atlas
//260943621
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"
#include "Motion.h"
#include "DAGNode.h"

using namespace std;

GLFWwindow* window; // Main application window
string RES_DIR = ""; // Where data files live
shared_ptr<Program> prog;
shared_ptr<Program> prog2; // for drawing with colours
shared_ptr<Shape> shape;
shared_ptr<Shape> cube;
shared_ptr<Motion> motion;


GLuint vao; // initialize an array of VAOs for use
GLuint posBufID; // position buffer(s) for drawing a lines
GLuint aPosLocation = 0; // location set in col_vert.glsl (or can be queried)
const GLuint NumVertices = 6;
GLfloat vertices[NumVertices][3] = {
	{ 0, 0, 0 },
	{ 1, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 1, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 1 }, };

//For part 4: declaring a frame counter and a max frame variable
int frameCounter = 0;
int maxFrames;
int jointsDrawn = 0;

//For part 5: 
//declare a boolean to determine if the motion is paused
//declare a boolean to determine if frames are being incremented or decremented
//declare and int to determine the stepping rate
bool paused = false;
bool incrementing = true;
int stepRate = 1;

static void error_callback(int error, const char* description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		paused = !paused;
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		incrementing = !incrementing;
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		stepRate++;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && stepRate >= 2)
	{
		stepRate--;
	}
}

static void init()
{
	GLSL::checkVersion();

	// Check how many texture units are supported in the vertex shader
	int tmp;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &tmp);
	cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << tmp << endl;
	// Check how many uniforms are supported in the vertex shader
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &tmp);
	cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS = " << tmp << endl;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &tmp);
	cout << "GL_MAX_VERTEX_ATTRIBS = " << tmp << endl;

	// Set background color.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	// Initialize mesh.
	shape = make_shared<Shape>();
	shape->loadMesh(RES_DIR + "sphere.obj");
	shape->init();
	cube = make_shared<Shape>();
	cube->loadMesh(RES_DIR + "cube.obj");
	cube->init();

	//Matan: Initialize motion file.
	motion = make_shared<Motion>();
	motion->loadBVH(RES_DIR + "0019_AdvanceBollywoodDance001.bvh");
	//For part 4: initialize the maximum number of frames from the motion file
	maxFrames = motion->numFrames;
	
	// Initialize the GLSL programs.
	prog = make_shared<Program>();
	prog->setVerbose(true);
	prog->setShaderNames(RES_DIR + "nor_vert.glsl", RES_DIR + "nor_frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->setVerbose(false);

	prog2 = make_shared<Program>();
	prog2->setVerbose(true);
	prog2->setShaderNames(RES_DIR + "col_vert.glsl", RES_DIR + "col_frag.glsl");
	prog2->init();
	prog2->addUniform("P");
	prog2->addUniform("MV");
	prog2->addUniform("col");
	prog2->addAttribute("aPos");
	prog2->setVerbose(false);

	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);

	// Create a buffers for doing some line drawing
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(aPosLocation);
	glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

}

static void drawCoordinateFrame(const std::shared_ptr<Program> prog2, const std::shared_ptr<MatrixStack> MV, const std::shared_ptr<MatrixStack> P)
{
	//Draw lines (from original L00 code).
	prog2->bind(); //bind the program to draw lines with colors
	MV->pushMatrix(); //push a matrix onto the matrix stack MV
	glUniformMatrix4fv(prog2->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
	glUniformMatrix4fv(prog2->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
	glUniform3f(prog2->getUniform("col"), 1, 0, 0); //sets the color of the lines to red
	glBindVertexArray(vao);
	/*
	* glDrawArrays
	* arg 1: the OpenGL primitive type to draw. GL_LINE_LOOP specifies that a line should also be drawn to connect the first and last vertices
	* arg 2: specifies the starting index to draw from
	* arg 3: how many vertices to draw
	*/
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3f(prog2->getUniform("col"), 0, 1, 0); //sets the color of the lines to green
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3f(prog2->getUniform("col"), 0, 0, 1); //sets the color of the lines to green
	glDrawArrays(GL_LINES, 4, 2);
	MV->popMatrix();
	prog2->unbind();
}


static void render()
{
	//make all reachedEnd bool values in the DAGNodes false so they traverse correctly later
	motion->root->traverseAndReset(*motion->root);
	
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = width / (float)height;
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create matrix stacks.
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	// Apply projection.
	P->pushMatrix();
	P->multMatrix(glm::perspective((float)(45.0 * M_PI / 180.0), aspect, 0.01f, 100.0f));
	// Apply camera transform.
	MV->pushMatrix();
	MV->translate(glm::vec3(0, -.5, -3)); // was originally (0, 0, -3) but this is better to see all the axes

	MV->scale(glm::vec3(.02, .02, .02));
	//MV->scale(glm::vec3(.01, .01, .01));
	//For part 2: drawing a world frame
	drawCoordinateFrame(prog2, MV, P);
	
	//For part 6: Draw coordinate frames for all DAGNodes
	motion->root->draw(prog, shape, cube, prog2, vao, MV, P,  motion->data, *motion->root, frameCounter, motion->numChannels);

	
	// Draw sphere.
	//prog->bind();
	//MV->scale(glm::vec3(.1, .1, .1));
	//double t = glfwGetTime();
	//MV->pushMatrix();
	//MV->translate(0.0f, -0.4f, 0.0f);
	//
	////double c = cos(t);
	////double s = sin(t);
	////double tmp[16] = { c, 0, s, 0,
	////				   0, 1, 0, 0,
	////				  -s, 0, c, 0,
	////				   0, 0, 0, 1 };
	////glm::mat4 M = glm::make_mat4(tmp);
	////M = glm::transpose(M);
	////MV->multMatrix(M);

	//MV->rotate( (float) t, 0, 1, 0 );
	//glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
	//glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
	
	//shape->draw(prog);
	//MV->popMatrix();
	//prog->unbind();

	// Pop matrix stacks.
	MV->popMatrix();
	P->popMatrix();

	//For part 5: if paused by pressing space then the frames are no longer incremented
	if (!paused)
	{
		if (incrementing)
		{
			//For part 4: loop frame counter if reached the end 
			if (frameCounter >= maxFrames)
			{
				frameCounter = 0;
			}
			else
			{
				frameCounter+= stepRate;
			}
		}
		else
		{
			//For part 4: loop frame counter if reached the end 
			if (frameCounter <= 0)
			{
				frameCounter = maxFrames;
			}
			else
			{
				frameCounter-= stepRate;
			}
		}
	}
	GLSL::checkError(GET_FILE_LINE);
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RES_DIR = argv[1] + string("/");

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if (!glfwInit()) {
		return -1;
	}

	// https://en.wikipedia.org/wiki/OpenGL
	// hint to use OpenGL 4.1 on all paltforms
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "Matan Atlas: 260943621", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Initialize scene.
	init();
	// Loop until the user closes the window.
	while (!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
