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
        throw std::invalid_argument("Student cannot be null.");
    }

    if (course == nullptr) {
        throw std::invalid_argument("Course cannot be null.");
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

void Enrollment::setExamScore(const Exam* exam, double score) {
    // If a score already exists for this Exam, update it instead of
    // creating a duplicate ExamScore entry.
    for (auto& examScore : examScores) {
        if (examScore.getExam() == exam) {
            examScore.setScore(score);
            return;
        }
    }

    // Otherwise create a new ExamScore directly inside the vector.
    examScores.emplace_back(exam, score);
}

const std::vector<ExamScore>& Enrollment::getExamScores() const {
    // Return the existing collection without copying it.
    return examScores;
}

std::optional<double> Enrollment::getFinalScore() const {
    return finalScore;
}

void Enrollment::setFinalScore(double score) {
    if (score < 0.0 || score > 100.0) {
        throw std::invalid_argument(
            "Final score must be between 0 and 100."
        );
    }

    // Setting a value changes finalScore from an empty optional
    // into a calculated result.
    finalScore = score;
}

std::optional<LetterGrade> Enrollment::getLetterGrade() const {
    return letterGrade;
}

void Enrollment::setLetterGrade(LetterGrade grade) {
    // Letter grade becomes available after final grade calculation.
    letterGrade = grade;
}