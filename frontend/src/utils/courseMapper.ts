/*
 * Converts API course data into the frontend course model.
 */

import type {
  Course as ApiCourse,
} from "../services/api";

import type {
  Course,
} from "../types/Course";


export function mapCourse(
  course: ApiCourse
): Course {

  return {
    id: course.id,
    code: course.code,
    name: course.name,
    credits: course.credits,

    type:
      course.type ===
      "GRADUATE"
        ? "Graduate"
        : "Undergraduate",

    examCount:
      course.examCount,
  };
}