#pragma once

namespace VendingMachine { namespace Core {

class IDisplay
{
public:
    virtual ~IDisplay();

    virtual void init()=0;
};

}}
