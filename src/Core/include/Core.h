#pragma once

#include "interfaces/ICore.h"
#include "ConfigManager.h"

namespace VendingMachine { namespace Core {

class HALFactory;

class Core : public ICore
{
public:
    Core(HALFactory* halFactory);
    ~Core();

    bool isInService() const override;
    void setOutOfService(bool outOfService) override;

    ConfigManager& getConfigManager() override;

private:
    HALFactory* halFactory;
    ConfigManager configManager;
    bool inService;
};

}}
