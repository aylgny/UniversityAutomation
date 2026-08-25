#include "policies/UnlimitedCreditPolicy.h"

std::optional<int> UnlimitedCreditPolicy::getMaxCredits(double) const {
    return std::nullopt;
}