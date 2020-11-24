/*
 * ModuleFourA.cpp
 *
 *
 *  Created on: Nov 16, 2020
 *      Author: Cassandra Melms
 */
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// GLM include
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace std;

#define WINDOW_TITLE "4-1 Activity 5"


// Shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif


// Variable declaration for shader, window size, initialization, buffer and array objects
GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO, EBO, texture;

// Function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);


// Vertex shader source
const GLchar* vertexShaderSource = GLSL(330,
	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 color;

	out vec3 mobileColor;


	// Global variable for transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;


void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f); //transforms vertices to clip coordinates
	mobileColor = color; //references incoming color data
	}

);

// Fragment shader source code

const GLchar* fragmentShaderSource = GLSL(330,
		in vec3 mobileColor; //Variable to hold incoming color data from vertex shader
		out vec4 gpuColor; //Variable to pass color data to the GPU


	void main() {

		gpuColor = vec4(mobileColor, 1.0); //Sends color data to the GPU for rendering

	}

);


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WINDOW_TITLE);

	glutReshapeFunc(UResizeWindow);

	glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK)
			{
				std::cout << "Failed to initialize GLEW" << std::endl;
				return -1;
			}

	UCreateShader();

	UCreateBuffers();

// Use the shader program
	 glUseProgram(shaderProgram);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color

	glutDisplayFunc(URenderGraphics);

	glutMainLoop(); // Create and run OpenGL Loop

	// Destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;

}


// Resize window

void UResizeWindow(int w, int h)
{
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}


// Renders graphics

void URenderGraphics(void)
{
	glEnable(GL_DEPTH_TEST); // Enable z-depth

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clears the screen

	 glBindVertexArray(VAO); // Activate the vertex array object before rendering and transforming them

	 glm::mat4 model(1.0f); // Declare a 4x4 identity matrix
	 model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f)); // Place the object at the center of the viewport
	 model = glm::rotate(model, -270.0f, glm::vec3(1.0, 0.0f, 0.0f)); //Rotate the object 15 degrees on x axis
	 model = glm::rotate(model, 45.0f, glm::vec3(0.0, 1.0f, 0.0f)); //Rotate the object 45 degrees on y axis
	 model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); //Increase object by scale of 2

	 //Transforms the camera
	 glm::mat4 view;
	 view = glm::translate(view, glm::vec3(0.5f, 0.0f, -5.0f));

	 // Perspective projection
	 glm::mat4 projection;
	 projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	 // Retrieves and passes transform matrices to the shader program
	 GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	 GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	 GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

	 glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	 glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	 glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	 glutPostRedisplay();

	 // Draws the triangles
	 glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	 glBindVertexArray(0);
	 glutSwapBuffers();
}

// Creates the shader program
void UCreateShader()
{
	// Vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Messed up this part too, had it clearing anything before enacting

	//Shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);;
	glLinkProgram(shaderProgram);

	// Delete the vertex and fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// creates the buffer and array objects

void UCreateBuffers()
{
	//Had this part messed up

	// Position and color data
	GLfloat verticies[] = {
							// Vertex Positions    // Color Data // Vertex position
							-0.5f, -0.5f,  0.0f, 0.0, 1.0f, 0.0f, // 0
							0.0f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 1
							0.5f, -0.5f,  0.0f, 0.5f, 0.0f, 1.0f, // 2
							-0.5f, -0.5f, -1.0f, -1.0f, 0.0f, 1.0f, // 3
							0.5f, -0.5f, -1.0f, -0.5f, 0.5f, 1.0f, // 4
							};

	// Index data to share position data
		GLuint indices[] = {
								0, 1, 2, // Front Triangle
								0, 3, 1, // Right Side triangle
								3, 1, 4, // Back triangle
								4, 1, 2, // Left back side triangle
								0, 3, 4, // Bottom Triangle
								0, 4, 2 // Bottom triangle
							};
		// Generate buffer ids
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Activate the vertex array object before binding and settomg any VBOs or vertex attribute pointers
		glBindVertexArray(VAO);

		// Activate the VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

		// Activate the element buffer object / indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// set attribute pointer 0 to hold position data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Set attribute pointer 1 to hold Color data
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);


		glBindVertexArray(0);

}
