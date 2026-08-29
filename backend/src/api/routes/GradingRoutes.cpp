/*
 * Registers grading-related endpoints.
 * Handles exam setup, grading strategies, score entry and final results.
 * Grading operations are delegated to GradingController.
 */

#include "api/routes/GradingRoutes.h"

#include <map>
#include <vector>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "api/ApiUtils.h"

#include "application/ApplicationState.h"
#include "application/GradingController.h"

#include "domain/Course.h"
#include "domain/Enrollment.h"
#include "domain/StudentType.h"
#include "domain/LetterGrade.h"

#include "policies/CourseGradingPolicy.h"

#include "strategies/WeightedAverageStrategy.h"
#include "strategies/ThresholdStrategy.h"


namespace api {

    using json = nlohmann::json;


    void registerGradingRoutes(
        httplib::Server& server,
        ApplicationState& state,
        GradingController& gradingController,
        const std::string& dataFile
    ) {

        // =====================================================
        // CONFIGURE EXAMS
        // =====================================================

        server.Put(
            R"(/api/courses/(\d+)/exams)",
            [
                &state,
                &gradingController,
                &dataFile
            ](
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
                         * ExamScore keeps non-owning Exam pointers.
                         * Prevent exam changes after score entry.
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


                        // Delegate exam creation.
                        gradingController
                            .configureExams(
                                *course,
                                examCount
                            );


                        state.saveToFile(
                            dataFile
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
            [
                &state,
                &gradingController,
                &dataFile
            ](
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


                        // Convert JSON weights into a map.
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


                        // Configure weighted grading.
                        gradingController
                            .configureWeightedAverage(
                                *course,
                                studentType,
                                weights
                            );


                        state.saveToFile(
                            dataFile
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
            [&state](
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


                        // Serialize grading for each student type.
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


                            // Detect weighted strategy.
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
                                // Detect threshold strategy.
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
            [
                &state,
                &gradingController,
                &dataFile
            ](
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


                        // Configure threshold grading.
                        gradingController
                            .configureThreshold(
                                *course,
                                studentType,
                                threshold,
                                thresholdExamIds
                            );


                        state.saveToFile(
                            dataFile
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
            [
                &state,
                &gradingController,
                &dataFile
            ](
                const httplib::Request& request,
                httplib::Response& response
                ) {
                    try {
                        // Read IDs from the URL.
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


                        // Delegate score validation and update.
                        gradingController
                            .enterExamScore(
                                *enrollment,
                                examId,
                                score
                            );


                        state.saveToFile(
                            dataFile
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
            [
                &state,
                &gradingController,
                &dataFile
            ](
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


                        // Calculate final score and letter grade.
                        gradingController
                            .calculateFinalResult(
                                *enrollment
                            );


                        state.saveToFile(
                            dataFile
                        );


                        const LetterGrade letterGrade =
                            enrollment
                            ->getLetterGrade()
                            .value();


                        // Apply the course passing policy.
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
    }

}