#include "mod/Config.h"

#include <gtest/gtest.h>

namespace serverinfo_rest {
namespace {

TEST(ConfigTest, DefaultsToApiVersionTwoContract) {
    const Config config;
    EXPECT_EQ(config.version, 7);
    EXPECT_EQ(config.apiPrefix, "/api/v2");
    EXPECT_FALSE(config.syncBindingsToBdsAllowlist);
    EXPECT_FALSE(config.requireWhitelistAuthorizationOnJoin);
    EXPECT_EQ(config.playerActivityHistoryRetentionDays, 365);
}

} // namespace
} // namespace serverinfo_rest
