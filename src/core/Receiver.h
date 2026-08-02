#pragma once

#include "Test.h"

#include <memory>

namespace Core
{
/*
 * Abstract interface for the 'receiver' in the Command pattern. 'Command' objects hold
 * a reference to 'Receiver' rather than a concrete type (e.g. 'Application'), so they can
 * invoke SetActiveTest() without knowing which concrete class implements it.
 *
 * The alternative to this interface would be templating Command on the receiver type
 * (e.g. ConcreteCommand<Receiver>), which avoids requiring receivers to inherit from a common
 * base (e.g. this class) and resolves the call statically instead of virtually.
 *
 * Abstract Interface path was used since it easier to read and requires less code at call sites.
 */
class Receiver
{
public:

    virtual ~Receiver() = default;
    virtual void SetActiveTest(std::shared_ptr<tests::Test> test) = 0;
};
}// namespace Core
