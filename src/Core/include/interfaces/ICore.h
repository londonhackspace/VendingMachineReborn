#pragma once

namespace VendingMachine { namespace Core {

class ConfigManager;

/** 
 * \brief Interface to the core vending logic
 * 
 * Not intended to be implemented by modules, but rather
 * used to allow modules to call back into the core logic to get
 * status and similar stuff
 */
class ICore
{
public:
    virtual ~ICore();

    /**
     * \brief Check if the vending machine is usable or not
     *
     * \returns true if the machine is in service, false otherwise
     */
    virtual bool isInService() const =0;

    /**
     * \brief Put machine out of service
     *
     */
    virtual void setOutOfService(bool outOfService) =0;

    
    /**
     * \brief get the configuration manager
     *
     * \returns a reference to the configuration manager
     */
    virtual ConfigManager& getConfigManager()=0;  
};

}}
