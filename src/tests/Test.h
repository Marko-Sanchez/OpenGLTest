#pragma once

#include <string_view>

namespace tests
{
// Base class of which all test inherit from.
//
// virtual destructor needed, since we up-cast our derived test;
// otherwise, only ~Test() runs and the derived class destructor never gets called.
class Test
{
    public:

        Test()          = default;
        virtual ~Test() = default;

        virtual std::string_view GetName() const {return "Test";}
        virtual void OnRender()     {}
        virtual void OnImGuiRender(){}
};
}// namespace tests
