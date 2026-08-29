#include "domain/Enrollment.h"

#include <stdexcept>


Enrollment::Enrollment(
    int id,
    Student* student,
    Course* course
)
    : id(id),
    student(student),
    course(course) {

    // Enrollment must always reference a valid Student and Course.
    if (student == nullptr) {
        throw std::invalid_argument(
            "Student cannot be null."
        );
    }

    if (course == nullptr) {
        throw std::invalid_argument(
            "Course cannot be null."
        );
    }
}


int Enrollment::getId() const {
    return id;
}


Student* Enrollment::getStudent() const {
    return student;
}


Course* Enrollment::getCourse() const {
    return course;
}


void Enrollment::setExamScore(
    const Exam* exam,
    double score
) {
    // If a score already exists for this Exam,
    // update it instead of creating a duplicate.
    for (auto& examScore : examScores) {
        if (examScore.getExam() == exam) {
            examScore.setScore(
                score
            );

            /*
             * The previously calculated final result
             * is no longer valid after an exam score changes.
             */
            finalScore.reset();
            letterGrade.reset();

            return;
        }
    }

    // Otherwise create a new ExamScore.
    examScores.emplace_back(
        exam,
        score
    );

    /*
     * Adding a new exam score also invalidates
     * any previously calculated final result.
     */
    finalScore.reset();
    letterGrade.reset();
}


const std::vector<ExamScore>&
Enrollment::getExamScores() const {
    // Return the existing collection without copying it.
    return examScores;
}


std::optional<double>
Enrollment::getFinalScore() const {
    return finalScore;
}


void Enrollment::setFinalScore(
    double score
) {
    if (
        score < 0.0 ||
        score > 100.0
        ) {
        throw std::invalid_argument(
            "Final score must be between 0 and 100."
        );
    }

    finalScore = score;
}


std::optional<LetterGrade>
Enrollment::getLetterGrade() const {
    return letterGrade;
}


void Enrollment::setLetterGrade(
    LetterGrade grade
) {
    letterGrade = grade;
}