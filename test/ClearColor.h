#pragma once

#include "Test.h"

namespace tests
{
    class ClearColor: public Test
    {
        private:
        float m_ClearColor[4];

        public:
        ClearColor();

        void OnRender() override;
        void OnImGuiRender() override;
    };
}
