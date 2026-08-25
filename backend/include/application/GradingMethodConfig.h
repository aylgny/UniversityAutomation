#pragma once

#include <variant>

#include "application/WeightedAverageConfig.h"
#include "application/ThresholdConfig.h"

using GradingMethodConfig =
std::variant<
    WeightedAverageConfig,
    ThresholdConfig
>;