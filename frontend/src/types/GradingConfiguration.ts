export type StudentGroup =
  | 'Undergraduate'
  | 'Graduate';


export type WeightedGradingConfiguration = {
  method: 'weighted';

  weights: number[];
};


export type ThresholdGradingConfiguration = {
  method: 'threshold';

  threshold: number;

  thresholdExamIds: number[];
};


export type GradingMethodConfiguration =
  | WeightedGradingConfiguration
  | ThresholdGradingConfiguration;


export type CourseGradingConfiguration = {
  examCount: number;

  gradingByStudentType: {
    Undergraduate?:
      GradingMethodConfiguration;

    Graduate?:
      GradingMethodConfiguration;
  };
};


export type GradingConfigurations =
  Record<
    number,
    CourseGradingConfiguration
  >;