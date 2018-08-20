#pragma once
/*
! jednostki: https://www.boost.org/doc/libs/1_65_0/doc/html/boost_units.html
namespace unit {
    AU, km,

    speed: km/s[kms], km/h[kmh], powinno wystarczyć, czas i tak będzie chyba przyspieszony, nie?
    energy:
    force: kN
    masa: tony[t]
    AU = 150'000'000 km
    * polarCoordinates<rad, AU>
}
*/

struct polar
{
    float angle;
    float radius;
};

class Celestial
{
public:
    virtual ~Celestial() = default;
    Celestial(const std::string& name, polar position):
        m_name(name),
        m_position(position),
    {}

    std::string m_name;
    polar m_position; // * AU
};