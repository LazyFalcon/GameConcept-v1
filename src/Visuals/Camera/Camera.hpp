#pragma once
#include "Frustum.hpp"
#include "Filters.hpp"

template<typename T>
struct constr
{
    std::array<T, 2> minMax;
    bool disabled;
    void operator() (T& t){
        if(not disabled) t = glm::clamp(t, minMax[0], minMax[1]);
    }
};

struct CameraConstraints
{
    constr<float> yaw {{ -pi, pi }};
    constr<float> pitch {{ -pi * 0.95f , 0  }};
    constr<float> roll {{ -pi * 0.95f , 0  }};
    constr<float> fov {{ 2.5f * toRad, 150.f * toRad }};
    constr<glm::vec4> offset {{{ {-5,-5, -5, 0}, {5,5,25, 0} }}};
};

class Camera
{
protected:
    ~Camera() = default;
public:
    // * from camera transform calculate view matrix. call once per frame, before rendering phase
    void evaluate();
    void recalucuateProjectionMatrix();
    void recalucuateFrustum();
    void changeFov(float delta); // ! radians please!
    void printDebug();
    float convertDepthToWorld(float);
    void changeOffset(float x, float y, float z);

    // * controlled params
    glm::mat4 orientation;

    // * control parameters
    float fov;
    float aspectRatio;
    float nearDistance;
    float farDistance;

    float inertia;
    glm::vec4 offset;

    // * calculated. each cam has it's own, not sure if useful
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 projectionInverse;
    glm::mat4 PV;
    glm::mat4 invPV;

    glm::vec4 at; // -Z
    glm::vec4 right; // X
    glm::vec4 up; // Y

    glm::mat4 getPV() const {
        return PV;
    }
    const glm::vec4& position() const {
        return orientation[3];
    }

    Frustum frustum {};
    const Frustum& getFrustum() const {
        return frustum;
    }
};
