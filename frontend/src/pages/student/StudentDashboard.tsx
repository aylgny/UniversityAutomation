/*
 * Manages the student side of the application.
 * Loads student data and defines student routes.
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
} from "react-router";

import AvailableCourses from
  "../AvailableCourses";

import EnrolledCourses from
  "../EnrolledCourses";

import MyGrades from
  "../MyGrades";

import {
  createEnrollment,
  getCourses,
  getStudentEnrollments,
} from "../../services/api";

import type {
  Enrollment,
  LoginUser,
} from "../../services/api";

import type {
  Course,
} from "../../types/Course";

import {
  mapCourse,
} from "../../utils/courseMapper";


// Student-only version of LoginUser.
type StudentUser =
  Extract<
    LoginUser,
    { role: "STUDENT" }
  >;


interface StudentDashboardProps {
  user: StudentUser;

  onLogout: () => void;
}


function StudentDashboard({
  user,
  onLogout,
}: StudentDashboardProps) {

  const navigate =
    useNavigate();


  // All courses returned by the backend.
  const [
    courses,
    setCourses,
  ] =
    useState<Course[]>([]);


  // Enrollments belonging to this student.
  const [
    enrollments,
    setEnrollments,
  ] =
    useState<Enrollment[]>([]);


  // =======================================================
  // LOAD STUDENT DATA
  // =======================================================

  useEffect(() => {

    const loadStudentData =
      async () => {

        try {

          // Load courses and enrollments together.
          const [
            courseResponse,
            enrollmentResponse,
          ] =
            await Promise.all([
              getCourses(),

              getStudentEnrollments(
                user.id
              ),
            ]);


          setCourses(
            courseResponse.map(
              mapCourse
            )
          );


          setEnrollments(
            enrollmentResponse
          );

        }
        catch (error) {

          console.error(
            "Could not load student data:",
            error
          );
        }
      };


    loadStudentData();

  }, [user.id]);


  // =======================================================
  // COURSE DATA
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

      try {

        // Backend validates enrollment rules.
        const response =
          await createEnrollment(
            user.id,
            course.id
          );


        // Update local state after registration.
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

            {user.studentType ===
            "GRADUATE"
              ? "Graduate Student"
              : "Undergraduate Student"}

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

          {/* Student dashboard */}
          <Route
            index
            element={
              <>

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
                        {user.gpa.toFixed(
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
                        {user.maxCredits}
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
                        navigate(
                          "/student/available-courses"
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
                        navigate(
                          "/student/enrolled-courses"
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
                        navigate(
                          "/student/my-grades"
                        )
                      }
                    >
                      View Grades
                    </button>

                  </section>

                </div>

              </>
            }
          />


          {/* Available courses */}
          <Route
            path="available-courses"
            element={
              <AvailableCourses
                courses={
                  availableCourses
                }
                currentCredits={
                  currentCredits
                }
                maxCredits={
                  user.maxCredits
                }
                onRegister={
                  handleRegister
                }
                onBack={() =>
                  navigate(
                    "/student"
                  )
                }
              />
            }
          />


          {/* Enrolled courses */}
          <Route
            path="enrolled-courses"
            element={
              <EnrolledCourses
                courses={
                  enrolledCourses
                }
                currentCredits={
                  currentCredits
                }
                maxCredits={
                  user.maxCredits
                }
                onBack={() =>
                  navigate(
                    "/student"
                  )
                }
              />
            }
          />


          {/* Grades */}
          <Route
            path="my-grades"
            element={
              <MyGrades
                studentId={
                  user.id
                }
                onBack={() =>
                  navigate(
                    "/student"
                  )
                }
              />
            }
          />


          {/* Unknown student route */}
          <Route
            path="*"
            element={
              <Navigate
                to="/student"
                replace
              />
            }
          />

        </Routes>

      </main>

    </div>
  );
}


export default StudentDashboard;