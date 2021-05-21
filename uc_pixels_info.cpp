#include "uc_pixels_info.h"

UC_pixels_info::UC_pixels_info(UTE_pixels_area pixels_area) {
    this->pixels_area = pixels_area;
    U_reset();
    data_enable.id1_enable = true;
    data_enable.min_enable = true;
    data_enable.max_enable = true;
    data_enable.sum_enable = true;
    data_enable.mean_enable = true;
    data_enable.median_enable = true;
    data_enable.std_dev_enable = true;
    data_enable.zeros_enable = true;
    data_enable.snr_enable = true;
}

void UC_pixels_info::U_reset() {
    n = 0;
    min = 99999999999;
    max = -99999999999;
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

void UC_pixels_info::U_add_pixel_1(double z) {
    n++;
    if (data_enable.min_enable) if (min > z) min = z;
    if (data_enable.max_enable) if (max < z) max = z;
    if (data_enable.sum_enable | data_enable.mean_enable | data_enable.snr_enable) sum += z;
    if (data_enable.zeros_enable) if (qAbs(z) < 1e-10) zeros++;
    if (data_enable.median_enable) data.push_back(z);
}

void UC_pixels_info::U_add_masked() {
    masked++;
}

void UC_pixels_info::U_add_overflow() {
    overflows++;
}

void UC_pixels_info::U_finalize_1() {
    std::sort(data.begin(), data.end());
    if (n == 0) {
        mean = 0;
        median = 0;
    } else {
        if (data_enable.mean_enable | data_enable.snr_enable) mean = sum / n;
        if (data_enable.median_enable) {
            if ((n % 2) == 0) {
                median = data[static_cast<unsigned long>(n / 2)];
            } else {
                int n1 = n / 2;
                double z1 = data[static_cast<unsigned long>(n1)];
                double z2 = data[static_cast<unsigned long>(n1 + 1)];
                median = (z1 + z2) / 2;
            }
        }

    }
    if (data_enable.id1_enable) {
        if (qAbs(id_1_data_2) < 1e-10) {
            id_1_data_1 = 0;
        } else {
            id_1_data_1 /= id_1_data_2;
        }
    }
}

void UC_pixels_info::U_add_pixel_id_1(double z) {
    id_1_data_1 += z;
}

void UC_pixels_info::U_add_pixel_id_2(double z) {
    id_1_data_2 += z;
}
////
void UC_pixels_info::U_add_pixel_2(double z) {
    if (data_enable.std_dev_enable | data_enable.snr_enable) std_dev += (mean - z) * (mean - z);
}

void UC_pixels_info::U_finalize_2() {
    if ((n == 1) || (n == 0)) {
        std_dev = 0;
        snr = 0;
    } else {
        if (data_enable.std_dev_enable) std_dev = qSqrt(std_dev / (n - 1));
        if (data_enable.snr_enable) snr = qSqrt(mean) / std_dev;
    }
}
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

double UC_pixels_info::U_get_id_1_data() {
    return id_1_data_1;
}

void UC_pixels_info::U_set_data_enable(UTStr_data_enable enable) {
    data_enable = enable;
}


