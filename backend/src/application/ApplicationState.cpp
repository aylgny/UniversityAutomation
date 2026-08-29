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

    students.clear();
    courses.clear();
    instructors.clear();
    enrollments.clear();


    // =====================================================
    // STUDENTS
    // =====================================================

    // Undergraduate students with different GPA levels.
    students.push_back(
        std::make_unique<UndergraduateStudent>(
            1,
            "Lisans Öğrencisi A",
            3.60
        )
    );

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


    // Graduate students with different GPA levels.
    students.push_back(
        std::make_unique<GraduateStudent>(
            4,
            "Yüksek Lisans Öğrencisi A",
            3.80
        )
    );

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

    cs101->createExams(
        2
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

    cs201->createExams(
        3
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

    cs301->createExams(
        2
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

    cs501->createExams(
        2
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

    cs502->createExams(
        3
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

    cs503->createExams(
        2
    );

    Course* cs503Ptr =
        cs503.get();

    courses.push_back(
        std::move(cs503)
    );


    // =====================================================
    // INSTRUCTOR - COURSE RELATIONSHIPS
    // =====================================================

    instructorA->addCourse(
        cs101Ptr
    );

    instructorA->addCourse(
        cs301Ptr
    );

    instructorA->addCourse(
        cs501Ptr
    );


    instructorB->addCourse(
        cs201Ptr
    );

    instructorB->addCourse(
        cs502Ptr
    );

    instructorB->addCourse(
        cs503Ptr
    );


    // =====================================================
    // GRADING CONFIGURATIONS
    // =====================================================

    GradingController gradingController;


    // -----------------------------------------------------
    // CS101
    // Undergraduate - Weighted Average
    // -----------------------------------------------------

    gradingController.configureWeightedAverage(
        *cs101Ptr,
        StudentType::UNDERGRADUATE,
        {
            {1, 0.40},
            {2, 0.60}
        }
    );


    // -----------------------------------------------------
    // CS201
    // Undergraduate - Threshold
    // Exam 1 is the threshold exam.
    // -----------------------------------------------------

    gradingController.configureThreshold(
        *cs201Ptr,
        StudentType::UNDERGRADUATE,
        50.0,
        { 1 }
    );


    // -----------------------------------------------------
    // CS301
    // Same course uses different grading strategies
    // depending on StudentType.
    // -----------------------------------------------------

    gradingController.configureWeightedAverage(
        *cs301Ptr,
        StudentType::UNDERGRADUATE,
        {
            {1, 0.30},
            {2, 0.70}
        }
    );

    gradingController.configureThreshold(
        *cs301Ptr,
        StudentType::GRADUATE,
        60.0,
        { 1 }
    );


    // -----------------------------------------------------
    // CS501
    // Graduate - Weighted Average
    // -----------------------------------------------------

    gradingController.configureWeightedAverage(
        *cs501Ptr,
        StudentType::GRADUATE,
        {
            {1, 0.50},
            {2, 0.50}
        }
    );


    // -----------------------------------------------------
    // CS502
    // Graduate - Threshold
    // -----------------------------------------------------

    gradingController.configureThreshold(
        *cs502Ptr,
        StudentType::GRADUATE,
        50.0,
        { 1 }
    );


    // -----------------------------------------------------
    // CS503
    // Graduate - Weighted Average
    // -----------------------------------------------------

    gradingController.configureWeightedAverage(
        *cs503Ptr,
        StudentType::GRADUATE,
        {
            {1, 0.40},
            {2, 0.60}
        }
    );


    // =====================================================
    // ENROLLMENTS
    // =====================================================

    RegistrationController registrationController;


    // -----------------------------------------------------
    // Undergraduate Student A
    // -----------------------------------------------------

    Enrollment* undergraduateACs101 =
        registrationController.enroll(
            *undergraduateA,
            *cs101Ptr,
            enrollments
        );

    Enrollment* undergraduateACs301 =
        registrationController.enroll(
            *undergraduateA,
            *cs301Ptr,
            enrollments
        );


    // -----------------------------------------------------
    // Undergraduate Student B
    // -----------------------------------------------------

    Enrollment* undergraduateBCs101 =
        registrationController.enroll(
            *undergraduateB,
            *cs101Ptr,
            enrollments
        );

    Enrollment* undergraduateBCs201 =
        registrationController.enroll(
            *undergraduateB,
            *cs201Ptr,
            enrollments
        );


    // -----------------------------------------------------
    // Undergraduate Student C
    // -----------------------------------------------------

    Enrollment* undergraduateCCs201 =
        registrationController.enroll(
            *undergraduateC,
            *cs201Ptr,
            enrollments
        );


    // -----------------------------------------------------
    // Graduate Student A
    //
    // This student is also enrolled in CS301.
    // It demonstrates a different grading strategy
    // for a different StudentType in the same course.
    // -----------------------------------------------------

    Enrollment* graduateACs501 =
        registrationController.enroll(
            *graduateA,
            *cs501Ptr,
            enrollments
        );

    Enrollment* graduateACs502 =
        registrationController.enroll(
            *graduateA,
            *cs502Ptr,
            enrollments
        );

    Enrollment* graduateACs301 =
        registrationController.enroll(
            *graduateA,
            *cs301Ptr,
            enrollments
        );


    // -----------------------------------------------------
    // Graduate Student B
    // -----------------------------------------------------

    Enrollment* graduateBCs501 =
        registrationController.enroll(
            *graduateB,
            *cs501Ptr,
            enrollments
        );

    Enrollment* graduateBCs503 =
        registrationController.enroll(
            *graduateB,
            *cs503Ptr,
            enrollments
        );


    // -----------------------------------------------------
    // Graduate Student C
    // -----------------------------------------------------

    Enrollment* graduateCCs503 =
        registrationController.enroll(
            *graduateC,
            *cs503Ptr,
            enrollments
        );


    // =====================================================
    // EXAM SCORE DEMO DATA
    // =====================================================


    // -----------------------------------------------------
    // Undergraduate Student A - CS101
    // Complete scores and calculated final result.
    // -----------------------------------------------------

    gradingController.enterExamScore(
        *undergraduateACs101,
        1,
        80.0
    );

    gradingController.enterExamScore(
        *undergraduateACs101,
        2,
        90.0
    );

    gradingController.calculateFinalResult(
        *undergraduateACs101
    );


    // -----------------------------------------------------
    // Undergraduate Student A - CS301
    // Partial score entry.
    // Final result is intentionally not calculated.
    // -----------------------------------------------------

    gradingController.enterExamScore(
        *undergraduateACs301,
        1,
        75.0
    );


    // -----------------------------------------------------
    // Undergraduate Student B - CS101
    // No scores entered yet.
    // -----------------------------------------------------

    (void)undergraduateBCs101;


    // -----------------------------------------------------
    // Undergraduate Student B - CS201
    // Complete threshold example.
    // -----------------------------------------------------

    gradingController.enterExamScore(
        *undergraduateBCs201,
        1,
        60.0
    );

    gradingController.enterExamScore(
        *undergraduateBCs201,
        2,
        70.0
    );

    gradingController.enterExamScore(
        *undergraduateBCs201,
        3,
        80.0
    );

    gradingController.calculateFinalResult(
        *undergraduateBCs201
    );


    // -----------------------------------------------------
    // Undergraduate Student C - CS201
    // Partial score entry.
    // Threshold exam is currently below the threshold.
    // -----------------------------------------------------

    gradingController.enterExamScore(
        *undergraduateCCs201,
        1,
        45.0
    );


    // -----------------------------------------------------
    // Graduate Student A - CS501
    // Complete weighted-average example.
    // -----------------------------------------------------

    gradingController.enterExamScore(
        *graduateACs501,
        1,
        88.0
    );

    gradingController.enterExamScore(
        *graduateACs501,
        2,
        92.0
    );

    gradingController.calculateFinalResult(
        *graduateACs501
    );


    // -----------------------------------------------------
    // Graduate Student A - CS502
    // Partial score entry.
    // -----------------------------------------------------

    gradingController.enterExamScore(
        *graduateACs502,
        1,
        70.0
    );

    gradingController.enterExamScore(
        *graduateACs502,
        2,
        85.0
    );


    // -----------------------------------------------------
    // Graduate Student A - CS301
    //
    // Same course as Undergraduate Student A,
    // but the graduate-specific ThresholdStrategy is used.
    // -----------------------------------------------------

    gradingController.enterExamScore(
        *graduateACs301,
        1,
        65.0
    );

    gradingController.enterExamScore(
        *graduateACs301,
        2,
        90.0
    );

    gradingController.calculateFinalResult(
        *graduateACs301
    );


    // -----------------------------------------------------
    // Graduate Student B - CS501
    // No scores entered yet.
    // -----------------------------------------------------

    (void)graduateBCs501;


    // -----------------------------------------------------
    // Graduate Student B - CS503
    // Complete weighted-average example.
    // -----------------------------------------------------

    gradingController.enterExamScore(
        *graduateBCs503,
        1,
        75.0
    );

    gradingController.enterExamScore(
        *graduateBCs503,
        2,
        85.0
    );

    gradingController.calculateFinalResult(
        *graduateBCs503
    );


    // -----------------------------------------------------
    // Graduate Student C - CS503
    // No scores entered yet.
    // -----------------------------------------------------

    (void)graduateCCs503;
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

//ApplicationState uygulamadaki ana domain nesnelerinin ownership'ini tutuyor.
//Verileri unique_ptr ile yönetiyor, ID'ler üzerinden nesneler arası ilişkileri
//kuruyor ve uygulama state'ini JSON formatında serialize/deserialize ediyor.