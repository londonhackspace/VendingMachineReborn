#pragma once

namespace VendingMachine { namespace Core {

class ICardReader
{
public:
    virtual ~ICardReader();

    virtual void init()=0;
};

}}
