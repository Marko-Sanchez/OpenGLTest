#pragma once

#include <functional>
#include <string>
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

    // When we first run the app, TestMenu will present a series of buttons representing
    // different test. When a test is selected via OnImGuiRender we return a lambda and construct
    // test on demand, instead of loading all the test in the beginning of the application.
    class TestMenu: public Test
    {
        private:
        // Pointer to window context, to be shared with tests.
        std::shared_ptr<void> g_window;
        // Reference to a ptr outside this class, current selected test is updated via this variable.
        std::shared_ptr<Test>& m_currentTest;
        std::vector<std::pair<std::string, std::function<std::shared_ptr<Test>(std::shared_ptr<void>)>>> m_tests;

        public:
        TestMenu(std::shared_ptr<void> window, std::shared_ptr<Test>& currentTestPointer);

        void OnImGuiRender() override;

        // creates a vector of test, using a lambda to build test on demand.
        template<typename T>
        void RegisterTest(const std::string& name)
        {
            m_tests.push_back(std::make_pair(name, [](std::shared_ptr<void> g_window){return std::make_shared<T>(g_window); }));
        }
    };
}
