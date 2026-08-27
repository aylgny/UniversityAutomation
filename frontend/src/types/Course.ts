export type Course = {
  id: number;
  code: string;
  name: string;
  credits: number;
  type:
    | "Undergraduate"
    | "Graduate";

  examCount?: number;
};