/*
 * Manages the instructor side of the application.
 * Loads instructor data and defines instructor routes.
 */

import {
  useEffect,
  useState,
} from "react";

import {
  Navigate,
  Route,
  Routes,
  useNavigate,
  useParams,
} from "react-router";

import TaughtCourses from
  "../TaughtCourses";

import EnrolledStudents from
  "../EnrolledStudents";

import ExamConfiguration from
  "../ExamConfiguration";

import GradeManagement from
  "../GradeManagement";

import {
  getGradingConfiguration,
  getInstructorCourses,
} from "../../services/api";

import type {
  Course as ApiCourse,
  LoginUser,
} from "../../services/api";

import type {
  Course,
} from "../../types/Course";

import type {
  GradingConfigurations,
  GradingMethodConfiguration,
  StudentGroup,
} from "../../types/GradingConfiguration";

import {
  mapCourse,
} from "../../utils/courseMapper";


// Instructor-only version of LoginUser.
type InstructorUser =
  Extract<
    LoginUser,
    { role: "INSTRUCTOR" }
  >;


interface InstructorDashboardProps {
  user: InstructorUser;

  onLogout: () => void;
}


interface EnrolledStudentsRouteProps {
  courses: Course[];
}


function EnrolledStudentsRoute({
  courses,
}: EnrolledStudentsRouteProps) {

  const navigate =
    useNavigate();


  const {
    courseId,
  } =
    useParams();


  const selectedCourse =
    courses.find(
      (course) =>
        course.id ===
        Number(courseId)
    );


  if (
    selectedCourse ===
    undefined
  ) {

    return (
      <Navigate
        to="/instructor/taught-courses"
        replace
      />
    );
  }


  return (
    <EnrolledStudents
      course={
        selectedCourse
      }
      onBack={() =>
        navigate(
          "/instructor/taught-courses"
        )
      }
    />
  );
}


function InstructorDashboard({
  user,
  onLogout,
}: InstructorDashboardProps) {

  const navigate =
    useNavigate();


  // Courses assigned to the logged-in instructor.
  const [
    taughtCourses,
    setTaughtCourses,
  ] =
    useState<Course[]>([]);


  // Frontend cache of grading configurations.
  const [
    gradingConfigurations,
    setGradingConfigurations,
  ] =
    useState<GradingConfigurations>(
      {}
    );


  // =======================================================
  // LOAD GRADING CONFIGURATIONS
  // =======================================================

  const loadGradingConfigurations =
    async (
      instructorCourses:
        ApiCourse[]
    ) => {

      const loadedConfigurations:
        GradingConfigurations =
          {};


      for (
        const course of
        instructorCourses
      ) {

        const response =
          await getGradingConfiguration(
            course.id
          );


        const gradingByStudentType: {
          Undergraduate?:
            GradingMethodConfiguration;

          Graduate?:
            GradingMethodConfiguration;
        } = {};


        const undergraduate =
          response
            .gradingByStudentType
            .UNDERGRADUATE;


        if (
          undergraduate
        ) {

          gradingByStudentType
            .Undergraduate =
              undergraduate.method ===
              "weighted"
                ? {
                    method:
                      "weighted",

                    weights:
                      [...undergraduate.weights]
                        .sort(
                          (a, b) =>
                            a.examId -
                            b.examId
                        )
                        .map(
                          (item) =>
                            item.weight
                        ),
                  }
                : {
                    method:
                      "threshold",

                    threshold:
                      undergraduate
                        .threshold,

                    thresholdExamIds:
                      undergraduate
                        .thresholdExamIds,
                  };
        }


        const graduate =
          response
            .gradingByStudentType
            .GRADUATE;


        if (
          graduate
        ) {

          gradingByStudentType
            .Graduate =
              graduate.method ===
              "weighted"
                ? {
                    method:
                      "weighted",

                    weights:
                      [...graduate.weights]
                        .sort(
                          (a, b) =>
                            a.examId -
                            b.examId
                        )
                        .map(
                          (item) =>
                            item.weight
                        ),
                  }
                : {
                    method:
                      "threshold",

                    threshold:
                      graduate.threshold,

                    thresholdExamIds:
                      graduate
                        .thresholdExamIds,
                  };
        }


        loadedConfigurations[
          course.id
        ] = {
          examCount:
            response.examCount,

          gradingByStudentType,
        };
      }


      setGradingConfigurations(
        loadedConfigurations
      );
    };


  // =======================================================
  // LOAD INSTRUCTOR DATA
  // =======================================================

  useEffect(() => {

    const loadInstructorData =
      async () => {

        try {

          const instructorCourses =
            await getInstructorCourses(
              user.id
            );


          setTaughtCourses(
            instructorCourses.map(
              mapCourse
            )
          );


          await loadGradingConfigurations(
            instructorCourses
          );

        }
        catch (error) {

          console.error(
            "Could not load instructor data:",
            error
          );
        }
      };


    loadInstructorData();

  }, [user.id]);


  // =======================================================
  // COURSE SELECTION
  // =======================================================

  const handleCourseSelection =
    (
      course: Course
    ) => {

      navigate(
        `/instructor/enrolled-students/${course.id}`
      );
    };


  // =======================================================
  // UPDATE GRADING CACHE
  // =======================================================

  const handleSaveGradingConfiguration =
    (
      courseId: number,
      examCount: number,
      studentType:
        StudentGroup,
      configuration:
        GradingMethodConfiguration
    ) => {

      setGradingConfigurations(
        (
          previousConfigurations
        ) => {

          const previousCourseConfiguration =
            previousConfigurations[
              courseId
            ] ?? {
              examCount,

              gradingByStudentType:
                {},
            };


          return {
            ...previousConfigurations,

            [courseId]: {
              ...previousCourseConfiguration,

              examCount,

              gradingByStudentType: {
                ...previousCourseConfiguration
                  .gradingByStudentType,

                [studentType]:
                  configuration,
              },
            },
          };
        }
      );
    };


  // =======================================================
  // LOGOUT
  // =======================================================

  const handleLogoutClick =
    () => {

      onLogout();

      navigate(
        "/login",
        {
          replace: true,
        }
      );
    };


  // =======================================================
  // APPLICATION
  // =======================================================

  return (
    <div className="app">

      {/* Header */}
      <header className="app-header">

        <div>

          <h1>
            University Automation
          </h1>

          <p>
            {user.name}
          </p>

        </div>


        <div className="header-user-actions">

          <div className="user-role-label">
            Instructor
          </div>


          <button
            className="logout-button"
            onClick={
              handleLogoutClick
            }
          >
            Logout
          </button>

        </div>

      </header>


      <main className="dashboard">

        <Routes>

          {/* Instructor dashboard */}
          <Route
            index
            element={
              <>

                <section className="welcome-card">

                  <div>

                    <h2>
                      Instructor Dashboard
                    </h2>

                    <p>
                      Manage courses,
                      configure grading methods,
                      and enter exam scores.
                    </p>

                  </div>

                </section>


                <div className="card-grid">

                  <section className="dashboard-card">

                    <h3>
                      Taught Courses
                    </h3>

                    <p>
                      View the courses
                      assigned to you.
                    </p>

                    <button
                      onClick={() =>
                        navigate(
                          "/instructor/taught-courses"
                        )
                      }
                    >
                      View Courses
                    </button>

                  </section>


                  <section className="dashboard-card">

                    <h3>
                      Exam Configuration
                    </h3>

                    <p>
                      Define exams and configure
                      grading methods.
                    </p>

                    <button
                      onClick={() =>
                        navigate(
                          "/instructor/exam-configuration"
                        )
                      }
                    >
                      Configure
                    </button>

                  </section>


                  <section className="dashboard-card">

                    <h3>
                      Grade Management
                    </h3>

                    <p>
                      Enter exam scores and
                      calculate final grades.
                    </p>

                    <button
                      onClick={() =>
                        navigate(
                          "/instructor/grade-management"
                        )
                      }
                    >
                      Manage Grades
                    </button>

                  </section>

                </div>

              </>
            }
          />


          {/* Taught courses */}
          <Route
            path="taught-courses"
            element={
              <TaughtCourses
                courses={
                  taughtCourses
                }
                onSelectCourse={
                  handleCourseSelection
                }
                onBack={() =>
                  navigate(
                    "/instructor"
                  )
                }
              />
            }
          />


          {/* Students enrolled in a course */}
          <Route
            path="enrolled-students/:courseId"
            element={
              <EnrolledStudentsRoute
                courses={
                  taughtCourses
                }
              />
            }
          />


          {/* Exam configuration */}
          <Route
            path="exam-configuration"
            element={
              <ExamConfiguration
                courses={
                  taughtCourses
                }
                configurations={
                  gradingConfigurations
                }
                onSaveConfiguration={
                  handleSaveGradingConfiguration
                }
                onBack={() =>
                  navigate(
                    "/instructor"
                  )
                }
              />
            }
          />


          {/* Grade management */}
          <Route
            path="grade-management"
            element={
              <GradeManagement
                courses={
                  taughtCourses
                }
                configurations={
                  gradingConfigurations
                }
                onBack={() =>
                  navigate(
                    "/instructor"
                  )
                }
              />
            }
          />


          {/* Unknown instructor route */}
          <Route
            path="*"
            element={
              <Navigate
                to="/instructor"
                replace
              />
            }
          />

        </Routes>

      </main>

    </div>
  );
}


export default InstructorDashboard;