#pragma once

#include <memory>
#include <string_view>

namespace tests
{
// Base class of which all test inherit from.
class Test
{
    public:

        Test(){}
        Test(std::shared_ptr<void> window){}

        virtual ~Test(){}

        virtual std::string_view GetName() const {return "Test";}
        virtual void OnRender(){}
        virtual void OnImGuiRender(){}
};
}// namespace tests
