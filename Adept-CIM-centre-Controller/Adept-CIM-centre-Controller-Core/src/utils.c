#include "utils.h"

int range_map(int input_min, int input_max, int output_min, int output_max,
              int input_value) {
  double slope = 1.0 * (output_max - output_min) / (input_max - input_min);
  return output_min + slope * (input_value - input_min);
}
