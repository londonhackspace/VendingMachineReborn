#pragma once

namespace VendingMachine { namespace Core {

/** 
 * \brief Interface for something that can or should be polled
 *
 * This interface is implemented by things that can or should be 
 * regularly called in the main event loop.
 *
 * There's a fair chance you want to virtually inherit from this,
 * in case multiple base classes are all pollable
 * 
 */
class IPollable
{
public:
    virtual ~IPollable();

    /**
     * \brief poll
     *
     * Perform any periodic tasks
     */
    virtual void poll()=0;
};

}}
