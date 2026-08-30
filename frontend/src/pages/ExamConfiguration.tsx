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
  getGradingConfiguration,
} from "../services/api";

import type {
  StudentType,
} from "../services/api";

import "./ExamConfiguration.css";


type GradingMethod =
  | "weighted"
  | "threshold"
  | null;


type GradingDraft = {
  method: GradingMethod;
  weights: string[];
  threshold: string;
  thresholdExamIds: number[];
};


type DraftConfigurations = {
  Undergraduate: GradingDraft;
  Graduate: GradingDraft;
};


type GradingLockState = {
  UNDERGRADUATE: boolean;
  GRADUATE: boolean;
};


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
    lockedByStudentType,
    setLockedByStudentType,
  ] =
    useState<GradingLockState>({
      UNDERGRADUATE: false,
      GRADUATE: false,
    });


  const isCurrentGroupLocked =
    studentType ===
    "Undergraduate"
      ? lockedByStudentType
          .UNDERGRADUATE
      : lockedByStudentType
          .GRADUATE;


  const isExamCountLocked =
    lockedByStudentType
      .UNDERGRADUATE ||
    lockedByStudentType
      .GRADUATE;


  const areAllGroupsLocked =
    lockedByStudentType
      .UNDERGRADUATE &&
    lockedByStudentType
      .GRADUATE;


  const createEmptyDraft = (
    count: number
  ): GradingDraft => ({
    method: null,
    weights: Array.from(
      {
        length: count,
      },
      () => ""
    ),
    threshold: "",
    thresholdExamIds: [],
  });


  const [
    draftConfigurations,
    setDraftConfigurations,
  ] =
    useState<DraftConfigurations>({
      Undergraduate:
        createEmptyDraft(
          courses[0]?.examCount ??
          2
        ),

      Graduate:
        createEmptyDraft(
          courses[0]?.examCount ??
          2
        ),
    });


  const currentDraft =
    draftConfigurations[
      studentType
    ];


  const gradingMethod =
    currentDraft.method;

  const weights =
    currentDraft.weights;

  const threshold =
    currentDraft.threshold;

  const thresholdExamIds =
    currentDraft.thresholdExamIds;


  const updateCurrentDraft = (
    updater:
      (
        draft: GradingDraft
      ) => GradingDraft
  ) => {

    setDraftConfigurations(
      (
        previousDrafts
      ) => ({
        ...previousDrafts,

        [studentType]:
          updater(
            previousDrafts[
              studentType
            ]
          ),
      })
    );
  };


  const setGradingMethod = (
    method: GradingMethod
  ) => {

    updateCurrentDraft(
      (draft) => ({
        ...draft,
        method,
      })
    );
  };


  const setWeights = (
    value:
      | string[]
      | (
          (
            previousWeights:
              string[]
          ) => string[]
        )
  ) => {

    updateCurrentDraft(
      (draft) => ({
        ...draft,

        weights:
          typeof value ===
          "function"
            ? value(
                draft.weights
              )
            : value,
      })
    );
  };


  const setThreshold = (
    value: string
  ) => {

    updateCurrentDraft(
      (draft) => ({
        ...draft,
        threshold: value,
      })
    );
  };


  const setThresholdExamIds = (
    value:
      | number[]
      | (
          (
            previousIds:
              number[]
          ) => number[]
        )
  ) => {

    updateCurrentDraft(
      (draft) => ({
        ...draft,

        thresholdExamIds:
          typeof value ===
          "function"
            ? value(
                draft.thresholdExamIds
              )
            : value,
      })
    );
  };


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


  // Load score-based lock state for the selected course.
  useEffect(
    () => {

      let cancelled =
        false;


      const loadLockState =
        async () => {

          if (
            selectedCourseId ===
            0
          ) {
            return;
          }


          try {

            const response =
              await getGradingConfiguration(
                selectedCourseId
              ) as Awaited<
                ReturnType<
                  typeof getGradingConfiguration
                >
              > & {
                lockedByStudentType:
                  GradingLockState;
              };


            if (
              !cancelled
            ) {
              setLockedByStudentType(
                response
                  .lockedByStudentType
              );
            }
          }
          catch {

            if (
              !cancelled
            ) {
              setLockedByStudentType({
                UNDERGRADUATE:
                  false,
                GRADUATE:
                  false,
              });
            }
          }
        };


      loadLockState();


      return () => {
        cancelled =
          true;
      };
    },
    [
      selectedCourseId,
    ]
  );


  // Load persisted configurations when the selected course changes.
  // Drafts are kept separately for undergraduate and graduate students.
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


      const createDraftFromConfiguration = (
        configuration:
          GradingMethodConfiguration |
          undefined
      ): GradingDraft => {

        if (
          !configuration
        ) {
          return createEmptyDraft(
            courseExamCount
          );
        }


        if (
          configuration.method ===
          "weighted"
        ) {
          return {
            method:
              "weighted",

            weights:
              configuration
                .weights
                .map(
                  (weight) =>
                    String(
                      weight * 100
                    )
                ),

            threshold:
              "",

            thresholdExamIds:
              [],
          };
        }


        return {
          method:
            "threshold",

          weights:
            Array.from(
              {
                length:
                  courseExamCount,
              },
              () => ""
            ),

          threshold:
            String(
              configuration.threshold
            ),

          thresholdExamIds:
            [
              ...configuration
                .thresholdExamIds,
            ],
        };
      };


      setDraftConfigurations({
        Undergraduate:
          createDraftFromConfiguration(
            courseConfiguration
              ?.gradingByStudentType
              .Undergraduate
          ),

        Graduate:
          createDraftFromConfiguration(
            courseConfiguration
              ?.gradingByStudentType
              .Graduate
          ),
      });

    },
    [
      selectedCourseId,
      selectedCourse,
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


    const resizeDraft = (
      draft: GradingDraft
    ): GradingDraft => {

      const resizedWeights =
        draft.method ===
        "weighted"
          ? Array.from(
              {
                length: count,
              },
              () =>
                (
                  100 /
                  count
                ).toFixed(2)
            )
          : Array.from(
              {
                length: count,
              },
              () => ""
            );


      let resizedThresholdExamIds =
        draft.thresholdExamIds
          .filter(
            (examId) =>
              examId <=
              count
          );


      if (
        draft.method ===
          "threshold" &&
        resizedThresholdExamIds
          .length ===
          0
      ) {
        resizedThresholdExamIds =
          [1];
      }


      return {
        ...draft,
        weights:
          resizedWeights,
        thresholdExamIds:
          resizedThresholdExamIds,
      };
    };


    setDraftConfigurations(
      (
        previousDrafts
      ) => ({
        Undergraduate:
          resizeDraft(
            previousDrafts
              .Undergraduate
          ),

        Graduate:
          resizeDraft(
            previousDrafts
              .Graduate
          ),
      })
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


      const buildConfiguration = (
        group:
          StudentGroup,
        draft:
          GradingDraft
      ):
        GradingMethodConfiguration |
        null => {

        if (
          draft.method ===
          null
        ) {
          return null;
        }


        if (
          draft.method ===
          "weighted"
        ) {

          const hasEmptyWeight =
            draft.weights.some(
              (weight) =>
                weight.trim() ===
                ""
            );


          if (
            hasEmptyWeight
          ) {
            throw new Error(
              `${group}: all exam weights must be entered.`
            );
          }


          const numericWeights =
            draft.weights.map(
              (weight) =>
                Number(
                  weight
                )
            );


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
            throw new Error(
              `${group}: each exam weight must be between 0 and 100.`
            );
          }


          const total =
            numericWeights.reduce(
              (
                sum,
                weight
              ) =>
                sum +
                weight,
              0
            );


          if (
            Math.abs(
              total -
              100
            ) >
            0.01
          ) {
            throw new Error(
              `${group}: weights must sum to 100%. Current total: ${total.toFixed(2)}%.`
            );
          }


          return {
            method:
              "weighted",

            weights:
              numericWeights.map(
                (weight) =>
                  weight /
                  100
              ),
          };
        }


        if (
          draft.threshold.trim() ===
          ""
        ) {
          throw new Error(
            `${group}: threshold score must be entered.`
          );
        }


        const numericThreshold =
          Number(
            draft.threshold
          );


        if (
          Number.isNaN(
            numericThreshold
          ) ||
          numericThreshold < 0 ||
          numericThreshold > 100
        ) {
          throw new Error(
            `${group}: threshold must be between 0 and 100.`
          );
        }


        if (
          draft
            .thresholdExamIds
            .length ===
          0
        ) {
          throw new Error(
            `${group}: at least one threshold exam must be selected.`
          );
        }


        return {
          method:
            "threshold",

          threshold:
            numericThreshold,

          thresholdExamIds:
            [
              ...draft
                .thresholdExamIds,
            ],
        };
      };


      try {

        const undergraduateConfiguration =
          lockedByStudentType
            .UNDERGRADUATE
            ? null
            : buildConfiguration(
                "Undergraduate",
                draftConfigurations
                  .Undergraduate
              );

        const graduateConfiguration =
          lockedByStudentType
            .GRADUATE
            ? null
            : buildConfiguration(
                "Graduate",
                draftConfigurations
                  .Graduate
              );


        if (
          undergraduateConfiguration ===
            null &&
          graduateConfiguration ===
            null
        ) {

          setMessageType(
            "error"
          );

          setMessage(
            areAllGroupsLocked
              ? "Grading configuration is locked because exam scores have already been entered."
              : "Please configure at least one editable student group."
          );

          return;
        }


        setSaving(
          true
        );

        setMessage(
          ""
        );


        // Exam structure belongs to the course and is shared by both groups.
        // Once any score exists, the exam count is locked.
        if (
          !isExamCountLocked
        ) {
          await configureExams(
            selectedCourse.id,
            examCount
          );
        }


        const saveGroupConfiguration =
          async (
            group:
              StudentGroup,
            apiStudentType:
              StudentType,
            configuration:
              GradingMethodConfiguration |
              null
          ) => {

            if (
              configuration ===
              null
            ) {
              return false;
            }


            if (
              configuration.method ===
              "weighted"
            ) {

              await configureWeightedAverage(
                selectedCourse.id,
                apiStudentType,
                configuration
                  .weights
                  .map(
                    (
                      weight,
                      index
                    ) => ({
                      examId:
                        index +
                        1,

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


            onSaveConfiguration(
              selectedCourse.id,
              examCount,
              group,
              configuration
            );


            return true;
          };


        let savedGroupCount =
          0;


        if (
          await saveGroupConfiguration(
            "Undergraduate",
            "UNDERGRADUATE",
            undergraduateConfiguration
          )
        ) {
          savedGroupCount++;
        }


        if (
          await saveGroupConfiguration(
            "Graduate",
            "GRADUATE",
            graduateConfiguration
          )
        ) {
          savedGroupCount++;
        }


        setMessageType(
          "success"
        );

        setMessage(
          `${selectedCourse.code} grading configuration saved for ${savedGroupCount} student group(s).`
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
              disabled={
                isExamCountLocked
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


            {isExamCountLocked && (
              <p className="configuration-description">
                Exam count is locked because exam scores have already been entered.
              </p>
            )}

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


        {isCurrentGroupLocked && (

          <section className="configuration-card full-width">

            <h3>
              Configuration Locked
            </h3>

            <p className="configuration-description">
              Exam scores have already been entered for this student group.
              The grading configuration can no longer be changed.
            </p>

          </section>

        )}


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
              disabled={
                isCurrentGroupLocked
              }
              onClick={() => {

                setGradingMethod(
                  "weighted"
                );

                setWeights(
                  Array.from(
                    {
                      length:
                        examCount,
                    },
                    () =>
                      (
                        100 /
                        examCount
                      ).toFixed(2)
                  )
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
              disabled={
                isCurrentGroupLocked
              }
              onClick={() => {

                setGradingMethod(
                  "threshold"
                );

                setThreshold(
                  "50"
                );

                setThresholdExamIds(
                  [1]
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
        null && (

          <section className="configuration-card full-width">

            <h3>
              Grading Method Not Selected
            </h3>

            <p className="configuration-description">
              Select Weighted Average or Threshold Method
              to configure grading for this student group.
            </p>

          </section>

        )}


        {gradingMethod ===
        "weighted" && (

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
                        disabled={
                          isCurrentGroupLocked
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

        )}


        {gradingMethod ===
        "threshold" && (

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
                  disabled={
                    isCurrentGroupLocked
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
                            disabled={
                              isCurrentGroupLocked
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
              saving ||
              areAllGroupsLocked
            }
          >
            {saving
              ? "Saving..."
              : areAllGroupsLocked
                ? "Configuration Locked"
                : "Save Configuration"}
          </button>

        </div>

      </div>

    </div>
  );
}


export default ExamConfiguration;