#include "AABB.h"
#include "Ray.h"
#include "IntersectionData.h"

static float AABB_EPSILON = 1e-4; // To prevent shadow acne
float t_min = FLT_MIN;
float t_max = FLT_MAX;

AABB::AABB() :
    minpt(0.0f, 0.0f, 0.0f),
    maxpt(10.0f, 10.0f, 10.0f)
{
}

AABB::AABB(glm::vec3 size, glm::vec3 position)
{
    // Compute the min and max points from the size and com
    minpt = position - size / 2.0f;
    maxpt = position + size / 2.0f;
}

AABB::~AABB()
{
}

void AABB::intersect(const std::shared_ptr<Ray> ray, std::shared_ptr<IntersectionData> intersection)
{
    // TODO: Objective 6: intersection of Ray with axis aligned box
    // 1. compute the entry and exit t values for the 3 slabs
    float txmin = (this->minpt.x - ray->origin.x) / ray->direction.x;
    float txmax = (this->maxpt.x - ray->origin.x) / ray->direction.x;
    float tymin = (this->minpt.y - ray->origin.y) / ray->direction.y;
    float tymax = (this->maxpt.y - ray->origin.y) / ray->direction.y;
    float tzmin = (this->minpt.z - ray->origin.z) / ray->direction.z;
    float tzmax = (this->maxpt.z - ray->origin.z) / ray->direction.z;
    // 2. compute the interval where the ray is inside the box
    float txlow = glm::min(txmin, txmax);
    float tylow = glm::min(tymin, tymax);
    float tzlow = glm::min(tzmin, tzmax);
    float txhigh = glm::max(txmin, txmax);
    float tyhigh = glm::max(tymin, tymax);
    float tzhigh = glm::max(tzmin, tzmax);
    // 3. compute last entry and first exit
    float tmin = glm::max(txlow, tylow);
    tmin = glm::max(tmin, tzlow);
    float tmax = glm::min(txhigh, tyhigh);
    tmax = glm::min(tmax, tzhigh);
    // non overlapping intersection of slabs, meaning no intersection with the box
    if (tmax < tmin)
    {
        intersection->t = -1;
    }
    else
    {
        // find which plane the intersection with the box occurs at and set the intersection normal
        intersection->t = tmin;
        ray->computePoint(tmin, intersection->p);
        if (intersection->p.x == minpt.x || intersection->p.x == maxpt.x)
        {
            intersection->n = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else if (intersection->p.y == minpt.y || intersection->p.y == maxpt.y)
        {
            intersection->n = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        else if (intersection->p.z == minpt.z || intersection->p.z == maxpt.z)
        {
            intersection->n = glm::vec3(0.0f, 0.0f, 1.0f);
        }
    }
    return;

}