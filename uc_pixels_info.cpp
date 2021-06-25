#include "uc_pixels_info.h"

UC_pixels_info::UC_pixels_info(UTE_pixels_area pixels_area) {
    this->pixels_area = pixels_area;
    U_reset();
}

void UC_pixels_info::U_reset() {
    n = 0;
    min = 1e300;
    max = -1e300;
    sum = 0;
    mean = 0;
    median = 0;
    zeros = 0;
    overflows = 0;
    masked = 0;
    std_dev = 0;
    snr = 0;
    data.clear();
}

void UC_pixels_info::U_add_pixel(double z) {
    data << z;
}

void UC_pixels_info::U_add_masked() {
    masked++;
}

void UC_pixels_info::U_add_overflow() {
    overflows++;
}

void UC_pixels_info::U_finalize() {
    n = data.size();
    double z = 0;
    for (int i = 0; i < n; i++) {
        z = data[i];
        if (min > z) min = z;
        if (max < z) max = z;
        sum += z;
        if (qAbs(z) < 1e-10) zeros++;
    }
    if (n == 0) return;
    mean = sum / n;
    std::vector<double> data_stdv = data.toStdVector();
    std::sort(data_stdv.begin(), data_stdv.end());
    if ((n % 2) == 0) {
        median = data_stdv[static_cast<unsigned long>(n / 2)];
    } else {
        double z1 = data_stdv[static_cast<unsigned long>(n / 2)];
        double z2 = data_stdv[static_cast<unsigned long>(n / 2 + 1)];
        median = (z1 + z2) / 2;
    }
    if (n == 1) return;
    for (int i = 0; i < n; i++) {
        std_dev += (mean - data[i]) * (mean - data[i]);
    }
    if (std_dev <= 0) return;
    std_dev = qSqrt(std_dev / (n - 1));
    if (mean < 0) return;
    snr = qSqrt(mean) / std_dev;
}

////

////////////////////////////////////////////////////////////////////////////
UC_pixels_info::UTE_pixels_area UC_pixels_info::U_get_pixel_area() {
    return pixels_area;
}

int UC_pixels_info::U_get_n() {
    return n;
}

double UC_pixels_info::U_get_min() {
    return min;
}

double UC_pixels_info::U_get_max() {
    return max;
}

double UC_pixels_info::U_get_sum() {
    return sum;
}

double UC_pixels_info::U_get_mean() {
    return mean;
}

double UC_pixels_info::U_get_median() {
    return median;
}

int UC_pixels_info::U_get_zeros() {
    return zeros;
}

int UC_pixels_info::U_get_overflows() {
    return overflows;
}

int UC_pixels_info::U_get_masked() {
    return masked;
}

double UC_pixels_info::U_get_std_dev() {
    return std_dev;
}

double UC_pixels_info::U_get_snr() {
    return snr;
}

void UC_pixels_info::U_set_data_enable(UTStr_data_enable enable) {
    data_enable = enable;
}


