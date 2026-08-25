#include "domain/UndergraduateCourse.h"

UndergraduateCourse::UndergraduateCourse(
    int id,
    const std::string& code,
    const std::string& name,
    int credits
)
    : Course(
        id,
        code,
        name,
        credits,
        PassingPolicy(LetterGrade::DD)
    ) {
}