#pragma once

#include "IPollable.h"

namespace VendingMachine { namespace Core {

/**
 * \brief Interface for something that can show stuff to the user
 * 
 * Generic display class. Display might also implement other interfaces,
 * such as a keypad, if it is a touchscreen for example.
 */
class IDisplay : public virtual IPollable
{
public:
    
    /**
     * \brief Intialise the card reader interface
     *
     * Set up any hardware requirements and get ready to display stuff
     */
    virtual void init()=0;
};

}}
