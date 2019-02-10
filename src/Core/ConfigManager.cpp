#include "ConfigManager.h"

namespace VendingMachine { namespace Core {

ConfigManager::ConfigManager(IConfigPersistence* persistence) :
    persistence(persistence)
{
    persistence->loadConfig(&currentConfig);
}

ConfigManager::~ConfigManager()
{

}

Config* ConfigManager::getConfig()
{
    return &currentConfig;
}

void ConfigManager::updateConfig()
{
    persistence->saveConfig(&currentConfig);
}

}}
