import {
  useEffect,
  useMemo,
  useState,
} from "react";

import type {
  Course,
} from "../types/Course";

import type {
  GradingConfigurations,
  GradingMethodConfiguration,
} from "../types/GradingConfiguration";

import {
  calculateFinalResult,
  enterExamScore,
  getStudentEnrollments,
  getStudents,
} from "../services/api";

import type {
  Student,
} from "../services/api";

import "./GradeManagement.css";


type StudentType =
  | "Undergraduate"
  | "Graduate";


type StudentRecord = {
  id: number;
  name: string;
  studentType: StudentType;
  enrollmentId: number;
};


type GradeResult = {
  finalScore: number;
  letterGrade: string;
  passed: boolean;
};


type GradeManagementProps = {
  courses: Course[];

  configurations:
    GradingConfigurations;

  onBack: () => void;
};


function describeConfiguration(
  configuration:
    GradingMethodConfiguration
): string {

  if (
    configuration.method ===
    "weighted"
  ) {

    return configuration
      .weights
      .map(
        (
          weight,
          index
        ) =>
          `Exam ${index + 1}: ${(weight * 100).toFixed(0)}%`
      )
      .join(
        " · "
      );
  }


  return (
    `Threshold: ${configuration.threshold} · ` +
    `Exam(s): ${configuration.thresholdExamIds.join(", ")}`
  );
}


function GradeManagement({
  courses,
  configurations,
  onBack,
}: GradeManagementProps) {

  const firstCourseId =
    courses.length > 0
      ? courses[0].id
      : 0;


  const [
    selectedCourseId,
    setSelectedCourseId,
  ] =
    useState<number>(
      firstCourseId
    );


  const [
    students,
    setStudents,
  ] =
    useState<StudentRecord[]>([]);


  const [
    scores,
    setScores,
  ] =
    useState<
      Record<
        number,
        string[]
      >
    >({});


  const [
    results,
    setResults,
  ] =
    useState<
      Record<
        number,
        GradeResult
      >
    >({});


  const [
    message,
    setMessage,
  ] =
    useState("");


  const [
    messageType,
    setMessageType,
  ] =
    useState<
      "success" | "error"
    >(
      "success"
    );


  const [
    isCalculating,
    setIsCalculating,
  ] =
    useState(false);


  const [
    loadingStudents,
    setLoadingStudents,
  ] =
    useState(false);


  const selectedCourse =
    useMemo(
      () =>
        courses.find(
          (course) =>
            course.id ===
            selectedCourseId
        ),
      [
        courses,
        selectedCourseId,
      ]
    );


  const courseConfiguration =
    configurations[
      selectedCourseId
    ];


  const examCount =
    courseConfiguration
      ?.examCount ??
    selectedCourse
      ?.examCount ??
    2;


  const undergraduateConfiguration =
    courseConfiguration
      ?.gradingByStudentType
      .Undergraduate;


  const graduateConfiguration =
    courseConfiguration
      ?.gradingByStudentType
      .Graduate;


  // Load real enrollments for selected course.
  useEffect(
    () => {

      const loadStudents =
        async () => {

          if (
            selectedCourseId ===
            0
          ) {
            return;
          }


          try {

            setLoadingStudents(
              true
            );

            setMessage(
              ""
            );


            const allStudents:
              Student[] =
              await getStudents();


            const studentRecords:
              StudentRecord[] = [];

            const initialScores:
              Record<
                number,
                string[]
              > = {};


            for (
              const student of
              allStudents
            ) {

              const enrollments =
                await getStudentEnrollments(
                  student.id
                );


              const enrollment =
                enrollments.find(
                  (item) =>
                    item.courseId ===
                    selectedCourseId
                );


              if (
                !enrollment
              ) {
                continue;
              }


              studentRecords.push(
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

                  enrollmentId:
                    enrollment.id,
                }
              );


              const studentScores =
                Array(
                  examCount
                ).fill(
                  ""
                );


              for (
                const examScore of
                enrollment.examScores
              ) {

                const index =
                  examScore.examId -
                  1;


                if (
                  index >= 0 &&
                  index <
                  studentScores.length
                ) {
                  studentScores[
                    index
                  ] =
                    String(
                      examScore.score
                    );
                }
              }


              initialScores[
                student.id
              ] =
                studentScores;
            }


            setStudents(
              studentRecords
            );

            setScores(
              initialScores
            );

            setResults(
              {}
            );

          }
          catch (error) {

            setMessageType(
              "error"
            );

            setMessage(
              error instanceof Error
                ? error.message
                : "Could not load enrolled students."
            );
          }
          finally {

            setLoadingStudents(
              false
            );
          }
        };


      loadStudents();

    },
    [
      selectedCourseId,
      examCount,
    ]
  );


  const handleScoreChange = (
    studentId: number,
    examIndex: number,
    value: string
  ) => {

    setScores(
      (
        previousScores
      ) => ({
        ...previousScores,

        [studentId]:
          previousScores[
            studentId
          ].map(
            (
              score,
              currentIndex
            ) =>
              currentIndex ===
              examIndex
                ? value
                : score
          ),
      })
    );


    setResults(
      (
        previousResults
      ) => {

        const updatedResults = {
          ...previousResults,
        };


        delete updatedResults[
          studentId
        ];


        return updatedResults;
      }
    );


    setMessage(
      ""
    );
  };


  const validateStudentScores = (
    student:
      StudentRecord
  ): number[] | null => {

    const studentScores =
      scores[
        student.id
      ];


    if (
      !studentScores
    ) {
      return null;
    }


    const hasEmptyScore =
      studentScores.some(
        (score) =>
          score.trim() ===
          ""
      );


    if (
      hasEmptyScore
    ) {

      setMessageType(
        "error"
      );

      setMessage(
        `All exam scores must be entered for ${student.name}.`
      );

      return null;
    }


    const numericScores =
      studentScores.map(
        (score) =>
          Number(
            score
          )
      );


    const hasInvalidScore =
      numericScores.some(
        (score) =>
          Number.isNaN(
            score
          ) ||
          score < 0 ||
          score > 100
      );


    if (
      hasInvalidScore
    ) {

      setMessageType(
        "error"
      );

      setMessage(
        `Exam scores for ${student.name} must be between 0 and 100.`
      );

      return null;
    }


    return numericScores;
  };


  const handleCalculateAll =
    async () => {

      if (
        !selectedCourse
      ) {

        setMessageType(
          "error"
        );

        setMessage(
          "Please select a course."
        );

        return;
      }


      if (
        students.length ===
        0
      ) {

        setMessageType(
          "error"
        );

        setMessage(
          "No enrolled students were found for this course."
        );

        return;
      }


      const validatedScores:
        Record<
          number,
          number[]
        > = {};


      for (
        const student of
        students
      ) {

        const numericScores =
          validateStudentScores(
            student
          );


        if (
          !numericScores
        ) {
          return;
        }


        validatedScores[
          student.id
        ] =
          numericScores;
      }


      try {

        setIsCalculating(
          true
        );

        setMessage(
          ""
        );


        const calculatedResults:
          Record<
            number,
            GradeResult
          > = {};


        for (
          const student of
          students
        ) {

          const numericScores =
            validatedScores[
              student.id
            ];


          // Persist every exam score through REST.
          for (
            let index = 0;
            index <
            numericScores.length;
            index++
          ) {

            await enterExamScore(
              student.enrollmentId,
              index + 1,
              numericScores[
                index
              ]
            );
          }


          // Let the C++ backend calculate the result.
          const result =
            await calculateFinalResult(
              student.enrollmentId
            );


          calculatedResults[
            student.id
          ] = {
            finalScore:
              result.finalScore,

            letterGrade:
              result.letterGrade,

            passed:
              result.passed,
          };
        }


        setResults(
          calculatedResults
        );


        setMessageType(
          "success"
        );

        setMessage(
          `${selectedCourse.code} final grades calculated successfully by the C++ backend.`
        );

      }
      catch (error) {

        setMessageType(
          "error"
        );

        setMessage(
          error instanceof Error
            ? error.message
            : "Could not communicate with the C++ backend."
        );

      }
      finally {

        setIsCalculating(
          false
        );
      }
    };


  const handleCourseChange = (
    courseId: number
  ) => {

    setSelectedCourseId(
      courseId
    );

    setStudents(
      []
    );

    setScores(
      {}
    );

    setResults(
      {}
    );

    setMessage(
      ""
    );
  };


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
            Grade Management
          </h2>


          <p>
            Enter exam scores and
            calculate final grades.
          </p>

        </div>


        {selectedCourse && (

          <div className="course-summary">

            <span>
              Course
            </span>

            <strong>
              {selectedCourse.code}
            </strong>


            <span>
              Credits
            </span>

            <strong>
              {selectedCourse.credits}
            </strong>


            <span>
              Exams
            </span>

            <strong>
              {examCount}
            </strong>

          </div>

        )}

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


      <section className="grading-settings-card">

        <div className="form-group">

          <label>
            Course
          </label>


          <select
            value={
              selectedCourseId
            }
            onChange={
              (event) =>
                handleCourseChange(
                  Number(
                    event.target.value
                  )
                )
            }
          >

            {courses.map(
              (course) => (

                <option
                  key={
                    course.id
                  }
                  value={
                    course.id
                  }
                >
                  {course.code}
                  {" — "}
                  {course.name}
                </option>

              )
            )}

          </select>

        </div>


        <div className="grading-config-summary">

          <div>

            <span>
              Undergraduate
            </span>

            <strong>
              {undergraduateConfiguration
                ? undergraduateConfiguration
                    .method ===
                  "weighted"
                  ? "Weighted Average"
                  : "Threshold Method"
                : "Backend Configuration"}
            </strong>


            <small>
              {undergraduateConfiguration
                ? describeConfiguration(
                    undergraduateConfiguration
                  )
                : "Configuration is managed by the backend."}
            </small>

          </div>


          <div>

            <span>
              Graduate
            </span>

            <strong>
              {graduateConfiguration
                ? graduateConfiguration
                    .method ===
                  "weighted"
                  ? "Weighted Average"
                  : "Threshold Method"
                : "Backend Configuration"}
            </strong>


            <small>
              {graduateConfiguration
                ? describeConfiguration(
                    graduateConfiguration
                  )
                : "Configuration is managed by the backend."}
            </small>

          </div>

        </div>

      </section>


      {loadingStudents ? (

        <div className="table-card">
          Loading enrolled students...
        </div>

      ) : students.length ===
        0 ? (

        <div className="table-card">
          No students are enrolled
          in this course.
        </div>

      ) : (

        <div className="table-card grade-management-table-card">

          <table className="course-table grade-management-table">

            <thead>

              <tr>

                <th>
                  Student
                </th>

                <th>
                  Type
                </th>


                {Array.from(
                  {
                    length:
                      examCount,
                  },
                  (
                    _,
                    index
                  ) => (

                    <th key={index}>
                      Exam {index + 1}
                    </th>

                  )
                )}


                <th>
                  Final Score
                </th>

                <th>
                  Letter Grade
                </th>

                <th>
                  Result
                </th>

              </tr>

            </thead>


            <tbody>

              {students.map(
                (student) => {

                  const result =
                    results[
                      student.id
                    ];


                  return (

                    <tr
                      key={
                        student.id
                      }
                    >

                      <td>

                        <div className="student-cell">

                          <strong>
                            {student.name}
                          </strong>

                          <span>
                            ID:{" "}
                            {student.id}
                          </span>

                        </div>

                      </td>


                      <td>
                        {student.studentType}
                      </td>


                      {(scores[
                        student.id
                      ] ?? []).map(
                        (
                          score,
                          index
                        ) => (

                          <td key={index}>

                            <input
                              className="score-input"
                              type="number"
                              min="0"
                              max="100"
                              value={
                                score
                              }
                              placeholder="0-100"
                              disabled={
                                isCalculating
                              }
                              onChange={
                                (event) =>
                                  handleScoreChange(
                                    student.id,
                                    index,
                                    event.target.value
                                  )
                              }
                            />

                          </td>

                        )
                      )}


                      <td>
                        {result
                          ? result.finalScore
                          : "—"}
                      </td>


                      <td>

                        {result ? (

                          <strong className="grade-value">
                            {result.letterGrade}
                          </strong>

                        ) : (
                          "—"
                        )}

                      </td>


                      <td>

                        {result ? (

                          <span
                            className={
                              result.passed
                                ? "result-badge passed"
                                : "result-badge failed"
                            }
                          >
                            {result.passed
                              ? "Passed"
                              : "Failed"}
                          </span>

                        ) : (

                          <span className="result-badge pending">
                            Pending
                          </span>

                        )}

                      </td>

                    </tr>

                  );
                }
              )}

            </tbody>

          </table>

        </div>

      )}


      <div className="grade-management-actions">

        <button
          className="primary-action-button"
          onClick={
            handleCalculateAll
          }
          disabled={
            isCalculating ||
            loadingStudents ||
            students.length ===
            0
          }
        >
          {isCalculating
            ? "Calculating..."
            : "Save Scores & Calculate Final Grades"}
        </button>

      </div>

    </div>
  );
}


export default GradeManagement;