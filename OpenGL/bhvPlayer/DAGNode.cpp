//Matan Atlas
//260943621
#include "DAGNode.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"

#include <iostream>
#include <cassert>

#include "GLSL.h"

using namespace std;

class Shape;

string RES_DIR1 = ""; // Where data files live


DAGNode::DAGNode()
{
	visited = false;
}

DAGNode::~DAGNode()
{
}

void DAGNode::init()
{
	
}

void DAGNode::traverseAndReset(DAGNode root)
{
	root.visited = false;
	if (root.name.compare("EndSite") == 0)
	{
		return;
	}
	for (int i = 0; i < root.children.size(); i++)
	{
		traverseAndReset(*root.children[i]);
	}
}


void DAGNode::draw(const std::shared_ptr<Program> prog, std::shared_ptr<Shape> shape, std::shared_ptr<Shape> cube, const std::shared_ptr<Program> prog2, GLuint vao, const std::shared_ptr<MatrixStack> MV, const std::shared_ptr<MatrixStack> P, float* frameData, DAGNode root, int currentFrame, int totalChannels) const
{
	root.visited = true;
	// TODO: implement this!
	float xpos = 0.0;
	float ypos = 0.0;
	float zpos = 0.0;
	float xrot = 0.0;
	float yrot = 0.0;
	float zrot = 0.0;
	int numChannels = root.channels.size();
	int startIndex = root.channelDataStartIndex +(currentFrame * totalChannels); //should be used later to actually include motion updates
	// store the channel values in the correct variables 
	for (int i = 0; i < root.channels.size(); i++)
	{
		//cout << channels[i] << endl;
		if (root.channels[i].compare("Xposition") == 0)
		{
			xpos = frameData[startIndex + i];
		}
		else if (root.channels[i].compare("Yposition") == 0)
		{
			ypos = frameData[startIndex + i];
		}
		else if (root.channels[i].compare("Zposition") == 0)
		{
			zpos = frameData[startIndex + i];
		}
		else if (root.channels[i].compare("Xrotation") == 0)
		{
			xrot = frameData[startIndex + i];
		}
		else if (root.channels[i].compare("Yrotation") == 0)
		{
			yrot = frameData[startIndex + i];
		}
		else if (root.channels[i].compare("Zrotation") == 0)
		{
			zrot = frameData[startIndex + i];
		}
	} 
	xpos += root.offset.x;
	ypos += root.offset.y;
	zpos += root.offset.z;


	// all variables for position and rotation are successfully stored
	
	MV->translate(glm::vec3(xpos, ypos, zpos));
	MV->rotate(glm::radians(xrot), 1, 0, 0);
	MV->rotate(glm::radians(yrot), 0, 1, 0);
	MV->rotate(glm::radians(zrot), 0, 0, 1);
	
	//Draw the frame for the current joint/DAGNode
	prog2->bind(); //bind the program to draw lines with colors
	MV->pushMatrix(); //push a matrix onto the matrix stack MV
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
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

	//draw the gemoetry for joints
	if (root.name.compare("EndSite") == 0 && root.parent->name.compare("Head") == 0)
	{
		MV->scale(glm::vec3(4, 4, 4));
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
		shape->draw(prog);
		MV->scale(glm::vec3(.25, .25, .25));
	}
	else
	{
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
		shape->draw(prog);
	}
	prog->unbind();

	//Use cubes to connect DAGNodes to parents, check if NULL because the hips have a null parent 
	if (root.parent != NULL)
	{
		prog->bind();
		MV->translate(glm::vec3(-xpos/2, -ypos/2, -zpos/2));
		float scaleFactor = (sqrt(xpos * xpos + ypos * ypos + zpos * zpos))/2;
		if (root.name.find("Leg") != string::npos || root.name.find("Foot") != string::npos)
		{
			//cout << "found leg" << endl;
			MV->scale(glm::vec3(.5, 1, scaleFactor));
		}
		else
		{
			MV->scale(glm::vec3(scaleFactor, .5, .5));
		}
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &P->topMatrix()[0][0]);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, &MV->topMatrix()[0][0]);
		cube->draw(prog);
		if (root.name.find("Leg") != string::npos || root.name.find("Foot") != string::npos)
		{
			MV->scale(glm::vec3(2, 1, 1 / scaleFactor));
		}
		else
		{
			MV->scale(glm::vec3(1/scaleFactor, 2, 2));

		}
		MV->translate(glm::vec3(xpos / 2, ypos / 2, zpos / 2));
		prog->unbind();
	}

	//cout << "Drew a frame for " << root.name << " " << xpos << ", " << ypos << ", " << zpos << endl;

	if (root.name.compare("EndSite") == 0)
	{
		MV->rotate(glm::radians(-zrot), 0, 0, 1);
		MV->rotate(glm::radians(-yrot), 0, 1, 0);
		MV->rotate(glm::radians(-xrot), 1, 0, 0);
		MV->translate(glm::vec3(-xpos, -ypos, -zpos));
		//cout << "Reached endsite, untranslated" << endl;
		return;
	}
	for (int i = 0; i < root.children.size(); i++)
	{
		if (root.children[i]->visited == false)
		{
			draw(prog, shape, cube, prog2, vao, MV, P, frameData, *root.children[i], currentFrame, totalChannels);
		}
	}
	MV->rotate(glm::radians(-zrot), 0, 0, 1);
	MV->rotate(glm::radians(-yrot), 0, 1, 0);
	MV->rotate(glm::radians(-xrot), 1, 0, 0);
	MV->translate(glm::vec3(-xpos, -ypos, -zpos));
	//cout << "reached all children of node " << root.name << " untranslating" << endl;
}