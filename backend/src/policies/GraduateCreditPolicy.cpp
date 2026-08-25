#include "policies/GraduateCreditPolicy.h"

std::optional<int> GraduateCreditPolicy::getMaxCredits(double gpa) const {
    if (gpa > 3.5) {
        return 15;
    }

    if (gpa >= 3.0) {
        return 10;
    }

    return 6;
}