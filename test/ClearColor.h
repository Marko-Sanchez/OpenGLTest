#pragma once

#include "Test.h"

namespace tests
{
    class ClearColor: public Test
    {
        private:
        float m_ClearColor[4];

        public:
        ClearColor(std::shared_ptr<void> window);

        void OnRender() override;
        void OnImGuiRender() override;
    };
}
