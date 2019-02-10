#pragma once

namespace VendingMachine { namespace Core {

/**
 * \brief Interface to a card reader device
 * 
 * 
 */
class ICardReader
{
public:
    virtual ~ICardReader();

    /**
     * \brief Intialise the card reader interface
     *
     * Set up any hardware requirements and get ready to read cards
     */
    virtual void init()=0;

};

}}
