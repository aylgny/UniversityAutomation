#include "application/ApplicationState.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "application/GradingController.h"
#include "application/RegistrationController.h"

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

    // Reset the existing state before creating sample data.
    students.clear();
    courses.clear();
    instructors.clear();
    enrollments.clear();


    // =====================================================
    // STUDENTS
    // =====================================================

    /*
     * Student A accounts are used as demo users.
     * They intentionally start without any enrollments.
     */

     // Undergraduate demo student.
    students.push_back(
        std::make_unique<UndergraduateStudent>(
            1,
            "Lisans Öğrencisi A",
            3.80
        )
    );


    // Additional undergraduate students.
    students.push_back(
        std::make_unique<UndergraduateStudent>(
            2,
            "Lisans Öğrencisi B",
            2.70
        )
    );

    students.push_back(
        std::make_unique<UndergraduateStudent>(
            3,
            "Lisans Öğrencisi C",
            1.80
        )
    );


    // Graduate demo student.
    students.push_back(
        std::make_unique<GraduateStudent>(
            4,
            "Yüksek Lisans Öğrencisi A",
            3.60
        )
    );


    // Additional graduate students.
    students.push_back(
        std::make_unique<GraduateStudent>(
            5,
            "Yüksek Lisans Öğrencisi B",
            3.20
        )
    );

    students.push_back(
        std::make_unique<GraduateStudent>(
            6,
            "Yüksek Lisans Öğrencisi C",
            2.70
        )
    );


    // Keep non-owning pointers for configuring relationships.
    Student* undergraduateA =
        students[0].get();

    Student* undergraduateB =
        students[1].get();

    Student* undergraduateC =
        students[2].get();

    Student* graduateA =
        students[3].get();

    Student* graduateB =
        students[4].get();

    Student* graduateC =
        students[5].get();


    // Demo users intentionally start without enrollments.
    (void)undergraduateA;
    (void)graduateA;


    // =====================================================
    // INSTRUCTORS
    // =====================================================

    instructors.push_back(
        std::make_unique<Instructor>(
            1,
            "Öğretim Üyesi A"
        )
    );

    instructors.push_back(
        std::make_unique<Instructor>(
            2,
            "Öğretim Üyesi B"
        )
    );


    Instructor* instructorA =
        instructors[0].get();

    Instructor* instructorB =
        instructors[1].get();


    // =====================================================
    // COURSES
    // =====================================================

    /*
     * Courses intentionally start without exams or grading
     * strategies so they can be configured during the demo.
     */


     // -----------------------------------------------------
     // Undergraduate Courses
     // -----------------------------------------------------

    auto cs101 =
        std::make_unique<UndergraduateCourse>(
            101,
            "CS101",
            "Programlamaya Giriş",
            5
        );

    Course* cs101Ptr =
        cs101.get();

    courses.push_back(
        std::move(cs101)
    );


    auto cs201 =
        std::make_unique<UndergraduateCourse>(
            102,
            "CS201",
            "Veri Yapıları",
            5
        );

    Course* cs201Ptr =
        cs201.get();

    courses.push_back(
        std::move(cs201)
    );


    auto cs301 =
        std::make_unique<UndergraduateCourse>(
            103,
            "CS301",
            "Yazılım Mühendisliği",
            5
        );

    Course* cs301Ptr =
        cs301.get();

    courses.push_back(
        std::move(cs301)
    );


    // -----------------------------------------------------
    // Graduate Courses
    // -----------------------------------------------------

    auto cs501 =
        std::make_unique<GraduateCourse>(
            201,
            "CS501",
            "İleri Yazılım Mühendisliği",
            5
        );

    Course* cs501Ptr =
        cs501.get();

    courses.push_back(
        std::move(cs501)
    );


    auto cs502 =
        std::make_unique<GraduateCourse>(
            202,
            "CS502",
            "İleri Algoritmalar",
            4
        );

    Course* cs502Ptr =
        cs502.get();

    courses.push_back(
        std::move(cs502)
    );


    auto cs503 =
        std::make_unique<GraduateCourse>(
            203,
            "CS503",
            "Araştırma Yöntemleri",
            3
        );

    Course* cs503Ptr =
        cs503.get();

    courses.push_back(
        std::move(cs503)
    );


    // =====================================================
    // INSTRUCTOR - COURSE RELATIONSHIPS
    // =====================================================

    /*
     * Instructor A is the demo instructor and is assigned
     * both undergraduate and graduate courses.
     */

    instructorA->addCourse(
        cs101Ptr
    );

    instructorA->addCourse(
        cs301Ptr
    );

    instructorA->addCourse(
        cs501Ptr
    );

    instructorA->addCourse(
        cs503Ptr
    );


    instructorB->addCourse(
        cs201Ptr
    );

    instructorB->addCourse(
        cs502Ptr
    );


    // =====================================================
    // ENROLLMENTS
    // =====================================================

    /*
     * Demo students A intentionally have no enrollments.
     * Other students are pre-enrolled so instructor pages
     * still contain representative application data.
     *
     * CS301 contains both undergraduate and graduate
     * students to demonstrate student-type-specific
     * grading strategies within the same course.
     */

    RegistrationController registrationController;


    // -----------------------------------------------------
    // Undergraduate Student B
    // GPA: 2.70 -> Maximum credits: 20
    // Current credits: 10
    // -----------------------------------------------------

    registrationController.enroll(
        *undergraduateB,
        *cs101Ptr,
        enrollments
    );

    registrationController.enroll(
        *undergraduateB,
        *cs301Ptr,
        enrollments
    );


    // -----------------------------------------------------
    // Undergraduate Student C
    // GPA: 1.80 -> Maximum credits: 15
    // Current credits: 5
    // -----------------------------------------------------

    registrationController.enroll(
        *undergraduateC,
        *cs201Ptr,
        enrollments
    );


    // -----------------------------------------------------
    // Graduate Student B
    // GPA: 3.20 -> Maximum credits: 10
    // Current credits: 10
    // -----------------------------------------------------

    registrationController.enroll(
        *graduateB,
        *cs301Ptr,
        enrollments
    );

    registrationController.enroll(
        *graduateB,
        *cs501Ptr,
        enrollments
    );


    // -----------------------------------------------------
    // Graduate Student C
    // GPA: 2.70 -> Maximum credits: 6
    // Current credits: 4
    // -----------------------------------------------------

    registrationController.enroll(
        *graduateC,
        *cs502Ptr,
        enrollments
    );


    /*
     * No grading configuration, exam scores or final results
     * are created here. These operations are demonstrated
     * interactively through the application.
     */
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
) {
    for (auto& student : students) {
        if (student->getId() == id) {
            return student.get();
        }
    }

    return nullptr;
}


const Student* ApplicationState::findStudentById(
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
) {
    for (auto& course : courses) {
        if (course->getId() == id) {
            return course.get();
        }
    }

    return nullptr;
}


const Course* ApplicationState::findCourseById(
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
) {
    for (auto& instructor : instructors) {
        if (instructor->getId() == id) {
            return instructor.get();
        }
    }

    return nullptr;
}


const Instructor* ApplicationState::findInstructorById(
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
) {
    for (auto& enrollment : enrollments) {
        if (enrollment->getId() == id) {
            return enrollment.get();
        }
    }

    return nullptr;
}


const Enrollment* ApplicationState::findEnrollmentById(
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

    // Clear the current state before reconstructing it from JSON.
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

        // Restore object relationships using persisted identifiers.
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

            const CourseGradingPolicy*
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


            // Serialize strategy-specific configuration according to its concrete type.
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
