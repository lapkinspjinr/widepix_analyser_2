#include "uc_pixels_info.h"

UC_roi::UC_roi(QString name, int x_min, int y_min, int x_max, int y_max) {
    this->name = name;
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
}

UC_roi::UC_roi() {
}

bool UC_roi::U_contains(int x, int y) {
    bool result;
    result = (x < x_max) && (x >= x_min);
    result &= (y < y_max) && (y >= y_min);
    return result;
}

void UC_roi::U_set_roi(QString name, int x_min, int y_min, int x_max, int y_max) {
    this->name = name;
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
}

QString UC_roi::U_get_name() {
    return name;
}

int UC_roi::U_get_x_min() {
    return x_min;
}

int UC_roi::U_get_y_min() {
    return y_min;
}

int UC_roi::U_get_x_max() {
    return x_max;
}

int UC_roi::U_get_y_max() {
    return y_max;
}

//

UC_pixels_info::UC_pixels_info(QString name, int x_min, int y_min, int x_max, int y_max)
                : UC_roi(name, x_min, y_min, x_max, y_max) {

    U_reset();
}

UC_pixels_info::UC_pixels_info(UC_roi roi) {
    U_set_roi(roi.U_get_name(), roi.U_get_x_min(), roi.U_get_y_min(), roi.U_get_x_max(), roi.U_get_y_max());
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

void UC_pixels_info::U_add_pixel(double z, int x, int y, bool mask, bool overflow) {
    if (U_contains(x, y)) {
        if (mask) {
            masked++;
            return;
        }
        if (overflow) {
            overflows++;
            return;
        }
        data << z;
    }
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
    if (n == 0) {
        data.clear();
        return;
    }
    mean = sum / n;
    std::vector<double> data_stdv = data.toStdVector();
    std::sort(data_stdv.begin(), data_stdv.end());
    if ((n % 2) == 1) {
        median = data_stdv[static_cast<unsigned long>(n / 2)];
    } else {
        double z1 = data_stdv[static_cast<unsigned long>(n / 2 - 1)];
        double z2 = data_stdv[static_cast<unsigned long>(n / 2)];
        median = (z1 + z2) / 2;
    }
    max_density = data[0];
    if (n == 1) {
        data.clear();
        return;
    }
    for (int i = 0; i < n; i++) {
        std_dev += (mean - data[i]) * (mean - data[i]);
    }
    if (std_dev <= 0) return;
    variance = std_dev / (n - 1);
    std_dev = qSqrt(variance);
    if (mean < 0) return;
    snr = qSqrt(mean) / std_dev;

    const int density = 100;
    int bins;
    if (n < 100) bins = 20;
        else bins = n / density;
    TH1D * h = new TH1D("h", "h", bins, min, max);
    for (int i = 0; i < n; i++) {
        h->Fill(data[i]);
    }
    max_density = h->GetBinCenter(h->GetMaximumBin());
    delete h;

    data.clear();
}

////

////////////////////////////////////////////////////////////////////////////
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

double UC_pixels_info::U_get_max_density() {
    return max_density;
}

double UC_pixels_info::U_get_variance() {
    return variance;
}
//


