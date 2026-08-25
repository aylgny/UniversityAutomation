#include "policies/UndergraduateCreditPolicy.h"

std::optional<int> UndergraduateCreditPolicy::getMaxCredits(double gpa) const {
    if (gpa > 3.0) {
        return 25;
    }

    if (gpa >= 2.0) {
        return 20;
    }

    return 15;
}