#include "domain/Exam.h"

#include <stdexcept>


Exam::Exam(
    int id,
    const std::string& name
)
    : id(id),
    name(name) {

    // Exam identifiers must be positive.
    if (id <= 0) {
        throw std::invalid_argument(
            "Exam id must be positive."
        );
    }

    // Every exam must have a descriptive name.
    if (name.empty()) {
        throw std::invalid_argument(
            "Exam name cannot be empty."
        );
    }
}


int Exam::getId() const {
    return id;
}


const std::string& Exam::getName() const {
    return name;
}