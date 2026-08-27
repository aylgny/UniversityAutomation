import {
  useState,
} from 'react';

import type {
  Course,
} from '../types/Course';


type AvailableCoursesProps = {
  courses:
    Course[];

  currentCredits:
    number;

  maxCredits:
    number;

  onRegister: (
    course: Course
  ) => Promise<string>;

  onBack:
    () => void;
};


function AvailableCourses({
  courses,
  currentCredits,
  maxCredits,
  onRegister,
  onBack,
}: AvailableCoursesProps) {

  const [
    message,
    setMessage,
  ] =
    useState('');


  const [
    messageType,
    setMessageType,
  ] =
    useState<
      'success' | 'error'
    >(
      'success'
    );


  const [
    registeringCourseId,
    setRegisteringCourseId,
  ] =
    useState<
      number | null
    >(
      null
    );


  const handleRegister = async (
    course: Course
  ) => {

    setRegisteringCourseId(
      course.id
    );


    setMessage('');


    try {

      const result =
        await onRegister(
          course
        );


      const isSuccess =
        result
          .toLowerCase()
          .includes(
            'success'
          );


      setMessageType(
        isSuccess
          ? 'success'
          : 'error'
      );


      setMessage(
        result
      );

    } catch {

      setMessageType(
        'error'
      );


      setMessage(
        'Registration failed.'
      );

    } finally {

      setRegisteringCourseId(
        null
      );
    }
  };


  const remainingCredits =
    Math.max(
      maxCredits -
      currentCredits,
      0
    );


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
            Available Courses
          </h2>


          <p>
            Select a course to register.
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


          <span>
            Remaining
          </span>

          <strong>
            {remainingCredits}
          </strong>

        </div>

      </div>


      {message && (

        <div
          className={
            `status-message ${messageType}`
          }
        >
          {message}
        </div>

      )}


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

            {courses.length ===
            0 ? (

              <tr>

                <td
                  className="empty-table"
                  colSpan={5}
                >
                  No courses are currently
                  available for registration.
                </td>

              </tr>

            ) : (

              courses.map(
                (course) => {

                  const isRegistering =
                    registeringCourseId ===
                    course.id;


                  return (

                    <tr
                      key={
                        course.id
                      }
                    >

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
                          disabled={
                            registeringCourseId !==
                            null
                          }
                          onClick={() =>
                            handleRegister(
                              course
                            )
                          }
                        >
                          {isRegistering
                            ? 'Registering...'
                            : 'Register'}
                        </button>

                      </td>

                    </tr>

                  );
                }
              )

            )}

          </tbody>

        </table>

      </div>

    </div>
  );
}


export default AvailableCourses;