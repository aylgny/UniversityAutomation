# University Automation

A university automation system developed as a software engineering case study.

The project focuses on object-oriented analysis and design, extensibility, maintainability, and clean separation of responsibilities.

## Technologies

- C++17
- CMake
- GoogleTest
- Visual Studio
- React + TypeScript (frontend)
- Electron (desktop application)

## Current Status

Backend domain logic is implemented and tested.

Current automated test status:

- 91 tests passed
- 0 failed

The frontend and Electron integration will be added next.

## Architecture

The project follows a layered architecture:

```text
React / Electron UI
        |
        v
Application Layer
        |
        v
Domain Model
        |
        v
Policies / Strategies
Application Layer

Application controllers coordinate use cases without containing core business rules.

Main controllers:

RegistrationController
GradingController
Domain Layer

Main domain objects include:

Person
Student
UndergraduateStudent
GraduateStudent
Instructor
Course
UndergraduateCourse
GraduateCourse
Enrollment
Exam
ExamScore
Design Principles

The design applies object-oriented principles including:

Encapsulation
Abstraction
Inheritance
Polymorphism
Composition
Single Responsibility Principle
Open/Closed Principle
Liskov Substitution Principle
Interface Segregation Principle
Dependency Inversion Principle

Inheritance is used for true IS-A relationships, while composition is preferred for configurable and changeable behaviors.

Example:

GraduateStudent IS-A Student

Student HAS-A CreditPolicy
Student HAS-A LetterGradePolicy
Design Patterns
Strategy Pattern

The Strategy Pattern is used for grade calculation algorithms.

GradeCalculationStrategy
        |
        +-- WeightedAverageStrategy
        |
        +-- ThresholdStrategy

This allows grading algorithms to be changed or extended without modifying the code that uses them.

Policy-Based Design

Changeable academic rules are isolated in policy objects.

Examples:

CreditPolicy
UndergraduateCreditPolicy
GraduateCreditPolicy
UnlimitedCreditPolicy
LetterGradePolicy
UndergraduateLetterGradePolicy
GraduateLetterGradePolicy
PassingPolicy
CourseGradingPolicy

This prevents frequently changing academic rules from being hard-coded into domain entities.

Course Registration

Student credit limits are determined according to student type and GPA.

The registration flow delegates credit-rule calculation through the student:

RegistrationController
        |
        v
Student
        |
        v
CreditPolicy

The controller does not know how credit limits are calculated.

Grade Calculation

A course can use different grade calculation strategies for undergraduate and graduate students.

The grading flow is:

GradingController
        |
        v
CourseGradingPolicy
        |
        v
GradeCalculationStrategy
        |
        +-- WeightedAverageStrategy
        |
        +-- ThresholdStrategy

After the numerical grade is calculated, the student's letter-grade policy determines the final letter grade.

Student
   |
   v
LetterGradePolicy
Testing

GoogleTest is used for automated testing.

The test suite currently covers:

Undergraduate credit rules
Graduate credit rules
Unlimited credit policy
Credit boundary values
Undergraduate letter grades
Graduate letter grades
Letter-grade boundary values
Weighted average calculation
Threshold-based calculation
Invalid grading configurations
Course registration
Duplicate enrollment prevention
Credit-limit violations
Grading controller workflows
Student validation
Course validation
Exam score validation
Enrollment validation
Passing rules
Course grading policies

Current result:

91 tests passed
0 tests failed
Project Structure
UniversityAutomation/
|
|-- backend/
|   |
|   |-- include/
|   |   |-- application/
|   |   |-- domain/
|   |   |-- policies/
|   |   `-- strategies/
|   |
|   |-- src/
|   |   |-- application/
|   |   |-- domain/
|   |   |-- policies/
|   |   |-- strategies/
|   |   `-- main.cpp
|   |
|   `-- tests/
|
|-- frontend/
|
|-- electron/
|
|-- docs/
|
|-- CMakeLists.txt
|-- CMakePresets.json
`-- README.md
Build

The project requires a C++17-compatible compiler and CMake.

Example build process:

cmake -S . -B build
cmake --build build
Running Tests

After building the project, the GoogleTest executable can be run using:

UniversityTests
Future Work

Planned next steps:

React + TypeScript frontend
Electron desktop integration
Student course registration interface
Instructor grading interface
UI integration with the C++ backend
Final UML documentation
Project report and user guide