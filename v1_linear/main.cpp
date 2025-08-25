#include <iostream>
#include <fstream>
#include <vector>
#include "Regressor.hpp"
#include <cmath>

int main() {
    std::ifstream file("data.txt");
    if (!file) {
        std::cerr << "data.txt dosyası açılamadı.\n";
        return 1;
    }

    std::vector<double> x, y;
    double xVal, yVal;

    // Dosyadan x ve y verilerini oku
    while (file >> xVal >> yVal) {
        x.push_back(xVal);
        y.push_back(yVal);
    }

    file.close();

    Regressor reg;
    try {
        auto [m, b] = reg.linearRegression(x, y);
        std::cout << "Lineer regresyon sonucu:\n";
        std::cout << "Eğim (m): " << m << "\n";
        std::cout << "Kesişim (b): " << b << "\n";


    	// Tahmin edilen y değerlerini oluştur.
    	std::vector<double> predictedY;
    	for (size_t i = 0; i < x.size(); ++i) {
        	predictedY.push_back(m * x[i] + b);
    	}
    	double mse = reg.meanSquaredError(y, predictedY);
    	std::cout << "Ortalama karesel hata (MSE): " << mse << "\n";


    } catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
