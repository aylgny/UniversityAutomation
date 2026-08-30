/*
 * Manages exam-score entry and final-grade calculation for a course.
 * Scores may be saved partially during the semester, while final grades
 * are calculated only for students whose required exam scores are complete.
 */

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
  finalScore: number | null;
  letterGrade: string;
  passed: boolean;
};


type GradeManagementProps = {
  courses: Course[];

  configurations:
    GradingConfigurations;

  onBack: () => void;
};

function formatFinalScore(
  score: number | null
): string {

  if (score === null) {
    return "—";
  }

  return Number(
    score.toFixed(2)
  ).toString();
}

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
  /*
  * Snapshot of scores currently stored in the backend.
  * It is used to avoid resending unchanged scores, because updating
  * a score would invalidate that student's previously calculated result.
  */
  const [
  persistedScores,
  setPersistedScores,
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
    isSaving,
    setIsSaving,
  ] =
    useState(false);


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


  const hasConfigurationForStudent = (
    studentType: StudentType
  ) => {

    if (
      studentType ===
      "Undergraduate"
    ) {
      return (
        undergraduateConfiguration !==
        undefined
      );
    }

    return (
      graduateConfiguration !==
      undefined
    );
  };

  /*
  * Score entry and final calculation require grading configuration
  * for every student group currently enrolled in the course.
  */
  const hasAnyUnconfiguredStudent =
    students.some(
      (student) =>
        !hasConfigurationForStudent(
          student.studentType
        )
    );


  // =====================================================
  // LOAD STUDENTS AND EXISTING SCORES
  // =====================================================

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

            const initialResults:
              Record<
                number,
                GradeResult
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


              // Restore an already calculated result.
              if (                
                enrollment.letterGrade !==
                  null &&
                enrollment.passed !==
                  null
              ) {

                initialResults[
                  student.id
                ] = {
                  finalScore:
                    enrollment.finalScore,

                  letterGrade:
                    enrollment.letterGrade,

                  passed:
                    enrollment.passed,
                };
              }
            }


            setStudents(
              studentRecords
            );


            
            setScores(
              initialScores
            );
            /*
            * Preserve the initially loaded values as the persisted snapshot
            * used later to detect actual score changes.
            */
            setPersistedScores(
              Object.fromEntries(
                Object.entries(
                  initialScores
                ).map(
                  (
                    [
                      studentId,
                      studentScores,
                    ]
                  ) => [
                    Number(
                      studentId
                    ),
                    [
                      ...studentScores,
                    ],
                  ]
                )
              )
            );

            setResults(
              initialResults
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


  // =====================================================
  // SCORE INPUT
  // =====================================================

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


    /*
     * A changed score invalidates only this student's
     * previously displayed calculated result.
     */
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


  // =====================================================
  // VALIDATION FOR SAVING
  // =====================================================

  const validateEnteredScore = (
    score: string
  ): number | null => {

    if (
      score.trim() ===
      ""
    ) {
      return null;
    }


    const numericScore =
      Number(
        score
      );


    if (
      Number.isNaN(
        numericScore
      ) ||
      numericScore < 0 ||
      numericScore > 100
    ) {
      throw new Error(
        "Exam scores must be between 0 and 100."
      );
    }


    return numericScore;
  };


  // =====================================================
  // SAVE PARTIAL SCORES
  // =====================================================

  const handleSaveScores =
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


      if (
        hasAnyUnconfiguredStudent
      ) {

        setMessageType(
          "error"
        );

        setMessage(
          "Configure grading for all enrolled student groups before entering exam scores."
        );

        return;
      }

      try {

        setIsSaving(
          true
        );

        setMessage(
          ""
        );


        let savedScoreCount =
          0;


        for (
          const student of
          students
        ) {

          const studentScores =
            scores[
              student.id
            ] ?? [];

          const persistedStudentScores =
            persistedScores[
              student.id
            ] ?? [];


          for (
            let index = 0;
            index <
            studentScores.length;
            index++
          ) {

            const currentRawScore =
              studentScores[
                index
              ];

            const persistedRawScore =
              persistedStudentScores[
                index
              ] ?? "";


            const numericScore =
              validateEnteredScore(
                currentRawScore
              );


            /*
            * Empty values are ignored so exam scores can be saved
            * gradually as exams take place during the semester.
            */
            if (
              numericScore ===
              null
            ) {
              continue;
            }


            const hadPersistedScore =
              persistedRawScore
                .trim() !== "";


            const persistedNumericScore =
              hadPersistedScore
                ? Number(
                    persistedRawScore
                  )
                : null;


            /*
            * Do not resend unchanged scores.
            * Sending them again would unnecessarily invalidate
            * an already calculated result for that enrollment.
            */
            if (
              persistedNumericScore !==
                null &&
              persistedNumericScore ===
                numericScore
            ) {
              continue;
            }


            await enterExamScore(
              student.enrollmentId,
              index + 1,
              numericScore
            );


            savedScoreCount++;
          }
        }


        if (
          savedScoreCount ===
          0
        ) {

          setMessageType(
            "success"
          );

          setMessage(
            "No score changes to save."
          );

          return;
        }
        /*
        * After a successful save, the current UI values become
        * the new backend snapshot for future change detection.
        */
        setPersistedScores(
          Object.fromEntries(
            Object.entries(
              scores
            ).map(
              (
                [
                  studentId,
                  studentScores,
                ]
              ) => [
                Number(
                  studentId
                ),
                [
                  ...studentScores,
                ],
              ]
            )
          )
        );


        setMessageType(
          "success"
        );

        setMessage(
          `${selectedCourse.code} exam scores saved successfully.`
        );

      }
      catch (error) {

        setMessageType(
          "error"
        );

        setMessage(
          error instanceof Error
            ? error.message
            : "Could not save exam scores."
        );

      }
      finally {

        setIsSaving(
          false
        );
      }
    };


  // =====================================================
  // VALIDATION FOR FINAL GRADE CALCULATION
  // =====================================================

  const validateAllStudentScores = (
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
      throw new Error(
        `Exam scores for ${student.name} must be between 0 and 100.`
      );
    }


    return numericScores;
  };


  // =====================================================
  // CALCULATE FINAL GRADES
  // =====================================================

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


      if (
        hasAnyUnconfiguredStudent
      ) {

        setMessageType(
          "error"
        );

        setMessage(
          "Configure grading for all enrolled student groups before calculating final grades."
        );

        return;
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


        let calculatedCount =
          0;

        let skippedCount =
          0;


        /*
         * Calculate only students whose complete exam
         * scores are available. Incomplete students remain pending.
         */
        for (
          const student of
          students
        ) {

          const numericScores =
            validateAllStudentScores(
              student
            );


          if (
            !numericScores
          ) {
            skippedCount++;
            continue;
          }


          /*
           * Save the values currently visible in the UI
           * before calculating. This prevents calculation
           * from using an older persisted score.
           */
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


          calculatedCount++;
        }


        if (
          calculatedCount ===
          0
        ) {

          setMessageType(
            "error"
          );

          setMessage(
            "No students have complete exam scores yet."
          );

          return;
        }


        // Preserve existing results for students that were skipped.
        setResults(
          (
            previousResults
          ) => ({
            ...previousResults,
            ...calculatedResults,
          })
        );


        setMessageType(
          "success"
        );


        if (
          skippedCount >
          0
        ) {

          setMessage(
            `${calculatedCount} final grade(s) calculated. ${skippedCount} student(s) skipped because exam scores are incomplete.`
          );
        }
        else {

          setMessage(
            `${calculatedCount} final grade(s) calculated successfully.`
          );
        }

      }
      catch (error) {

        setMessageType(
          "error"
        );

        setMessage(
          error instanceof Error
            ? error.message
            : "Could not calculate final grades."
        );

      }
      finally {

        setIsCalculating(
          false
        );
      }
    };


  // =====================================================
  // COURSE CHANGE
  // =====================================================
 
  /*
  * Clear course-specific UI state before loading
  * the newly selected course.
  */
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


  // =====================================================
  // UI
  // =====================================================

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
            Enter and save exam scores
            throughout the semester,
            then calculate final grades
            when all exams are completed.
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
                : "Not Configured"}
            </strong>


            <small>
              {undergraduateConfiguration
                ? describeConfiguration(
                    undergraduateConfiguration
                  )
                : "Configure a grading method before entering grades."}
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
                : "Not Configured"}
            </strong>


            <small>
              {graduateConfiguration
                ? describeConfiguration(
                    graduateConfiguration
                  )
                : "Configure a grading method before entering grades."}
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
                                isSaving ||
                                isCalculating ||
                                !hasConfigurationForStudent(
                                  student.studentType
                                )
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
                          ? formatFinalScore(
                              result.finalScore
                            )
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
          className="secondary-action-button"
          onClick={
            handleSaveScores
          }
          disabled={
            isSaving ||
            isCalculating ||
            loadingStudents ||
            students.length ===
            0 ||
            hasAnyUnconfiguredStudent
          }
        >
          {isSaving
            ? "Saving..."
            : "Save Scores"}
        </button>


        <button
          className="primary-action-button"
          onClick={
            handleCalculateAll
          }
          disabled={
            isSaving ||
            isCalculating ||
            loadingStudents ||
            students.length ===
            0 ||
            hasAnyUnconfiguredStudent
          }
        >
          {isCalculating
            ? "Calculating..."
            : "Calculate Final Grades"}
        </button>

      </div>

    </div>
  );
}


export default GradeManagement;