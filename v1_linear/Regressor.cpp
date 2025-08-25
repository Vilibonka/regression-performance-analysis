#include "Regressor.hpp"
#include <numeric>   // std::accumulate
#include <stdexcept> // hata fırlatmak için

// Lineer regresyon fonksiyonu: y = mx + b
std::pair<double, double> Regressor::linearRegression(
    const std::vector<double>& x,
    const std::vector<double>& y
) {
    if (x.size() != y.size() || x.empty()) {
        throw std::invalid_argument("Geçersiz veri boyutu.");
    }

    size_t n = x.size();
    double sumX = std::accumulate(x.begin(), x.end(), 0.0);
    double sumY = std::accumulate(y.begin(), y.end(), 0.0);

    double meanX = sumX / n;
    double meanY = sumY / n;

    double num = 0.0; // pay
    double den = 0.0; // payda

    for (size_t i = 0; i < n; ++i) {
        num += (x[i] - meanX) * (y[i] - meanY);
        den += (x[i] - meanX) * (x[i] - meanX);
    }

    double m = num / den;
    double b = meanY - m * meanX;

    return {m, b};
}


double Regressor::meanSquaredError(
    const std::vector<double>& actual,
    const std::vector<double>& predicted
) {
    if (actual.size() != predicted.size() || actual.empty()) {
        throw std::invalid_argument("Geçersiz veri boyutu.");
    }

    double sum = 0.0;
    for (size_t i = 0; i < actual.size(); ++i) {
        double diff = actual[i] - predicted[i];
        sum += diff * diff;
    }

    return sum / actual.size();
}
