#include "policies/UnlimitedCreditPolicy.h"

// GPA does not affect the result because this policy has no credit limit.
std::optional<int> UnlimitedCreditPolicy::getMaxCredits(double) const {
    return std::nullopt;
}