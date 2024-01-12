#include <fftw3.h>
#include <iostream>
#include <complex>
#include <vector>
#include <fstream>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    using namespace std;

    constexpr size_t SIZE = 8'000'000; 

    // Dosyadan veri okuma
    vector<int16_t> data(SIZE);
    ifstream file("IQdataName.bin", ios::binary); 
        if (!file) {
        cerr << "Dosya açılamadı." << endl;
        return 1;
    }
    file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * SIZE);

    // verileri karmaşık sayılara dönüştürür
    vector<complex<double>> complexData(SIZE/2);
    for (size_t i = 0; i < SIZE/2; i++) {
        complexData[i] = complex<double>(data[2*i], data[2*i+1]);
    }

    // planı oluşturur
    fftw_plan plan = fftw_plan_dft_1d(SIZE/2, reinterpret_cast<fftw_complex*>(complexData.data()), reinterpret_cast<fftw_complex*>(complexData.data()), FFTW_FORWARD, FFTW_ESTIMATE);

    // FFT'yi hesaplar
    fftw_execute(plan);

    // sonuçları işler
    vector<double> magnitude(SIZE/2);
    for (size_t i = 0; i < SIZE/2; i++) {
        magnitude[i] = abs(complexData[i]);
    }
    
    vector<double> Lspace = linspace(0., static_cast<double>(SIZE/2-1), SIZE/2);
    
    plot(Lspace,magnitude);
    
    hold(true);
    
    show();

    // Kaynakları serbest bırakır
    fftw_destroy_plan(plan);

    return 0;
}
