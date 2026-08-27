import {
  useEffect,
  useState,
} from "react";

import {
  getCourses,
  getStudentEnrollments,
} from "../services/api";

import type {
  Course as ApiCourse,
  Enrollment,
} from "../services/api";


type GradeRecord = {
  courseId: number;

  code: string;

  name: string;

  examScores: {
    name: string;
    score: number;
  }[];

  finalScore?: number;

  letterGrade?: string;

  passed?: boolean;
};


type MyGradesProps = {
  studentId: number;

  onBack: () => void;
};


function MyGrades({
  studentId,
  onBack,
}: MyGradesProps) {

  const [
    grades,
    setGrades,
  ] =
    useState<GradeRecord[]>([]);


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

      const loadGrades =
        async () => {

          try {

            setLoading(
              true
            );

            setError(
              ""
            );


            const [
              enrollments,
              courses,
            ] =
              await Promise.all([
                getStudentEnrollments(
                  studentId
                ),
                getCourses(),
              ]);


            const records:
              GradeRecord[] =
              enrollments.map(
                (
                  enrollment:
                    Enrollment
                ) => {

                  const course:
                    ApiCourse | undefined =
                    courses.find(
                      (
                        item
                      ) =>
                        item.id ===
                        enrollment.courseId
                    );


                  const examScores =
                    enrollment.examScores
                      .map(
                        (
                          examScore
                        ) => ({
                          name:
                            `Exam ${examScore.examId}`,

                          score:
                            examScore.score,
                        })
                      );


                  return {
                    courseId:
                      enrollment.courseId,

                    code:
                      course?.code ??
                      `Course ${enrollment.courseId}`,

                    name:
                      course?.name ??
                      "Unknown Course",

                    examScores,

                    finalScore:
                      enrollment.finalScore ??
                      undefined,

                    letterGrade:
                      enrollment.letterGrade ??
                      undefined,

                    passed:
                      enrollment.passed ??
                      undefined,
                  };
                }
              );


            setGrades(
              records
            );

          }
          catch (error) {

            setError(
              error instanceof Error
                ? error.message
                : "Could not load grades."
            );

          }
          finally {

            setLoading(
              false
            );
          }
        };


      loadGrades();

    },
    [
      studentId,
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
            My Grades
          </h2>


          <p>
            Review exam scores,
            final scores, and letter grades.
          </p>

        </div>

      </div>


      {loading && (

        <div className="table-card">
          Loading grades...
        </div>

      )}


      {error && (

        <div className="status-message error">
          {error}
        </div>

      )}


      {!loading &&
       !error &&
       grades.length ===
         0 && (

        <div className="table-card">
          No enrolled courses found.
        </div>

      )}


      {!loading &&
       !error && (

        <div className="grade-list">

          {grades.map(
            (record) => (

              <section
                className="grade-card"
                key={
                  record.courseId
                }
              >

                <div className="grade-card-header">

                  <div>

                    <span className="course-code">
                      {record.code}
                    </span>

                    <h3>
                      {record.name}
                    </h3>

                  </div>


                  {record.finalScore !==
                  undefined ? (

                    <div className="grade-result">

                      <div>

                        <span>
                          Final Score
                        </span>

                        <strong>
                          {record.finalScore}
                        </strong>

                      </div>


                      <div>

                        <span>
                          Letter Grade
                        </span>

                        <strong className="letter-grade">
                          {record.letterGrade}
                        </strong>

                      </div>


                      {record.passed !==
                      undefined && (

                        <span
                          className={
                            record.passed
                              ? "result-badge passed"
                              : "result-badge failed"
                          }
                        >
                          {record.passed
                            ? "Passed"
                            : "Failed"}
                        </span>

                      )}

                    </div>

                  ) : (

                    <span className="result-badge pending">
                      Not Calculated
                    </span>

                  )}

                </div>


                <div className="exam-score-section">

                  <h4>
                    Exam Scores
                  </h4>


                  {record.examScores
                    .length ===
                  0 ? (

                    <p className="no-score-message">
                      No exam scores have been
                      entered yet.
                    </p>

                  ) : (

                    <div className="exam-score-grid">

                      {record.examScores.map(
                        (
                          exam
                        ) => (

                          <div
                            className="exam-score-item"
                            key={
                              exam.name
                            }
                          >

                            <span>
                              {exam.name}
                            </span>

                            <strong>
                              {exam.score}
                            </strong>

                          </div>

                        )
                      )}

                    </div>

                  )}

                </div>

              </section>

            )
          )}

        </div>

      )}

    </div>
  );
}


export default MyGrades;