#include "Regressor.hpp"
#include <numeric>   // std::accumulate
#include <stdexcept> // hata fırlatmak için
#include <cmath> // pow()
#include <iostream>
#include <random>
#include <algorithm>


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


// Recursive fonksiyon: Gauss eleme ile çözüm
std::vector<double> solveRecursive(std::vector<std::vector<double>>& mat, std::vector<double>& vec, int n) {
    if (n == 1) {
        return {vec[0] / mat[0][0]};
    }

    // Pivotlama
    for (int i = 1; i < n; ++i) {
        double factor = mat[i][0] / mat[0][0];
        for (int j = 0; j < n; ++j) {
            mat[i][j] -= factor * mat[0][j];
        }
        vec[i] -= factor * vec[0];
    }

    // Alt matris oluştur
    std::vector<std::vector<double>> subMat(n - 1, std::vector<double>(n - 1));
    std::vector<double> subVec(n - 1);
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            subMat[i - 1][j - 1] = mat[i][j];
        }
        subVec[i - 1] = vec[i];
    }

    // Rekürsif çözüm
    std::vector<double> subSolution = solveRecursive(subMat, subVec, n - 1);

    // En baştaki bilinmeyeni çöz
    double sum = 0;
    for (int j = 1; j < n; ++j) {
        sum += mat[0][j] * subSolution[j - 1];
    }

    double x0 = (vec[0] - sum) / mat[0][0];

    // Çözüm vektörünü oluştur
    std::vector<double> result = {x0};
    result.insert(result.end(), subSolution.begin(), subSolution.end());

    return result;
}

std::vector<double> Regressor::polynomialRegression(const std::vector<double>& x, const std::vector<double>& y, int degree) {
    int n = degree + 1;
    std::vector<std::vector<double>> X(n, std::vector<double>(n, 0.0));
    std::vector<double> Y(n, 0.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (size_t k = 0; k < x.size(); ++k) {
                X[i][j] += std::pow(x[k], i + j);
            }
        }
        for (size_t k = 0; k < x.size(); ++k) {
            Y[i] += std::pow(x[k], i) * y[k];
        }
    }

    return solveRecursive(X, Y, n);
}


// Rastgele örnekleme fonksiyonu
std::pair<std::vector<double>, std::vector<double>> Regressor::sampleData(
    const std::vector<double>& x,
    const std::vector<double>& y,
    int sampleSize
) {
    if (x.size() != y.size() || x.empty() || sampleSize <= 0 || sampleSize > x.size()) {
        throw std::invalid_argument("Geçersiz örnekleme parametreleri.");
    }

    std::vector<int> indices(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        indices[i] = i;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    std::vector<double> sampledX, sampledY;
    for (int i = 0; i < sampleSize; ++i) {
        sampledX.push_back(x[indices[i]]);
        sampledY.push_back(y[indices[i]]);
    }

    return {sampledX, sampledY};
}
