const API_BASE_URL = "http://localhost:8080/api";

async function request<T>(
  endpoint: string,
  options?: RequestInit
): Promise<T> {
  const response = await fetch(
    `${API_BASE_URL}${endpoint}`,
    {
      ...options,
      headers: {
        "Content-Type": "application/json",
        ...options?.headers,
      },
    }
  );

  const data = await response.json();

  if (!response.ok) {
    throw new Error(
      data.error ?? "Request failed."
    );
  }

  return data as T;
}


// =========================================================
// TYPES
// =========================================================

export type StudentType =
  | "UNDERGRADUATE"
  | "GRADUATE";

export type UserRole =
  | "STUDENT"
  | "INSTRUCTOR";

export type LoginUser =
  | {
      id: number;
      name: string;
      role: "STUDENT";
      studentType: StudentType;
      gpa: number;
      maxCredits: number;
    }
  | {
      id: number;
      name: string;
      role: "INSTRUCTOR";
    };

export interface LoginResponse {
  success: boolean;
  user: LoginUser;
}

export interface Student {
  id: number;
  name: string;
  gpa: number;
  studentType: StudentType;
}

export interface Course {
  id: number;
  code: string;
  name: string;
  credits: number;
  type?: StudentType;
  examCount: number;
}

export interface Instructor {
  id: number;
  name: string;
  courseIds: number[];
}

export interface ExamScore {
  examId: number;
  score: number;
}

export interface Enrollment {
  id: number;
  studentId: number;
  courseId: number;
  examScores: ExamScore[];
  finalScore: number | null;
  letterGrade: string | null;
  passed: boolean | null;
}

export interface CalculationResult {
  success: boolean;
  finalScore: number;
  letterGrade: string;
  passed: boolean;
}

export type WeightedGradingApiConfiguration = {
  method: "weighted";

  weights: {
    examId: number;
    weight: number;
  }[];
};


export type ThresholdGradingApiConfiguration = {
  method: "threshold";

  threshold: number;

  thresholdExamIds: number[];
};


export type GradingApiConfiguration =
  | WeightedGradingApiConfiguration
  | ThresholdGradingApiConfiguration;


export type CourseGradingResponse = {
  success: boolean;

  courseId: number;

  examCount: number;

  gradingByStudentType: {
    UNDERGRADUATE?:
      GradingApiConfiguration;

    GRADUATE?:
      GradingApiConfiguration;
  };
};


// =========================================================
// HEALTH
// =========================================================

export function getHealth() {
  return request<{
    success: boolean;
    status: string;
  }>("/health");
}


// =========================================================
// LOGIN
// =========================================================

export function login(
  username: string,
  password: string
) {
  return request<LoginResponse>(
    "/login",
    {
      method: "POST",
      body: JSON.stringify({
        username,
        password,
      }),
    }
  );
}


// =========================================================
// STUDENTS
// =========================================================

export function getStudents() {
  return request<Student[]>(
    "/students"
  );
}

export function getStudentEnrollments(
  studentId: number
) {
  return request<Enrollment[]>(
    `/students/${studentId}/enrollments`
  );
}


// =========================================================
// COURSES
// =========================================================

export function getCourses() {
  return request<Course[]>(
    "/courses"
  );
}


// =========================================================
// INSTRUCTORS
// =========================================================

export function getInstructors() {
  return request<Instructor[]>(
    "/instructors"
  );
}

export function getInstructorCourses(
  instructorId: number
) {
  return request<Course[]>(
    `/instructors/${instructorId}/courses`
  );
}


// =========================================================
// ENROLLMENTS
// =========================================================

export function createEnrollment(
  studentId: number,
  courseId: number
) {
  return request<{
    success: boolean;
    enrollment: Enrollment;
  }>(
    "/enrollments",
    {
      method: "POST",
      body: JSON.stringify({
        studentId,
        courseId,
      }),
    }
  );
}

export function getEnrollment(
  enrollmentId: number
) {
  return request<Enrollment>(
    `/enrollments/${enrollmentId}`
  );
}


// =========================================================
// EXAM CONFIGURATION
// =========================================================

export function configureExams(
  courseId: number,
  examCount: number
) {
  return request<{
    success: boolean;
    courseId: number;
    examCount: number;
  }>(
    `/courses/${courseId}/exams`,
    {
      method: "PUT",
      body: JSON.stringify({
        examCount,
      }),
    }
  );
}


// =========================================================
// GRADING CONFIGURATION
// =========================================================

export function configureWeightedAverage(
  courseId: number,
  studentType: StudentType,
  weights: {
    examId: number;
    weight: number;
  }[]
) {
  return request(
    `/courses/${courseId}/grading/weighted`,
    {
      method: "PUT",
      body: JSON.stringify({
        studentType,
        weights,
      }),
    }
  );
}


export function configureThreshold(
  courseId: number,
  studentType: StudentType,
  threshold: number,
  thresholdExamIds: number[]
) {
  return request(
    `/courses/${courseId}/grading/threshold`,
    {
      method: "PUT",
      body: JSON.stringify({
        studentType,
        threshold,
        thresholdExamIds,
      }),
    }
  );
}

export function getGradingConfiguration(
  courseId: number
) {
  return request<CourseGradingResponse>(
    `/courses/${courseId}/grading`
  );
}


// =========================================================
// SCORE ENTRY
// =========================================================

export function enterExamScore(
  enrollmentId: number,
  examId: number,
  score: number
) {
  return request(
    `/enrollments/${enrollmentId}/scores/${examId}`,
    {
      method: "PUT",
      body: JSON.stringify({
        score,
      }),
    }
  );
}


// =========================================================
// FINAL RESULT
// =========================================================

export function calculateFinalResult(
  enrollmentId: number
) {
  return request<CalculationResult>(
    `/enrollments/${enrollmentId}/calculate`,
    {
      method: "POST",
    }
  );
}