#include "domain/GraduateCourse.h"

GraduateCourse::GraduateCourse(
    int id,
    const std::string& code,
    const std::string& name,
    int credits
)
    // Graduate courses require at least CC to pass.
    : Course(
        id,
        code,
        name,
        credits,
        PassingPolicy(LetterGrade::CC)
    ) {
}