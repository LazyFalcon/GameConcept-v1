﻿#pragma once
#include <array>

union FrustmCorners
{
    struct u {
        glm::vec4 farTopRight;
        glm::vec4 farTopLeft;
        glm::vec4 farBottomRight;
        glm::vec4 farBottomLeft;
        glm::vec4 nearTopRight;
        glm::vec4 nearTopLeft;
        glm::vec4 nearBottomRight;
        glm::vec4 nearBottomLeft;
        glm::vec4 nearCenter;
        glm::vec4 farCenter;
    } m;
    std::array<glm::vec4, 10> array {};
};

union FrustumPlanes
{
    struct u {
        glm::vec4 topPlane;
        glm::vec4 bottomPlane;
        glm::vec4 rightPlane;
        glm::vec4 leftPlane;
        glm::vec4 nearPlane;
        glm::vec4 farPlane;
    } m;
    std::array<glm::vec4, 6> array {};
};

union FrustumVectors
{
    struct u {
        glm::vec4 right;
        glm::vec4 up;
        glm::vec4 forward;
    } m;
    std::array<glm::vec4, 3> array {};
};

/**
 *  normalne są skierowane na zewnątrz,
 *  kolejnośc punktów ma znaczenie?
 *  odleglosc jest miezona w metryce manhattan
 *  przenieć obliczanie odleglosci
 *  test przesaniania również powinien si znaleźć wyżej
 */
class Frustum
{
public:
    bool testSphere(glm::vec4 position, float radius) const;
    bool testAABB(glm::vec4 box) const;
    bool intersects(glm::vec4 position, glm::vec2 size) const;
    std::vector<FrustmCorners> splitForCSM(u32 parts=4);
    std::vector<float> splitDistances;

    FrustmCorners corners {};
    FrustumPlanes planes {};
    std::array<glm::vec4, 5> cornerVectors;
    FrustumVectors vectors {};
    glm::vec4 eye;
    float fov;
    float aspect;
    float zNear, zFar;
};
