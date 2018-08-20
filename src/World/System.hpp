#pragma once
#include "Celestial.hpp"

class System
{
public:
    std::vector<Celestial> m_celestials;
    System(){
        m_celestials.emplace_back("Alfa", polar{0.0, 0.1});
        m_celestials.emplace_back("Beta", polar{0.0, 0.4});
        m_celestials.emplace_back("Home", polar{0.0, 1});
        m_celestials.emplace_back("Shelter", polar{0.0, 2});
    }
};