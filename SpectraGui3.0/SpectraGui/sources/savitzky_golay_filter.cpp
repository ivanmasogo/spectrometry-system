#include "headers/savitzky_golay_filter.h"
#include <gram_savitzky_golay/gram_savitzky_golay.h>
#include <iostream>
#include <algorithm>

std::vector<double> applyFilter(const std::vector<double>& data, std::size_t window_size, std::size_t poly_order, int deriv_order) {

    std::vector<double> normalized_data = normalize(data);
    // Window size is 2*m+1
    const size_t m = (window_size-1) /2;

    // Polynomial Order
    const size_t n = poly_order;

    // Initial Point Smoothing (ie evaluate polynomial at first point in the window)
    // Points are defined in range [-m;m]
    const size_t t = m;

    // Derivation order? 0: no derivation, 1: first derivative, 2: second derivative...
    const int d = deriv_order;

    gram_sg::SavitzkyGolayFilter filter(m, t, n, d);

    std::vector<double> filtered_data;

    // Filtrar los puntos de datos
    for (std::size_t i = 0; i < normalized_data.size(); ++i) {
        std::vector<double> window;

        for (std::size_t j = 0; j < window_size; ++j) {

            std::size_t index = i + j - m;

            if (index < 0) {

                index = 0; // Utiliza el valor del borde inicial
            }

            if (index >= normalized_data.size()) {

                index = normalized_data.size() - 1; // Utiliza el valor del borde final
            }
            window.push_back(normalized_data[index]);
        }

        double result = filter.filter(window);
        filtered_data.push_back(result);
    }

    // Desnormalizar los datos filtrados
    double min = *std::min_element(data.begin(), data.end());
    double max = *std::max_element(data.begin(), data.end());
    std::vector<double> denormalized_data = denormalize(filtered_data, min, max);

    return denormalized_data;
}

// Funcion para normalizar los valores en el rango [0, 1]
std::vector<double> normalize(const std::vector<double>& data) {
    double min = *std::min_element(data.begin(), data.end());
    double max = *std::max_element(data.begin(), data.end());
    std::vector<double> normalized_data;
    for (double value : data) {
        normalized_data.push_back((value - min) / (max - min));
    } return normalized_data;

}

// Funcion para desnormalizar los valores en el rango
std::vector<double> denormalize(const std::vector<double>& normalized_data, double original_min, double original_max) {
    std::vector<double> denormalized_data;
    for (double value : normalized_data) {
        denormalized_data.push_back(value * (original_max - original_min) + original_min);
    }
    return denormalized_data;

}
