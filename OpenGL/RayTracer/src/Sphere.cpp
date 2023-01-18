#include "Sphere.h"
#include "Ray.h"
#include "IntersectionData.h"

float SPHERE_EPSILON = 1e-4; // To prevent shadow acne

Sphere::Sphere() :
    radius(1.0f),
    center(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Sphere::Sphere(float _radius, glm::vec3 _center) :
    radius(_radius),
    center(_center)
{
}

Sphere::Sphere(float _radius) :
    radius(_radius),
    center(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Sphere::~Sphere()
{
}

void Sphere::intersect(const std::shared_ptr<Ray> ray, std::shared_ptr<IntersectionData> intersection)
{
    // TODO: Objective 2: intersection of ray with sphere
    glm::vec3 originMinusCenter = ray->origin - this->center;
    float a = glm::dot(ray->direction, ray->direction); //with normalized direction this should always be 1
    float b = 2.0f * glm::dot((ray->direction), originMinusCenter);
    float c = glm::dot(originMinusCenter, originMinusCenter) - radius * radius;
    float discriminant = (b * b) - (4.0f * a * c); // test the discriminant first to see if roots exist
    // a zero discriminant means no roots exist
    if (discriminant < 0)
    {
        intersection->t = -1; // a negative t indicates that the intersection occurs behind the camera and shouldn't be rendered
    }
    else if (discriminant == 0) // if disciminant = 0 there is one root which can be computed by t = -b/2a
    {
        intersection->t = -0.5 * b / a;
        ray->computePoint(intersection->t, intersection->p);
        intersection->n = normalize(intersection->p - this->center);
    }
    else // positive discriminant: 2 roots
    {
        float t0, t1;
        float sqrtDiscriminant = glm::sqrt(discriminant);
        // only need to compute one intersection point because t0 will always be greater in this instance 
        //t0 = (-b + sqrtDiscriminant) / (2.0f * a);
        t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        intersection->t = t1;
        ray->computePoint(t1, intersection->p);
        intersection->n = glm::normalize(intersection->p - this->center);
        intersection->material = this->materials.at(0);
    }
    return;
}