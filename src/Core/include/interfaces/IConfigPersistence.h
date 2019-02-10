#pragma once

#include "Config.h"

namespace VendingMachine { namespace Core {

/**
 * \interface IConfigPersistence
 * \brief Interface to store and load configuration from persistent storage
 *
 * Typically implemented by a HAL
 */
class IConfigPersistence
{
public:
    virtual ~IConfigPersistence();

    /**
     * \brief Save configuration to persistent storage
     *
     * \param[in] cfg Configuration to store
     */
    virtual void saveConfig(Config* cfg)=0;

    /**
     * \brief Load configuration from persistent storage
     *
     * \param[out] cfg Current configuration
     */
    virtual void loadConfig(Config* cfg)=0;
};

}}
