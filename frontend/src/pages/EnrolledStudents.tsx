import {
  useEffect,
  useState,
} from "react";

import type {
  Course,
} from "../types/Course";

import {
  getStudentEnrollments,
  getStudents,
} from "../services/api";

import type {
  Student,
} from "../services/api";


type StudentRecord = {
  id: number;

  name: string;

  studentType:
    | "Undergraduate"
    | "Graduate";

  gpa: number;
};


type EnrolledStudentsProps = {
  course: Course;

  onBack: () => void;
};


function EnrolledStudents({
  course,
  onBack,
}: EnrolledStudentsProps) {

  const [
    students,
    setStudents,
  ] =
    useState<StudentRecord[]>([]);


  const [
    loading,
    setLoading,
  ] =
    useState(true);


  const [
    error,
    setError,
  ] =
    useState("");


  useEffect(
    () => {

      const loadStudents =
        async () => {

          try {

            setLoading(
              true
            );

            setError(
              ""
            );


            const allStudents:
              Student[] =
              await getStudents();


            const enrolledStudents:
              StudentRecord[] = [];


            for (
              const student of
              allStudents
            ) {

              const enrollments =
                await getStudentEnrollments(
                  student.id
                );


              const enrolled =
                enrollments.some(
                  (enrollment) =>
                    enrollment.courseId ===
                    course.id
                );


              if (
                !enrolled
              ) {
                continue;
              }


              enrolledStudents.push(
                {
                  id:
                    student.id,

                  name:
                    student.name,

                  studentType:
                    student.studentType ===
                    "GRADUATE"
                      ? "Graduate"
                      : "Undergraduate",

                  gpa:
                    student.gpa,
                }
              );
            }


            setStudents(
              enrolledStudents
            );

          }
          catch (error) {

            setError(
              error instanceof Error
                ? error.message
                : "Could not load enrolled students."
            );

          }
          finally {

            setLoading(
              false
            );
          }
        };


      loadStudents();

    },
    [
      course.id,
    ]
  );


  return (
    <div className="page">

      <div className="page-header">

        <div>

          <button
            className="back-button"
            onClick={
              onBack
            }
          >
            ← Back
          </button>


          <h2>
            Enrolled Students
          </h2>


          <p>
            {course.code}
            {" — "}
            {course.name}
          </p>

        </div>


        <div className="course-summary">

          <span>
            Course Type
          </span>

          <strong>
            {course.type}
          </strong>


          <span>
            Credits
          </span>

          <strong>
            {course.credits}
          </strong>


          <span>
            Students
          </span>

          <strong>
            {students.length}
          </strong>

        </div>

      </div>


      {loading && (

        <div className="table-card">
          Loading enrolled students...
        </div>

      )}


      {error && (

        <div className="status-message error">
          {error}
        </div>

      )}


      {!loading &&
       !error &&
       students.length ===
         0 && (

        <div className="table-card">
          No students are enrolled
          in this course.
        </div>

      )}


      {!loading &&
       !error &&
       students.length >
         0 && (

        <div className="table-card">

          <table className="course-table">

            <thead>

              <tr>

                <th>
                  Student ID
                </th>

                <th>
                  Name
                </th>

                <th>
                  Student Type
                </th>

                <th>
                  GPA
                </th>

              </tr>

            </thead>


            <tbody>

              {students.map(
                (
                  student
                ) => (

                  <tr
                    key={
                      student.id
                    }
                  >

                    <td>
                      {student.id}
                    </td>

                    <td>

                      <strong>
                        {student.name}
                      </strong>

                    </td>

                    <td>
                      {student.studentType}
                    </td>

                    <td>
                      {student.gpa.toFixed(
                        2
                      )}
                    </td>

                  </tr>

                )
              )}

            </tbody>

          </table>

        </div>

      )}

    </div>
  );
}


export default EnrolledStudents;