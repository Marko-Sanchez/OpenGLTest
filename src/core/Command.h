#pragma once

namespace Core
{
/*
 * The 'Command' design pattern encapsulates a request as an object, decoupling the
 * invoker (whoever calls Execute()) from the receiver (whoever actually performs the
 * request).
 *
 * This lets 'TestMenu' (the invoker) trigger a test launch without
 * knowing how a Test is constructed or who handles the request. That knowledge living
 * in each concrete Command and its Receiver.
 */
class Command
{
protected:

    Command() = default;

public:

    virtual ~Command() = default;

    virtual void Execute() = 0;
    virtual std::string_view GetLabel() const = 0;
};
}// namespace Core
