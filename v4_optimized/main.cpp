#include <iostream>
#include <fstream>
#include <vector>
#include "Regressor.hpp"
#include <cmath>
#include <thread>

void processSample(const std::vector<double>& x, const std::vector<double>& y, int id, int degree) {
    Regressor reg;

    try {
        auto coeffs = reg.polynomialRegression(x, y, degree);
        std::vector<double> predicted;
        for (double xi : x) {
            double yi = 0;
            for (size_t j = 0; j < coeffs.size(); ++j) {
                yi += coeffs[j] * std::pow(xi, j);
            }
            predicted.push_back(yi);
        }

        double mse = reg.meanSquaredError(y, predicted);

        std::cout << "Thread #" << id << " - Katsayılar: ";
        for (double c : coeffs) std::cout << c << " ";
        std::cout << "\nThread #" << id << " - MSE: " << mse << "\n\n";

    } catch (const std::exception& e) {
        std::cerr << "Thread #" << id << " - Hata: " << e.what() << "\n";
    }
}

int main() {
    std::ifstream file("data_2.5M_quadratic.txt");
    if (!file) {
        std::cerr << "data.txt dosyası açılamadı.\n";
        return 1;
    }

    std::vector<double> x, y;
    double xVal, yVal;
    while (file >> xVal >> yVal) {
        x.push_back(xVal);
        y.push_back(yVal);
    }
    file.close();

    Regressor reg;
    int sampleCount = std::thread::hardware_concurrency(); //  tüm işlemci çekirdeklerini verimli kullanmak için
    if (sampleCount == 0) sampleCount = 4; // fallback
    int sampleSize = x.size() / 2;
    int degree = 2;

    std::vector<std::pair<std::vector<double>, std::vector<double>>> samples;

    // Farklı örneklem kümeleri oluştur
    for (int i = 0; i < sampleCount; ++i) {
        samples.push_back(reg.sampleData(x, y, sampleSize));
    }

    // Thread'leri başlat
    std::vector<std::thread> threads;
    for (int i = 0; i < sampleCount; ++i) {
        threads.emplace_back(processSample, samples[i].first, samples[i].second, i + 1, degree);
    }

    // Thread'leri join et
    for (auto& t : threads) {
        t.join();
    }


    try {
        auto [m, b] = reg.linearRegression(x, y);
        std::cout << "Lineer regresyon sonucu:\n";
        std::cout << "Eğim (m): " << m << "\n";
        std::cout << "Kesişim (b): " << b << "\n";


    	// Tahmin edilen y değerlerini oluştur. Lineer Regresyon Tahmini
    	std::vector<double> predictedY;
    	for (size_t i = 0; i < x.size(); ++i) {
        	predictedY.push_back(m * x[i] + b);
    	}
    	double mse = reg.meanSquaredError(y, predictedY);
    	std::cout << "Ortalama karesel hata (MSE): " << mse << "\n";


    	// Polinomal regresyon testi
    	// int degree = 2; // polinom derecesi tekrar tanımlamaya gerek yok
    	auto coeffs = reg.polynomialRegression(x, y, degree);

    	std::cout << "\nPolinomial regresyon sonucu (derece " << degree << "):\n";
    	std::cout << "Katsayılar (en düşük dereceden): ";
    	for (double c : coeffs) {
        	std::cout << c << " ";
    	}
    	std::cout << "\n";

    	// Tahmin edilen y değerleri
    	std::vector<double> polyPredicted;
    	for (size_t i = 0; i < x.size(); ++i) {
        	double yi = 0.0;
        	for (size_t j = 0; j < coeffs.size(); ++j) {
            	yi += coeffs[j] * std::pow(x[i], j);
        	}
        	polyPredicted.push_back(yi);
    	}

    	double polyMSE = reg.meanSquaredError(y, polyPredicted);
    	std::cout << "Polinomial regresyon MSE: " << polyMSE << "\n";


        // Sample data ile polinomial regresyon testi.
        int sampleSize = 50000; // Örnek alınacak veri sayısı
        try {
            auto samplePair = reg.sampleData(x, y, sampleSize);
            std::vector<double> sampledX = samplePair.first;
            std::vector<double> sampledY = samplePair.second;

            auto coeffs = reg.polynomialRegression(sampledX, sampledY, degree); // degree = 2

            std::vector<double> samplePredicted;
            for (size_t i = 0; i < sampledX.size(); ++i) {
                double yi = 0.0;
                for (size_t j = 0; j < coeffs.size(); ++j) {	
                    yi += coeffs[j] * std::pow(sampledX[i], j);
                }
                samplePredicted.push_back(yi);
            }

            double sampleMSE = reg.meanSquaredError(sampledY, samplePredicted);

            std::cout << "\nSample veriden polinomial regresyon sonucu:\n";
            std::cout << "Katsayılar (en düşük dereceden): ";
            for (double c : coeffs) std::cout << c << " ";
            std::cout << "\nSample veride MSE: " << sampleMSE << "\n";

        } catch (const std::exception& e) {
            std::cerr << "Sample veri işlem hatası: " << e.what() << "\n";
        }


    } catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << "\n";
        return 1;
    }


    return 0;
}
