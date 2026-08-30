#include "domain/Person.h"

#include <stdexcept>


Person::Person(
    int id,
    const std::string& name
)
    : id(id),
    name(name) {

    // Ensures that every person has a valid positive identifier.
    if (id <= 0) {
        throw std::invalid_argument(
            "Person id must be positive."
        );
    }

    // Prevents creation of a person without a name.
    if (name.empty()) {
        throw std::invalid_argument(
            "Person name cannot be empty."
        );
    }
}


int Person::getId() const {
    return id;
}


const std::string& Person::getName() const {
    return name;
}