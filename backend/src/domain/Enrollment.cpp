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
    for (auto& examScore : examScores) {
        if (examScore.getExam() == exam) {
            examScore.setScore(score);
            return;
        }
    }

    examScores.emplace_back(exam, score);
}

const std::vector<ExamScore>& Enrollment::getExamScores() const {
    return examScores;
}

std::optional<double> Enrollment::getFinalScore() const {
    return finalScore;
}

void Enrollment::setFinalScore(double score) {
    if (score < 0.0 || score > 100.0) {
        throw std::invalid_argument("Final score must be between 0 and 100.");
    }

    finalScore = score;
}

std::optional<LetterGrade> Enrollment::getLetterGrade() const {
    return letterGrade;
}

void Enrollment::setLetterGrade(LetterGrade grade) {
    letterGrade = grade;
}