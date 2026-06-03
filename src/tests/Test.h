#pragma once

#include <memory>

namespace tests
{
// Base class of which all test inherit from.
class Test
{
    public:

        Test(){}
        Test(std::shared_ptr<void> window){}

        virtual ~Test(){}

        virtual void OnRender(){}
        virtual void OnImGuiRender(){}
};
}// namespace tests
