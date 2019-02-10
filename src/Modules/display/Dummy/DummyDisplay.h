#pragma once

#include <interfaces/IDisplay.h>

namespace VendingMachine { namespace Display {

/**
 * \brief A dummy implementation of IDisplay
 */
class DummyDisplay : public ::VendingMachine::Core::IDisplay
{
public:
    DummyDisplay();
    ~DummyDisplay();

    void init() override;
    void poll() override;
};

}}