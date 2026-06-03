#pragma once

#include "Test.h"

#include <array>

namespace tests
{
class ClearColor: public Test
{
    private:

        std::array<float, 4> m_ClearColor;

    public:

        ClearColor(std::shared_ptr<void> window);

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}
