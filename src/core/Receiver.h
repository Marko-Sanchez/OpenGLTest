#pragma once

#include "Test.h"

#include <memory>

namespace Core
{
class Receiver
{
public:

    virtual ~Receiver() = default;
    virtual void SetActiveTest(std::shared_ptr<tests::Test> test) = 0;
};
}// namespace Core
