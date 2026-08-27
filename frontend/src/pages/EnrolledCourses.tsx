import type { Course } from '../types/Course';


type EnrolledCoursesProps = {
  courses: Course[];
  currentCredits: number;
  maxCredits: number;
  onBack: () => void;
};


function EnrolledCourses({
  courses,
  currentCredits,
  maxCredits,
  onBack,
}: EnrolledCoursesProps) {

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
            Enrolled Courses
          </h2>

          <p>
            Courses you are currently
            registered for.
          </p>

        </div>


        <div className="credit-summary">

          <span>
            Current Credits
          </span>

          <strong>
            {currentCredits}
          </strong>


          <span>
            Maximum Credits
          </span>

          <strong>
            {maxCredits}
          </strong>

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
                Status
              </th>

            </tr>
          </thead>


          <tbody>

            {courses.length === 0 ? (

              <tr>
                <td
                  colSpan={5}
                  className="empty-table"
                >
                  You are not enrolled in
                  any courses.
                </td>
              </tr>

            ) : (

              courses.map(
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
                      <span className="status-badge">
                        Enrolled
                      </span>
                    </td>

                  </tr>

                )
              )

            )}

          </tbody>

        </table>

      </div>
    </div>
  );
}


export default EnrolledCourses;