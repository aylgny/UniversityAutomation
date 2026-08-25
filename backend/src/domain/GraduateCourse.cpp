#include "domain/GraduateCourse.h"

GraduateCourse::GraduateCourse(
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
        PassingPolicy(LetterGrade::CC)
    ) {
}