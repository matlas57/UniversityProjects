#include "Mesh.h"
#include "Ray.h"
#include "IntersectionData.h"

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

float MESH_EPSILON = 1e-4; // To prevent shadow acne

Mesh::Mesh(const std::string& meshName) : filepath(meshName)
{
	loadOBJ(meshName);
}

Mesh::Mesh(const std::string& meshName, glm::vec3 position) : filepath(meshName)
{
	loadOBJ(meshName);
	translate(position);
}

Mesh::~Mesh()
{
}

void Mesh::translate(glm::vec3 position) 
{
	int j = 0;
	for (int i = 0; i < faceCount; i++) 
	{
		j = 9 * (i+1) - 9;
		// v1
		//posBuf[j + 0] = 1.0f;
		posBuf[j + 0] = posBuf[j + 0] + position.x;
		posBuf[j + 1] = posBuf[j + 1] + position.y;
		posBuf[j + 2] = posBuf[j + 2] + position.z;

		// v2
		posBuf[j + 3] = posBuf[j + 3] + position.x;
		posBuf[j + 4] = posBuf[j + 4] + position.y;
		posBuf[j + 5] = posBuf[j + 5] + position.z;

		// v3
		posBuf[j + 6] = posBuf[j + 6] + position.x;
		posBuf[j + 7] = posBuf[j + 7] + position.y;
		posBuf[j + 8] = posBuf[j + 8] + position.z;
	}
}

void Mesh::scale(float s) 
{
	for (int i = 0; i < faceCount; i++) 
	{
		posBuf[i] = s * posBuf[i];
	}
}

void Mesh::loadOBJ(const std::string& meshName)
{
	// Load geometry
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warnStr, errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &warnStr, &errStr, meshName.c_str());
	if(!rc) {
		std::cerr << errStr << std::endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
					if(!attrib.normals.empty()) {
						norBuf.push_back(attrib.normals[3*idx.normal_index+0]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+1]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+2]);
					}
					if(!attrib.texcoords.empty()) {
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+0]);
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	faceCount = int(posBuf.size() / 9.0f);
}

void Mesh::intersect(const std::shared_ptr<Ray> ray, std::shared_ptr<IntersectionData> intersection)
{
	// TODO 9: Do triangle mesh intersection here
	
	//for each ray intersection call, iterate through the mesh and find intersecting triangles
	int j = 0;
	std::shared_ptr<IntersectionData> nearestIntersection = std::make_shared<IntersectionData>();
	for (int i = 0; i < this->faceCount; i++)
	{
		j = 9 * (i + 1) - 9;
		//1. create vertices for the triangle using the buffer
		glm::vec3 v0 = glm::vec3(this->posBuf[j + 0], this->posBuf[j + 1], this->posBuf[j + 2]);
		glm::vec3 v1 = glm::vec3(this->posBuf[j + 3], this->posBuf[j + 4], this->posBuf[j + 5]);
		glm::vec3 v2 = glm::vec3(this->posBuf[j + 6], this->posBuf[j + 7], this->posBuf[j + 8]);
		glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);
		//glm::vec3 normal = glm::normalize(glm::cross(v2 - v0, v1 - v0));
		// if the normal and the ray direction are perpendicular this garuntees there is no intersection
		// this check is also necessary to avoid division by zero error when solving for t
		float normalDotDir = glm::dot(normal, ray->direction);
		if (abs(normalDotDir) <= MESH_EPSILON) // used epsilon instead of 0 because if the triangle is almost parallel it doesnt need to be shown 
		{
			intersection->t = -1;
		}
		else 
		{
			float d = -glm::dot(normal, v0); // compute d in the plane formula
			float t = -(glm::dot(normal, ray->origin) + d) / normalDotDir; // find the intersection time
			//found the intersection point of the ray and the plane of the triangle, now need to check if the triangle is behind the camera
			if (t < 0)
			{
				intersection->t = -1;
			}
			else
			{
				//It is now known that the itnersection occurs infront of the camera, now check if the intersection lies within the triangle
				//compute the intersection point and store in a temporary intersection record
				std::shared_ptr<IntersectionData> tmpData = std::make_shared<IntersectionData>();
				ray->computePoint(t, tmpData->p);
				//inside out test
				glm::vec3 testVec(0.0f);
				//test edge defined by v1 and v0
				glm::vec3 e0 = v1 - v0;
				glm::vec3 ep0 = tmpData->p - v0;
				testVec = glm::cross(e0, ep0);
				bool inside = true;
				if (glm::dot(normal, testVec) < 0)
				{
					intersection->t = -1;
					inside = false;
				}
				//test edge defined by v2 and v1
				glm::vec3 e1 = v2 - v1;
				glm::vec3 ep1 = tmpData->p - v1;
				testVec = glm::cross(e1, ep1);
				if (glm::dot(normal, testVec) < 0)
				{
					intersection->t = -1;
					inside = false;
				}
				//test edge defined by v0 and v2
				glm::vec3 e2 = v0 - v2;
				glm::vec3 ep2 = tmpData->p - v2;
				testVec = glm::cross(e2, ep2);
				if (glm::dot(normal, testVec) < 0)
				{
					intersection->t = -1;
					inside = false;
				}
				//if execution reaches this point then the ray intersects the current triangle
				//There may be a closer intersection so must store the intersection value and compare the t to the nearest to determine the closer intersection to return 
				if (inside && t < nearestIntersection->t)
				{
					nearestIntersection->t = t;
					nearestIntersection->n = normal;
					nearestIntersection->p = tmpData->p;
					nearestIntersection->material = this->materials.at(0);
				}
			}			
		}
	}
	//after iterating throgh all the triangles in the mesh return the nearestIntersection record
	intersection = nearestIntersection;
	return;
}