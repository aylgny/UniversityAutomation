cmake_minimum_required(VERSION 3.16)

project(UniversityAutomation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# =========================================================
# EXTERNAL DEPENDENCIES
# =========================================================

include(FetchContent)


# ---------------------------------------------------------
# JSON
# ---------------------------------------------------------

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)


# ---------------------------------------------------------
# HTTP SERVER
# ---------------------------------------------------------

FetchContent_Declare(
    cpp_httplib
    URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.18.0.zip
)

FetchContent_MakeAvailable(cpp_httplib)


# =========================================================
# CORE BACKEND LIBRARY
# =========================================================

add_library(
    UniversityCore

    # Domain
    backend/src/domain/Person.cpp
    backend/src/domain/Student.cpp
    backend/src/domain/UndergraduateStudent.cpp
    backend/src/domain/GraduateStudent.cpp
    backend/src/domain/Instructor.cpp
    backend/src/domain/Course.cpp
    backend/src/domain/UndergraduateCourse.cpp
    backend/src/domain/GraduateCourse.cpp
    backend/src/domain/Exam.cpp
    backend/src/domain/ExamScore.cpp
    backend/src/domain/Enrollment.cpp

    # Policies
    backend/src/policies/UndergraduateCreditPolicy.cpp
    backend/src/policies/GraduateCreditPolicy.cpp
    backend/src/policies/UnlimitedCreditPolicy.cpp
    backend/src/policies/UndergraduateLetterGradePolicy.cpp
    backend/src/policies/GraduateLetterGradePolicy.cpp
    backend/src/policies/PassingPolicy.cpp
    backend/src/policies/CourseGradingPolicy.cpp

    # Strategies
    backend/src/strategies/WeightedAverageStrategy.cpp
    backend/src/strategies/ThresholdStrategy.cpp

    # Application
    backend/src/application/RegistrationController.cpp
    backend/src/application/GradingController.cpp
    backend/src/application/ApplicationState.cpp
    
    # API

    backend/src/api/ApiServer.cpp
    backend/src/api/ApiUtils.cpp

    backend/src/api/routes/SystemRoutes.cpp
    backend/src/api/routes/AuthRoutes.cpp
    backend/src/api/routes/StudentRoutes.cpp
    backend/src/api/routes/CourseRoutes.cpp
    backend/src/api/routes/InstructorRoutes.cpp
    backend/src/api/routes/EnrollmentRoutes.cpp
    backend/src/api/routes/GradingRoutes.cpp
)


target_include_directories(
    UniversityCore
    PUBLIC
    backend/include
)


target_link_libraries(
    UniversityCore
    PRIVATE
    nlohmann_json::nlohmann_json
    httplib::httplib
)


# =========================================================
# MAIN APPLICATION
# =========================================================

add_executable(
    UniversityAutomation
    backend/src/main.cpp
)


target_link_libraries(
    UniversityAutomation
    PRIVATE
    UniversityCore
)


# =========================================================
# TESTING
# =========================================================

include(CTest)


if(BUILD_TESTING)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    )

    set(
        gtest_force_shared_crt
        ON
        CACHE BOOL ""
        FORCE
    )

    FetchContent_MakeAvailable(googletest)


    add_executable(
        UniversityTests

        backend/tests/CreditPolicyTests.cpp
        backend/tests/LetterGradePolicyTests.cpp
        backend/tests/WeightedAverageStrategyTests.cpp
        backend/tests/ThresholdStrategyTests.cpp
        backend/tests/RegistrationControllerTests.cpp
        backend/tests/GradingControllerTests.cpp
        backend/tests/DomainValidationTests.cpp
        backend/tests/CoursePolicyTests.cpp
    )


    target_link_libraries(
        UniversityTests
        PRIVATE
        UniversityCore
        GTest::gtest_main
    )


    include(GoogleTest)


    gtest_discover_tests(
        UniversityTests
    )


    add_test(
        NAME UniversityTests
        COMMAND UniversityTests
    )

endif()