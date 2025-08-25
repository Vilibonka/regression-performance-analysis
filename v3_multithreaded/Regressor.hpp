#ifndef REGRESSOR_HPP
#define REGRESSOR_HPP

#include <vector>
#include <utility>  // std::pair
#include <mutex>

class Regressor {
public:
    // Data sampling: veri kümesinden örnekleme
    std::pair<std::vector<double>, std::vector<double>> sampleData(
        const std::vector<double>& x,
        const std::vector<double>& y,
        int sampleSize
    );

    // Lineer regresyon (y = mx + b)
    std::pair<double, double> linearRegression(
        const std::vector<double>& x,
        const std::vector<double>& y
    );

    // Polinomial regresyon (y = ax^2 + bx + c...), n. dereceye kadar
    std::vector<double> polynomialRegression(
        const std::vector<double>& x,
        const std::vector<double>& y,
        int degree
    );

    // Ortalama karesel hata (MSE)
    double meanSquaredError(
        const std::vector<double>& actual,
        const std::vector<double>& predicted
    );

private:
    std::mutex mtx; // Çoklu iş parçacığında güvenli erişim için
};

#endif
