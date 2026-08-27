#include "application/ApplicationState.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "application/GradingController.h"

#include "domain/Student.h"
#include "domain/StudentType.h"
#include "domain/UndergraduateStudent.h"
#include "domain/GraduateStudent.h"

#include "domain/Course.h"
#include "domain/UndergraduateCourse.h"
#include "domain/GraduateCourse.h"

#include "domain/Instructor.h"
#include "domain/Enrollment.h"
#include "domain/Exam.h"
#include "domain/LetterGrade.h"

#include "policies/CourseGradingPolicy.h"

#include "strategies/GradeCalculationStrategy.h"
#include "strategies/WeightedAverageStrategy.h"
#include "strategies/ThresholdStrategy.h"


using json = nlohmann::json;


ApplicationState::ApplicationState() = default;

ApplicationState::~ApplicationState() = default;


// =========================================================
// SAMPLE DATA
// =========================================================

void ApplicationState::seedSampleData() {
    students.clear();
    courses.clear();
    instructors.clear();
    enrollments.clear();


    // -----------------------------------------------------
    // Students
    // -----------------------------------------------------

    students.push_back(
        std::make_unique<UndergraduateStudent>(
            1,
            "Undergraduate Student",
            3.20
        )
    );

    students.push_back(
        std::make_unique<GraduateStudent>(
            2,
            "Graduate Student",
            3.60
        )
    );


    // -----------------------------------------------------
    // Instructor
    // -----------------------------------------------------

    instructors.push_back(
        std::make_unique<Instructor>(
            1,
            "Instructor"
        )
    );


    // -----------------------------------------------------
    // Undergraduate course
    // -----------------------------------------------------

    auto undergraduateCourse =
        std::make_unique<UndergraduateCourse>(
            101,
            "CS301",
            "Software Engineering",
            5
        );

    undergraduateCourse->createExams(2);

    Course* undergraduateCoursePtr =
        undergraduateCourse.get();

    courses.push_back(
        std::move(undergraduateCourse)
    );


    // -----------------------------------------------------
    // Graduate course
    // -----------------------------------------------------

    auto graduateCourse =
        std::make_unique<GraduateCourse>(
            201,
            "CS501",
            "Advanced Software Engineering",
            5
        );

    graduateCourse->createExams(2);

    Course* graduateCoursePtr =
        graduateCourse.get();

    courses.push_back(
        std::move(graduateCourse)
    );


    // -----------------------------------------------------
    // Instructor-course relationships
    // -----------------------------------------------------

    instructors.front()->addCourse(
        undergraduateCoursePtr
    );

    instructors.front()->addCourse(
        graduateCoursePtr
    );


    // -----------------------------------------------------
    // Sample grading configurations
    // -----------------------------------------------------

    GradingController gradingController;

    gradingController.configureWeightedAverage(
        *undergraduateCoursePtr,
        StudentType::UNDERGRADUATE,
        {
            {1, 0.40},
            {2, 0.60}
        }
    );

    gradingController.configureThreshold(
        *graduateCoursePtr,
        StudentType::GRADUATE,
        50.0,
        { 1 }
    );
}


// =========================================================
// ACCESSORS
// =========================================================

const std::vector<std::unique_ptr<Student>>&
ApplicationState::getStudents() const {
    return students;
}


const std::vector<std::unique_ptr<Course>>&
ApplicationState::getCourses() const {
    return courses;
}


const std::vector<std::unique_ptr<Instructor>>&
ApplicationState::getInstructors() const {
    return instructors;
}

std::vector<std::unique_ptr<Enrollment>>&
ApplicationState::getEnrollments() {
    return enrollments;
}

const std::vector<std::unique_ptr<Enrollment>>&
ApplicationState::getEnrollments() const {
    return enrollments;
}


// =========================================================
// LOOKUPS
// =========================================================

Student* ApplicationState::findStudentById(
    int id
) const {

    for (const auto& student : students) {
        if (student->getId() == id) {
            return student.get();
        }
    }

    return nullptr;
}


Course* ApplicationState::findCourseById(
    int id
) const {

    for (const auto& course : courses) {
        if (course->getId() == id) {
            return course.get();
        }
    }

    return nullptr;
}


Instructor* ApplicationState::findInstructorById(
    int id
) const {

    for (const auto& instructor : instructors) {
        if (instructor->getId() == id) {
            return instructor.get();
        }
    }

    return nullptr;
}


Enrollment* ApplicationState::findEnrollmentById(
    int id
) const {

    for (const auto& enrollment : enrollments) {
        if (enrollment->getId() == id) {
            return enrollment.get();
        }
    }

    return nullptr;
}


// =========================================================
// LOAD
// =========================================================

void ApplicationState::loadFromFile(
    const std::string& filePath
) {
    std::ifstream inputFile(
        filePath
    );

    if (!inputFile.is_open()) {
        throw std::runtime_error(
            "Could not open JSON file for reading."
        );
    }

    json data;

    inputFile >> data;

    students.clear();
    courses.clear();
    instructors.clear();
    enrollments.clear();


    // =====================================================
    // STUDENTS
    // =====================================================

    for (const auto& studentJson :
        data.at("students")) {

        const int id =
            studentJson.at("id").get<int>();

        const std::string name =
            studentJson.at("name")
            .get<std::string>();

        const double gpa =
            studentJson.at("gpa")
            .get<double>();

        const std::string type =
            studentJson.at("type")
            .get<std::string>();

        if (type == "UNDERGRADUATE") {

            students.push_back(
                std::make_unique<
                UndergraduateStudent
                >(
                    id,
                    name,
                    gpa
                )
            );
        }
        else if (type == "GRADUATE") {

            students.push_back(
                std::make_unique<
                GraduateStudent
                >(
                    id,
                    name,
                    gpa
                )
            );
        }
        else {
            throw std::runtime_error(
                "Unknown student type in JSON file."
            );
        }
    }


    // =====================================================
    // INSTRUCTORS
    // =====================================================

    for (const auto& instructorJson :
        data.at("instructors")) {

        const int id =
            instructorJson.at("id")
            .get<int>();

        const std::string name =
            instructorJson.at("name")
            .get<std::string>();

        instructors.push_back(
            std::make_unique<Instructor>(
                id,
                name
            )
        );
    }


    // =====================================================
    // COURSES
    // =====================================================

    GradingController gradingController;

    for (const auto& courseJson :
        data.at("courses")) {

        const int id =
            courseJson.at("id")
            .get<int>();

        const std::string code =
            courseJson.at("code")
            .get<std::string>();

        const std::string name =
            courseJson.at("name")
            .get<std::string>();

        const int credits =
            courseJson.at("credits")
            .get<int>();

        const std::string type =
            courseJson.at("type")
            .get<std::string>();

        const int examCount =
            courseJson.value(
                "examCount",
                0
            );


        std::unique_ptr<Course> course;


        if (type == "UNDERGRADUATE") {

            course =
                std::make_unique<
                UndergraduateCourse
                >(
                    id,
                    code,
                    name,
                    credits
                );
        }
        else if (type == "GRADUATE") {

            course =
                std::make_unique<
                GraduateCourse
                >(
                    id,
                    code,
                    name,
                    credits
                );
        }
        else {
            throw std::runtime_error(
                "Unknown course type in JSON file."
            );
        }


        if (examCount > 0) {
            course->createExams(
                examCount
            );
        }


        // ---------------------------------------------
        // Grading configurations
        // ---------------------------------------------

        if (
            courseJson.contains(
                "gradingConfigurations"
            )
            ) {
            for (
                const auto& gradingJson :
                courseJson.at(
                    "gradingConfigurations"
                )
                ) {

                const std::string studentTypeValue =
                    gradingJson
                    .at("studentType")
                    .get<std::string>();

                StudentType studentType;

                if (
                    studentTypeValue ==
                    "UNDERGRADUATE"
                    ) {
                    studentType =
                        StudentType::UNDERGRADUATE;
                }
                else if (
                    studentTypeValue ==
                    "GRADUATE"
                    ) {
                    studentType =
                        StudentType::GRADUATE;
                }
                else {
                    throw std::runtime_error(
                        "Unknown grading student type."
                    );
                }


                const std::string method =
                    gradingJson
                    .at("method")
                    .get<std::string>();


                if (method == "WEIGHTED_AVERAGE") {

                    std::map<int, double>
                        weights;

                    for (
                        const auto& weightJson :
                        gradingJson.at("weights")
                        ) {
                        const int examId =
                            weightJson
                            .at("examId")
                            .get<int>();

                        const double weight =
                            weightJson
                            .at("weight")
                            .get<double>();

                        weights[examId] =
                            weight;
                    }

                    gradingController
                        .configureWeightedAverage(
                            *course,
                            studentType,
                            weights
                        );
                }
                else if (method == "THRESHOLD") {

                    const double threshold =
                        gradingJson
                        .at("threshold")
                        .get<double>();

                    const std::vector<int>
                        thresholdExamIds =
                        gradingJson
                        .at(
                            "thresholdExamIds"
                        )
                        .get<
                        std::vector<int>
                        >();

                    gradingController
                        .configureThreshold(
                            *course,
                            studentType,
                            threshold,
                            thresholdExamIds
                        );
                }
                else {
                    throw std::runtime_error(
                        "Unknown grading method."
                    );
                }
            }
        }


        courses.push_back(
            std::move(course)
        );
    }


    // =====================================================
    // RESTORE INSTRUCTOR -> COURSE RELATIONSHIPS
    // =====================================================

    for (const auto& instructorJson :
        data.at("instructors")) {

        const int instructorId =
            instructorJson
            .at("id")
            .get<int>();

        Instructor* instructor =
            findInstructorById(
                instructorId
            );

        if (instructor == nullptr) {
            throw std::runtime_error(
                "Instructor could not be restored."
            );
        }


        if (
            instructorJson.contains(
                "courseIds"
            )
            ) {
            for (
                const int courseId :
            instructorJson.at(
                "courseIds"
            )
                ) {

                Course* course =
                    findCourseById(
                        courseId
                    );

                if (course == nullptr) {
                    throw std::runtime_error(
                        "Instructor references an unknown course."
                    );
                }

                instructor->addCourse(
                    course
                );
            }
        }
    }


    // =====================================================
    // ENROLLMENTS
    // =====================================================

    for (const auto& enrollmentJson :
        data.at("enrollments")) {

        const int id =
            enrollmentJson.at("id")
            .get<int>();

        const int studentId =
            enrollmentJson
            .at("studentId")
            .get<int>();

        const int courseId =
            enrollmentJson
            .at("courseId")
            .get<int>();

        Student* student =
            findStudentById(
                studentId
            );

        Course* course =
            findCourseById(
                courseId
            );

        if (
            student == nullptr ||
            course == nullptr
            ) {
            throw std::runtime_error(
                "Enrollment references an unknown student or course."
            );
        }


        auto enrollment =
            std::make_unique<Enrollment>(
                id,
                student,
                course
            );


        // ---------------------------------------------
        // Exam scores
        // ---------------------------------------------

        for (
            const auto& examScoreJson :
            enrollmentJson.at(
                "examScores"
            )
            ) {

            const int examId =
                examScoreJson
                .at("examId")
                .get<int>();

            const double score =
                examScoreJson
                .at("score")
                .get<double>();

            const Exam* selectedExam =
                nullptr;


            for (
                const auto& exam :
                course->getExams()
                ) {
                if (
                    exam->getId() ==
                    examId
                    ) {
                    selectedExam =
                        exam.get();

                    break;
                }
            }


            if (selectedExam == nullptr) {
                throw std::runtime_error(
                    "Enrollment score references an unknown exam."
                );
            }


            enrollment->setExamScore(
                selectedExam,
                score
            );
        }


        // ---------------------------------------------
        // Final score
        // ---------------------------------------------

        if (
            !enrollmentJson
            .at("finalScore")
            .is_null()
            ) {
            enrollment->setFinalScore(
                enrollmentJson
                .at("finalScore")
                .get<double>()
            );
        }


        // ---------------------------------------------
        // Letter grade
        // ---------------------------------------------

        if (
            !enrollmentJson
            .at("letterGrade")
            .is_null()
            ) {
            enrollment->setLetterGrade(
                static_cast<LetterGrade>(
                    enrollmentJson
                    .at("letterGrade")
                    .get<int>()
                    )
            );
        }


        enrollments.push_back(
            std::move(enrollment)
        );
    }
}


// =========================================================
// SAVE
// =========================================================

void ApplicationState::saveToFile(
    const std::string& filePath
) const {

    json data;


    // =====================================================
    // STUDENTS
    // =====================================================

    data["students"] =
        json::array();


    for (const auto& student :
        students) {

        json studentJson;

        studentJson["id"] =
            student->getId();

        studentJson["name"] =
            student->getName();

        studentJson["gpa"] =
            student->getGpa();

        studentJson["type"] =
            (
                student->getStudentType() ==
                StudentType::UNDERGRADUATE
                )
            ? "UNDERGRADUATE"
            : "GRADUATE";


        data["students"].push_back(
            studentJson
        );
    }


    // =====================================================
    // INSTRUCTORS
    // =====================================================

    data["instructors"] =
        json::array();


    for (const auto& instructor :
        instructors) {

        json instructorJson;

        instructorJson["id"] =
            instructor->getId();

        instructorJson["name"] =
            instructor->getName();

        instructorJson["courseIds"] =
            json::array();


        for (
            Course* course :
            instructor->getCourses()
            ) {
            if (course != nullptr) {
                instructorJson["courseIds"]
                    .push_back(
                        course->getId()
                    );
            }
        }


        data["instructors"].push_back(
            instructorJson
        );
    }


    // =====================================================
    // COURSES
    // =====================================================

    data["courses"] =
        json::array();


    for (const auto& course :
        courses) {

        json courseJson;

        courseJson["id"] =
            course->getId();

        courseJson["code"] =
            course->getCode();

        courseJson["name"] =
            course->getName();

        courseJson["credits"] =
            course->getCredits();

        courseJson["examCount"] =
            static_cast<int>(
                course->getExams().size()
                );


        if (
            dynamic_cast<
            const UndergraduateCourse*
            >(
                course.get()
                ) != nullptr
            ) {
            courseJson["type"] =
                "UNDERGRADUATE";
        }
        else {
            courseJson["type"] =
                "GRADUATE";
        }


        // ---------------------------------------------
        // Grading configurations
        // ---------------------------------------------

        courseJson[
            "gradingConfigurations"
        ] = json::array();


        const StudentType studentTypes[] = {
            StudentType::UNDERGRADUATE,
            StudentType::GRADUATE
        };


        for (
            const StudentType studentType :
        studentTypes
            ) {

            CourseGradingPolicy*
                gradingPolicy =
                course
                ->getGradingPolicy(
                    studentType
                );

            if (
                gradingPolicy == nullptr ||
                !gradingPolicy->hasStrategy()
                ) {
                continue;
            }


            const GradeCalculationStrategy*
                strategy =
                gradingPolicy
                ->getStrategy();


            json gradingJson;

            gradingJson["studentType"] =
                (
                    studentType ==
                    StudentType::UNDERGRADUATE
                    )
                ? "UNDERGRADUATE"
                : "GRADUATE";


            if (
                const auto*
                weightedStrategy =
                dynamic_cast<
                const WeightedAverageStrategy*
                >(strategy)
                ) {

                gradingJson["method"] =
                    "WEIGHTED_AVERAGE";

                gradingJson["weights"] =
                    json::array();


                for (
                    const auto& [
                        examId,
                        weight
                    ] :
                    weightedStrategy
                    ->getWeights()
                    ) {

                    gradingJson["weights"]
                        .push_back(
                            {
                                {
                                    "examId",
                                    examId
                                },
                                {
                                    "weight",
                                    weight
                                }
                            }
                        );
                }
            }
            else if (
                const auto*
                thresholdStrategy =
                dynamic_cast<
                const ThresholdStrategy*
                >(strategy)
                ) {

                gradingJson["method"] =
                    "THRESHOLD";

                gradingJson["threshold"] =
                    thresholdStrategy
                    ->getThreshold();

                gradingJson[
                    "thresholdExamIds"
                ] =
                    thresholdStrategy
                        ->getThresholdExamIds();
            }
            else {
                continue;
            }


            courseJson[
                "gradingConfigurations"
            ].push_back(
                gradingJson
            );
        }


        data["courses"].push_back(
            courseJson
        );
    }


    // =====================================================
    // ENROLLMENTS
    // =====================================================

    data["enrollments"] =
        json::array();


    for (const auto& enrollment :
        enrollments) {

        json enrollmentJson;

        enrollmentJson["id"] =
            enrollment->getId();

        enrollmentJson["studentId"] =
            enrollment
            ->getStudent()
            ->getId();

        enrollmentJson["courseId"] =
            enrollment
            ->getCourse()
            ->getId();

        enrollmentJson["examScores"] =
            json::array();


        for (
            const auto& examScore :
            enrollment->getExamScores()
            ) {

            enrollmentJson["examScores"]
                .push_back(
                    {
                        {
                            "examId",
                            examScore
                                .getExam()
                                ->getId()
                        },
                        {
                            "score",
                            examScore
                                .getScore()
                        }
                    }
                );
        }


        if (
            enrollment
            ->getFinalScore()
            .has_value()
            ) {
            enrollmentJson["finalScore"] =
                enrollment
                ->getFinalScore()
                .value();
        }
        else {
            enrollmentJson["finalScore"] =
                nullptr;
        }


        if (
            enrollment
            ->getLetterGrade()
            .has_value()
            ) {
            enrollmentJson["letterGrade"] =
                static_cast<int>(
                    enrollment
                    ->getLetterGrade()
                    .value()
                    );
        }
        else {
            enrollmentJson["letterGrade"] =
                nullptr;
        }


        data["enrollments"].push_back(
            enrollmentJson
        );
    }


    // =====================================================
    // WRITE FILE
    // =====================================================

    std::ofstream outputFile(
        filePath
    );

    if (!outputFile.is_open()) {
        throw std::runtime_error(
            "Could not open JSON file for writing."
        );
    }


    outputFile <<
        data.dump(4);
}