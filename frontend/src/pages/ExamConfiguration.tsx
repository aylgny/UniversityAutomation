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
  StudentGroup,
} from "../types/GradingConfiguration";

import {
  configureExams,
  configureThreshold,
  configureWeightedAverage,
} from "../services/api";

import type {
  StudentType,
} from "../services/api";

import "./ExamConfiguration.css";


type GradingMethod =
  | "weighted"
  | "threshold";


type ExamConfigurationProps = {
  courses: Course[];

  configurations:
    GradingConfigurations;

  onSaveConfiguration: (
    courseId: number,
    examCount: number,
    studentType: StudentGroup,
    configuration:
      GradingMethodConfiguration
  ) => void;

  onBack: () => void;
};


function ExamConfiguration({
  courses,
  configurations,
  onSaveConfiguration,
  onBack,
}: ExamConfigurationProps) {

  const [
    selectedCourseId,
    setSelectedCourseId,
  ] =
    useState<number>(
      courses.length > 0
        ? courses[0].id
        : 0
    );


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


  const [
    examCount,
    setExamCount,
  ] =
    useState<number>(
      courses[0]?.examCount ??
      2
    );


  const [
    studentType,
    setStudentType,
  ] =
    useState<StudentGroup>(
      "Undergraduate"
    );


  const [
    gradingMethod,
    setGradingMethod,
  ] =
    useState<GradingMethod>(
      "weighted"
    );


  const [
    weights,
    setWeights,
  ] =
    useState<string[]>([
      "40",
      "60",
    ]);


  const [
    threshold,
    setThreshold,
  ] =
    useState<string>(
      "50"
    );


  const [
    thresholdExamIds,
    setThresholdExamIds,
  ] =
    useState<number[]>([
      1,
    ]);


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
    saving,
    setSaving,
  ] =
    useState(false);


  // Load existing frontend configuration
  // when course/student group changes.
  useEffect(
    () => {

        const courseConfiguration =
        configurations[
            selectedCourseId
        ];

        const courseExamCount =
        courseConfiguration
            ?.examCount ??
        selectedCourse
            ?.examCount ??
        2;

        setExamCount(
        courseExamCount
        );

        const gradingConfiguration =
        courseConfiguration
            ?.gradingByStudentType[
            studentType
            ];

        if (
        !gradingConfiguration
        ) {

        setGradingMethod(
            "weighted"
        );

        setWeights(
            Array.from(
            {
                length:
                courseExamCount,
            },
            () =>
                (
                100 /
                courseExamCount
                ).toFixed(2)
            )
        );

        setThreshold(
            "50"
        );

        setThresholdExamIds(
            [1]
        );

        return;
        }

        if (
        gradingConfiguration.method ===
        "weighted"
        ) {

        setGradingMethod(
            "weighted"
        );

        setWeights(
            gradingConfiguration
            .weights
            .map(
                (weight) =>
                String(
                    weight * 100
                )
            )
        );
        }
        else {

        setGradingMethod(
            "threshold"
        );

        setThreshold(
            String(
            gradingConfiguration
                .threshold
            )
        );

        setThresholdExamIds(
            gradingConfiguration
            .thresholdExamIds
        );
        }

    },
    [
        selectedCourseId,
        selectedCourse,
        studentType,
        configurations,
    ]
  );


  const handleExamCountChange = (
    count: number
  ) => {

    if (
      count < 1 ||
      count > 10
    ) {
      return;
    }


    setExamCount(
      count
    );


    setWeights(
      Array.from(
        {
          length: count,
        },
        () =>
          (
            100 / count
          ).toFixed(2)
      )
    );


    setThresholdExamIds(
      [1]
    );


    setMessage(
      ""
    );
  };


  const handleWeightChange = (
    index: number,
    value: string
  ) => {

    setWeights(
      (
        previousWeights
      ) =>
        previousWeights.map(
          (
            weight,
            currentIndex
          ) =>
            currentIndex ===
            index
              ? value
              : weight
        )
    );


    setMessage(
      ""
    );
  };


  const toggleThresholdExam = (
    examId: number
  ) => {

    setThresholdExamIds(
      (
        previousIds
      ) => {

        if (
          previousIds.includes(
            examId
          )
        ) {
          return previousIds.filter(
            (id) =>
              id !== examId
          );
        }


        return [
          ...previousIds,
          examId,
        ];
      }
    );


    setMessage(
      ""
    );
  };


  const numericWeights =
    weights.map(
      (weight) =>
        Number(
          weight
        )
    );


  const totalWeight =
    numericWeights.reduce(
      (
        total,
        weight
      ) =>
        total +
        (
          Number.isNaN(
            weight
          )
            ? 0
            : weight
        ),
      0
    );


  const handleSave =
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
        examCount < 1
      ) {

        setMessageType(
          "error"
        );

        setMessage(
          "Exam count must be positive."
        );

        return;
      }


      const apiStudentType:
        StudentType =
        studentType ===
        "Undergraduate"
          ? "UNDERGRADUATE"
          : "GRADUATE";


      let configuration:
        GradingMethodConfiguration;


      if (
        gradingMethod ===
        "weighted"
      ) {

        const hasEmptyWeight =
          weights.some(
            (weight) =>
              weight.trim() ===
              ""
          );


        if (
          hasEmptyWeight
        ) {

          setMessageType(
            "error"
          );

          setMessage(
            "All exam weights must be entered."
          );

          return;
        }


        const invalidWeight =
          numericWeights.some(
            (weight) =>
              Number.isNaN(
                weight
              ) ||
              weight < 0 ||
              weight > 100
          );


        if (
          invalidWeight
        ) {

          setMessageType(
            "error"
          );

          setMessage(
            "Each exam weight must be between 0 and 100."
          );

          return;
        }


        if (
          Math.abs(
            totalWeight - 100
          ) > 0.01
        ) {

          setMessageType(
            "error"
          );

          setMessage(
            `Weights must sum to 100%. Current total: ${totalWeight.toFixed(2)}%.`
          );

          return;
        }


        configuration = {
          method:
            "weighted",

          weights:
            numericWeights.map(
              (weight) =>
                weight / 100
            ),
        };
      }
      else {

        if (
          threshold.trim() ===
          ""
        ) {

          setMessageType(
            "error"
          );

          setMessage(
            "Threshold score must be entered."
          );

          return;
        }


        const numericThreshold =
          Number(
            threshold
          );


        if (
          Number.isNaN(
            numericThreshold
          ) ||
          numericThreshold < 0 ||
          numericThreshold > 100
        ) {

          setMessageType(
            "error"
          );

          setMessage(
            "Threshold must be between 0 and 100."
          );

          return;
        }


        if (
          thresholdExamIds.length ===
          0
        ) {

          setMessageType(
            "error"
          );

          setMessage(
            "At least one threshold exam must be selected."
          );

          return;
        }


        configuration = {
          method:
            "threshold",

          threshold:
            numericThreshold,

          thresholdExamIds,
        };
      }


      try {

        setSaving(
          true
        );

        setMessage(
          ""
        );


        // First persist exam structure.
        await configureExams(
          selectedCourse.id,
          examCount
        );


        // Then persist selected grading strategy.
        if (
          configuration.method ===
          "weighted"
        ) {

          await configureWeightedAverage(
            selectedCourse.id,
            apiStudentType,
            configuration.weights.map(
              (
                weight,
                index
              ) => ({
                examId:
                  index + 1,

                weight,
              })
            )
          );
        }
        else {

          await configureThreshold(
            selectedCourse.id,
            apiStudentType,
            configuration.threshold,
            configuration
              .thresholdExamIds
          );
        }


        // Keep existing frontend pages in sync.
        onSaveConfiguration(
          selectedCourse.id,
          examCount,
          studentType,
          configuration
        );


        setMessageType(
          "success"
        );

        setMessage(
          `${selectedCourse.code} grading configuration saved for ${studentType} students.`
        );
      }
      catch (error) {

        setMessageType(
          "error"
        );

        setMessage(
          error instanceof Error
            ? error.message
            : "Could not save grading configuration."
        );
      }
      finally {

        setSaving(
          false
        );
      }
    };


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
            Exam Configuration
          </h2>


          <p>
            Define exams and configure
            the grading method.
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
              Type
            </span>

            <strong>
              {selectedCourse.type}
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


      <div className="configuration-layout">

        <section className="configuration-card">

          <h3>
            Course and Exams
          </h3>


          <div className="form-group">

            <label>
              Course
            </label>


            <select
              value={
                selectedCourseId
              }
              onChange={
                (event) => {

                  setSelectedCourseId(
                    Number(
                      event.target.value
                    )
                  );

                  setMessage(
                    ""
                  );
                }
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


          <div className="form-group">

            <label>
              Number of Exams
            </label>


            <input
              type="number"
              min="1"
              max="10"
              value={
                examCount
              }
              onChange={
                (event) =>
                  handleExamCountChange(
                    Number(
                      event.target.value
                    )
                  )
              }
            />

          </div>

        </section>


        <section className="configuration-card">

          <h3>
            Student Group
          </h3>


          <p className="configuration-description">
            The same course may use
            different grading methods for
            undergraduate and graduate
            students.
          </p>


          <div className="segmented-control">

            <button
              className={
                studentType ===
                "Undergraduate"
                  ? "selected"
                  : ""
              }
              onClick={() =>
                setStudentType(
                  "Undergraduate"
                )
              }
            >
              Undergraduate
            </button>


            <button
              className={
                studentType ===
                "Graduate"
                  ? "selected"
                  : ""
              }
              onClick={() =>
                setStudentType(
                  "Graduate"
                )
              }
            >
              Graduate
            </button>

          </div>

        </section>


        <section className="configuration-card full-width">

          <h3>
            Grading Method
          </h3>


          <div className="grading-method-grid">

            <button
              className={
                gradingMethod ===
                "weighted"
                  ? "grading-method-card selected"
                  : "grading-method-card"
              }
              onClick={() => {

                setGradingMethod(
                  "weighted"
                );

                setMessage(
                  ""
                );
              }}
            >

              <strong>
                Weighted Average
              </strong>

              <span>
                Final score is calculated
                using exam weights.
              </span>

            </button>


            <button
              className={
                gradingMethod ===
                "threshold"
                  ? "grading-method-card selected"
                  : "grading-method-card"
              }
              onClick={() => {

                setGradingMethod(
                  "threshold"
                );

                setMessage(
                  ""
                );
              }}
            >

              <strong>
                Threshold Method
              </strong>

              <span>
                Selected exams must satisfy
                the defined threshold.
              </span>

            </button>

          </div>

        </section>


        {gradingMethod ===
        "weighted" ? (

          <section className="configuration-card full-width">

            <div className="configuration-title-row">

              <div>

                <h3>
                  Exam Weights
                </h3>

                <p className="configuration-description">
                  The total weight must equal 100%.
                </p>

              </div>


              <div
                className={
                  Math.abs(
                    totalWeight -
                    100
                  ) < 0.01
                    ? "weight-total valid"
                    : "weight-total invalid"
                }
              >
                Total:{" "}
                {totalWeight.toFixed(
                  2
                )}
                %
              </div>

            </div>


            <div className="exam-config-grid">

              {weights.map(
                (
                  weight,
                  index
                ) => (

                  <div
                    className="exam-config-item"
                    key={index}
                  >

                    <label>
                      Exam {index + 1}
                    </label>


                    <div className="weight-input">

                      <input
                        type="number"
                        min="0"
                        max="100"
                        step="0.01"
                        value={
                          weight
                        }
                        onChange={
                          (event) =>
                            handleWeightChange(
                              index,
                              event.target.value
                            )
                        }
                      />

                      <span>
                        %
                      </span>

                    </div>

                  </div>

                )
              )}

            </div>

          </section>

        ) : (

          <section className="configuration-card full-width">

            <h3>
              Threshold Configuration
            </h3>


            <div className="threshold-layout">

              <div className="form-group">

                <label>
                  Threshold Score
                </label>


                <input
                  type="number"
                  min="0"
                  max="100"
                  value={
                    threshold
                  }
                  onChange={
                    (event) => {

                      setThreshold(
                        event.target.value
                      );

                      setMessage(
                        ""
                      );
                    }
                  }
                />

              </div>


              <div className="form-group">

                <label>
                  Threshold Exams
                </label>


                <div className="threshold-exams">

                  {Array.from(
                    {
                      length:
                        examCount,
                    },
                    (
                      _,
                      index
                    ) => {

                      const examId =
                        index + 1;


                      return (

                        <label
                          className="checkbox-item"
                          key={
                            examId
                          }
                        >

                          <input
                            type="checkbox"
                            checked={
                              thresholdExamIds
                                .includes(
                                  examId
                                )
                            }
                            onChange={() =>
                              toggleThresholdExam(
                                examId
                              )
                            }
                          />

                          Exam {examId}

                        </label>

                      );
                    }
                  )}

                </div>

              </div>

            </div>

          </section>

        )}


        <div className="configuration-actions">

          <button
            className="primary-action-button"
            onClick={
              handleSave
            }
            disabled={
              saving
            }
          >
            {saving
              ? "Saving..."
              : "Save Configuration"}
          </button>

        </div>

      </div>

    </div>
  );
}


export default ExamConfiguration;