#include <gtest/gtest.h>

#include "policies/UndergraduateCreditPolicy.h"
#include "policies/GraduateCreditPolicy.h"
#include "policies/UnlimitedCreditPolicy.h"


// =========================================================
// UNDERGRADUATE CREDIT POLICY TESTS
// =========================================================

TEST(UndergraduateCreditPolicyTest, GpaAboveThreeAllows25Credits) {
    UndergraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(3.50);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 25);
}


TEST(UndergraduateCreditPolicyTest, GpaBetweenTwoAndThreeAllows20Credits) {
    UndergraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(2.50);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 20);
}


TEST(UndergraduateCreditPolicyTest, GpaBelowTwoAllows15Credits) {
    UndergraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(1.50);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 15);
}


TEST(UndergraduateCreditPolicyTest, GpaExactlyThreeAllows20Credits) {
    UndergraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(3.00);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 20);
}


TEST(UndergraduateCreditPolicyTest, GpaExactlyTwoAllows20Credits) {
    UndergraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(2.00);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 20);
}


// =========================================================
// GRADUATE CREDIT POLICY TESTS
// =========================================================

TEST(GraduateCreditPolicyTest, GpaAboveThreePointFiveAllows15Credits) {
    GraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(3.80);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 15);
}


TEST(GraduateCreditPolicyTest, GpaBetweenThreeAndThreePointFiveAllows10Credits) {
    GraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(3.20);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 10);
}


TEST(GraduateCreditPolicyTest, GpaBelowThreeAllows6Credits) {
    GraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(2.80);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 6);
}


TEST(GraduateCreditPolicyTest, GpaExactlyThreePointFiveAllows10Credits) {
    GraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(3.50);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 10);
}


TEST(GraduateCreditPolicyTest, GpaExactlyThreeAllows10Credits) {
    GraduateCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(3.00);

    ASSERT_TRUE(maxCredits.has_value());
    EXPECT_EQ(maxCredits.value(), 10);
}


// =========================================================
// UNLIMITED CREDIT POLICY TESTS
// =========================================================

TEST(UnlimitedCreditPolicyTest, ReturnsNoCreditLimit) {
    UnlimitedCreditPolicy policy;

    const auto maxCredits =
        policy.getMaxCredits(2.50);

    EXPECT_FALSE(maxCredits.has_value());
}