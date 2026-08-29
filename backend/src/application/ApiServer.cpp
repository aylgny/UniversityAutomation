#include "application/ApiServer.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "application/ApplicationState.h"
#include "application/RegistrationController.h"
#include "application/GradingController.h"

#include "domain/Student.h"
#include "domain/StudentType.h"
#include "domain/Course.h"
#include "domain/Instructor.h"
#include "domain/Enrollment.h"
#include "domain/Exam.h"
#include "domain/LetterGrade.h"
#include "domain/UndergraduateCourse.h"
#include "policies/CourseGradingPolicy.h"

#include "strategies/WeightedAverageStrategy.h"
#include "strategies/ThresholdStrategy.h"


using json = nlohmann::json;


namespace {

    const std::string DATA_FILE =
        "data.json";


    void addCorsHeaders(
        httplib::Response& response
    ) {
        response.set_header(
            "Access-Control-Allow-Origin",
            "*"
        );

        response.set_header(
            "Access-Control-Allow-Headers",
            "Content-Type"
        );

        response.set_header(
            "Access-Control-Allow-Methods",
            "GET, POST, PUT, OPTIONS"
        );
    }


    void sendJson(
        httplib::Response& response,
        const json& body,
        int status = 200
    ) {
        response.status = status;

        response.set_content(
            body.dump(),
            "application/json"
        );

        addCorsHeaders(
            response
        );
    }


    void sendError(
        httplib::Response& response,
        const std::string& message,
        int status
    ) {
        sendJson(
            response,
            {
                {"success", false},
                {"error", message}
            },
            status
        );
    }


    std::string studentTypeToString(
        StudentType type
    ) {
        if (
            type ==
            StudentType::UNDERGRADUATE
            ) {
            return "UNDERGRADUATE";
        }

        return "GRADUATE";
    }


    StudentType parseStudentType(
        const std::string& value
    ) {
        if (
            value ==
            "UNDERGRADUATE"
            ) {
            return StudentType::UNDERGRADUATE;
        }

        if (
            value ==
            "GRADUATE"
            ) {
            return StudentType::GRADUATE;
        }

        throw std::invalid_argument(
            "Unsupported student type."
        );
    }


    std::string letterGradeToString(
        LetterGrade grade
    ) {
        switch (grade) {

        case LetterGrade::AA:
            return "AA";

        case LetterGrade::BA:
            return "BA";

        case LetterGrade::BB:
            return "BB";

        case LetterGrade::CB:
            return "CB";

        case LetterGrade::CC:
            return "CC";

        case LetterGrade::DC:
            return "DC";

        case LetterGrade::DD:
            return "DD";

        case LetterGrade::F:
            return "F";
        }

        return "UNKNOWN";
    }


    json enrollmentToJson(
        const Enrollment& enrollment
    ) {
        json result;


        result["id"] =
            enrollment.getId();


        result["studentId"] =
            enrollment
            .getStudent()
            ->getId();


        result["courseId"] =
            enrollment
            .getCourse()
            ->getId();


        result["examScores"] =
            json::array();


        for (
            const auto& examScore :
            enrollment.getExamScores()
            ) {
            result["examScores"]
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
            .getFinalScore()
            .has_value()
            ) {
            result["finalScore"] =
                enrollment
                .getFinalScore()
                .value();
        }
        else {
            result["finalScore"] =
                nullptr;
        }


        if (
            enrollment
            .getLetterGrade()
            .has_value()
            ) {

            const LetterGrade grade =
                enrollment
                .getLetterGrade()
                .value();


            result["letterGrade"] =
                letterGradeToString(
                    grade
                );


            result["passed"] =
                enrollment
                .getCourse()
                ->isPassed(
                    grade
                );
        }
        else {

            result["letterGrade"] =
                nullptr;


            result["passed"] =
                nullptr;
        }


        return result;
    }

} // namespace


ApiServer::ApiServer(
    ApplicationState& state
)
    : state(state) {
}


void ApiServer::run(
    int port
) {
    httplib::Server server;

    RegistrationController
        registrationController;

    GradingController
        gradingController;


    // =====================================================
    // CORS
    // =====================================================

    server.Options(
        R"(.*)",
        [](
            const httplib::Request&,
            httplib::Response& response
            ) {
                addCorsHeaders(
                    response
                );

                response.status =
                    204;
        }
    );


    // =====================================================
    // HEALTH
    // =====================================================

    server.Get(
        "/api/health",
        [](
            const httplib::Request&,
            httplib::Response& response
            ) {
                sendJson(
                    response,
                    {
                        {"success", true},
                        {"status", "UP"}
                    }
                );
        }
    );


    // =====================================================
    // DEMO LOGIN
    // =====================================================

    server.Post(
        "/api/login",
        [this](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const json body =
                        json::parse(
                            request.body
                        );

                    const std::string username =
                        body.at("username")
                        .get<std::string>();

                    const std::string password =
                        body.at("password")
                        .get<std::string>();


                    /*
                     * Predefined credentials are used only for
                     * demonstration purposes.
                     *
                     * No production-level password storage,
                     * hashing, session management, or JWT
                     * authentication is implemented.
                     */


                     // -----------------------------------------
                     // Undergraduate student
                     // -----------------------------------------

                    if (
                        username == "undergrad" &&
                        password == "1234"
                        ) {
                        Student* student =
                            state.findStudentById(
                                1
                            );

                        if (
                            student ==
                            nullptr
                            ) {
                            sendError(
                                response,
                                "Demo undergraduate student was not found.",
                                500
                            );

                            return;
                        }


                        sendJson(
                            response,
                            {
                                {"success", true},
                                {
                                    "user",
                                    {
                                        {
                                            "id",
                                            student->getId()
                                        },
                                        {
                                            "name",
                                            student->getName()
                                        },
                                        {
                                            "role",
                                            "STUDENT"
                                        },
                                        {
                                            "studentType",
                                            studentTypeToString(
                                                student
                                                    ->getStudentType()
                                            )
                                        },
                                        {
                                            "gpa",
                                            student->getGpa()
                                        },
                                        {
                                            "maxCredits",
                                            student
                                                ->getMaxCredits()
                                                .value()
                                        }
                                    }
                                }
                            }
                        );

                        return;
                    }


                    // -----------------------------------------
                    // Graduate student
                    // -----------------------------------------

                    if (
                        username == "graduate" &&
                        password == "1234"
                        ) {
                        Student* student =
                            state.findStudentById(
                                2
                            );

                        if (
                            student ==
                            nullptr
                            ) {
                            sendError(
                                response,
                                "Demo graduate student was not found.",
                                500
                            );

                            return;
                        }


                        sendJson(
                            response,
                            {
                                {"success", true},
                                {
                                    "user",
                                    {
                                        {
                                            "id",
                                            student->getId()
                                        },
                                        {
                                            "name",
                                            student->getName()
                                        },
                                        {
                                            "role",
                                            "STUDENT"
                                        },
                                        {
                                            "studentType",
                                            studentTypeToString(
                                                student
                                                    ->getStudentType()
                                            )
                                        },
                                        {
                                            "gpa",
                                            student->getGpa()
                                        },
                                        {
                                            "maxCredits",
                                            student
                                                ->getMaxCredits()
                                                .value()
                                        }
                                    }
                                }
                            }
                        );

                        return;
                    }


                    // -----------------------------------------
                    // Instructor
                    // -----------------------------------------

                    if (
                        username == "instructor" &&
                        password == "1234"
                        ) {
                        Instructor* instructor =
                            state.findInstructorById(
                                1
                            );

                        if (
                            instructor ==
                            nullptr
                            ) {
                            sendError(
                                response,
                                "Demo instructor was not found.",
                                500
                            );

                            return;
                        }


                        sendJson(
                            response,
                            {
                                {"success", true},
                                {
                                    "user",
                                    {
                                        {
                                            "id",
                                            instructor->getId()
                                        },
                                        {
                                            "name",
                                            instructor->getName()
                                        },
                                        {
                                            "role",
                                            "INSTRUCTOR"
                                        }
                                    }
                                }
                            }
                        );

                        return;
                    }


                    // -----------------------------------------
                    // Invalid credentials
                    // -----------------------------------------

                    sendError(
                        response,
                        "Invalid username or password.",
                        401
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // GET STUDENTS
    // =====================================================

    server.Get(
        "/api/students",
        [this](
            const httplib::Request&,
            httplib::Response& response
            ) {
                json result =
                    json::array();


                for (
                    const auto& student :
                    state.getStudents()
                    ) {
                    result.push_back(
                        {
                            {
                                "id",
                                student->getId()
                            },
                            {
                                "name",
                                student->getName()
                            },
                            {
                                "gpa",
                                student->getGpa()
                            },
                            {
                                "studentType",
                                studentTypeToString(
                                    student
                                        ->getStudentType()
                                )
                            }
                        }
                    );
                }


                sendJson(
                    response,
                    result
                );
        }
    );


    // =====================================================
    // GET COURSES
    // =====================================================

    server.Get(
        "/api/courses",
        [this](
            const httplib::Request&,
            httplib::Response& response
            ) {
                json result =
                    json::array();


                for (
                    const auto& course :
                    state.getCourses()
                    ) {

                    const bool undergraduate =
                        dynamic_cast<
                        const UndergraduateCourse*
                        >(
                            course.get()
                            ) != nullptr;


                    result.push_back(
                        {
                            {
                                "id",
                                course->getId()
                            },
                            {
                                "code",
                                course->getCode()
                            },
                            {
                                "name",
                                course->getName()
                            },
                            {
                                "credits",
                                course->getCredits()
                            },
                            {
                                "type",
                                undergraduate
                                    ? "UNDERGRADUATE"
                                    : "GRADUATE"
                            },
                            {
                                "examCount",
                                course
                                    ->getExams()
                                    .size()
                            }
                        }
                    );
                }


                sendJson(
                    response,
                    result
                );
        }
    );


    // =====================================================
    // GET INSTRUCTORS
    // =====================================================

    server.Get(
        "/api/instructors",
        [this](
            const httplib::Request&,
            httplib::Response& response
            ) {
                json result =
                    json::array();


                for (
                    const auto& instructor :
                    state.getInstructors()
                    ) {

                    json courseIds =
                        json::array();


                    for (
                        Course* course :
                        instructor
                        ->getCourses()
                        ) {
                        if (
                            course !=
                            nullptr
                            ) {
                            courseIds.push_back(
                                course->getId()
                            );
                        }
                    }


                    result.push_back(
                        {
                            {
                                "id",
                                instructor->getId()
                            },
                            {
                                "name",
                                instructor->getName()
                            },
                            {
                                "courseIds",
                                courseIds
                            }
                        }
                    );
                }


                sendJson(
                    response,
                    result
                );
        }
    );


    // =====================================================
    // CREATE ENROLLMENT
    // =====================================================

    server.Post(
        "/api/enrollments",
        [this, &registrationController](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const json body =
                        json::parse(
                            request.body
                        );

                    const int studentId =
                        body.at("studentId")
                        .get<int>();

                    const int courseId =
                        body.at("courseId")
                        .get<int>();


                    Student* student =
                        state.findStudentById(
                            studentId
                        );

                    Course* course =
                        state.findCourseById(
                            courseId
                        );


                    if (
                        student ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Student not found.",
                            404
                        );

                        return;
                    }


                    if (
                        course ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Course not found.",
                            404
                        );

                        return;
                    }


                    Enrollment* enrollment =
                        registrationController
                        .enroll(
                            *student,
                            *course,
                            state.getEnrollments()
                        );


                    state.saveToFile(
                        DATA_FILE
                    );


                    sendJson(
                        response,
                        {
                            {"success", true},
                            {
                                "enrollment",
                                enrollmentToJson(
                                    *enrollment
                                )
                            }
                        },
                        201
                    );
                }
                catch (
                    const std::logic_error& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        409
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // STUDENT ENROLLMENTS
    // =====================================================

    server.Get(
        R"(/api/students/(\d+)/enrollments)",
        [this](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int studentId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );


                    Student* student =
                        state.findStudentById(
                            studentId
                        );


                    if (
                        student ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Student not found.",
                            404
                        );

                        return;
                    }


                    json result =
                        json::array();


                    for (
                        const auto& enrollment :
                        state.getEnrollments()
                        ) {
                        if (
                            enrollment
                            ->getStudent()
                            ->getId() ==
                            studentId
                            ) {
                            result.push_back(
                                enrollmentToJson(
                                    *enrollment
                                )
                            );
                        }
                    }


                    sendJson(
                        response,
                        result
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // INSTRUCTOR COURSES
    // =====================================================

    server.Get(
        R"(/api/instructors/(\d+)/courses)",
        [this](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int instructorId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );


                    Instructor* instructor =
                        state.findInstructorById(
                            instructorId
                        );


                    if (
                        instructor ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Instructor not found.",
                            404
                        );

                        return;
                    }


                    json result =
                        json::array();


                    for (
                        Course* course :
                        instructor
                        ->getCourses()
                        ) {
                        if (
                            course ==
                            nullptr
                            ) {
                            continue;
                        }


                        result.push_back(
                            {
                                {
                                    "id",
                                    course->getId()
                                },
                                {
                                    "code",
                                    course->getCode()
                                },
                                {
                                    "name",
                                    course->getName()
                                },
                                {
                                    "credits",
                                    course->getCredits()
                                },
                                {
                                    "examCount",
                                    course
                                        ->getExams()
                                        .size()
                                }
                            }
                        );
                    }


                    sendJson(
                        response,
                        result
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // CONFIGURE EXAMS
    // =====================================================

    server.Put(
        R"(/api/courses/(\d+)/exams)",
        [this, &gradingController](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int courseId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );

                    const json body =
                        json::parse(
                            request.body
                        );

                    const int examCount =
                        body.at("examCount")
                        .get<int>();


                    Course* course =
                        state.findCourseById(
                            courseId
                        );


                    if (
                        course ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Course not found.",
                            404
                        );

                        return;
                    }


                    const int currentExamCount =
                        static_cast<int>(
                            course
                            ->getExams()
                            .size()
                            );


                    /*
                     * Existing ExamScore objects keep non-owning
                     * pointers to the current Exam objects.
                     *
                     * Changing the exam structure after scores
                     * have been entered would invalidate those
                     * pointers, so reject that operation.
                     */
                    if (
                        currentExamCount !=
                        examCount
                        ) {
                        for (
                            const auto& enrollment :
                            state.getEnrollments()
                            ) {
                            if (
                                enrollment
                                ->getCourse() ==
                                course &&
                                !enrollment
                                ->getExamScores()
                                .empty()
                                ) {
                                sendError(
                                    response,
                                    "Exam count cannot be changed after exam scores have been entered.",
                                    409
                                );

                                return;
                            }
                        }
                    }


                    gradingController
                        .configureExams(
                            *course,
                            examCount
                        );


                    state.saveToFile(
                        DATA_FILE
                    );


                    sendJson(
                        response,
                        {
                            {"success", true},
                            {"courseId", courseId},
                            {"examCount", examCount}
                        }
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // CONFIGURE WEIGHTED AVERAGE
    // =====================================================

    server.Put(
        R"(/api/courses/(\d+)/grading/weighted)",
        [this, &gradingController](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int courseId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );

                    const json body =
                        json::parse(
                            request.body
                        );


                    const StudentType studentType =
                        parseStudentType(
                            body.at("studentType")
                            .get<std::string>()
                        );


                    std::map<int, double>
                        weights;


                    for (
                        const auto& weightJson :
                        body.at("weights")
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


                    Course* course =
                        state.findCourseById(
                            courseId
                        );


                    if (
                        course ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Course not found.",
                            404
                        );

                        return;
                    }


                    gradingController
                        .configureWeightedAverage(
                            *course,
                            studentType,
                            weights
                        );


                    state.saveToFile(
                        DATA_FILE
                    );


                    sendJson(
                        response,
                        {
                            {"success", true},
                            {"courseId", courseId},
                            {
                                "studentType",
                                studentTypeToString(
                                    studentType
                                )
                            },
                            {
                                "method",
                                "WEIGHTED_AVERAGE"
                            }
                        }
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );

    // =====================================================
    // GET GRADING CONFIGURATION
    // =====================================================

    server.Get(
        R"(/api/courses/(\d+)/grading)",
        [this](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int courseId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );


                    Course* course =
                        state.findCourseById(
                            courseId
                        );


                    if (
                        course ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Course not found.",
                            404
                        );

                        return;
                    }


                    json gradingByStudentType =
                        json::object();


                    const std::vector<StudentType>
                        studentTypes = {
                            StudentType::UNDERGRADUATE,
                            StudentType::GRADUATE
                    };


                    for (
                        StudentType studentType :
                    studentTypes
                        ) {
                        const CourseGradingPolicy*
                            policy =
                            course
                            ->getGradingPolicy(
                                studentType
                            );


                        if (
                            policy ==
                            nullptr ||
                            !policy
                            ->hasStrategy()
                            ) {
                            continue;
                        }


                        const GradeCalculationStrategy*
                            strategy =
                            policy
                            ->getStrategy();


                        if (
                            strategy ==
                            nullptr
                            ) {
                            continue;
                        }


                        json configuration;


                        const auto*
                            weightedStrategy =
                            dynamic_cast<
                            const WeightedAverageStrategy*
                            >(
                                strategy
                                );


                        if (
                            weightedStrategy !=
                            nullptr
                            ) {
                            configuration["method"] =
                                "weighted";


                            configuration["weights"] =
                                json::array();


                            for (
                                const auto& [
                                    examId,
                                    weight
                                ] :
                                weightedStrategy
                                ->getWeights()
                                ) {
                                configuration["weights"]
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
                        else {
                            const auto*
                                thresholdStrategy =
                                dynamic_cast<
                                const ThresholdStrategy*
                                >(
                                    strategy
                                    );


                            if (
                                thresholdStrategy !=
                                nullptr
                                ) {
                                configuration["method"] =
                                    "threshold";

                                configuration["threshold"] =
                                    thresholdStrategy
                                    ->getThreshold();

                                configuration[
                                    "thresholdExamIds"
                                ] =
                                    thresholdStrategy
                                        ->getThresholdExamIds();
                            }
                            else {
                                continue;
                            }
                        }


                        gradingByStudentType[
                            studentTypeToString(
                                studentType
                            )
                        ] =
                            configuration;
                    }


                    sendJson(
                        response,
                        {
                            {
                                "success",
                                true
                            },
                            {
                                "courseId",
                                courseId
                            },
                            {
                                "examCount",
                                course
                                    ->getExams()
                                    .size()
                            },
                            {
                                "gradingByStudentType",
                                gradingByStudentType
                            }
                        }
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );

    // =====================================================
    // CONFIGURE THRESHOLD
    // =====================================================

    server.Put(
        R"(/api/courses/(\d+)/grading/threshold)",
        [this, &gradingController](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int courseId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );

                    const json body =
                        json::parse(
                            request.body
                        );


                    const StudentType studentType =
                        parseStudentType(
                            body.at("studentType")
                            .get<std::string>()
                        );


                    const double threshold =
                        body.at("threshold")
                        .get<double>();


                    const std::vector<int>
                        thresholdExamIds =
                        body.at(
                            "thresholdExamIds"
                        )
                        .get<
                        std::vector<int>
                        >();


                    Course* course =
                        state.findCourseById(
                            courseId
                        );


                    if (
                        course ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Course not found.",
                            404
                        );

                        return;
                    }


                    gradingController
                        .configureThreshold(
                            *course,
                            studentType,
                            threshold,
                            thresholdExamIds
                        );


                    state.saveToFile(
                        DATA_FILE
                    );


                    sendJson(
                        response,
                        {
                            {"success", true},
                            {"courseId", courseId},
                            {
                                "studentType",
                                studentTypeToString(
                                    studentType
                                )
                            },
                            {
                                "method",
                                "THRESHOLD"
                            }
                        }
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // ENTER EXAM SCORE
    // =====================================================

    server.Put(
        R"(/api/enrollments/(\d+)/scores/(\d+))",
        [this, &gradingController](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int enrollmentId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );

                    const int examId =
                        std::stoi(
                            request.matches[2]
                            .str()
                        );

                    const json body =
                        json::parse(
                            request.body
                        );

                    const double score =
                        body.at("score")
                        .get<double>();


                    Enrollment* enrollment =
                        state.findEnrollmentById(
                            enrollmentId
                        );


                    if (
                        enrollment ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Enrollment not found.",
                            404
                        );

                        return;
                    }


                    gradingController
                        .enterExamScore(
                            *enrollment,
                            examId,
                            score
                        );


                    state.saveToFile(
                        DATA_FILE
                    );


                    sendJson(
                        response,
                        {
                            {"success", true},
                            {"enrollmentId", enrollmentId},
                            {"examId", examId},
                            {"score", score}
                        }
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // CALCULATE FINAL RESULT
    // =====================================================

    server.Post(
        R"(/api/enrollments/(\d+)/calculate)",
        [this, &gradingController](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int enrollmentId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );


                    Enrollment* enrollment =
                        state.findEnrollmentById(
                            enrollmentId
                        );


                    if (
                        enrollment ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Enrollment not found.",
                            404
                        );

                        return;
                    }


                    gradingController
                        .calculateFinalResult(
                            *enrollment
                        );


                    state.saveToFile(
                        DATA_FILE
                    );


                    const LetterGrade letterGrade =
                        enrollment
                        ->getLetterGrade()
                        .value();


                    const bool passed =
                        enrollment
                        ->getCourse()
                        ->isPassed(
                            letterGrade
                        );


                    sendJson(
                        response,
                        {
                            {"success", true},
                            {
                                "finalScore",
                                enrollment
                                    ->getFinalScore()
                                    .value()
                            },
                            {
                                "letterGrade",
                                letterGradeToString(
                                    letterGrade
                                )
                            },
                            {
                                "passed",
                                passed
                            }
                        }
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // GET ENROLLMENT
    // =====================================================

    server.Get(
        R"(/api/enrollments/(\d+))",
        [this](
            const httplib::Request& request,
            httplib::Response& response
            ) {
                try {
                    const int enrollmentId =
                        std::stoi(
                            request.matches[1]
                            .str()
                        );


                    Enrollment* enrollment =
                        state.findEnrollmentById(
                            enrollmentId
                        );


                    if (
                        enrollment ==
                        nullptr
                        ) {
                        sendError(
                            response,
                            "Enrollment not found.",
                            404
                        );

                        return;
                    }


                    sendJson(
                        response,
                        enrollmentToJson(
                            *enrollment
                        )
                    );
                }
                catch (
                    const std::exception& exception
                    ) {
                    sendError(
                        response,
                        exception.what(),
                        400
                    );
                }
        }
    );


    // =====================================================
    // START SERVER
    // =====================================================

    std::cout
        << "University Automation REST API running on port "
        << port
        << std::endl;


    server.listen(
        "0.0.0.0",
        port
    );
}