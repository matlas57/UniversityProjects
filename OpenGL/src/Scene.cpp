#include "Scene.h"
#include "Ray.h"
#include "IntersectionData.h"
#include "Camera.h"
#include "Light.h"
#include "Shape.h"

#include <stdlib.h> 
#include <iostream>

float SHADOW_EPSILON = 1e-6; // To prevent shadow acne

// Default values, they get overwritten if scene file contains them
Scene::Scene(const std::string name) :
	width(1080), height(720), aspect(1080.0f / 720.0f),
	shininess(64),
	ambient(0.1f, 0.1f, 0.1f)
{
	outputFilename = name;
}

void Scene::init() 
{
	// Initialize camera	
	cam = make_shared<Camera>();

	// Init image
	image = make_shared<Image>(width, height);
}

Scene::~Scene()
{
}

void Scene::addObject(shared_ptr<Shape> shape) 
{
	shapes.push_back(shape);
}

void Scene::addLight(shared_ptr<Light> light) 
{
	lights.push_back(light);
}

void Scene::render() 
{
	std::shared_ptr<IntersectionData> intersection = make_shared<IntersectionData>();
	std::shared_ptr<IntersectionData> shadowIntersection = make_shared<IntersectionData>();
	glm::vec3 camDir = -cam->lookAt + cam->position;
	float d = 1;
	float top = d * glm::tan(0.5 * M_PI * cam->fovy / 180.f);
	float right = aspect * top;
	float bottom = -top;
	float left = -right;

	// Compute basis
	glm::vec3 w = glm::normalize(camDir);
	glm::vec3 u = glm::normalize(glm::cross(cam->up, w));
	glm::vec3 v = glm::cross(w, u);
	glm::mat4 camToWorld(1.0f);
	camToWorld[0] = glm::vec4(u, 0.0);
	camToWorld[1] = glm::vec4(v, 0.0);
	camToWorld[2] = glm::vec4(w, 0.0);
	camToWorld[3] = glm::vec4(cam->position, 1.0);

	std::shared_ptr<Ray> ray = make_shared<Ray>();
	std::shared_ptr<Ray> shadowRay = make_shared<Ray>();

	glm::vec3 eyeVec = glm::vec3();
	glm::vec3 lightVec = glm::vec3();
	glm::vec3 h = glm::vec3(); // Bisection vector

	ray->origin = cam->position;

	glm::vec3 pixel = glm::vec3();
	glm::vec3 colour = glm::vec3();

	// used to determine where to sample for AA
	float pixelDistx = (2.0f / width) * aspect;
	float pixelDisty = (2.0f / height);

	// turn AA off by just sampling in the center of the pixel
	bool AA = true;
	if (!AA)
	{
		pixelDistx = 0;
		pixelDisty = 0;
	}

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			colour = glm::vec3(0.0f, 0.0f, 0.0f);	
			//transform window to [-1,1]^2
			float dx = (2.0f * ((float)i + 0.5f) / (float)width - 1.0f) * d * aspect;
			float dy = -(1.0f - 2.0f * ((float)j + 0.5f) / (float)height) * d;
			//looping for anti-aliasing sample 4 times in center of 4 quadrants of each pixel
			for (int sx = 0; sx < 2; sx++)
			{
				if (sx == 0)
				{
					dx -= (pixelDistx * 0.25);
				}
				else
				{
					dx += pixelDistx*0.5;
				}
				for (int sy = 0; sy < 2; sy++)
				{
					if (sy == 0)
					{
						dy -= pixelDisty * 0.25;
					}
					else
					{
						dy += pixelDisty*0.5;
					}
					// TODO: 1. generate a ray
					/*glm::vec4 rayOrigHomogeneous = glm::vec4(ray->origin, 1.0);
					glm::vec4 rayOrigWorld = camToWorld * rayOrigHomogeneous;
					ray->origin = rayOrigWorld;*/


					//glm::vec3 dir = camToWorld * glm::vec4(dx, dy, -1, 0);
					//ray->direction = glm::normalize(dir - ray->origin);

					ray->direction = glm::normalize(glm::vec3(dx, dy, -1));

					//ray->direction = glm::normalize(glm::vec3(dx, dy, -1));
					//ray->direction -= glm::normalize(ray->origin);
					//ray->direction = glm::normalize(ray->direction);

					//ray->direction = glm::normalize(glm::vec3(dx, -dy, -2) - ray->origin);
					//ray->direction = glm::normalize(ray->direction - ray->origin);

					// TODO: 2. test for intersection with scene surfaces
					// a second intersection record to store data for nearest intersection and for comparison when new intersections are found
					std::shared_ptr<IntersectionData> nearestIntersection = make_shared<IntersectionData>(); 
					nearestIntersection->t = FLT_MAX;
					//float nearest = intersection->t;
					int shapeIndex = -1; // store the index of the shape that is intersected with 
					// iterate through the shapes in the scene, see if they intersect the current ray
					// if an intersection is found, store the intersection time in nearest to compare to any future intersections with other shapes
					for (int k = 0; k < shapes.size(); k++)
					{
						shapes.at(k)->intersect(ray, intersection);
						if (intersection->t > 0.0 && intersection->t < nearestIntersection->t)
						{
							nearestIntersection->t = intersection->t;
							nearestIntersection->n = intersection->n;
							shapeIndex = k;
						}
					}

					// TODO: 3. compute the shaded result for the intersection point
					//iterate through the light sources of the scene and compute the lighting 
					if (shapeIndex >= 0)
					{
						// if an intersection was found find the intersection point
						ray->computePoint(nearestIntersection->t, nearestIntersection->p);
						nearestIntersection->material = shapes.at(shapeIndex)->materials.at(0);
						// if the shape has multiple materials (checkerboard) determine material usage based on intersection point
						if (shapes.at(shapeIndex)->materials.size() > 1)
						{
							int xInt = nearestIntersection->p.x;
							int zInt = nearestIntersection->p.z;
							int xMod = abs(xInt % 2);
							int zMod = abs(zInt % 2);
							int materialIndex = 0;
							if (xMod != zMod)
							{
								materialIndex = 1;
							}
							if (nearestIntersection->p.x < 0)
							{
								if (materialIndex == 0)
								{
									materialIndex = 1;
								}
								else
								{
									materialIndex = 0;
								}
							}
							nearestIntersection->material = shapes.at(shapeIndex)->materials.at(materialIndex);
						}
						// create vecs for each type of lighting used
						glm::vec3 lambertian = glm::vec3(0.0f);
						glm::vec3 blinnPhong = glm::vec3(0.0f);
						glm::vec3 occlusion = glm::vec3(0.0f);
						glm::vec3 reflection = glm::vec3(0.0f);
						bool reflect = false;
						for (int l = 0; l < lights.size(); l++)
						{
							if (lights.at(l)->type == "point" || lights.at(l)->type == "area")
							{
								glm::vec3 lightColor = lights.at(l)->colour;
								glm::vec3 lightdir = normalize(lights.at(l)->position - nearestIntersection->p);
								//compute lambertian shading
								glm::vec3 kd = nearestIntersection->material->diffuse;
								lambertian += kd * lightColor * glm::max(glm::dot(nearestIntersection->n, lightdir), 0.0f);
								//compute blinn-phong specular shading
								glm::vec3 halfVector = (-ray->direction + lightdir) / glm::length(-ray->direction + lightdir);
								glm::vec3 ks = nearestIntersection->material->specular;
								float shinyness = nearestIntersection->material->hardness;
								blinnPhong += ks * lightColor * (glm::pow(glm::max(glm::dot(nearestIntersection->n, halfVector), 0.0f), shininess));

								//see if point is occluded
								//if area light, apply soft shadows
								std::shared_ptr<Ray> rayToLight = make_shared<Ray>();
								rayToLight->origin = nearestIntersection->p + SHADOW_EPSILON;
								rayToLight->direction = lightdir;
								// soft shadows
								// done with a similar method to AA, add a offset in x and y when sending occlusion rays and average the shadow color
								if (lights.at(l)->type == "area")
								{
									float offSetScale = 12.0;
									float xOffset = offSetScale / width;
									float yOffset = offSetScale / height;
									for (int softx = 0; softx < 2; softx++)
									{
										if (softx == 0)
										{
											rayToLight->direction.x -= xOffset/2.0;
										}
										else
										{
											rayToLight->direction.x += xOffset;

										}
										for (int softy = 0; softy < 2; softy++)
										{
											if (softy == 0)
											{
												rayToLight->direction.y -= yOffset / 2.0;
											}
											else
											{
												rayToLight->direction.y += yOffset;

											}
											std::shared_ptr<IntersectionData> lightOcclusion = make_shared<IntersectionData>();
											for (int i = 0; i < shapes.size(); i++)
											{
												if (i != shapeIndex)
												{
													shapes.at(i)->intersect(rayToLight, lightOcclusion);
													if (lightOcclusion->t > 0 && lightOcclusion->t < FLT_MAX)
													{
														occlusion += glm::vec3(0.2f);
													}
												}
											}
										}
									}
									occlusion = glm::vec3(occlusion.r / 4.0);
								}
								else
								{
									
									rayToLight->origin = nearestIntersection->p + SHADOW_EPSILON;
									rayToLight->direction = lightdir;
									std::shared_ptr<IntersectionData> lightOcclusion = make_shared<IntersectionData>();
									for (int i = 0; i < shapes.size(); i++)
									{
										if (i != shapeIndex)
										{
											shapes.at(i)->intersect(rayToLight, lightOcclusion);
											if (lightOcclusion->t > 0 && lightOcclusion->t < FLT_MAX)
											{
												occlusion = glm::vec3(0.2f);
											}
										}
									}
								}
								//if the material is reflective (represented by a hardness attribute of 100) apply reflective lighting
								if (shinyness == 100.0)
								{
									reflect = true;
									//construct the reflection ray
									std::shared_ptr<Ray> reflectionRay = make_shared<Ray>();
									reflectionRay->origin = nearestIntersection->p;
									reflectionRay->direction = 2 * (glm::dot(nearestIntersection->n, -ray->direction)) * nearestIntersection->n - (-ray->direction);
									std::shared_ptr<IntersectionData> reflectionData = make_shared<IntersectionData>();
									for (int r = 0; r < shapes.size(); r++)
									{
										if (r != shapeIndex)
										{
											shapes.at(r)->intersect(reflectionRay, reflectionData);
											if (reflectionData->t > 0.0 && reflectionData->t < FLT_MAX)
											{
												reflectionData->material = shapes.at(r)->materials.at(0);
											}
											reflection = reflectionData->material->diffuse;
										}
									}
								}
							}
						}
						if (reflect)
						{
							colour = lambertian + ambient + reflection - occlusion;
						}
						else
						{
							colour += lambertian + blinnPhong + ambient - occlusion;
						}
					}
				}
				
			}
			colour = glm::vec3(colour.r / 4, colour.g / 4, colour.b / 4);
	
			// Clamp colour values to 1
			colour.r = glm::min(1.0f, colour.r);
			colour.g = glm::min(1.0f, colour.g);
			colour.b = glm::min(1.0f, colour.b);
			

			// Write pixel colour to image
			colour *= 255;
			image->setPixel(i, j, colour.r, colour.g, colour.b);
		}
	}
	//cout << (intersectionCounter / (width * height)) << endl;
	image->writeToFile( outputFilename );
}
