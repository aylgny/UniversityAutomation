#include "policies/UndergraduateCreditPolicy.h"

std::optional<int> UndergraduateCreditPolicy::getMaxCredits(double gpa) const {
    // Undergraduate students with GPA above 3.0 may take up to 25 credits.
    if (gpa > 3.0) {
        return 25;
    }

    // Students with GPA between 2.0 and 3.0 may take up to 20 credits.
    if (gpa >= 2.0) {
        return 20;
    }

    // Students with GPA below 2.0 may take up to 15 credits.
    return 15;
}