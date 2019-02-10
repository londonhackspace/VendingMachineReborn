#include "Core.h"
#include "HALFactory.h"
#include "ConfigManager.h"

namespace VendingMachine { namespace Core {

Core::Core(HALFactory* halFactory) :
    halFactory(halFactory),
    configManager(halFactory->getConfigPersistence()),
    inService(configManager.getConfig()->inService)
{

}

Core::~Core()
{

}

bool Core::isInService() const
{
    return inService;
}

void Core::setOutOfService(bool outOfService)
{
    inService = !outOfService;
    configManager.getConfig()->inService = inService;
    configManager.updateConfig();
}

ConfigManager& Core::getConfigManager()
{
    return configManager;
}

}}
