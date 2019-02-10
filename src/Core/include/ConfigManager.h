#pragma once

#include "Config.h"
#include "interfaces/IConfigPersistence.h"

namespace VendingMachine { namespace Core {

/**
 * \brief Base class for configuration manager
 * 
 * Provides common functionality for getting/setting config
 */
class ConfigManager
{
public:
    ConfigManager(IConfigPersistence* persistence);
    ~ConfigManager();

    /**
     * \brief get access to the running config
     * 
     * \returns A pointer to the current configuration
     */
    Config* getConfig();

    /**
     * \brief processes updates to configuration
     *
     * You must call this after updating any fields in configuration
     * for them to be saved
     */
    void updateConfig();

private:
    Config currentConfig;

    IConfigPersistence* persistence;
};

}}
