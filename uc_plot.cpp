#include "uc_plot.h"

UC_pixels_info::UC_pixels_info(UTE_pixels_area pixels_area) {
    this->pixels_area = pixels_area;
    U_reset();
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
    if (min > z) min = z;
    if (max < z) max = z;
    sum += z;
    if (qAbs(z) < 1e-10) zeros++;
    data.push_back(z);
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
        mean = sum / n;
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
////
void UC_pixels_info::U_add_pixel_2(double z) {
    std_dev += (mean - z) * (mean - z);
}

void UC_pixels_info::U_finalize_2() {
    if ((n == 1) || (n == 0)) {
        std_dev = 0;
        snr = 0;
    } else {
        std_dev = qSqrt(std_dev / (n - 1));
        snr = qSqrt(mean) / std_dev;
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
///////////////////////////////////////////////////////////////////////////////////////////////


UC_plot::UC_plot(QObject *parent) : QObject(parent) {
    both_counters = true;
    count = 1; count_df = 1; count_ff = 1;
    time = 1; time_df = 1; time_ff = 1;
    scale_df = 1; scale_ff = 1;
    n_thl = 0;
    x_min = 0; x_max = 15 * 256; y_min = 0; y_max = 256;

    frame_type = UTE_FT_average;
    pixel_type = UTE_PT_cnt1;
//    detection_type = UTE_DT_max;
//    fitting_type = UTE_FT_without_fitting;

    U_reset_mask();
    for (int i = 0; i < 256 * 256 * 15; i++) {
        data_cnt0_mean[i] = 0;
        data_df_cnt0_mean[i] = 0;
        data_ff_cnt0_mean[i] = 0;
    }
}

UC_plot::~UC_plot() {
    delete [] data;
    delete [] data_ff;
    delete [] data_df;
    delete [] data_x;
    delete [] ff_minus_df_mean;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int UC_plot::U_get_coord_x_chip(int chip, int x) {
    return chip * 256 + x;
}

int UC_plot::U_get_chip_coord_x(int x) {
    return x / 255;
}

int UC_plot::U_get_coord_x_in_chip(int x) {
    return x % 255;
}
///////////////////////////////////////////////////////////////////////////////////////////////

int UC_plot::U_get_data(int thl_index, int x, int y, int th) {
    return data[thl_index * 256 * 15 * 256 * 2 + y * 15 * 256 * 2 + x * 2 + th];
}

void UC_plot::U_set_data(int thl_index, int x, int y, int th, int cnt_data) {
    int index = thl_index * 256 * 15 * 256 * 2 + y * 15 * 256 * 2 + x * 2 + th;
    data[index] = cnt_data;
}
///
int UC_plot::U_get_data_x(int thl_index) {
    return data_x[thl_index];
}

void UC_plot::U_set_data_x(int thl_index, int thl) {
    data_x[thl_index] = thl;
}
///
int UC_plot::U_get_data_ff(int thl_index, int x, int y, int th) {
    return data_ff[thl_index * 256 * 15 * 256 * 2 + y * 15 * 256 * 2 + x * 2 + th];
}

void UC_plot::U_set_data_ff(int thl_index, int x, int y, int th, int cnt_data) {
    int index = thl_index * 256 * 15 * 256 * 2 + y * 15 * 256 * 2 + x * 2 + th;
    data_ff[index] = cnt_data;
}
///
int UC_plot::U_get_data_df(int thl_index, int x, int y, int th) {
    return data_df[thl_index * 256 * 15 * 256 * 2 + y * 15 * 256 * 2 + x * 2 + th];
}

void UC_plot::U_set_data_df(int thl_index, int x, int y, int th, int cnt_data) {
    int index = thl_index * 256 * 15 * 256 * 2 + y * 15 * 256 * 2 + x * 2 + th;
    data_df[index] = cnt_data;
}
///////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_data_mem_alloc(int thl_counts) {
    int n = thl_counts * 256 * 15 * 256 * 2;
    data = new int[n];
    for (int i = 0; i < n; i++) {
        data[i] = 0;
    }
}

void UC_plot::U_data_x_mem_alloc(int thl_counts) {
    int n = thl_counts;
    data_x = new int[n];
    for (int i = 0; i < n; i++) {
        data_x[i] = 0;
    }
}

void UC_plot::U_data_df_mem_alloc(int thl_counts) {
    int n = thl_counts * 256 * 15 * 256 * 2;
    data_df = new int[n];
    for (int i = 0; i < n; i++) {
        data_df[i] = 0;
    }
}

void UC_plot::U_data_ff_mem_alloc(int thl_counts) {
    int n = thl_counts * 256 * 15 * 256 * 2;
    data_ff = new int[n];
    for (int i = 0; i < n; i++) {
        data_ff[i] = 0;
    }
}
////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_add_data_cnt0_mean(int x, int y, int cnt_data) {
    int index = y * 15 * 256 + x;
    data_cnt0_mean[index] += cnt_data;
}

void UC_plot::U_divide_data_cnt0_mean() {
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < (15 * 256); x++) {
            int index = y * 15 * 256 + x;
            data_cnt0_mean[index] /= n_thl;
        }
    }
}

double UC_plot::U_get_data_cnt0_mean(int x, int y) {
    int index = y * 15 * 256 + x;
    return data_cnt0_mean[index];
}
///
void UC_plot::U_add_data_df_cnt0_mean(int x, int y, int cnt_data) {
    int index = y * 15 * 256 + x;
    data_df_cnt0_mean[index] += cnt_data;
}

void UC_plot::U_divide_data_df_cnt0_mean() {
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < (15 * 256); x++) {
            int index = y * 15 * 256 + x;
            data_df_cnt0_mean[index] /= n_thl;
        }
    }
}

double UC_plot::U_get_data_df_cnt0_mean(int x, int y) {
    int index = y * 15 * 256 + x;
    return data_df_cnt0_mean[index];
}
///
void UC_plot::U_add_data_ff_cnt0_mean(int x, int y, int cnt_data) {
    int index = y * 15 * 256 + x;
    data_ff_cnt0_mean[index] += cnt_data;
}

void UC_plot::U_divide_data_ff_cnt0_mean() {
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < (15 * 256); x++) {
            int index = y * 15 * 256 + x;
            data_ff_cnt0_mean[index] /= n_thl;
        }
    }
}

double UC_plot::U_get_data_ff_cnt0_mean(int x, int y) {
    int index = y * 15 * 256 + x;
    return data_ff_cnt0_mean[index];
}
//////////////////////////////////////////////////////////////////////////////////////

double UC_plot::U_get_data_corrected(int thl_index, int x, int y) {
    int cnt0 = U_get_data(thl_index, x, y, 0);
    if (cnt0 == 0) return 0;
    double data_corrected = U_get_data(thl_index, x, y, 1) * U_get_data_cnt0_mean(x, y);
    data_corrected /= U_get_data(thl_index, x, y, 0);
    return data_corrected;
}

double UC_plot::U_get_data_ff_corrected(int thl_index, int x, int y) {
    int cnt0 = U_get_data_ff(thl_index, x, y, 0);
    if (cnt0 == 0) return 0;
    double data_corrected = U_get_data_ff(thl_index, x, y, 1) * U_get_data_ff_cnt0_mean(x, y);
    data_corrected /= U_get_data_ff(thl_index, x, y, 0);
    return data_corrected;
}

double UC_plot::U_get_data_df_corrected(int thl_index, int x, int y) {
    int cnt0 = U_get_data_df(thl_index, x, y, 0);
    if (cnt0 == 0) return 0;
    double data_corrected = U_get_data_df(thl_index, x, y, 1) * U_get_data_df_cnt0_mean(x, y);
    data_corrected /= U_get_data_df(thl_index, x, y, 0);
    return data_corrected;
}
////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_alloc_ff_minus_df_mean() {
    ff_minus_df_mean = new double[n_thl * 15];
    for (int i = 0; i < 15 * n_thl; i++) {
        ff_minus_df_mean[i] = 0;
    }
}

void UC_plot::U_make_ff_minus_df_mean() {
    for (int chip = 0; chip < 15; chip++) {
        for (int thl_index = 0; thl_index < n_thl; thl_index++) {
            ff_minus_df_mean[chip + 15 * thl_index] = 0;
            for (int x = 0; x < 256; x++) {
                for (int y = 0; y < 256; y++) {
                    ff_minus_df_mean[chip + 15 * thl_index] += U_get_data_ff(thl_index, U_get_coord_x_chip(chip, x), y, 1) * scale_ff - U_get_data_df(thl_index, U_get_coord_x_chip(chip, x), y, 1) * scale_df;
                }
            }
            ff_minus_df_mean[chip + 15 * thl_index] /= (256 * 256);
        }
    }
}

double UC_plot::U_get_ff_minus_df_mean(int thl_index, int x) {
    return ff_minus_df_mean[U_get_chip_coord_x(x) + 15 * thl_index];
}
////////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_set_mask(int x, int y, bool value) {
    mask[y * 256 * 15 + x] = value;
}

bool UC_plot::U_get_mask(int x, int y) {
    return mask[y * 256 * 15 + x];
}

////////////////////////////////////////////////////////////////////////////////////////////////
double UC_plot::U_get_pixel_data(UTE_pixel_type type, int thl_index, int x, int y) {
    switch (type) {
        case UTE_PT_cnt0 : {
            return U_get_pixel_data_1(thl_index, x, y);
        }
        case UTE_PT_cnt1 : {
            return U_get_pixel_data_2(thl_index, x, y);
        }
        case UTE_PT_cnt0ff : {
            return U_get_pixel_data_3(thl_index, x, y);
        }
        case UTE_PT_cnt1ff : {
            return U_get_pixel_data_4(thl_index, x, y);
        }
        case UTE_PT_cnt0df : {
            return U_get_pixel_data_5(thl_index, x, y);
        }
        case UTE_PT_cnt1df : {
            return U_get_pixel_data_6(thl_index, x, y);
        }
        case UTE_PT_cnt1_divide_on_cnt0 : {
            return U_get_pixel_data_7(thl_index, x, y);
        }
        case UTE_PT_cnt0_subtr_cnt1 : {
            return U_get_pixel_data_8(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1 : {
            return U_get_pixel_data_9(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1ff : {
            return U_get_pixel_data_10(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1df : {
            return U_get_pixel_data_11(thl_index, x, y);
        }
        case UTE_PT_ffc : {
            return U_get_pixel_data_12(thl_index, x, y);
        }
        case UTE_PT_mu : {
            return U_get_pixel_data_13(thl_index, x, y);
        }
        case UTE_PT_diff_ffc : {
            return U_get_pixel_data_14(thl_index, x, y);
        }
        case UTE_PT_diff_mu : {
            return U_get_pixel_data_15(thl_index, x, y);
        }
        case UTE_PT_ffc_diff : {
            return U_get_pixel_data_16(thl_index, x, y);
        }
        case UTE_PT_mu_diff : {
            return U_get_pixel_data_17(thl_index, x, y);
        }
        ////
        case UTE_PT_cnt1_corr_cnt0 : {
            return U_get_pixel_data_18(thl_index, x, y);
        }
        case UTE_PT_cnt1ff_corr_cnt0 : {
            return U_get_pixel_data_19(thl_index, x, y);
        }
        case UTE_PT_cnt1df_corr_cnt0 : {
            return U_get_pixel_data_20(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1_corr_cnt0 : {
            return U_get_pixel_data_21(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1ff_corr_cnt0 : {
            return U_get_pixel_data_22(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1df_corr_cnt0 : {
            return U_get_pixel_data_23(thl_index, x, y);
        }
        case UTE_PT_ffc_corr_cnt0 : {
            return U_get_pixel_data_24(thl_index, x, y);
        }
        case UTE_PT_mu_corr_cnt0 : {
            return U_get_pixel_data_25(thl_index, x, y);
        }
        case UTE_PT_diff_ffc_corr_cnt0 : {
            return U_get_pixel_data_26(thl_index, x, y);
        }
        case UTE_PT_diff_mu_corr_cnt0 : {
            return U_get_pixel_data_27(thl_index, x, y);
        }
        case UTE_PT_ffc_diff_corr_cnt0 : {
            return U_get_pixel_data_28(thl_index, x, y);
        }
        case UTE_PT_mu_diff_corr_cnt0 : {
            return U_get_pixel_data_29(thl_index, x, y);
        }
        ////
        case UTE_PT_mu_max : {
            return U_get_pixel_data_30(thl_index, x, y);
        }
        case UTE_PT_ffc_min : {
            return U_get_pixel_data_31(thl_index, x, y);
        }
        case UTE_PT_noise_edge : {
            return U_get_pixel_data_32(thl_index, x, y);
        }
        case UTE_PT_ffc_non_xray : {
            return U_get_pixel_data_33(thl_index, x, y);
        }
        case UTE_PT_cnt0_deviation : {
            return U_get_pixel_data_34(thl_index, x, y);
        }
        case UTE_PT_cnt0ff_deviation : {
            return U_get_pixel_data_35(thl_index, x, y);
        }
        case UTE_PT_cnt0df_deviation : {
            return U_get_pixel_data_36(thl_index, x, y);
        }
    }
    return 0;
}

int UC_plot::U_get_pixel_data_1(int thl_index, int x, int y) { //UTE_PT_cnt0
    return U_get_data(thl_index, x, y, 0);
}

int UC_plot::U_get_pixel_data_2(int thl_index, int x, int y) { //UTE_PT_cnt1
    return U_get_data(thl_index, x, y, 1);
}

int UC_plot::U_get_pixel_data_3(int thl_index, int x, int y) { //UTE_PT_cnt0ff
    return U_get_data_ff(thl_index, x, y, 0);
}

int UC_plot::U_get_pixel_data_4(int thl_index, int x, int y) { //UTE_PT_cnt1ff
    return U_get_data_ff(thl_index, x, y, 1);
}

int UC_plot::U_get_pixel_data_5(int thl_index, int x, int y) { //UTE_PT_cnt0df
    return U_get_data_df(thl_index, x, y, 0);
}

int UC_plot::U_get_pixel_data_6(int thl_index, int x, int y) { //UTE_PT_cnt1df
    return U_get_data_df(thl_index, x, y, 1);
}

double UC_plot::U_get_pixel_data_7(int thl_index, int x, int y) { //UTE_PT_cnt1_divide_on_cnt0
    int cnt0 = U_get_data(thl_index, x, y, 0);
    if (cnt0 == 0) return 0;
    return (static_cast<double>(U_get_data(thl_index, x, y, 1)) / cnt0);
}

int UC_plot::U_get_pixel_data_8(int thl_index, int x, int y) { //UTE_PT_cnt0_subtr_cnt1
    return (U_get_data(thl_index, x, y, 0) - U_get_data(thl_index, x, y, 1));
}

double UC_plot::U_get_pixel_data_9(int thl_index, int x, int y) { //UTE_PT_diff_cnt1
//    if (thl_index == (n_thl - 1)) return 0;
//    if (thl_index == 0) return 0;
//    double d = U_get_pixel_data_2(thl_index - 1, x, y) - U_get_pixel_data_2(thl_index + 1, x, y);
//    d /= 2;
//    return d;

    if (thl_index == (n_thl - 1)) {
        return U_get_pixel_data_2(thl_index, x, y);
    } else {
        return U_get_pixel_data_2(thl_index, x, y) - U_get_pixel_data_2(thl_index + 1, x, y);
    }
}

double UC_plot::U_get_pixel_data_10(int thl_index, int x, int y) { //UTE_PT_diff_cnt1ff
//    if (thl_index == (n_thl - 1)) return 0;
//    if (thl_index == 0) return 0;
//    double d = U_get_pixel_data_4(thl_index - 1, x, y) - U_get_pixel_data_4(thl_index + 1, x, y);
//    d /= 2;
//    return d;

    if (thl_index == (n_thl - 1)) {
        return U_get_pixel_data_4(thl_index, x, y);
    } else {
        return U_get_pixel_data_4(thl_index, x, y) - U_get_pixel_data_4(thl_index + 1, x, y);
    }
}

double UC_plot::U_get_pixel_data_11(int thl_index, int x, int y) { //UTE_PT_diff_cnt1df
//    if (thl_index == (n_thl - 1)) return 0;
//    if (thl_index == 0) return 0;
//    double d = U_get_pixel_data_6(thl_index - 1, x, y) - U_get_pixel_data_6(thl_index + 1, x, y);
//    d /= 2;
//    return d;

    if (thl_index == (n_thl - 1)) {
        return U_get_pixel_data_6(thl_index, x, y);
    } else {
        return U_get_pixel_data_6(thl_index, x, y) - U_get_pixel_data_6(thl_index + 1, x, y);
    }
}

double UC_plot::U_get_pixel_data_12(int thl_index, int x, int y) { //UTE_PT_ffc
    double ffc = 0;
    double delta_cnt1df = U_get_pixel_data_6(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_4(thl_index, x, y) * scale_ff - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    ffc = U_get_pixel_data_2(thl_index, x, y) - delta_cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_13(int thl_index, int x, int y) { //UTE_PT_mu
    double mu = 0;
    double delta_cnt1df = U_get_pixel_data_6(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_2(thl_index, x, y) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    mu = U_get_pixel_data_4(thl_index, x, y) * scale_ff - delta_cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return (mu);
}

double UC_plot::U_get_pixel_data_14(int thl_index, int x, int y) { //UTE_PT_diff_ffc
    if (thl_index == 0) return 0;
    if (thl_index == (n_thl - 1)) return 0;
    return U_get_pixel_data_12(thl_index + 1, x, y) - U_get_pixel_data_12(thl_index, x, y);
}

double UC_plot::U_get_pixel_data_15(int thl_index, int x, int y) { //UTE_PT_diff_mu
    if (thl_index == 0) return 0;
    if (thl_index == (n_thl - 1)) return 0;
    double mu = U_get_pixel_data_13(thl_index + 1, x, y) - U_get_pixel_data_13(thl_index, x, y);
    if (mu < 1e-10) return 0;
    return (mu);
}

double UC_plot::U_get_pixel_data_16(int thl_index, int x, int y) { //UTE_PT_ffc_diff
    double ffc = 0;
    double delta_cnt1df = U_get_pixel_data_11(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_10(thl_index, x, y) * scale_ff - delta_cnt1df;
    if (denom < 1e-10) return 0;
    ffc = U_get_pixel_data_9(thl_index, x, y) - delta_cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_17(int thl_index, int x, int y) { //UTE_PT_mu_diff
    double mu = 0;
    double delta_cnt1df = U_get_pixel_data_11(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_9(thl_index, x, y) - delta_cnt1df;
    if (denom < 1e-10) return 0;
    mu = U_get_pixel_data_10(thl_index, x, y) * scale_ff - delta_cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);
}
////

double UC_plot::U_get_pixel_data_18(int thl_index, int x, int y) { //UTE_PT_cnt1_corr_cnt0
    return U_get_data_corrected(thl_index, x, y);
}

double UC_plot::U_get_pixel_data_19(int thl_index, int x, int y) { //UTE_PT_cnt1ff_corr_cnt0
    return U_get_data_ff_corrected(thl_index, x, y);
}

double UC_plot::U_get_pixel_data_20(int thl_index, int x, int y) { //UTE_PT_cnt1df_corr_cnt0
    return U_get_data_df_corrected(thl_index, x, y);
}

double UC_plot::U_get_pixel_data_21(int thl_index, int x, int y) { //UTE_PT_diff_cnt1_corr_cnt0
//    if (thl_index == (n_thl - 1)) return 0;
//    if (thl_index == 0) return 0;
//    double d = U_get_pixel_data_18(thl_index - 1, x, y) - U_get_pixel_data_18(thl_index + 1, x, y);
//    d /= 2;
//    return d;

    if (thl_index == (n_thl - 1)) {
        return U_get_pixel_data_18(thl_index, x, y);
    } else {
        return U_get_pixel_data_18(thl_index, x, y) - U_get_pixel_data_18(thl_index + 1, x, y);
    }
}

double UC_plot::U_get_pixel_data_22(int thl_index, int x, int y) { //UTE_PT_diff_cnt1ff_corr_cnt0
//    if (thl_index == (n_thl - 1)) return 0;
//    if (thl_index == 0) return 0;
//    double d = U_get_pixel_data_19(thl_index - 1, x, y) - U_get_pixel_data_19(thl_index + 1, x, y);
//    d /= 2;
//    return d;

    if (thl_index == (n_thl - 1)) {
        return U_get_pixel_data_19(thl_index, x, y);
    } else {
        return U_get_pixel_data_19(thl_index, x, y) - U_get_pixel_data_19(thl_index + 1, x, y);
    }
}

double UC_plot::U_get_pixel_data_23(int thl_index, int x, int y) { //UTE_PT_diff_cnt1df_corr_cnt0
//    if (thl_index == (n_thl - 1)) return 0;
//    if (thl_index == 0) return 0;
//    double d = U_get_pixel_data_20(thl_index - 1, x, y) - U_get_pixel_data_20(thl_index + 1, x, y);
//    d /= 2;
//    return d;

    if (thl_index == (n_thl - 1)) {
        return U_get_pixel_data_20(thl_index, x, y);
    } else {
        return U_get_pixel_data_20(thl_index, x, y) - U_get_pixel_data_20(thl_index + 1, x, y);
    }
}

double UC_plot::U_get_pixel_data_24(int thl_index, int x, int y) { //UTE_PT_ffc_corr_cnt0
    double ffc = 0;
    double delta_cnt1df = U_get_pixel_data_20(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_19(thl_index, x, y) * scale_ff - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    ffc = U_get_pixel_data_18(thl_index, x, y) - delta_cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_25(int thl_index, int x, int y) { //UTE_PT_mu_corr_cnt0
    double mu = 0;
    double delta_cnt1df = U_get_pixel_data_20(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_18(thl_index, x, y) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    mu = U_get_pixel_data_19(thl_index, x, y) * scale_ff - delta_cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return (mu);
}


double UC_plot::U_get_pixel_data_26(int thl_index, int x, int y) { //UTE_PT_diff_ffc_corr_cnt0
    if (thl_index == 0) return 0;
    if (thl_index == (n_thl - 1)) return 0;
    return U_get_pixel_data_24(thl_index + 1, x, y) - U_get_pixel_data_24(thl_index, x, y);
}

double UC_plot::U_get_pixel_data_27(int thl_index, int x, int y) { //UTE_PT_diff_mu_corr_cnt0
    if (thl_index == 0) return 0;
    if (thl_index == (n_thl - 1)) return 0;
    double mu = U_get_pixel_data_25(thl_index + 1, x, y) - U_get_pixel_data_25(thl_index, x, y);
    if (mu < 1e-10) return 0;
    return (mu);
}

double UC_plot::U_get_pixel_data_28(int thl_index, int x, int y) { //UTE_PT_ffc_diff_corr_cnt0
    double ffc = 0;
    double delta_cnt1df = U_get_pixel_data_23(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_22(thl_index, x, y) * scale_ff - delta_cnt1df;
    if (denom < 1e-10) return 0;
    ffc = U_get_pixel_data_21(thl_index, x, y) - delta_cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_29(int thl_index, int x, int y) { //UTE_PT_mu_diff_corr_cnt0
    double mu = 0;
    double delta_cnt1df = U_get_pixel_data_23(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_21(thl_index, x, y) - delta_cnt1df;
    if (denom < 1e-10) return 0;
    mu = U_get_pixel_data_22(thl_index, x, y) * scale_ff - delta_cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);
}
///
int UC_plot::U_get_pixel_data_30(int thl_index, int x, int y) { //UTE_PT_mu_max
    if (thl_index == 0) return 0;
    if (thl_index == 1) return 0;
    if (thl_index == (n_thl - 1)) return 0;
    if (thl_index == (n_thl - 2)) return 0;
    double z0 = U_get_pixel_data_15(thl_index - 2, x, y);
    double z1 = U_get_pixel_data_15(thl_index - 1, x, y);
    double z2 = U_get_pixel_data_15(thl_index, x, y);
    double z3 = U_get_pixel_data_15(thl_index + 1, x, y);
    double z4 = U_get_pixel_data_15(thl_index + 2, x, y);
    if ((z2 > z0) && (z2 > z1) && (z2 > z3) && (z2 > z4)) return 1;
    return 0;
}

int UC_plot::U_get_pixel_data_31(int thl_index, int x, int y) { //UTE_PT_ffc_min
    if (thl_index == 0) return 0;
    if (thl_index == 1) return 0;
    if (thl_index == (n_thl - 1)) return 0;
    if (thl_index == (n_thl - 2)) return 0;
    double z0 = U_get_pixel_data_14(thl_index - 2, x, y);
    double z1 = U_get_pixel_data_14(thl_index - 1, x, y);
    double z2 = U_get_pixel_data_14(thl_index, x, y);
    double z3 = U_get_pixel_data_14(thl_index + 1, x, y);
    double z4 = U_get_pixel_data_14(thl_index + 2, x, y);
    if ((z2 < z0) && (z2 < z1) && (z2 < z3) && (z2 < z4)) return 1;
    return 0;
}

int UC_plot::U_get_pixel_data_32(int thl_index, int x, int y) { //UTE_PT_noise_edge
    if (thl_index == (n_thl - 1)) return 0;
    double z0 = U_get_pixel_data_2(thl_index, x, y);
    double z1 = U_get_pixel_data_2(thl_index + 1, x, y);
    if ((z0 > 10) && (z1 < 10)) return 1;
    return 0;
}

double UC_plot::U_get_pixel_data_33(int thl_index, int x, int y) { //UTE_PT_ffc_non_xray
    double ffc = 0;
    double delta_cnt1df = U_get_pixel_data_6(thl_index, x, y) * scale_df;
    double denom = U_get_pixel_data_4(thl_index, x, y) * scale_ff - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    ffc = U_get_pixel_data_2(thl_index, x, y) - delta_cnt1df;
    ffc /= denom;
    ffc *= U_get_ff_minus_df_mean(thl_index, x);
    return ffc;
}

double UC_plot::U_get_pixel_data_34(int thl_index, int x, int y) { //UTE_PT_cnt0_deviation
    double denom = U_get_data_cnt0_mean(x, y);
    if (qAbs(denom) < 1e-10) return 0;
    return U_get_pixel_data_1(thl_index, x, y) / denom;
}

double UC_plot::U_get_pixel_data_35(int thl_index, int x, int y) { //UTE_PT_cnt0ff_deviation
    double denom = U_get_data_ff_cnt0_mean(x, y);
    if (qAbs(denom) < 1e-10) return 0;
    return U_get_pixel_data_3(thl_index, x, y) / denom;
}

double UC_plot::U_get_pixel_data_36(int thl_index, int x, int y) { //UTE_PT_cnt0df_deviation
    double denom = U_get_data_df_cnt0_mean(x, y);
    if (qAbs(denom) < 1e-10) return 0;
    return U_get_pixel_data_5(thl_index, x, y) / denom;
}

/////////////////////////////////////////////////////////////////////////////////
double UC_plot::U_get_frame_data(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, int thl_index) {
    switch (type_spectra) {
        case UTE_FT_average : {
            return U_get_frame_data_1(type_pixel, thl_index);
        }
        case UTE_FT_sum : {
            return U_get_frame_data_2(type_pixel, thl_index);
        }
        case UTE_FT_median : {
            return U_get_frame_data_3(type_pixel, thl_index);
        }
        case UTE_FT_max : {
            return U_get_frame_data_4(type_pixel, thl_index);
        }
        case UTE_FT_min : {
            return U_get_frame_data_5(type_pixel, thl_index);
        }
        case UTE_FT_zeros : {
            return U_get_frame_data_6(type_pixel, thl_index);
        }
        case UTE_FT_overflowed : {
            return U_get_frame_data_7(thl_index);
        }
        case UTE_FT_standart_deviation : {
            return U_get_frame_data_8(type_pixel, thl_index);
        }
        case UTE_FT_signal_to_noise_resolution : {
            return U_get_frame_data_9(type_pixel, thl_index);
        }
    }
    return 0;
}

double UC_plot::U_get_frame_data_1(UTE_pixel_type type_pixel, int thl_index) { // UTE_FT_average
    double sum = 0;
    int cnt = 0;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (!U_get_mask(x, y)) {
                sum += U_get_pixel_data(type_pixel, thl_index, x, y);
                cnt++;
            }
        }
    }
    sum /= cnt;
    return sum;
}

double UC_plot::U_get_frame_data_2(UTE_pixel_type type_pixel, int thl_index) { // UTE_FT_sum
    double sum = 0;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (!U_get_mask(x, y)) sum += U_get_pixel_data(type_pixel, thl_index, x, y);
        }
    }
    return sum;
}



double UC_plot::U_get_frame_data_3(UTE_pixel_type type_pixel, int thl_index) { //UTE_FT_median
    std::vector<double> z;
    int cnt = 0;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (!U_get_mask(x, y)) {
                z.push_back(U_get_pixel_data(type_pixel, thl_index, x, y));
                cnt++;
            }
        }
    }
    std::sort(z.begin(), z.end());
    if ((cnt % 2) == 0) {
        cnt /= 2;
        return z[static_cast<unsigned long>(cnt)];
    } else {
        cnt /= 2;
        double z1 = z[static_cast<unsigned long>(cnt)];
        double z2 = z[static_cast<unsigned long>(cnt + 1)];
        return (z1 + z2) / 2;
    }
}

double UC_plot::U_get_frame_data_4(UTE_pixel_type type_pixel, int thl_index) { //UTE_FT_max
    double max = -9999999999999999, z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            z = U_get_pixel_data(type_pixel, thl_index, x, y);
            if (z > max) max = z;
        }
    }
    return max;
}

double UC_plot::U_get_frame_data_5(UTE_pixel_type type_pixel, int thl_index) { //UTE_FT_min
    double min = 9999999999999999, z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            z = U_get_pixel_data(type_pixel, thl_index, x, y);
            if (z < min) min = z;
        }
    }
    return min;
}

int UC_plot::U_get_frame_data_6(UTE_pixel_type type_pixel, int thl_index) { //UTE_FT_zeros
    int cnt = 0;
    double z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            z = U_get_pixel_data(type_pixel, thl_index, x, y);
            if (qAbs(z) < 1e-10) cnt++;
        }
    }
    return cnt;
}

int UC_plot::U_get_frame_data_7(int thl_index) { //UTE_FT_overflowed
    int cnt0, cnt1, cnt0ff, cnt1ff, cnt0df, cnt1df;
    int overflow, overflow_ff, overflow_df;
    bool overflowed;
    int cnt = 0;
    overflow = count * 4095;
    overflow_ff = count_ff * 4095;
    overflow_df = count_df * 4095;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            cnt0 = U_get_pixel_data_1(thl_index, x, y);
            cnt1 = U_get_pixel_data_2(thl_index, x, y);
            cnt0ff = U_get_pixel_data_3(thl_index, x, y);
            cnt1ff = U_get_pixel_data_4(thl_index, x, y);
            cnt0df = U_get_pixel_data_5(thl_index, x, y);
            cnt1df = U_get_pixel_data_6(thl_index, x, y);
            overflowed = (cnt0 == overflow) | (cnt1 == overflow);
            overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
            overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);
            if (overflowed) cnt++;
        }
    }
    return cnt;
}

double UC_plot::U_get_frame_data_8(UTE_pixel_type type_pixel, int thl_index) { //UTE_FT_standart_deviation
    int cnt = 0;
    double mean = U_get_frame_data_1(type_pixel, thl_index);
    double std_dev = 0, z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            z = U_get_pixel_data(type_pixel, thl_index, x, y);
            std_dev = (mean - z) * (mean - z);
            cnt++;
        }
    }
    if (cnt == 0) return 0;
    if (cnt == 1) return 0;
    std_dev = qSqrt(std_dev / (cnt - 1));
    return std_dev;
}

double UC_plot::U_get_frame_data_9(UTE_pixel_type type_pixel, int thl_index) { //UTE_FT_signal_to_noise_resolution
    double mean = U_get_frame_data_1(type_pixel, thl_index);
    if (mean < 0) return 0;
    double std_dev = U_get_frame_data_1(type_pixel, thl_index);
    if (qAbs(std_dev) < 1e-10) return 0;
    return qSqrt(mean) / std_dev;
}

double UC_plot::U_get_identification_data(UTE_identification_type type, int thl_index, int x, int y) {
    switch (type) {
        case UTE_IT_no_identification : {
            return U_get_pixel_data(pixel_type, thl_index, x, y);
        }
        case UTE_IT_1 : {
            return U_get_identification_data_1(x, y);
        }
    }
    return 0;
}

double UC_plot::U_get_identification_data_1(int x, int y) { //UTE_PT_cnt0
    double data_1 = U_get_pixel_data(pixel_type, thl_id_1, x, y);
    double data_2 = U_get_pixel_data(pixel_type, thl_id_2, x, y);
    double data_3 = U_get_pixel_data(pixel_type, thl_id_3, x, y);
    double data_4 = U_get_pixel_data(pixel_type, thl_id_4, x, y);
    double denom = data_3 - data_4;
    if (qAbs(denom) < 1e-10) return 0;
    return (data_1 - data_2) / denom;
}
///////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_renew_scale() {
    scale_ff = (count * time) / (count_ff * time_ff);
    scale_df = (count * time) / (count_df * time_df);
}

////////////////////////////!!!!!!!!!!SLOTS!!!!!!!!!!!///////////////////////////////////////////////////////////////////////////

void UC_plot::U_set_data(QString path, int count, double time, bool both_counters) {
    n_thl = 0;

    U_reset_mask();

    for (int i = 0; i < 256 * 256 * 15; i++) {
        data_cnt0_mean[i] = 0;
    }


    QFile file;
    QDir dir(path);
    QStringList file_names = dir.entryList(QDir::Files, QDir::Name);
    qulonglong pixel_data;
    QTextStream txt_str;
    QString str;
    bool first = true;

    this->count = count;
    this->time = time;
    U_renew_scale();

        int thl_counts = 0;
        for (int i = 0; i < file_names.size(); i++) {
            if (file_names[i].right(4) == ".pmf") {
                str = file_names[i].right(8);
                if (str == "THL0.pmf") continue;
                if (str == "THL1.pmf") continue;
                emit US_file_found(str);
                thl_counts++;
            }
        }

        n_thl = thl_counts;
        emit US_n_files(n_thl);

        U_data_mem_alloc(thl_counts);
        U_data_df_mem_alloc(thl_counts);
        U_data_ff_mem_alloc(thl_counts);
        U_data_x_mem_alloc(thl_counts);

        int thl_index = 0;
        int cnt0, cnt1, thl = 0;

        if (both_counters) {
            for (int i = 0; i < file_names.size(); i++) {
                if (file_names[i].right(4) == ".pmf") {
                    str = file_names[i].right(8);
                    if (str == "THL0.pmf") continue;
                    if (str == "THL1.pmf") continue;

                    file.setFileName(path + "/" + file_names[i]);
                    file.open(QFile::ReadOnly);
                    txt_str.setDevice(&file);

                    for (int y = 0; y < 256; y++) {
                        for (int x = 0; x < (15 * 256); x++) {
                            txt_str >> pixel_data;
                            cnt0 = static_cast<int>(pixel_data & 0xffffffff);
                            cnt1 = static_cast<int>((pixel_data >> 32) & 0xffffffff);
                            U_set_data(thl_index, x, y, 0, cnt0);
                            U_set_data(thl_index, x, y, 1, cnt1);
                            U_add_data_cnt0_mean(x, y, cnt0);
                        }
                    }


                    thl = file.fileName().right(7).left(3).toInt();
                    U_set_data_x(thl_index, thl);
                    emit US_new_thl(thl);

                    file.close();
                    emit US_renew_progress_bar(thl, n_thl);


                    if (first) {
                        emit US_thl_min(thl);
                        first = false;
                        thl_index_min = thl_index;
                    }
                    thl_index++;
                }
            }
            U_divide_data_cnt0_mean();
        } else {
            for (int i = 0; i < file_names.size(); i++) {
                if (file_names[i].right(4) == ".pmf") {
                    str = file_names[i].right(8);
                    if (str == "THL0.pmf") continue;
                    if (str == "THL1.pmf") continue;

                    file.setFileName(path + "/" + file_names[i]);
                    file.open(QFile::ReadOnly);
                    txt_str.setDevice(&file);

                    for (int y = 0; y < 256; y++) {
                        for (int x = 0; x < (15 * 256); x++) {
                            txt_str >> pixel_data;
                            cnt1 = static_cast<int>(pixel_data);
                            U_set_data(thl_index, x, y, 1, cnt1);
                        }
                    }

                    thl = file.fileName().right(7).left(3).toInt();
                    U_set_data_x(thl_index, thl);
                    emit US_new_thl(thl);

                    file.close();
                    emit US_renew_progress_bar(thl, n_thl);


                    if (first) {
                        emit US_thl_min(thl);
                        thl_index_min = thl_index;
                        first = false;
                    }
                    thl_index++;
                }
            }
        }
   emit US_thl_max(thl);
   thl_index_max = --thl_index;

}

void UC_plot::U_set_data_ff(QString path, int count, double time, bool both_counters) {
    for (int i = 0; i < 256 * 256 * 15; i++) {
        data_ff_cnt0_mean[i] = 0;
    }


    QFile file;
    QString file_name;
    quint64 pixel_data;
    QTextStream txt_str;

    this->count_ff = count;
    this->time_ff = time;
    U_renew_scale();

    int cnt0, cnt1;

    if (both_counters) {
        for (int thl_index = 0; thl_index < n_thl; thl_index++) {
            file_name = path + "/" + QString("%1.pmf").arg(U_get_data_x(thl_index), 3, 10, QChar('0'));
            file.setFileName(file_name);
            if (file.exists()) {
                file.open(QFile::ReadOnly);
                txt_str.setDevice(&file);

                for (int y = 0; y < 256; y++) {
                    for (int x = 0; x < (15 * 256); x++) {
                        txt_str >> pixel_data;
                        cnt0 = static_cast<int>(pixel_data & 0xffffffff);
                        cnt1 = static_cast<int>((pixel_data >> 32) & 0xffffffff);
                        U_set_data_ff(thl_index, x, y, 0, cnt0);
                        U_set_data_ff(thl_index, x, y, 1, cnt1);
                        U_add_data_ff_cnt0_mean(x, y, cnt0);
                    }
                }

                emit US_renew_progress_bar(thl_index, n_thl);
                file.close();
            }
            emit US_renew_progress_bar(thl_index, n_thl);
        }
        U_divide_data_ff_cnt0_mean();
        U_alloc_ff_minus_df_mean();
        U_make_ff_minus_df_mean();
    } else {
        for (int thl_index = 0; thl_index < n_thl; thl_index++) {
            file_name = path + "/" + QString("%1.pmf").arg(U_get_data_x(thl_index), 3, 10, QChar('0'));
            file.setFileName(file_name);
            if (file.exists()) {
                file.open(QFile::ReadOnly);
                txt_str.setDevice(&file);

                for (int y = 0; y < 256; y++) {
                    for (int x = 0; x < (15 * 256); x++) {
                        txt_str >> pixel_data;
                        cnt1 = static_cast<int>(pixel_data & 0xffffffff);
                        U_set_data_ff(thl_index, x, y, 1, cnt1);
                    }
                }

                file.close();
            }
            emit US_renew_progress_bar(thl_index, n_thl);
        }
        U_alloc_ff_minus_df_mean();
        U_make_ff_minus_df_mean();
    }
}

void UC_plot::U_set_data_df(QString path, int count, double time, bool both_counters) {
    for (int i = 0; i < 256 * 256 * 15; i++) {
        data_df_cnt0_mean[i] = 0;
    }

    QFile file;
    QString file_name;
    quint64 pixel_data;
    QTextStream txt_str;

    this->count_df = count;
    this->time_df = time;
    U_renew_scale();

    int cnt0, cnt1;

    if (both_counters) {
        for (int thl_index = 0; thl_index < n_thl; thl_index++) {
            file_name = path + "/" + QString("%1.pmf").arg(U_get_data_x(thl_index), 3, 10, QChar('0'));
            file.setFileName(file_name);
            if (file.exists()) {
                file.open(QFile::ReadOnly);
                txt_str.setDevice(&file);

                for (int y = 0; y < 256; y++) {
                    for (int x = 0; x < (15 * 256); x++) {
                        txt_str >> pixel_data;
                        cnt0 = static_cast<int>(pixel_data & 0xffffffff);
                        cnt1 = static_cast<int>((pixel_data >> 32) & 0xffffffff);
                        U_set_data_df(thl_index, x, y, 0, cnt0);
                        U_set_data_df(thl_index, x, y, 1, cnt1);
                        U_add_data_df_cnt0_mean(x, y, cnt0);
                    }
                }


                file.close();
            }
            emit US_renew_progress_bar(thl_index, n_thl);
        }
        U_divide_data_df_cnt0_mean();
        U_make_ff_minus_df_mean();
    } else {
        for (int thl_index = 0; thl_index < n_thl; thl_index++) {
            file_name = path + "/" + QString("%1.pmf").arg(U_get_data_x(thl_index), 3, 10, QChar('0'));
            file.setFileName(file_name);
            if (file.exists()) {
                file.open(QFile::ReadOnly);
                txt_str.setDevice(&file);

                for (int y = 0; y < 256; y++) {
                    for (int x = 0; x < (15 * 256); x++) {
                        txt_str >> pixel_data;
                        cnt1 = static_cast<int>(pixel_data & 0xffffffff);
                        U_set_data_df(thl_index, x, y, 1, cnt1);
                    }
                }
                emit US_renew_progress_bar(thl_index, n_thl);
                file.close();
            }
            emit US_renew_progress_bar(thl_index, n_thl);
        }
        U_make_ff_minus_df_mean();
    }
}

void UC_plot::U_reset_data() {
    delete [] data;
    delete [] data_ff;
    delete [] data_df;
    delete [] data_x;

    count = 1; count_df = 1; count_ff = 1;
    time = 1; time_df = 1; time_ff = 1;
    scale_df = 1; scale_ff = 1;
    n_thl = 0;

    U_reset_mask();

    for (int i = 0; i < 256 * 256 * 15; i++) {
        data_cnt0_mean[i] = 0;
        data_df_cnt0_mean[i] = 0;
        data_ff_cnt0_mean[i] = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_plot::U_generate_spectra() {
    double data_y;
    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        data_y = U_get_frame_data(frame_type, pixel_type, thl_index);
        emit US_plot_data(U_get_data_x(thl_index), data_y);
        emit US_renew_progress_bar(thl_index - thl_index_min, thl_index_max - thl_index_min);
    }
    emit US_replot_spectra();
}

void UC_plot::U_generate_frame(int thl_index) {
    double z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) {
                emit US_frame_data(x, y, 0);
            } else {
                z = U_get_identification_data(identification_type, thl_index, x, y);
                emit US_frame_data(x, y, z);
            }
        }
    }
    emit US_replot_frame();
}

void UC_plot::U_generate_table(int thl_index) {
    double z;
    int cnt0, cnt1, cnt0ff, cnt1ff, cnt0df, cnt1df;
    int overflow, overflow_ff, overflow_df;
    int x_widepix;
    int total = 256 * 256 * 15 * 2;
    bool in_roi, mask, overflowed;
    UC_pixels_info widepix(UC_pixels_info::UTE_PA_widepix);
    UC_pixels_info roi(UC_pixels_info::UTE_PA_roi);
    UC_pixels_info masked(UC_pixels_info::UTE_PA_masked);
    UC_pixels_info unmasked(UC_pixels_info::UTE_PA_unmasked);
    UC_pixels_info not_overflow(UC_pixels_info::UTE_PA_not_overflow);
    UC_pixels_info good_pixels_in_roi(UC_pixels_info::UTE_PA_roi_unmasked_not_ovf);
    UC_pixels_info* chip_info[15];

    overflow = count * 4095;
    overflow_ff = count_ff * 4095;
    overflow_df = count_df * 4095;

    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip] = new UC_pixels_info(static_cast<UC_pixels_info::UTE_pixels_area>(chip));
    }

    int i = 0;
    for (int chip = 0; chip < 15; chip++) {
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                x_widepix = U_get_coord_x_chip(chip, x);
                z = U_get_identification_data(identification_type, thl_index, x_widepix, y);

                widepix.U_add_pixel_1(z);
                chip_info[chip]->U_add_pixel_1(z);

                mask = U_get_mask(x_widepix, y);
                in_roi = (x_widepix >= x_min) & (x_widepix < x_max) & (y >= y_min) & (y < y_max);
                cnt0 = U_get_pixel_data_1(thl_index, x_widepix, y);
                cnt1 = U_get_pixel_data_2(thl_index, x_widepix, y);
                cnt0ff = U_get_pixel_data_3(thl_index, x_widepix, y);
                cnt1ff = U_get_pixel_data_4(thl_index, x_widepix, y);
                cnt0df = U_get_pixel_data_5(thl_index, x_widepix, y);
                cnt1df = U_get_pixel_data_6(thl_index, x_widepix, y);
                overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
                overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);

                if (mask) {
                    widepix.U_add_masked();
                    chip_info[chip]->U_add_masked();
                    masked.U_add_pixel_1(z);
                    masked.U_add_masked();
                    if (overflowed) masked.U_add_overflow();
                } else {
                    unmasked.U_add_pixel_1(z);
                    if (overflowed) unmasked.U_add_overflow();
                }
                if (overflowed) {
                    widepix.U_add_overflow();
                    chip_info[chip]->U_add_overflow();
                } else {
                    not_overflow.U_add_pixel_1(z);
                    if (mask) not_overflow.U_add_masked();
                }
                if (in_roi) {
                    roi.U_add_pixel_1(z);
                    if (mask) roi.U_add_masked();
                    if (overflowed) roi.U_add_overflow();
                    if (!(overflowed | mask)) good_pixels_in_roi.U_add_pixel_1(z);
                }
                i++;
                emit US_renew_progress_bar(i, total);

            }
        }
    }

    widepix.U_finalize_1();
    roi.U_finalize_1();
    masked.U_finalize_1();
    unmasked.U_finalize_1();
    not_overflow.U_finalize_1();
    good_pixels_in_roi.U_finalize_1();
    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip]->U_finalize_1();
    }

    for (int chip = 0; chip < 15; chip++) {
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                x_widepix = U_get_coord_x_chip(chip, x);
                z = U_get_identification_data(identification_type, thl_index, x_widepix, y);

                widepix.U_add_pixel_2(z);
                chip_info[chip]->U_add_pixel_2(z);

                mask = U_get_mask(x_widepix, y);
                in_roi = (x_widepix >= x_min) & (x_widepix < x_max) & (y >= y_min) & (y < y_max);
                cnt0 = U_get_pixel_data_1(thl_index, x_widepix, y);
                cnt1 = U_get_pixel_data_2(thl_index, x_widepix, y);
                cnt0ff = U_get_pixel_data_3(thl_index, x_widepix, y);
                cnt1ff = U_get_pixel_data_4(thl_index, x_widepix, y);
                cnt0df = U_get_pixel_data_5(thl_index, x_widepix, y);
                cnt1df = U_get_pixel_data_6(thl_index, x_widepix, y);
                overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
                overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);

                if (mask) {
                    masked.U_add_pixel_2(z);
                } else {
                    unmasked.U_add_pixel_2(z);
                }
                if (!overflowed) not_overflow.U_add_pixel_2(z);
                if (in_roi) {
                    roi.U_add_pixel_2(z);
                    if (!(overflowed | mask)) good_pixels_in_roi.U_add_pixel_2(z);
                }
                i++;
                emit US_renew_progress_bar(i, total);

            }
        }
    }

    widepix.U_finalize_2();
    roi.U_finalize_2();
    masked.U_finalize_2();
    unmasked.U_finalize_2();
    not_overflow.U_finalize_2();
    good_pixels_in_roi.U_finalize_2();

    emit US_chip_data(widepix);
    emit US_chip_data(roi);
    emit US_chip_data(masked);
    emit US_chip_data(unmasked);
    emit US_chip_data(not_overflow);
    emit US_chip_data(good_pixels_in_roi);
    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip]->U_finalize_2();
        emit US_chip_data(*chip_info[chip]);
    }
    emit US_rewrite_table();
    for (int chip = 0; chip < 15; chip++) {
        delete chip_info[chip];
    }
}

void UC_plot::U_generate_table() {
    double z;
    int cnt0, cnt1, cnt0ff, cnt1ff, cnt0df, cnt1df;
    int overflow, overflow_ff, overflow_df;
    int x_widepix;
    long total = 256 * 256 * 15 * 2 * (thl_index_max - thl_index_min);
    bool in_roi, mask, overflowed;
    UC_pixels_info widepix(UC_pixels_info::UTE_PA_widepix);
    UC_pixels_info roi(UC_pixels_info::UTE_PA_roi);
    UC_pixels_info masked(UC_pixels_info::UTE_PA_masked);
    UC_pixels_info unmasked(UC_pixels_info::UTE_PA_unmasked);
    UC_pixels_info not_overflow(UC_pixels_info::UTE_PA_not_overflow);
    UC_pixels_info good_pixels_in_roi(UC_pixels_info::UTE_PA_roi_unmasked_not_ovf);
    UC_pixels_info* chip_info[15];

    overflow = count * 4095;
    overflow_ff = count_ff * 4095;
    overflow_df = count_df * 4095;

    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip] = new UC_pixels_info(static_cast<UC_pixels_info::UTE_pixels_area>(chip));
    }

    int i = 0;
    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        for (int chip = 0; chip < 15; chip++) {
            for (int y = 0; y < 256; y++) {
                for (int x = 0; x < 256; x++) {
                    x_widepix = U_get_coord_x_chip(chip, x);
                    z = U_get_pixel_data(pixel_type, thl_index, x_widepix, y);

                    widepix.U_add_pixel_1(z);
                    chip_info[chip]->U_add_pixel_1(z);

                    mask = U_get_mask(x_widepix, y);
                    in_roi = (x >= x_min) & (x < x_max) & (y >= y_min) & (y < y_max);
                    cnt0 = U_get_pixel_data_1(thl_index, x_widepix, y);
                    cnt1 = U_get_pixel_data_2(thl_index, x_widepix, y);
                    cnt0ff = U_get_pixel_data_3(thl_index, x_widepix, y);
                    cnt1ff = U_get_pixel_data_4(thl_index, x_widepix, y);
                    cnt0df = U_get_pixel_data_5(thl_index, x_widepix, y);
                    cnt1df = U_get_pixel_data_6(thl_index, x_widepix, y);
                    overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                    overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
                    overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);

                    if (mask) {
                        widepix.U_add_masked();
                        chip_info[chip]->U_add_masked();
                        masked.U_add_pixel_1(z);
                        masked.U_add_masked();
                        if (overflowed) masked.U_add_overflow();
                    } else {
                        unmasked.U_add_pixel_1(z);
                        if (overflowed) unmasked.U_add_overflow();
                    }
                    if (overflowed) {
                        widepix.U_add_overflow();
                        chip_info[chip]->U_add_overflow();
                    } else {
                        not_overflow.U_add_pixel_1(z);
                        if (mask) not_overflow.U_add_masked();
                    }
                    if (in_roi) {
                        roi.U_add_pixel_1(z);
                        if (mask) roi.U_add_masked();
                        if (overflowed) roi.U_add_overflow();
                        if (!(overflowed | mask)) good_pixels_in_roi.U_add_pixel_1(z);
                    }
                    i++;
                    emit US_renew_progress_bar(i, total);

                }
            }
        }
    }

    widepix.U_finalize_1();
    roi.U_finalize_1();
    masked.U_finalize_1();
    unmasked.U_finalize_1();
    not_overflow.U_finalize_1();
    good_pixels_in_roi.U_finalize_1();
    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip]->U_finalize_1();
    }

    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        for (int chip = 0; chip < 15; chip++) {
            for (int y = 0; y < 256; y++) {
                for (int x = 0; x < 256; x++) {
                    x_widepix = U_get_coord_x_chip(chip, x);
                    z = U_get_pixel_data(pixel_type, thl_index, x_widepix, y);

                    widepix.U_add_pixel_2(z);
                    chip_info[chip]->U_add_pixel_2(z);

                    mask = U_get_mask(x_widepix, y);
                    in_roi = (x >= x_min) & (x < x_max) & (y >= y_min) & (y < y_max);
                    cnt0 = U_get_pixel_data_1(thl_index, x_widepix, y);
                    cnt1 = U_get_pixel_data_2(thl_index, x_widepix, y);
                    cnt0ff = U_get_pixel_data_3(thl_index, x_widepix, y);
                    cnt1ff = U_get_pixel_data_4(thl_index, x_widepix, y);
                    cnt0df = U_get_pixel_data_5(thl_index, x_widepix, y);
                    cnt1df = U_get_pixel_data_6(thl_index, x_widepix, y);
                    overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                    overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
                    overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);

                    if (mask) {
                        masked.U_add_pixel_2(z);
                    } else {
                        unmasked.U_add_pixel_2(z);
                    }
                    if (!overflowed) not_overflow.U_add_pixel_2(z);
                    if (in_roi) {
                        roi.U_add_pixel_2(z);
                        if (!(overflowed | mask)) good_pixels_in_roi.U_add_pixel_2(z);
                    }
                    i++;
                    emit US_renew_progress_bar(i, total);

                }
            }
        }
    }

    widepix.U_finalize_2();
    roi.U_finalize_2();
    masked.U_finalize_2();
    unmasked.U_finalize_2();
    not_overflow.U_finalize_2();
    good_pixels_in_roi.U_finalize_2();

    emit US_chip_data(widepix);
    emit US_chip_data(roi);
    emit US_chip_data(masked);
    emit US_chip_data(unmasked);
    emit US_chip_data(not_overflow);
    emit US_chip_data(good_pixels_in_roi);
    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip]->U_finalize_2();
        emit US_chip_data(*chip_info[chip]);
    }
    emit US_rewrite_table();
    for (int chip = 0; chip < 15; chip++) {
        delete chip_info[chip];
    }
}

void UC_plot::U_generate_distribution(int n_bins, double min, double max) {
    double z;
    QVector<double> bounds(n_bins + 1);
    QVector<int> data_y(n_bins);
    for (int i = 0; i < (n_bins + 1); i++) {
        bounds[i] = min + static_cast<double>((max - min) * i) / n_bins;
    }
    for (int i = 0; i < n_bins; i++) {
        data_y[i] = 0;
    }
    int i = 0;
    int total = (x_max - x_min) * (y_max - y_min) * (thl_index_max - thl_index_min);
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) continue;
            for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (bounds[0] > z) continue;
                for (int j = 1; j < (n_bins + 1); j++) {
                    if (bounds[j] > z) {
                        data_y[j-1]++;
                        break;
                    }
                }
                i++;
                emit US_renew_progress_bar(i, total);

            }
        }
    }
    for (int i = 0; i < n_bins; i++){
        emit US_distribution_data(bounds[i], data_y[i]);
    }
    emit US_replot_distribution();
}

void UC_plot::U_generate_frame_distribution(int n_bins, double min, double max, int thl_index) {
    double z;
    QVector<double> bounds(n_bins + 1);
    QVector<int> data_y(n_bins);
    for (int i = 0; i < (n_bins + 1); i++) {
        bounds[i] = min + static_cast<double>((max - min) * i) / n_bins;
    }
    for (int i = 0; i < n_bins; i++) {
        data_y[i] = 0;
    }
    int pixel = 0;
    int total = (x_max - x_min) * (y_max - y_min);
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            pixel++;
            emit US_renew_progress_bar(pixel, total);
            if (U_get_mask(x, y)) continue;
            z = U_get_identification_data(identification_type, thl_index, x, y);
            if (bounds[0] > z) continue;
            for (int j = 1; j < (n_bins + 1); j++) {
                if (bounds[j] > z) {
                    data_y[j-1]++;
                    break;
                }
            }



        }
    }
    for (int i = 0; i < n_bins; i++){
        emit US_distribution_data(bounds[i], data_y[i]);
    }
    emit US_replot_distribution();
}

void UC_plot::U_generate_range(int thl_index) {
    double z, max = -9999999999999, min = 9999999999999999;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) continue;
            z = U_get_identification_data(identification_type, thl_index, x, y);
            if (z > max) max = z;
            if (z < min) min = z;
        }
    }
    emit US_range_data(min, max);
}

void UC_plot::U_generate_range() {
    double z, max = -9999999999999, min = 9999999999999999;
    int i = 0;
    int total = (x_max - x_min) * (y_max - y_min);
    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (z > max) max = z;
                if (z < min) min = z;
                i++;
                emit US_renew_progress_bar(i, total);
            }
        }
    }
    emit US_range_data(min, max);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////


void UC_plot::U_set_frame_type(UTE_frame_type frame_type) {
    this->frame_type = frame_type;
}

void UC_plot::U_set_pixel_type(UTE_pixel_type pixel_type) {
    this->pixel_type = pixel_type;
}

void UC_plot::U_set_roi(int x_min, int x_max, int y_min, int y_max) {
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
}

void UC_plot::U_set_identification_type(UC_plot::UTE_identification_type identification_type) {
    this->identification_type = identification_type;
}

void UC_plot::U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4) {
    this->thl_id_1 = thl_id_1;
    this->thl_id_2 = thl_id_2;
    this->thl_id_3 = thl_id_3;
    this->thl_id_4 = thl_id_4;
}

void UC_plot::U_set_threshold_range(int thl_min, int thl_max) {
    this->thl_index_min = thl_min;
    this->thl_index_max = thl_max;
}
///////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_set_mask(bool mask, bool more, double value, bool in_roi, int thl_index) {
    int x_min = 0;
    int x_max = 15 * 256;
    int y_min = 0;
    int y_max = 256;
    double data;
    if (in_roi) {
        x_min = this->x_min;
        x_max = this->x_max;
        y_min = this->y_min;
        y_max = this->y_max;
    }

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            data = U_get_identification_data(identification_type, thl_index, x, y);
            if (more) {
                if (data > value) U_set_mask(x, y, mask);
            } else {
                if (data < value) U_set_mask(x, y, mask);
            }
        }
    }
}

void UC_plot::U_count_mask(bool more, double value, bool in_roi, int thl_index) {
    int x_min = 0;
    int x_max = 15 * 256;
    int y_min = 0;
    int y_max = 256;
    double data;
    if (in_roi) {
        x_min = this->x_min;
        x_max = this->x_max;
        y_min = this->y_min;
        y_max = this->y_max;
    }

    int n = 0;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) continue;
            data = U_get_pixel_data(pixel_type, thl_index, x, y);
            if (more) {
                if (data > value) n++;
            } else {
                if (data < value) n++;
            }
        }
    }
    emit US_count_mask(n);
}

void UC_plot::U_set_mask_overflowed(bool in_roi, int thl_index) {
    int x_min = 0;
    int x_max = 15 * 256;
    int y_min = 0;
    int y_max = 256;
    if (in_roi) {
        x_min = this->x_min;
        x_max = this->x_max;
        y_min = this->y_min;
        y_max = this->y_max;
    }

    int overflow = 4095 * count;
    int overflow_ff = 4095 * count_ff;
    int overflow_df = 4095 * count_df;
    int cnt0, cnt1, cnt0ff, cnt1ff, cnt0df, cnt1df;
    bool overflowed = false;

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            cnt0 = U_get_pixel_data_1(thl_index, x, y);
            cnt1 = U_get_pixel_data_2(thl_index, x, y);
            cnt0ff = U_get_pixel_data_3(thl_index, x, y);
            cnt1ff = U_get_pixel_data_4(thl_index, x, y);
            cnt0df = U_get_pixel_data_5(thl_index, x, y);
            cnt1df = U_get_pixel_data_6(thl_index, x, y);
            overflowed = (cnt0 == overflow) | (cnt1 == overflow);
            overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
            overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);
            if (overflowed) U_set_mask(x, y, true);
        }
    }
}

void UC_plot::U_set_mask_overflowed(bool in_roi) {
    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        U_set_mask_overflowed(in_roi, thl_index);
    }
}

void UC_plot::U_reset_mask() {
    for (int i = 0; i < (15 * 256 * 256); i++) {
        mask[i] = false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////


