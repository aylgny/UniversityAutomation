/*
 * Displays the courses taught by the current instructor.
 * Selecting a course forwards it to the parent component
 * for student and grading management.
 */

import type { Course } from '../types/Course';


type TaughtCoursesProps = {
  courses: Course[];
  onSelectCourse: (course: Course) => void;
  onBack: () => void;
};


function TaughtCourses({
  courses,
  onSelectCourse,
  onBack,
}: TaughtCoursesProps) {

  return (
    <div className="page">

      <div className="page-header">

        <div>

          <button
            className="back-button"
            onClick={onBack}
          >
            ← Back
          </button>

          <h2>
            Taught Courses
          </h2>

          <p>
            Select a course to view enrolled
            students and manage grading.
          </p>

        </div>

      </div>


      <div className="table-card">

        <table className="course-table">

          <thead>
            <tr>

              <th>
                Course Code
              </th>

              <th>
                Course Name
              </th>

              <th>
                Credits
              </th>

              <th>
                Type
              </th>

              <th>
                Action
              </th>

            </tr>
          </thead>


          <tbody>

            {courses.map(
              (course) => (

                <tr key={course.id}>

                  <td>
                    <strong>
                      {course.code}
                    </strong>
                  </td>

                  <td>
                    {course.name}
                  </td>

                  <td>
                    {course.credits}
                  </td>

                  <td>
                    {course.type}
                  </td>

                  <td>
                    <button
                      className="register-button"
                      onClick={() =>
                        onSelectCourse(course)
                      }
                    >
                      View Course
                    </button>
                  </td>

                </tr>

              )
            )}

          </tbody>

        </table>

      </div>

    </div>
  );
}


export default TaughtCourses;