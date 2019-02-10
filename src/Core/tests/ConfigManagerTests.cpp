#include "ConfigManager.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace VendingMachine::Core;
using namespace ::testing;

class MockConfigPersistence : public IConfigPersistence
{
public:
    MOCK_METHOD1(saveConfig, void(Config* cfg));
    MOCK_METHOD1(loadConfig, void(Config* cfg));
};

TEST(ConfigManagerTests, test_initial_config_load)
{
    MockConfigPersistence persistence;

    EXPECT_CALL(persistence, loadConfig(_)).WillOnce(Return());

    ConfigManager victim(&persistence);

    Config* cfg = victim.getConfig();
    EXPECT_NE(nullptr, cfg);
}

TEST(ConfigManagerTests, test_config_save_calls_persistence_layer)
{
    MockConfigPersistence persistence;

    EXPECT_CALL(persistence, loadConfig(_)).WillOnce(Return());

    ConfigManager victim(&persistence);

    EXPECT_CALL(persistence, saveConfig(_)).WillOnce(Return());

    victim.updateConfig();
}