import {
  useEffect,
  useState,
} from "react";

import "./App.css";

import LoginPage from "./pages/LoginPage";

import AvailableCourses from "./pages/AvailableCourses";
import EnrolledCourses from "./pages/EnrolledCourses";
import MyGrades from "./pages/MyGrades";

import TaughtCourses from "./pages/TaughtCourses";
import EnrolledStudents from "./pages/EnrolledStudents";
import ExamConfiguration from "./pages/ExamConfiguration";
import GradeManagement from "./pages/GradeManagement";

import {
  createEnrollment,
  getCourses,
  getGradingConfiguration,
  getInstructorCourses,
  getStudentEnrollments,
} from "./services/api";

import type {
  Course as ApiCourse,
  Enrollment,
  LoginUser,
} from "./services/api";

import type {
  Course,
} from "./types/Course";

import type {
  GradingConfigurations,
  GradingMethodConfiguration,
  StudentGroup,
} from "./types/GradingConfiguration";


// =========================================================
// PAGE TYPES
// =========================================================

type StudentPage =
  | "dashboard"
  | "available-courses"
  | "enrolled-courses"
  | "my-grades";


type InstructorPage =
  | "dashboard"
  | "taught-courses"
  | "enrolled-students"
  | "exam-configuration"
  | "grade-management";


// =========================================================
// API → FRONTEND COURSE MAPPING
// =========================================================

function mapCourse(
  course: ApiCourse
): Course {
  return {
    id: course.id,
    code: course.code,
    name: course.name,
    credits: course.credits,

    type:
      course.type === "GRADUATE"
        ? "Graduate"
        : "Undergraduate",

    examCount:
      course.examCount,
  };
}


// =========================================================
// APP
// =========================================================

function App() {

  // Currently logged-in predefined demo user.
  const [
    currentUser,
    setCurrentUser,
  ] =
    useState<LoginUser | null>(
      null
    );


  const [
    studentPage,
    setStudentPage,
  ] =
    useState<StudentPage>(
      "dashboard"
    );


  const [
    instructorPage,
    setInstructorPage,
  ] =
    useState<InstructorPage>(
      "dashboard"
    );


  const [
    selectedCourse,
    setSelectedCourse,
  ] =
    useState<Course | null>(
      null
    );


  // All courses returned by the backend.
  const [
    courses,
    setCourses,
  ] =
    useState<Course[]>([]);


  // Courses assigned to the logged-in instructor.
  const [
    taughtCourses,
    setTaughtCourses,
  ] =
    useState<Course[]>([]);


  // Enrollments belonging to the logged-in student.
  const [
    enrollments,
    setEnrollments,
  ] =
    useState<Enrollment[]>([]);


  // Frontend cache of grading configurations loaded from the backend.
  const [
    gradingConfigurations,
    setGradingConfigurations,
  ] =
    useState<GradingConfigurations>(
      {}
    );


  // =======================================================
  // LOAD COURSES
  // =======================================================

  useEffect(() => {

    const loadCourses =
      async () => {

        try {

          const response =
            await getCourses();

          setCourses(
            response.map(
              mapCourse
            )
          );

        }
        catch (error) {

          console.error(
            "Could not load courses:",
            error
          );
        }
      };


    loadCourses();

  }, []);


  // =======================================================
  // LOGIN COMPLETED
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

  const handleLoginCompleted =
    async (
      user: LoginUser
    ) => {

      setCurrentUser(
        user
      );


      // ---------------------------------------------------
      // Student data
      // ---------------------------------------------------

      if (
        user.role ===
        "STUDENT"
      ) {

        try {

          const studentEnrollments =
            await getStudentEnrollments(
              user.id
            );

          setEnrollments(
            studentEnrollments
          );

          setStudentPage(
            "dashboard"
          );

        }
        catch (error) {

          console.error(
            "Could not load student enrollments:",
            error
          );
        }

        return;
      }


      // ---------------------------------------------------
      // Instructor data
      // ---------------------------------------------------

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


        // Load persisted grading configurations
        // from the C++ backend.
        await loadGradingConfigurations(
          instructorCourses
        );


        setInstructorPage(
          "dashboard"
        );

      }
      catch (error) {

        console.error(
          "Could not load instructor courses:",
          error
        );
      }
    };


  // =======================================================
  // LOGOUT
  // =======================================================

  const handleLogout =
    () => {

      setCurrentUser(
        null
      );

      setEnrollments(
        []
      );

      setTaughtCourses(
        []
      );

      setSelectedCourse(
        null
      );

      setStudentPage(
        "dashboard"
      );

      setInstructorPage(
        "dashboard"
      );

      setGradingConfigurations(
        {}
      );
    };


  // =======================================================
  // STUDENT COURSE DATA
  // =======================================================

  const enrolledCourseIds =
    enrollments.map(
      (enrollment) =>
        enrollment.courseId
    );


  const enrolledCourses =
    courses.filter(
      (course) =>
        enrolledCourseIds.includes(
          course.id
        )
    );


  const availableCourses =
    courses.filter(
      (course) =>
        !enrolledCourseIds.includes(
          course.id
        )
    );


  const currentCredits =
    enrolledCourses.reduce(
      (
        total,
        course
      ) =>
        total +
        course.credits,
      0
    );


  // =======================================================
  // COURSE REGISTRATION
  // =======================================================

  const handleRegister =
    async (
      course: Course
    ): Promise<string> => {

      if (
        currentUser === null ||
        currentUser.role !==
        "STUDENT"
      ) {
        return (
          "Student login is required."
        );
      }


      try {

        const response =
          await createEnrollment(
            currentUser.id,
            course.id
          );


        // Update frontend state after successful backend registration.
        setEnrollments(
          (
            previousEnrollments
          ) => [
            ...previousEnrollments,
            response.enrollment,
          ]
        );


        return (
          `${course.code} registered successfully.`
        );

      }
      catch (error) {

        return (
          error instanceof Error
            ? error.message
            : "Registration failed."
        );
      }
    };


  // =======================================================
  // INSTRUCTOR COURSE SELECTION
  // =======================================================

  const handleInstructorCourseSelection =
    (
      course: Course
    ) => {

      setSelectedCourse(
        course
      );

      setInstructorPage(
        "enrolled-students"
      );
    };


  // =======================================================
  // UPDATE FRONTEND GRADING CACHE
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
  // LOGIN PAGE
  // =======================================================

  if (
    currentUser === null
  ) {

    return (
      <LoginPage
        onLogin={
          handleLoginCompleted
        }
      />
    );
  }

  const studentGpa =
    currentUser.role ===
    "STUDENT"
      ? currentUser.gpa
      : 0;


  const maxCredits =
    currentUser.role ===
    "STUDENT"
      ? currentUser.maxCredits
      : 0;

  // =======================================================
  // APPLICATION
  // =======================================================

  return (
    <div className="app">

      {/* ===================================================
          HEADER
          =================================================== */}

      <header className="app-header">

        <div>

          <h1>
            University Automation
          </h1>

          <p>
            {currentUser.name}
          </p>

        </div>


        <div className="header-user-actions">

          <div className="user-role-label">
            {currentUser.role ===
            "STUDENT"
              ? currentUser.studentType ===
                "GRADUATE"
                ? "Graduate Student"
                : "Undergraduate Student"
              : "Instructor"}
          </div>

          <button
            className="logout-button"
            onClick={
              handleLogout
            }
          >
            Logout
          </button>

        </div>

      </header>


      {/* ===================================================
          MAIN
          =================================================== */}

      <main className="dashboard">

        {/* =================================================
            STUDENT
            ================================================= */}

        {currentUser.role ===
        "STUDENT" ? (

          studentPage ===
          "available-courses" ? (

            <AvailableCourses
              courses={
                availableCourses
              }
              currentCredits={
                currentCredits
              }
              maxCredits={
                maxCredits
              }
              onRegister={
                handleRegister
              }
              onBack={() =>
                setStudentPage(
                  "dashboard"
                )
              }
            />

          ) : studentPage ===
            "enrolled-courses" ? (

            <EnrolledCourses
              courses={
                enrolledCourses
              }
              currentCredits={
                currentCredits
              }
              maxCredits={
                maxCredits
              }
              onBack={() =>
                setStudentPage(
                  "dashboard"
                )
              }
            />

          ) : studentPage ===
            "my-grades" ? (

            <MyGrades
              studentId={
                currentUser.id
              }
              onBack={() =>
                setStudentPage(
                  "dashboard"
                )
              }
            />

          ) : (

            <>
              {/* -------------------------------------------
                  STUDENT DASHBOARD
                  ------------------------------------------- */}

              <section className="welcome-card">

                <div>

                  <h2>
                    Student Dashboard
                  </h2>

                  <p>
                    View available courses,
                    manage enrollments,
                    and review your grades.
                  </p>

                </div>


                <div className="student-summary">

                  <div>

                    <span>
                      GPA
                    </span>

                    <strong>
                      {studentGpa.toFixed(
                        2
                      )}
                    </strong>

                  </div>


                  <div>

                    <span>
                      Current Credits
                    </span>

                    <strong>
                      {currentCredits}
                    </strong>

                  </div>


                  <div>

                    <span>
                      Maximum Credits
                    </span>

                    <strong>
                      {maxCredits}
                    </strong>

                  </div>

                </div>

              </section>


              <div className="card-grid">

                <section className="dashboard-card">

                  <h3>
                    Available Courses
                  </h3>

                  <p>
                    View courses available
                    for registration.
                  </p>

                  <button
                    onClick={() =>
                      setStudentPage(
                        "available-courses"
                      )
                    }
                  >
                    View Courses
                  </button>

                </section>


                <section className="dashboard-card">

                  <h3>
                    Enrolled Courses
                  </h3>

                  <p>
                    View your currently
                    registered courses.
                  </p>

                  <button
                    onClick={() =>
                      setStudentPage(
                        "enrolled-courses"
                      )
                    }
                  >
                    View Enrollments
                  </button>

                </section>


                <section className="dashboard-card">

                  <h3>
                    My Grades
                  </h3>

                  <p>
                    Review exam scores
                    and final letter grades.
                  </p>

                  <button
                    onClick={() =>
                      setStudentPage(
                        "my-grades"
                      )
                    }
                  >
                    View Grades
                  </button>

                </section>

              </div>
            </>

          )

        ) : (

          /* ===============================================
             INSTRUCTOR
             =============================================== */

          instructorPage ===
          "taught-courses" ? (

            <TaughtCourses
              courses={
                taughtCourses
              }
              onSelectCourse={
                handleInstructorCourseSelection
              }
              onBack={() =>
                setInstructorPage(
                  "dashboard"
                )
              }
            />

          ) : instructorPage ===
              "enrolled-students" &&
            selectedCourse ? (

            <EnrolledStudents
              course={
                selectedCourse
              }
              onBack={() =>
                setInstructorPage(
                  "taught-courses"
                )
              }
            />

          ) : instructorPage ===
            "exam-configuration" ? (

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
                setInstructorPage(
                  "dashboard"
                )
              }
            />

          ) : instructorPage ===
            "grade-management" ? (

            <GradeManagement
              courses={
                taughtCourses
              }
              configurations={
                gradingConfigurations
              }
              onBack={() =>
                setInstructorPage(
                  "dashboard"
                )
              }
            />

          ) : (

            <>
              {/* -------------------------------------------
                  INSTRUCTOR DASHBOARD
                  ------------------------------------------- */}

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
                      setInstructorPage(
                        "taught-courses"
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
                      setInstructorPage(
                        "exam-configuration"
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
                      setInstructorPage(
                        "grade-management"
                      )
                    }
                  >
                    Manage Grades
                  </button>

                </section>

              </div>
            </>

          )

        )}

      </main>

    </div>
  );
}


export default App;