#pragma once

#include "core/Test.h"

#include <functional>
#include <memory>
#include <string>

namespace tests
{

// When we first run the app, TestMenu will present a series of buttons representing
// different test. When a test is selected via OnImGuiRender we return a lambda and construct
// test on demand, instead of loading all the test in the beginning of the application.
class TestMenu final: public Test
{
    private:

        using TestFunc       = std::function<std::shared_ptr<Test> (std::shared_ptr<void>)>;
        using OnTestSelected = std::function<void(std::shared_ptr<Test>)>;

        struct Entry
        {
            std::string name;
            TestFunc    factory;

            Entry(std::string n, TestFunc f):
                name(n), factory(f)
            {}
        };

        OnTestSelected         m_onSelectedTest;
        std::shared_ptr<void>  g_window;
        std::vector<Entry>     m_tests;

    public:

        TestMenu(std::shared_ptr<void> window, OnTestSelected selectedTest);

        std::string_view GetName() const override;
        void OnImGuiRender() override;

        // creates a vector of test, using a lambda to build test on demand.
        template<typename T>
        void RegisterTest(const std::string name)
        {
            m_tests.emplace_back(std::move(name), [](std::shared_ptr<void> window)
                    {
                        return std::make_shared<T>(window);
                    });
        }
};
}//namespace tests
