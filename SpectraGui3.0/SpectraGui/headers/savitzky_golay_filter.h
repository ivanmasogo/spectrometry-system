#ifndef SAVITZKY_GOLAY_FILTER_H
#define SAVITZKY_GOLAY_FILTER_H

#include <vector>

std::vector<double> applyFilter(const std::vector<double>& data, std::size_t window_size, std::size_t poly_order, int deriv_order);
std::vector<double> normalize(const std::vector<double>& data);
std::vector<double> denormalize(const std::vector<double>& normalized_data, double original_min, double original_max);
#endif // SAVITZKY_GOLAY_FILTER_H
