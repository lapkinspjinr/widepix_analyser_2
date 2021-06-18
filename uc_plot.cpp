#include "uc_plot.h"


///////////////////////////////////////////////////////////////////////////////////////////////


UC_plot::UC_plot(QObject *parent) : QObject(parent) {
    /// Инициализация всех переменных класса
    x_min = 0;
    x_max = 15 * 256;
    y_min = 0;
    y_max = 256;
    thl_id_1 = 0;
    thl_id_2 = 511;
    thl_id_3 = 0;
    thl_id_4 = 511;
    thl_start = 0;
    thl_finish = 511;
    thl_min = 0;
    thl_max = 511;
    threshold_level = 10;   /// Стандартное значение Pixet'а при эквилизации
    rebin_x = 1;
    rebin_y = 1;
    rebin_thl = 1;

    using_calibration = false;
    scan_enable = false;
    ff_enable = false;
    df_enable = false;
    for (int i = 0; i < (256 * 256 * 15); i++) { /// Цикл для инициализации масок, индекс - i
        mask[i] = false;
    }


    for (int i = 0; i < 15; i++) { /// Цикл для инициализации масок, индекс - i
        calibration_p0[i] = 0;
        calibration_p1[i] = 0;
        calibration_p2[i] = 0;
    }

    frame_type = UTE_FT_average;
    pixel_type = UTE_PT_cnt1;





    data_enable.id1_enable = true;
    data_enable.min_enable = true;
    data_enable.max_enable = true;
    data_enable.sum_enable = true;
    data_enable.mean_enable = true;
    data_enable.median_enable = true;
    data_enable.std_dev_enable = true;
    data_enable.zeros_enable = true;
    data_enable.snr_enable = true;



    U_reset_data();
}

UC_plot::~UC_plot() {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
double UC_plot::U_get_energy_from_thl(int x, int thl) {
    if (using_calibration) {
        int chip = x / 256;
        double p0 = calibration_p0[chip];
        double p1 = calibration_p1[chip];
        double p2 = calibration_p2[chip];
        return p0 + p1 * thl + p2 * p2 * thl;
    } else {
        return thl;
    }
}

double UC_plot::U_get_energy_from_thl_chip(int chip, int thl) {
    if (using_calibration) {
        double p0 = calibration_p0[chip];
        double p1 = calibration_p1[chip];
        double p2 = calibration_p2[chip];
        return p0 + p1 * thl + p2 * p2 * thl;
    } else {
        return thl;
    }
}

int UC_plot::U_get_thl_from_energy(int x, double energy) {
    if (using_calibration) {
        int chip = x / 256;
        double p0 = calibration_p0[chip] - energy;
        double p1 = calibration_p1[chip];
        double p2 = calibration_p2[chip];
        double d = p1 * p1 - 4 * p2 * p0;
        double thl_d = (-p1 + qSqrt(d)) / (2 * p2);
        return static_cast<int>(round(thl_d));
    } else {
        return static_cast<int>(round(energy));
    }
}

int UC_plot::U_get_thl_from_energy_chip(int chip, double energy) {
    if (using_calibration) {
        double p0 = calibration_p0[chip] - energy;
        double p1 = calibration_p1[chip];
        double p2 = calibration_p2[chip];
        double d = p1 * p1 - 4 * p2 * p0;
        double thl_d = (-p1 + qSqrt(d)) / (2 * p2);
        return static_cast<int>(round(thl_d));
    } else {
        return static_cast<int>(round(energy));
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void UC_plot::U_alloc_ff_minus_df_mean() {
//    ff_minus_df_mean = new double[n_thl * 15];
//    for (int i = 0; i < 15 * n_thl; i++) {
//        ff_minus_df_mean[i] = 0;
//    }
//}

//void UC_plot::U_make_ff_minus_df_mean() {
//    for (int chip = 0; chip < 15; chip++) {
//        for (int thl_index = 0; thl_index < n_thl; thl_index++) {
//            ff_minus_df_mean[chip + 15 * thl_index] = 0;
//            for (int x = 0; x < 256; x++) {
//                for (int y = 0; y < 256; y++) {
//                    ff_minus_df_mean[chip + 15 * thl_index] += U_get_data_ff(thl_index, U_get_coord_x_chip(chip, x), y, 1) * scale_ff - U_get_data_df(thl_index, U_get_coord_x_chip(chip, x), y, 1) * scale_df;
//                }
//            }
//            ff_minus_df_mean[chip + 15 * thl_index] /= (256 * 256);
//        }
//    }
//}

//double UC_plot::U_get_ff_minus_df_mean(int thl_index, int x) {
//    return ff_minus_df_mean[U_get_chip_coord_x(x) + 15 * thl_index];
//}
////////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_set_mask(int x, int y, bool value) {
    mask[y * 256 * 15 + x] = value;
}

bool UC_plot::U_get_mask(int x, int y) {
    return mask[y * 256 * 15 + x];
}

////////////////////////////////////////////////////////////////////////////////////////////////
double UC_plot::U_get_pixel_data(UTE_pixel_type type, int thl, int x, int y) {
    switch (type) {
        case UTE_PT_cnt0 : {
            return U_get_pixel_data_1(thl, x, y);
        }
        case UTE_PT_cnt1 : {
            return U_get_pixel_data_2(thl, x, y);
        }
        case UTE_PT_cnt1_divide_on_cnt0 : {
            return U_get_pixel_data_3(thl, x, y);
        }
        case UTE_PT_cnt0_subtr_cnt1 : {
            return U_get_pixel_data_4(thl, x, y);
        }
        case UTE_PT_diff_cnt1 : {
            return U_get_pixel_data_5(thl, x, y);
        }
        case UTE_PT_ffc : {
            return U_get_pixel_data_6(thl, x, y);
        }
        case UTE_PT_mu : {
            return U_get_pixel_data_7(thl, x, y);
        }
        case UTE_PT_diff_ffc : {
            return U_get_pixel_data_8(thl, x, y);
        }
        case UTE_PT_diff_mu : {
            return U_get_pixel_data_9(thl, x, y);
        }
        case UTE_PT_ffc_diff : {
            return U_get_pixel_data_10(thl, x, y);
        }
        case UTE_PT_mu_diff : {
            return U_get_pixel_data_11(thl, x, y);
        }
        ////
        case UTE_PT_cnt1_corr_cnt0 : {
            return U_get_pixel_data_12(thl, x, y);
        }
        case UTE_PT_diff_cnt1_corr_cnt0 : {
            return U_get_pixel_data_13(thl, x, y);
        }
        case UTE_PT_ffc_corr_cnt0 : {
            return U_get_pixel_data_14(thl, x, y);
        }
        case UTE_PT_mu_corr_cnt0 : {
            return U_get_pixel_data_15(thl, x, y);
        }
        case UTE_PT_diff_ffc_corr_cnt0 : {
            return U_get_pixel_data_16(thl, x, y);
        }
        case UTE_PT_diff_mu_corr_cnt0 : {
            return U_get_pixel_data_17(thl, x, y);
        }
        case UTE_PT_ffc_diff_corr_cnt0 : {
            return U_get_pixel_data_18(thl, x, y);
        }
        case UTE_PT_mu_diff_corr_cnt0 : {
            return U_get_pixel_data_19(thl, x, y);
        }
        ////
        case UTE_PT_ffc_non_xray : {
            return U_get_pixel_data_20(thl, x, y);
        }
        case UTE_PT_cnt0_deviation : {
            return U_get_pixel_data_21(thl, x, y);
        }
        case UTE_PT_cnt1_act : {
            return U_get_pixel_data_22(thl, x, y);
        }
        case UTE_PT_cnt0_act : {
            return U_get_pixel_data_23(thl, x, y);
        }
        case UTE_PT_cnt1_rejected : {
            return U_get_pixel_data_24(thl, x, y);
        }
        case UTE_PT_diff_cnt1_rejected : {
            return U_get_pixel_data_25(thl, x, y);
        }
        case UTE_PT_cnt1ffc_div_cnt0ffc : {
            return U_get_pixel_data_26(thl, x, y);
        }
        case UTE_PT_additional_data : {
            return U_get_pixel_data_27(thl, x, y);
        }
        case UTE_PT_GA_request : {
            return U_get_pixel_data_28(thl, x, y);
        }
    }
    return 0;
}

int UC_plot::U_get_pixel_data_1(int thl, int x, int y) { //UTE_PT_cnt0
    return scan->U_get_data(thl, x, y, 0);
}

int UC_plot::U_get_pixel_data_2(int thl, int x, int y) { //UTE_PT_cnt1
    return scan->U_get_data(thl, x, y, 1);
}

double UC_plot::U_get_pixel_data_3(int thl, int x, int y) { //UTE_PT_cnt1_divide_on_cnt0
    int denom = scan->U_get_data(thl, x, y, 1);;
    if (denom == 0) return 0;
    return (static_cast<double>(scan->U_get_data(thl, x, y, 0)) / denom);
}

int UC_plot::U_get_pixel_data_4(int thl, int x, int y) { //UTE_PT_cnt0_subtr_cnt1
    return (scan->U_get_data(thl, x, y, 0) - scan->U_get_data(thl, x, y, 1));
}

double UC_plot::U_get_pixel_data_5(int thl, int x, int y) { //UTE_PT_diff_cnt1
    return scan->U_get_data(thl, x, y, 1) - scan->U_get_data(thl + 1, x, y, 1);
}

double UC_plot::U_get_pixel_data_6(int thl, int x, int y) { //UTE_PT_ffc
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl, x, y, 1);
    } else {
        cnt1df = 0;
    }
    double ffc = 0;
    double denom = ff->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    ffc = scan->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_7(int thl, int x, int y) { //UTE_PT_mu
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl, x, y, 1);
    } else {
        cnt1df = 0;
    }
    double mu = 0;
    double denom = scan->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    mu = ff->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);
}

double UC_plot::U_get_pixel_data_8(int thl, int x, int y) { //UTE_PT_diff_ffc
    if (!ff_enable) return 0;
    double cnt1df, cnt1df_1;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl, x, y, 1);
        cnt1df_1 = df->U_get_data_scaled(thl + 1, x, y, 1);
    } else {
        cnt1df = 0;
        cnt1df_1 = 0;
    }
    double ffc = 0;
    double denom = ff->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double ffc_1 = 0;
    double denom_1 = ff->U_get_data_scaled(thl + 1, x, y, 1) - cnt1df_1;
    if (qAbs(denom_1) < 1e-10) return 0;
    ffc = scan->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    ffc /= denom;
    ffc_1 = scan->U_get_data_scaled(thl + 1, x, y, 1) - cnt1df_1;
    ffc_1 /= denom_1;
    return ffc_1 - ffc;
}

double UC_plot::U_get_pixel_data_9(int thl, int x, int y) { //UTE_PT_diff_mu
    if (!ff_enable) return 0;
    double cnt1df, cnt1df_1;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl, x, y, 1);
        cnt1df_1 = df->U_get_data_scaled(thl + 1, x, y, 1);
    } else {
        cnt1df = 0;
        cnt1df_1 = 0;
    }
    double mu = 0;
    double denom = scan->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double mu_1 = 0;
    double denom_1 = scan->U_get_data_scaled(thl + 1, x, y, 1) - cnt1df_1;
    if (qAbs(denom_1) < 1e-10) return 0;
    mu = ff->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    mu_1 = ff->U_get_data_scaled(thl + 1, x, y, 1) - cnt1df_1;
    mu_1 /= denom_1;
    if (mu_1 < 1e-10) return 0;
    return qLn(mu_1) - qLn(mu);
}

double UC_plot::U_get_pixel_data_10(int thl, int x, int y) { //UTE_PT_ffc_diff
    if (!ff_enable) return 0;
    double delta_cnt1df;
    if (df_enable) {
        delta_cnt1df = df->U_get_data_scaled(thl, x, y, 1) - df->U_get_data_scaled(thl + 1, x, y, 1);
    } else {
        delta_cnt1df = 0;
    }
    double denom = ff->U_get_data_scaled(thl, x, y, 1) - ff->U_get_data_scaled(thl + 1, x, y, 1) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double ffc = scan->U_get_data_scaled(thl, x, y, 1) - scan->U_get_data_scaled(thl + 1, x, y, 1) - delta_cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_11(int thl, int x, int y) { //UTE_PT_mu_diff
    if (!ff_enable) return 0;
    double delta_cnt1df;
    if (df_enable) {
        delta_cnt1df = df->U_get_data_scaled(thl, x, y, 1) - df->U_get_data_scaled(thl + 1, x, y, 1);
    } else {
        delta_cnt1df = 0;
    }
    double denom = scan->U_get_data_scaled(thl, x, y, 1) - scan->U_get_data_scaled(thl + 1, x, y, 1) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double mu = ff->U_get_data_scaled(thl, x, y, 1) - ff->U_get_data_scaled(thl + 1, x, y, 1) - delta_cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);

}
////

double UC_plot::U_get_pixel_data_12(int thl, int x, int y) { //UTE_PT_cnt1_corr_cnt0
    return scan->U_get_corrected_cnt1(thl, x, y);
}

double UC_plot::U_get_pixel_data_13(int thl, int x, int y) { //UTE_PT_diff_cnt1_corr_cnt0
    return scan->U_get_corrected_cnt1(thl, x, y) - scan->U_get_corrected_cnt1(thl + 1, x, y);
}

double UC_plot::U_get_pixel_data_14(int thl, int x, int y) { //UTE_PT_ffc_corr_cnt0
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_corrected_cnt1_scaled(thl, x, y);
    } else {
        cnt1df = 0;
    }
    double denom = ff->U_get_corrected_cnt1_scaled(thl, x, y) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double ffc = scan->U_get_corrected_cnt1_scaled(thl, x, y) - cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_15(int thl, int x, int y) { //UTE_PT_mu_corr_cnt0
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_corrected_cnt1_scaled(thl, x, y);
    } else {
        cnt1df = 0;
    }
    double denom = scan->U_get_corrected_cnt1_scaled(thl, x, y) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double mu = ff->U_get_corrected_cnt1_scaled(thl, x, y) - cnt1df;
    mu /= denom;
    if (mu < 0) return 0;
    return qLn(mu);
}

double UC_plot::U_get_pixel_data_16(int thl, int x, int y) { //UTE_PT_diff_ffc_corr_cnt0
    if (!ff_enable) return 0;
    double cnt1df;
    double cnt1df_1;
    if (df_enable) {
        cnt1df = df->U_get_corrected_cnt1_scaled(thl, x, y);
        cnt1df_1 = df->U_get_corrected_cnt1_scaled(thl + 1, x, y);
    } else {
        cnt1df = 0;
        cnt1df_1 = 0;
    }
    double denom =  ff->U_get_corrected_cnt1_scaled(thl, x, y) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double denom_1 = ff->U_get_corrected_cnt1_scaled(thl + 1, x, y) - cnt1df_1;
    if (qAbs(denom_1) < 1e-10) return 0;
    double ffc = scan->U_get_corrected_cnt1_scaled(thl, x, y) - cnt1df;
    ffc /= denom;
    double ffc_1 = scan->U_get_corrected_cnt1_scaled(thl + 1, x, y) - cnt1df_1;
    ffc_1 /= denom_1;
    return ffc_1 - ffc;
}

double UC_plot::U_get_pixel_data_17(int thl, int x, int y) { //UTE_PT_diff_mu_corr_cnt0
    if (!ff_enable) return 0;
    double cnt1df;
    double cnt1df_1;
    if (df_enable) {
        cnt1df = df->U_get_corrected_cnt1_scaled(thl, x, y);
        cnt1df_1 = df->U_get_corrected_cnt1_scaled(thl + 1, x, y);
    } else {
        cnt1df = 0;
        cnt1df_1 = 0;
    }
    double denom = scan->U_get_corrected_cnt1_scaled(thl, x, y) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double denom_1 = scan->U_get_corrected_cnt1_scaled(thl + 1, x, y) - cnt1df_1;
    if (qAbs(denom_1) < 1e-10) return 0;
    double mu = ff->U_get_corrected_cnt1_scaled(thl, x, y) - cnt1df;
    mu /= denom;
    if (mu < 0) return 0;
    double mu_1 = ff->U_get_corrected_cnt1_scaled(thl + 1, x, y) - cnt1df_1;
    mu_1 /= denom_1;
    if (mu_1 < 0) return 0;
    return qLn(mu_1) - qLn(mu);
}

double UC_plot::U_get_pixel_data_18(int thl, int x, int y) { //UTE_PT_ffc_diff_corr_cnt0
    if (!ff_enable) return 0;
    double delta_cnt1df;
    if (df_enable) {
        delta_cnt1df = df->U_get_corrected_cnt1_scaled(thl, x, y, 1) - df->U_get_corrected_cnt1_scaled(thl + 1, x, y, 1);
    } else {
        delta_cnt1df = 0;
    }
    double denom = ff->U_get_corrected_cnt1_scaled(thl, x, y, 1) - ff->U_get_corrected_cnt1_scaled(thl + 1, x, y, 1) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double ffc = scan->U_get_corrected_cnt1_scaled(thl, x, y, 1) - scan->U_get_corrected_cnt1_scaled(thl + 1, x, y, 1) - delta_cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_19(int thl, int x, int y) { //UTE_PT_mu_diff_corr_cnt0
    if (!ff_enable) return 0;
    double delta_cnt1df;
    if (df_enable) {
        delta_cnt1df = df->U_get_corrected_cnt1_scaled(thl, x, y, 1) - df->U_get_corrected_cnt1_scaled(thl + 1, x, y, 1);
    } else {
        delta_cnt1df = 0;
    }
    double denom = scan->U_get_corrected_cnt1_scaled(thl, x, y, 1) - scan->U_get_corrected_cnt1_scaled(thl + 1, x, y, 1) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double mu = ff->U_get_corrected_cnt1_scaled(thl, x, y, 1) - ff->U_get_corrected_cnt1_scaled(thl + 1, x, y, 1) - delta_cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);
}
///
double UC_plot::U_get_pixel_data_20(int thl, int x, int y) { //UTE_PT_ffc_non_xray
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl, x, y, 1);
    } else {
        cnt1df = 0;
    }
    double ffc = 0;
    double denom = ff->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    ffc = scan->U_get_data_scaled(thl, x, y, 1) - cnt1df;
    ffc /= denom;
    //ffc *= U_get_ff_minus_df_mean(thl, x);
    return ffc;
}

double UC_plot::U_get_pixel_data_21(int thl, int x, int y) { //UTE_PT_cnt0_deviation
    double denom = scan->U_get_corrected_cnt1(thl, x, y);
    if (qAbs(denom) < 1e-10) return 0;
    return scan->U_get_data(thl, x, y, 1) / denom;
}

int UC_plot::U_get_pixel_data_22(int thl, int x, int y) { //UTE_PT_cnt1_act
    if (scan->U_get_data(thl, x, y, 1) > threshold_level) {
        return 1;
    } else {
        return 0;
    }
}

int UC_plot::U_get_pixel_data_23(int thl, int x, int y) { //UTE_PT_cnt0_act
    if (scan->U_get_data(thl, x, y, 0) > threshold_level) {
        return 1;
    } else {
        return 0;
    }
}

int UC_plot::U_get_pixel_data_24(int thl, int x, int y) { //UTE_PT_cnt1_rejected
    bool act0 = scan->U_get_data(thl, x, y, 0) > threshold_level;
    bool act1 = scan->U_get_data(thl, x, y, 1) > threshold_level;
    if (act0 && (!act1))  {
        return 1;
    } else {
        return 0;
    }
}

int UC_plot::U_get_pixel_data_25(int thl, int x, int y) { //UTE_PT_diff_cnt1_rejected

    bool act0 = scan->U_get_data(thl, x, y, 0) > threshold_level;
    bool act1 = scan->U_get_data(thl, x, y, 1) > threshold_level;
    bool act0_1 = scan->U_get_data(thl - 1, x, y, 0) > threshold_level;
    bool act1_1 = scan->U_get_data(thl - 1, x, y, 1) > threshold_level;
    int cnt0 = 0;
    if (act0 && (!act1))  {
        cnt0 =  1;
    } else {
        cnt0 =  0;
    }
    int cnt1 = 0;
    if (act0_1 && (!act1_1))  {
        cnt1 =  1;
    } else {
        cnt1 =  0;
    }
    return cnt0 - cnt1;
}

double UC_plot::U_get_pixel_data_26(int thl, int x, int y) { //UTE_PT_cnt1ffc_div_cnt0ffc
    if (!ff_enable) return 0;
    int cnt0 = scan->U_get_data(thl, x, y, 0);
    int cnt1 = scan->U_get_data(thl, x, y, 1);
    int cnt0ff = ff->U_get_data(thl, x, y, 0);
    int cnt1ff = ff->U_get_data(thl, x, y, 1);
    int denom = cnt0 * cnt1ff;
    if (denom == 0) return 0;
    return static_cast<double>(cnt1 * cnt0ff) / denom;
}

double UC_plot::U_get_pixel_data_27(int thl, int x, int y) { //UTE_PT_smoothing_mu_diff
    return scan->U_get_data_additional(thl, x, y);
}

double UC_plot::U_get_pixel_data_28(int thl, int x, int y) { //UTE_PT_GA_request
    double denom = U_get_pixel_data_6(thl, x, y);
    if (qAbs(denom) < 1e-10) return 0;
    return U_get_pixel_data_6(38, x, y) / denom;
}
/////////////////////////////////////////////////////////////////////////////////
double UC_plot::U_get_frame_data(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, int thl) {
    switch (type_spectra) {
        case UTE_FT_average : {
            return U_get_frame_data_1(type_pixel, thl);
        }
        case UTE_FT_sum : {
            return U_get_frame_data_2(type_pixel, thl);
        }
        case UTE_FT_median : {
            return U_get_frame_data_3(type_pixel, thl);
        }
        case UTE_FT_max : {
            return U_get_frame_data_4(type_pixel, thl);
        }
        case UTE_FT_min : {
            return U_get_frame_data_5(type_pixel, thl);
        }
        case UTE_FT_zeros : {
            return U_get_frame_data_6(type_pixel, thl);
        }
        case UTE_FT_overflowed : {
            return U_get_frame_data_7(thl);
        }
        case UTE_FT_standart_deviation : {
            return U_get_frame_data_8(type_pixel, thl);
        }
        case UTE_FT_signal_to_noise_resolution : {
            return U_get_frame_data_9(type_pixel, thl);
        }
    }
    return 0;
}

double UC_plot::U_get_frame_data_1(UTE_pixel_type type_pixel, int thl) { // UTE_FT_average
    double sum = 0;
    int cnt = 0;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (!U_get_mask(x, y)) {
                sum += U_get_pixel_data(type_pixel, thl, x, y);
                cnt++;
            }
        }
    }
    sum /= cnt;
    return sum;
}

double UC_plot::U_get_frame_data_2(UTE_pixel_type type_pixel, int thl) { // UTE_FT_sum
    double sum = 0;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (!U_get_mask(x, y)) sum += U_get_pixel_data(type_pixel, thl, x, y);
        }
    }
    return sum;
}



double UC_plot::U_get_frame_data_3(UTE_pixel_type type_pixel, int thl) { //UTE_FT_median
    std::vector<double> z;
    int cnt = 0;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (!U_get_mask(x, y)) {
                z.push_back(U_get_pixel_data(type_pixel, thl, x, y));
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

double UC_plot::U_get_frame_data_4(UTE_pixel_type type_pixel, int thl) { //UTE_FT_max
    double max = -9999999999999999, z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            z = U_get_pixel_data(type_pixel, thl, x, y);
            if (z > max) max = z;
        }
    }
    return max;
}

double UC_plot::U_get_frame_data_5(UTE_pixel_type type_pixel, int thl) { //UTE_FT_min
    double min = 9999999999999999, z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            z = U_get_pixel_data(type_pixel, thl, x, y);
            if (z < min) min = z;
        }
    }
    return min;
}

int UC_plot::U_get_frame_data_6(UTE_pixel_type type_pixel, int thl) { //UTE_FT_zeros
    int cnt = 0;
    double z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            z = U_get_pixel_data(type_pixel, thl, x, y);
            if (qAbs(z) < 1e-10) cnt++;
        }
    }
    return cnt;
}

int UC_plot::U_get_frame_data_7(int thl) { //UTE_FT_overflowed
    int cnt0, cnt1, cnt0ff, cnt1ff, cnt0df, cnt1df;
    int overflow, overflow_ff, overflow_df;
    bool overflowed;
    int cnt = 0;
    overflow = scan->U_get_count() * 4095;
    overflow_ff = ff->U_get_count() * 4095;
    overflow_df = df->U_get_count() * 4095;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            cnt0 = scan->U_get_data(thl, x, y, 0);
            cnt1 = scan->U_get_data(thl, x, y, 1);
            cnt0ff = ff->U_get_data(thl, x, y, 0);
            cnt1ff = ff->U_get_data(thl, x, y, 1);
            cnt0df = df->U_get_data(thl, x, y, 0);
            cnt1df = df->U_get_data(thl, x, y, 1);
            overflowed = (cnt0 == overflow) | (cnt1 == overflow);
            overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
            overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);
            if (overflowed) cnt++;
        }
    }
    return cnt;
}

double UC_plot::U_get_frame_data_8(UTE_pixel_type type_pixel, int thl) { //UTE_FT_standart_deviation
    int cnt = 0;
    double mean = U_get_frame_data_1(type_pixel, thl);
    double std_dev = 0, z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            z = U_get_pixel_data(type_pixel, thl, x, y);
            std_dev = (mean - z) * (mean - z);
            cnt++;
        }
    }
    if (cnt == 0) return 0;
    if (cnt == 1) return 0;
    std_dev = qSqrt(std_dev / (cnt - 1));
    return std_dev;
}

double UC_plot::U_get_frame_data_9(UTE_pixel_type type_pixel, int thl) { //UTE_FT_signal_to_noise_resolution
    double mean = U_get_frame_data_1(type_pixel, thl);
    if (mean < 0) return 0;
    double std_dev = U_get_frame_data_8(type_pixel, thl);
    if (qAbs(std_dev) < 1e-10) return 0;
    return qSqrt(mean) / std_dev;
}
//
double UC_plot::U_get_frame_data_energy(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, double e_min, double e_max) {
    switch (type_spectra) {
        case UTE_FT_average : {
            return U_get_frame_data_energy_1(type_pixel, e_min, e_max);
        }
        case UTE_FT_sum : {
            return U_get_frame_data_energy_2(type_pixel, e_min, e_max);
        }
        case UTE_FT_median : {
            return U_get_frame_data_energy_3(type_pixel, e_min, e_max);
        }
        case UTE_FT_max : {
            return U_get_frame_data_energy_4(type_pixel, e_min, e_max);
        }
        case UTE_FT_min : {
            return U_get_frame_data_energy_5(type_pixel, e_min, e_max);
        }
        case UTE_FT_zeros : {
            return U_get_frame_data_energy_6(type_pixel, e_min, e_max);
        }
        case UTE_FT_overflowed : {
            return U_get_frame_data_energy_7(e_min, e_max);
        }
        case UTE_FT_standart_deviation : {
            return U_get_frame_data_energy_8(type_pixel, e_min, e_max);
        }
        case UTE_FT_signal_to_noise_resolution : {
            return U_get_frame_data_energy_9(type_pixel, e_min, e_max);
        }
    }
    return 0;
}

double UC_plot::U_get_frame_data_energy_1(UTE_pixel_type type_pixel, double e_min, double e_max) { // UTE_FT_average
    double sum = 0;
    int cnt = 0;
    int thl_min = 0, thl_max = 0;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl < thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                if (!U_get_mask(x, y)) {
                    sum += U_get_pixel_data(type_pixel, thl, x, y);
                    cnt++;
                }
            }
        }
    }
    sum /= cnt;
    return sum;
}

double UC_plot::U_get_frame_data_energy_2(UTE_pixel_type type_pixel, double e_min, double e_max) { // UTE_FT_sum
    double sum = 0;
    int thl_min = 0, thl_max = 0;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl < thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                if (!U_get_mask(x, y)) sum += U_get_pixel_data(type_pixel, thl, x, y);
            }
        }
    }
    return sum;
}



double UC_plot::U_get_frame_data_energy_3(UTE_pixel_type type_pixel, double e_min, double e_max) { //UTE_FT_median
    std::vector<double> z;
    int cnt = 0;
    int thl_min = 0, thl_max = 0;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl < thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                if (!U_get_mask(x, y)) {
                    z.push_back(U_get_pixel_data(type_pixel, thl, x, y));
                    cnt++;
                }
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

double UC_plot::U_get_frame_data_energy_4(UTE_pixel_type type_pixel, double e_min, double e_max) { //UTE_FT_max
    double max = -1e300, z;
    int thl_min = 0, thl_max = 0;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl < thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                z = U_get_pixel_data(type_pixel, thl, x, y);
                if (z > max) max = z;
            }
        }
    }
    return max;
}

double UC_plot::U_get_frame_data_energy_5(UTE_pixel_type type_pixel, double e_min, double e_max) { //UTE_FT_min
    double min = 1e300, z;
    int thl_min = 0, thl_max = 0;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl < thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                z = U_get_pixel_data(type_pixel, thl, x, y);
                if (z < min) min = z;
            }
        }
    }
    return min;
}

int UC_plot::U_get_frame_data_energy_6(UTE_pixel_type type_pixel, double e_min, double e_max) { //UTE_FT_zeros
    int cnt = 0;
    double z;
    int thl_min = 0, thl_max = 0;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl < thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                z = U_get_pixel_data(type_pixel, thl, x, y);
                if (qAbs(z) < 1e-10) cnt++;
            }
        }
    }
    return cnt;
}

int UC_plot::U_get_frame_data_energy_7(double e_min, double e_max) { //UTE_FT_overflowed
    int cnt0, cnt1, cnt0ff, cnt1ff, cnt0df, cnt1df;
    int overflow, overflow_ff, overflow_df;
    bool overflowed;
    int cnt = 0;
    overflow = scan->U_get_count() * 4095;
    overflow_ff = ff->U_get_count() * 4095;
    overflow_df = df->U_get_count() * 4095;
    int thl_min = 0, thl_max = 0;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl < thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                cnt0 = scan->U_get_data(thl, x, y, 0);
                cnt1 = scan->U_get_data(thl, x, y, 1);
                cnt0ff = ff->U_get_data(thl, x, y, 0);
                cnt1ff = ff->U_get_data(thl, x, y, 1);
                cnt0df = df->U_get_data(thl, x, y, 0);
                cnt1df = df->U_get_data(thl, x, y, 1);
                overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
                overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);
                if (overflowed) cnt++;
            }
        }
    }
    return cnt;
}

double UC_plot::U_get_frame_data_energy_8(UTE_pixel_type type_pixel, double e_min, double e_max) { //UTE_FT_standart_deviation
    int cnt = 0;
    int thl_min = 0, thl_max = 0;
    double mean = U_get_frame_data_energy_1(type_pixel, e_min, e_max);
    double std_dev = 0, z;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl < thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                z = U_get_pixel_data(type_pixel, thl, x, y);
                std_dev = (mean - z) * (mean - z);
                cnt++;
            }
        }
    }
    if (cnt == 0) return 0;
    if (cnt == 1) return 0;
    std_dev = qSqrt(std_dev / (cnt - 1));
    return std_dev;
}

double UC_plot::U_get_frame_data_energy_9(UTE_pixel_type type_pixel, double e_min, double e_max) { //UTE_FT_signal_to_noise_resolution
    double mean = U_get_frame_data_energy_1(type_pixel, e_min, e_max);
    if (mean < 0) return 0;
    double std_dev = U_get_frame_data_energy_8(type_pixel, e_min, e_max);
    if (qAbs(std_dev) < 1e-10) return 0;
    return qSqrt(mean) / std_dev;
}

//
///////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_find_thl_range() {
    int max, min;
    thl_max = 0;
    thl_min = 512;
    for (int i = 0; i < scans_list.size(); i++) {
        max = scans_list[i].U_get_thl_max();
        min = scans_list[i].U_get_thl_min();
        if (max > thl_max) thl_max = max;
        if (min < thl_min) thl_min = min;
    }
    emit US_thl_range(thl_min, thl_max);
}
//
void UC_plot::U_identification_roi_1() {    //UTE_IT_GA_method
    UTStr_id_GA_data result;
    int current = 0;
    int total = thl_id_2 - thl_id_1 + thl_id_4 - thl_id_3;
    for (int thl = thl_id_1; thl <= thl_id_2; thl++) {
        result.sum_1_window += U_get_frame_data(frame_type, pixel_type, thl);
        current++;
        US_renew_progress_bar(current, total);
    }
    for (int thl = thl_id_3; thl <= thl_id_4; thl++) {
        result.sum_2_window += U_get_frame_data(frame_type, pixel_type, thl);
        current++;
        US_renew_progress_bar(current, total);
    }
    if (qAbs(result.sum_2_window) > 1e-10) result.id_data = result.sum_1_window / result.sum_2_window;
        else result.id_data = 0;
    result.thl_1_window_left    =   thl_id_1;
    result.thl_1_window_right   =   thl_id_2;
    result.thl_2_window_left    =   thl_id_3;
    result.thl_2_window_right   =   thl_id_4;
    result.frame_type           =   frame_type;
    result.pixel_type           =   pixel_type;
    emit US_id_roi_GA_data(result);
}

//

QVector<double> UC_plot::U_calculating_samples_value(UC_data_container * scan, QVector<int> thl_samples) {
    UC_data_container * scan_reserved, * ff;
    scan_reserved = this->scan;
    ff = &scans_list[scan->U_get_settings().ff_int];

    QVector<double> data_y_scan(thl_finish - thl_start + 1);
    QVector<double> data_y_ff(thl_finish - thl_start + 1);
    QVector<double> data_thl(thl_finish - thl_start + 1);
    QVector<double> result;

    this->scan = ff;
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        data_y_ff[thl - thl_start] = U_get_frame_data(frame_type, pixel_type, thl);
        data_thl[thl - thl_start] = thl;
    }
    data_y_ff = U_smooth(data_thl, data_y_ff, smoothing);
    data_y_ff = U_diff(data_thl, data_y_ff);
    data_y_ff = U_vector_minus(data_y_ff);
    data_y_ff = U_smooth(data_thl, data_y_ff, smoothing);
    this->scan = scan;
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        data_y_scan[thl - thl_start] = U_get_frame_data(frame_type, pixel_type, thl);
    }
    data_y_scan = U_smooth(data_thl, data_y_scan, smoothing);
    data_y_scan = U_diff(data_thl, data_y_scan);
    data_y_scan = U_vector_minus(data_y_scan);
    data_y_scan = U_smooth(data_thl, data_y_scan, smoothing);
    data_y_scan = U_vector_divide(data_y_ff, data_y_scan);
    data_y_scan = U_smooth(data_thl, data_y_scan, smoothing);
    data_y_scan = U_vector_log(data_y_scan);

    for (int i = 0; i < thl_samples.size(); i++) {
        result << data_y_scan[data_thl.indexOf(thl_samples[i])];
    }

    this->scan = scan_reserved;
    return result;
}

void UC_plot::U_identification_roi_2() {    //UTE_IT_linear_combination
    int current = 0;
    int total = 0;

    int n_scans = scans_list.size();
    int number_of_samples = 0;
    UTStr_id_LC_data result;
    UC_data_container * current_scan;
    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
        current_scan = &(scans_list[scan_i]);
        UC_data_container::UTStr_data_container_settings settings = current_scan->U_get_settings();
        if (!settings.sample_of_element) continue;
        number_of_samples++;
        result.thl_samples << settings.thl_sample;
        result.elements << settings.name;
    }
    result.number_of_samples = number_of_samples;
    result.frame_type = frame_type;
    result.pixel_type = pixel_type;
    total = number_of_samples + 2;


    TMatrixD eqv_matr(number_of_samples, number_of_samples);
    TVectorD scan_thl_data(number_of_samples);

    QVector<double> data_scan = U_calculating_samples_value(scan, result.thl_samples);
    current++;
    US_renew_progress_bar(current, total);
    for (int i = 0; i < number_of_samples; i++) {
        scan_thl_data[i] = data_scan[i];
        result.data << data_scan[i];
    }

    result.matrix.resize(number_of_samples);
    for (int index = 0; index < number_of_samples; index++) {
        result.matrix[index].resize(number_of_samples);
    }

    int sample_index = 0;
    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
        current_scan = &(scans_list[scan_i]);
        if (!current_scan->U_get_sample_of_element()) continue;
        QVector<double> data_samples = U_calculating_samples_value(current_scan, result.thl_samples);
        current++;
        US_renew_progress_bar(current, total);
        for (int thl_index = 0; thl_index < number_of_samples; thl_index++) {
            eqv_matr[thl_index][sample_index] = data_samples[thl_index];
            result.matrix[thl_index][sample_index] = data_samples[thl_index];
        }
        sample_index++;
    }

    TDecompLU decomp(eqv_matr);
    Bool_t ok;
    TVectorD result_vector = decomp.Solve(scan_thl_data, ok);
    int nr = 0;
    while (!ok) {
        decomp.SetMatrix(eqv_matr);
        decomp.SetTol(0.1 * decomp.GetTol());
        if (nr++ > 10) break;
        result_vector = decomp.Solve(scan_thl_data, ok);
    }
    current++;
    US_renew_progress_bar(current, total);

    sample_index = 0;
    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
        current_scan = &(scans_list[scan_i]);
        if (!current_scan->U_get_sample_of_element()) continue;
        result.id_values << result_vector[sample_index];
        sample_index++;
    }
    emit US_id_roi_LC_data(result);
}

void UC_plot::U_calculating_xmu(UC_data_container * scan, int i, int j, int rebin_x, int rebin_y, int rebin_thl) {
    UC_data_container * scan_reserved, * ff;
    scan_reserved = this->scan;
    ff = &scans_list[scan->U_get_settings().ff_int];

    int x_min_roi = this->x_min;
    int x_max_roi = this->x_max;
    int y_min_roi = this->y_min;
    int y_max_roi = this->y_max;

    this->x_min = i * rebin_x + x_min_roi;
    this->y_min = j * rebin_y + y_min_roi;
    this->x_max = this->x_min + rebin_x;
    this->y_max = this->y_min + rebin_y;
    if (this->x_max > 3840) {
        this->x_max = 3840;
    }
    if (this->y_max > 256) {
        this->y_max = 256;
    }
    int squar = (x_max - x_min) * (y_max - y_min);

    QVector<double> data_y_scan;
    QVector<double> data_y_ff;
    QVector<double> data_thl;

    int n = static_cast<int>(ceil(static_cast<double>(thl_finish - thl_start + 1) / rebin_thl));

    double value;
    this->scan = ff;

    for (int index = 0; index < n; index++) {
        int thl_start = index * rebin_thl + this->thl_start;
        int thl_finish = thl_start + rebin_thl - 1;
        if (thl_finish > this->thl_finish) {
            thl_finish = this->thl_finish;
        }
        value = 0;
        for (int thl = thl_start; thl <= thl_finish; thl++) {
            value += U_get_frame_data(frame_type, pixel_type, thl);
        }
        value /= squar * (thl_finish - thl_start + 1);
        data_y_ff << value;
        data_thl << (thl_start + thl_finish) / 2;
    }

    data_y_ff = U_smooth(data_thl, data_y_ff, smoothing);
    data_y_ff = U_diff(data_thl, data_y_ff);
    data_y_ff = U_vector_minus(data_y_ff);
    data_y_ff = U_smooth(data_thl, data_y_ff, smoothing);

    this->scan = scan;

    for (int index = 0; index < n; index++) {
        int thl_start = index * rebin_thl + this->thl_start;
        int thl_finish = thl_start + rebin_thl - 1;
        if (thl_finish > this->thl_finish) {
            thl_finish = this->thl_finish;
        }
        value = 0;
        for (int thl = thl_start; thl <= thl_finish; thl++) {
            value += U_get_frame_data(frame_type, pixel_type, thl);
        }
        value /= squar * (thl_finish - thl_start + 1);
        data_y_scan << value;
    }

    data_y_scan = U_smooth(data_thl, data_y_scan, smoothing);
    data_y_scan = U_diff(data_thl, data_y_scan);
    data_y_scan = U_vector_minus(data_y_scan);
    data_y_scan = U_smooth(data_thl, data_y_scan, smoothing);
    data_y_scan = U_vector_divide(data_y_ff, data_y_scan);
    data_y_scan = U_smooth(data_thl, data_y_scan, smoothing);
    data_y_scan = U_vector_log(data_y_scan);

    scan->U_create_data_additional();
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            for (int index = 0; index < n; index++) {
                int thl_start = index * rebin_thl + this->thl_start;
                int thl_finish = thl_start + rebin_thl - 1;
                if (thl_finish > this->thl_finish) {
                    thl_finish = this->thl_finish;
                }
                for (int thl = thl_start; thl <= thl_finish; thl++) {
                    scan->U_set_data_additional(thl, x, y, data_y_scan[index]);
                }
            }
        }
    }

    this->scan = scan_reserved;
    this->x_min = x_min_roi;
    this->x_max = x_max_roi;
    this->y_min = y_min_roi;
    this->y_max = y_max_roi;
}

void UC_plot::U_calculating_id(UC_data_container * scan, QList<UC_data_container *> id_samples) {
    int current = 0;
    int total = 0;

    int number_of_samples = id_samples.size();

    QVector<int> thl_samples(number_of_samples);
    for (int i = 0; i < number_of_samples; i++) {
        thl_samples[i] = id_samples[i]->U_get_thl_sample();
    }

    total = (x_max - x_min) * (y_max - y_min);
    for (int x = x_min; x < x_max; x += rebin_x) {
        for (int y = y_min; y < y_max; y += rebin_y) {
            TMatrixD eqv_matr(number_of_samples, number_of_samples);
            TVectorD scan_thl_data(number_of_samples);
            for (int sample_index = 0; sample_index < number_of_samples; sample_index++) {
                for (int thl_index = 0; thl_index < number_of_samples; thl_index++) {
                    eqv_matr[thl_index][sample_index] = id_samples[sample_index]->U_get_data_additional(thl_samples[thl_index], x, y);
                }
            }
            for (int thl_index = 0; thl_index < number_of_samples; thl_index++) {
                scan_thl_data[thl_index] = scan->U_get_data_additional(thl_samples[thl_index], x, y);
            }
            TDecompLU decomp(eqv_matr);
            Bool_t ok;
            TVectorD result_vector = decomp.Solve(scan_thl_data, ok);
            int nr = 0;
            while (!ok) {
                decomp.SetMatrix(eqv_matr);
                decomp.SetTol(0.1 * decomp.GetTol());
                if (nr++ > 10) break;
                result_vector = decomp.Solve(scan_thl_data, ok);
            }
            for (int ix = 0; ix < rebin_x; ix++) {
                for (int iy = 0; iy < rebin_y; iy++) {
                    for (int i = 0; i < number_of_samples; i++) {
                        scan->U_set_id_data(x + ix, y + iy, result_vector[i]);
                    }
                }
            }
            current++;
            emit US_renew_progress_bar(current, total);
        }
    }
}

////////////////////////////!!!!!!!!!!SLOTS!!!!!!!!!!!///////////////////////////////////////////////////////////////////////////

void UC_plot::U_set_data(UC_data_container::UTStr_data_container_settings settings) {
    QString path = settings.path;
    QFile file;
    QDir dir(path);
    QStringList file_names = dir.entryList(QDir::Files, QDir::Name);
    qulonglong pixel_data;
    QTextStream txt_str;
    QString str;

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

    settings.n_thl = thl_counts;
    int n_thl = thl_counts - 1;
    UC_data_container * data_container;
    data_container = new UC_data_container(settings);
    emit US_n_files(thl_counts);

    int j = 0;
    int cnt0, cnt1, thl = 0;

    if (settings.both_counters) {
        for (int i = 0; i < file_names.size(); i++) {
            if (file_names[i].right(4) == ".pmf") {
                str = file_names[i].right(8);
                if (str == "THL0.pmf") continue;
                if (str == "THL1.pmf") continue;

                file.setFileName(path + "/" + file_names[i]);
                file.open(QFile::ReadOnly);
                txt_str.setDevice(&file);
                thl = str.left(3).toInt();

                for (int y = 0; y < 256; y++) {
                    for (int x = 0; x < (15 * 256); x++) {
                        txt_str >> pixel_data;
                        cnt0 = static_cast<int>(pixel_data & 0xffffffff);
                        cnt1 = static_cast<int>((pixel_data >> 32) & 0xffffffff);
                        data_container->U_set_data(thl, x, y, cnt0, 0);
                        data_container->U_set_data(thl, x, y, cnt1, 1);
                    }
                }
                emit US_new_thl(thl);

                file.close();
                emit US_renew_progress_bar(j, n_thl);
                j++;
            }
        }
        data_container->U_calculate_mean();
    } else {
        for (int i = 0; i < file_names.size(); i++) {
            if (file_names[i].right(4) == ".pmf") {
                str = file_names[i].right(8);
                if (str == "THL0.pmf") continue;
                if (str == "THL1.pmf") continue;

                file.setFileName(path + "/" + file_names[i]);
                file.open(QFile::ReadOnly);
                txt_str.setDevice(&file);
                thl = str.left(3).toInt();

                for (int y = 0; y < 256; y++) {
                    for (int x = 0; x < (15 * 256); x++) {
                        txt_str >> pixel_data;
                        cnt1 = static_cast<int>(pixel_data);
                        data_container->U_set_data(thl, x, y, cnt1, 1);
                    }
                }
                emit US_new_thl(thl);

                file.close();
                j++;
                emit US_renew_progress_bar(j, n_thl);
            }
        }
    }

    scans_list << *data_container;
    if (scan_enable == false) {
        scan = &(scans_list[scans_list.length() - 1]);
        scan_enable = true;
    }

    U_find_thl_range();
}

void UC_plot::U_reset_data() {
    scans_list.clear();

    thl_min = 512;
    thl_max = 0;

    U_reset_mask();
}

void UC_plot::U_delete_scan(int index) {
    if (index == -1) {
        scan_enable = false;
        return;
    }
    for (int i = 0; i < scans_list.size(); i++) {
        UC_data_container::UTStr_data_container_settings settings = scans_list[i].U_get_settings();
        if (settings.ff_int == index) {
            settings.ff_int = -1;
            scans_list[i].U_set_settings(settings);
        }
        if (settings.ff_int > index) {
            settings.ff_int--;
            scans_list[i].U_set_settings(settings);
        }
        if (settings.df_int == index) {
            settings.df_int = -1;
            scans_list[i].U_set_settings(settings);
        }
        if (settings.df_int > index) {
            settings.df_int--;
            scans_list[i].U_set_settings(settings);
        }
    }
    UC_data_container * scan_ptr = &(scans_list[index]);
    if (scan_ptr == scan) {
        if (scans_list.size() == 1) {
            scan_enable = false;
        } else {
            if (index == 0) {
                scan = &(scans_list[1]);
            } else {
                scan = &(scans_list[0]);
            }
        }
    }
    scans_list.removeAt(index);
    U_find_thl_range();
}

void UC_plot::U_set_scan(int index) {
    if (index == -1) {
        scan_enable = false;
    } else {
        scan = &(scans_list[index]);
        scan_enable = true;
        if (scan->U_get_settings().ff_int == -1) {
            ff_enable = false;
        } else {
            ff_enable = true;
            ff = &scans_list[scan->U_get_settings().ff_int];
        }
        if (scan->U_get_settings().df_int == -1) {
            df_enable = false;
        } else {
            df_enable = true;
            df = &scans_list[scan->U_get_settings().df_int];
        }
    }
}

void UC_plot::U_set_settings(int index, UC_data_container::UTStr_data_container_settings settings) {
    if (index == -1) return;
    scans_list[index].U_set_settings(settings);
    if (&(scans_list[index]) == scan) {
        if (scan->U_get_settings().ff_int == -1) {
            ff_enable = false;
        } else {
            ff_enable = true;
            ff = &scans_list[scan->U_get_settings().ff_int];
        }
        if (scan->U_get_settings().df_int == -1) {
            df_enable = false;
        } else {
            df_enable = true;
            df = &scans_list[scan->U_get_settings().df_int];
        }
    }
}

void UC_plot::U_get_settings(int index) {
    if (index != -1) emit US_scan_settings(scans_list[index].U_get_settings());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_plot::U_generate_spectra() {
    double data_y;
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        data_y = U_get_frame_data(frame_type, pixel_type, thl);
        emit US_spectra_data(thl, data_y);
        emit US_renew_progress_bar(thl - thl_start, thl_finish - thl_start);
    }
    emit US_replot_spectra();
}

void UC_plot::U_generate_spectra(double energy_min, double energy_max, int n) {
    double data_y;
    double e_delta = (energy_max - energy_min) / n;
    double e_max = 0;
    int i = 0;
    for (double e_min = energy_min; e_min < energy_max; e_min += e_delta) {
        e_max = e_min + e_delta;
        data_y = U_get_frame_data_energy(frame_type, pixel_type, e_min, e_max);
        emit US_spectra_data(e_min + e_delta / 2, data_y);
        emit US_renew_progress_bar(i, n);
        i++;
    }
    emit US_replot_spectra();
}

void UC_plot::U_generate_frame(int thl) {
    double z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) {
                emit US_frame_data(x, y, 0);
            } else {
                z = U_get_pixel_data(pixel_type, thl, x, y);
                emit US_frame_data(x, y, z);
            }
        }
    }
    emit US_replot_frame();
}

void UC_plot::U_generate_frame(double energy) {
    double z;
    for (int x = x_min; x < x_max; x++) {
        int thl = U_get_thl_from_energy(x, energy);
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) {
                emit US_frame_data(x, y, 0);
            } else {
                z = U_get_pixel_data(pixel_type, thl, x, y);
                emit US_frame_data(x, y, z);
            }
        }
    }
    emit US_replot_frame();
}

void UC_plot::U_generate_table(int thl) {
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

    widepix.U_set_data_enable(data_enable);
    roi.U_set_data_enable(data_enable);
    masked.U_set_data_enable(data_enable);
    unmasked.U_set_data_enable(data_enable);
    not_overflow.U_set_data_enable(data_enable);
    good_pixels_in_roi.U_set_data_enable(data_enable);

    overflow = scan->U_get_count() * 4095;
    if (ff_enable) {
        overflow_ff = ff->U_get_count() * 4095;
    } else {
        overflow_ff = std::numeric_limits<int>::max();
    }
    if (df_enable) {
        overflow_df = df->U_get_count() * 4095;
    } else {
        overflow_df = std::numeric_limits<int>::max();
    }

    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip] = new UC_pixels_info(static_cast<UC_pixels_info::UTE_pixels_area>(chip));
        chip_info[chip]->U_set_data_enable(data_enable);
    }

    int i = 0;
    for (int chip = 0; chip < 15; chip++) {
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                x_widepix = U_get_coord_x_chip(chip, x);
                z = U_get_pixel_data(pixel_type, thl, x_widepix, y);

                widepix.U_add_pixel_1(z);
                chip_info[chip]->U_add_pixel_1(z);

                mask = U_get_mask(x_widepix, y);
                in_roi = (x_widepix >= x_min) & (x_widepix < x_max) & (y >= y_min) & (y < y_max);
                cnt0 = scan->U_get_data(thl, x_widepix, y, 0);
                cnt1 = scan->U_get_data(thl, x_widepix, y, 1);
                if (ff_enable) {
                    cnt0ff = ff->U_get_data(thl, x_widepix, y, 0);
                    cnt1ff = ff->U_get_data(thl, x_widepix, y, 1);
                } else {
                    cnt0ff = 0;
                    cnt1ff = 0;
                }
                if (df_enable) {
                    cnt0df = df->U_get_data(thl, x_widepix, y, 0);
                    cnt1df = df->U_get_data(thl, x_widepix, y, 1);
                } else {
                    cnt0df = 0;
                    cnt1df = 0;
                }
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

    if (data_enable.std_dev_enable | data_enable.snr_enable) {
        for (int chip = 0; chip < 15; chip++) {
            for (int y = 0; y < 256; y++) {
                for (int x = 0; x < 256; x++) {
                    x_widepix = U_get_coord_x_chip(chip, x);
                    z = U_get_pixel_data(pixel_type, thl, x_widepix, y);

                    widepix.U_add_pixel_2(z);
                    chip_info[chip]->U_add_pixel_2(z);

                    mask = U_get_mask(x_widepix, y);
                    in_roi = (x_widepix >= x_min) & (x_widepix < x_max) & (y >= y_min) & (y < y_max);
                    cnt0 = scan->U_get_data(thl, x_widepix, y, 0);
                    cnt1 = scan->U_get_data(thl, x_widepix, y, 1);
                    if (ff_enable) {
                        cnt0ff = ff->U_get_data(thl, x_widepix, y, 0);
                        cnt1ff = ff->U_get_data(thl, x_widepix, y, 1);
                    } else {
                        cnt0ff = 0;
                        cnt1ff = 0;
                    }
                    if (df_enable) {
                        cnt0df = df->U_get_data(thl, x_widepix, y, 0);
                        cnt1df = df->U_get_data(thl, x_widepix, y, 1);
                    } else {
                        cnt0df = 0;
                        cnt1df = 0;
                    }
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
    }

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

void UC_plot::U_generate_table(double energy) {
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

    widepix.U_set_data_enable(data_enable);
    roi.U_set_data_enable(data_enable);
    masked.U_set_data_enable(data_enable);
    unmasked.U_set_data_enable(data_enable);
    not_overflow.U_set_data_enable(data_enable);
    good_pixels_in_roi.U_set_data_enable(data_enable);

    overflow = scan->U_get_count() * 4095;
    if (ff_enable) {
        overflow_ff = ff->U_get_count() * 4095;
    } else {
        overflow_ff = std::numeric_limits<int>::max();
    }
    if (df_enable) {
        overflow_df = df->U_get_count() * 4095;
    } else {
        overflow_df = std::numeric_limits<int>::max();
    }

    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip] = new UC_pixels_info(static_cast<UC_pixels_info::UTE_pixels_area>(chip));
        chip_info[chip]->U_set_data_enable(data_enable);
    }

    int i = 0;
    for (int chip = 0; chip < 15; chip++) {
        int thl = U_get_thl_from_energy_chip(chip, energy);
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                x_widepix = U_get_coord_x_chip(chip, x);
                z = U_get_pixel_data(pixel_type, thl, x_widepix, y);

                widepix.U_add_pixel_1(z);
                chip_info[chip]->U_add_pixel_1(z);

                mask = U_get_mask(x_widepix, y);
                in_roi = (x_widepix >= x_min) & (x_widepix < x_max) & (y >= y_min) & (y < y_max);
                cnt0 = scan->U_get_data(thl, x_widepix, y, 0);
                cnt1 = scan->U_get_data(thl, x_widepix, y, 1);
                if (ff_enable) {
                    cnt0ff = ff->U_get_data(thl, x_widepix, y, 0);
                    cnt1ff = ff->U_get_data(thl, x_widepix, y, 1);
                } else {
                    cnt0ff = 0;
                    cnt1ff = 0;
                }
                if (df_enable) {
                    cnt0df = df->U_get_data(thl, x_widepix, y, 0);
                    cnt1df = df->U_get_data(thl, x_widepix, y, 1);
                } else {
                    cnt0df = 0;
                    cnt1df = 0;
                }
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

    if (data_enable.std_dev_enable | data_enable.snr_enable) {
        for (int chip = 0; chip < 15; chip++) {
            int thl = U_get_thl_from_energy_chip(chip, energy);
            for (int y = 0; y < 256; y++) {
                for (int x = 0; x < 256; x++) {
                    x_widepix = U_get_coord_x_chip(chip, x);
                    z = U_get_pixel_data(pixel_type, thl, x_widepix, y);

                    widepix.U_add_pixel_2(z);
                    chip_info[chip]->U_add_pixel_2(z);

                    mask = U_get_mask(x_widepix, y);
                    in_roi = (x_widepix >= x_min) & (x_widepix < x_max) & (y >= y_min) & (y < y_max);

                    cnt0 = scan->U_get_data(thl, x_widepix, y, 0);
                    cnt1 = scan->U_get_data(thl, x_widepix, y, 1);
                    if (ff_enable) {
                        cnt0ff = ff->U_get_data(thl, x_widepix, y, 0);
                        cnt1ff = ff->U_get_data(thl, x_widepix, y, 1);
                    } else {
                        cnt0ff = 0;
                        cnt1ff = 0;
                    }
                    if (df_enable) {
                        cnt0df = df->U_get_data(thl, x_widepix, y, 0);
                        cnt1df = df->U_get_data(thl, x_widepix, y, 1);
                    } else {
                        cnt0df = 0;
                        cnt1df = 0;
                    }
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
    }

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
    long total;
    if (data_enable.std_dev_enable | data_enable.snr_enable) {
        total = 256 * 256 * 15 * 2 * (thl_finish - thl_start);
    } else {
        total = 256 * 256 * 15 * 1 * (thl_finish - thl_start);
    }
    bool in_roi, mask, overflowed, in_thl1_d, in_thl2_d;
    UC_pixels_info widepix(UC_pixels_info::UTE_PA_widepix);
    UC_pixels_info roi(UC_pixels_info::UTE_PA_roi);
    UC_pixels_info masked(UC_pixels_info::UTE_PA_masked);
    UC_pixels_info unmasked(UC_pixels_info::UTE_PA_unmasked);
    UC_pixels_info not_overflow(UC_pixels_info::UTE_PA_not_overflow);
    UC_pixels_info good_pixels_in_roi(UC_pixels_info::UTE_PA_roi_unmasked_not_ovf);
    UC_pixels_info roi_thlid1_thlid2(UC_pixels_info::UTE_PA_roi_thlid1_thlid2);
    UC_pixels_info roi_thlid3_thlid4(UC_pixels_info::UTE_PA_roi_thlid3_thlid4);
    UC_pixels_info* chip_info[15];

    widepix.U_set_data_enable(data_enable);
    roi.U_set_data_enable(data_enable);
    masked.U_set_data_enable(data_enable);
    unmasked.U_set_data_enable(data_enable);
    not_overflow.U_set_data_enable(data_enable);
    good_pixels_in_roi.U_set_data_enable(data_enable);
    roi_thlid1_thlid2.U_set_data_enable(data_enable);
    roi_thlid3_thlid4.U_set_data_enable(data_enable);

    overflow = scan->U_get_count() * 4095;
    if (ff_enable) {
        overflow_ff = ff->U_get_count() * 4095;
    } else {
        overflow_ff = std::numeric_limits<int>::max();
    }
    if (df_enable) {
        overflow_df = df->U_get_count() * 4095;
    } else {
        overflow_df = std::numeric_limits<int>::max();
    }

    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip] = new UC_pixels_info(static_cast<UC_pixels_info::UTE_pixels_area>(chip));
        chip_info[chip]->U_set_data_enable(data_enable);
    }

    int i = 0;
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        for (int chip = 0; chip < 15; chip++) {
            for (int y = 0; y < 256; y++) {
                for (int x = 0; x < 256; x++) {
                    x_widepix = U_get_coord_x_chip(chip, x);
                    z = U_get_pixel_data(pixel_type, thl, x_widepix, y);

                    widepix.U_add_pixel_1(z);
                    chip_info[chip]->U_add_pixel_1(z);

                    mask = U_get_mask(x_widepix, y);
                    in_roi = (x_widepix >= x_min) & (x_widepix < x_max) & (y >= y_min) & (y < y_max);
                    in_thl1_d = (thl >= thl_id_1) & (thl <= thl_id_2);
                    in_thl2_d = (thl >= thl_id_3) & (thl <= thl_id_4);
                    cnt0 = scan->U_get_data(thl, x_widepix, y, 0);
                    cnt1 = scan->U_get_data(thl, x_widepix, y, 1);
                    if (ff_enable) {
                        cnt0ff = ff->U_get_data(thl, x_widepix, y, 0);
                        cnt1ff = ff->U_get_data(thl, x_widepix, y, 1);
                    } else {
                        cnt0ff = 0;
                        cnt1ff = 0;
                    }
                    if (df_enable) {
                        cnt0df = df->U_get_data(thl, x_widepix, y, 0);
                        cnt1df = df->U_get_data(thl, x_widepix, y, 1);
                    } else {
                        cnt0df = 0;
                        cnt1df = 0;
                    }
                    overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                    overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
                    overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);

                    if (in_thl1_d) {
                        widepix.U_add_pixel_id_1(z);
                        chip_info[chip]->U_add_pixel_id_1(z);
                    }
                    if (in_thl2_d) {
                        widepix.U_add_pixel_id_2(z);
                        chip_info[chip]->U_add_pixel_id_2(z);
                    }

                    if (mask) {
                        widepix.U_add_masked();
                        chip_info[chip]->U_add_masked();
                        masked.U_add_pixel_1(z);
                        masked.U_add_masked();
                        if (overflowed) masked.U_add_overflow();
                        if (in_thl1_d) {
                            masked.U_add_pixel_id_1(z);
                        }
                        if (in_thl2_d) {
                            masked.U_add_pixel_id_2(z);
                        }
                    } else {
                        unmasked.U_add_pixel_1(z);
                        if (overflowed) unmasked.U_add_overflow();
                        if (in_thl1_d) unmasked.U_add_pixel_id_1(z);
                        if (in_thl2_d) unmasked.U_add_pixel_id_2(z);
                    }
                    if (overflowed) {
                        widepix.U_add_overflow();
                        chip_info[chip]->U_add_overflow();
                    } else {
                        not_overflow.U_add_pixel_1(z);
                        if (mask) not_overflow.U_add_masked();
                        if (in_thl1_d) not_overflow.U_add_pixel_id_1(z);
                        if (in_thl2_d) not_overflow.U_add_pixel_id_2(z);
                    }
                    if (in_roi) {
                        roi.U_add_pixel_1(z);
                        if (mask) roi.U_add_masked();
                        if (overflowed) roi.U_add_overflow();
                        if (in_thl1_d) {
                            roi.U_add_pixel_id_1(z);
                            roi_thlid1_thlid2.U_add_pixel_1(z);
                            roi_thlid1_thlid2.U_add_pixel_id_1(z);
                            if (mask) roi_thlid1_thlid2.U_add_masked();
                            if (overflowed) roi_thlid1_thlid2.U_add_overflow();
                        }
                        if (in_thl2_d) {
                            roi.U_add_pixel_id_2(z);
                            roi_thlid3_thlid4.U_add_pixel_1(z);
                            roi_thlid3_thlid4.U_add_pixel_id_2(z);
                            if (mask) roi_thlid3_thlid4.U_add_masked();
                            if (overflowed) roi_thlid3_thlid4.U_add_overflow();
                        }
                        if (!(overflowed | mask)) {
                            good_pixels_in_roi.U_add_pixel_1(z);
                            if (in_thl1_d) good_pixels_in_roi.U_add_pixel_id_1(z);
                            if (in_thl2_d) good_pixels_in_roi.U_add_pixel_id_2(z);
                        }
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
    roi_thlid1_thlid2.U_finalize_1();
    roi_thlid3_thlid4.U_finalize_1();
    for (int chip = 0; chip < 15; chip++) {
        chip_info[chip]->U_finalize_1();
    }
    if (data_enable.std_dev_enable | data_enable.snr_enable) {
        for (int thl = thl_start; thl <= thl_finish; thl++) {
            for (int chip = 0; chip < 15; chip++) {
                for (int y = 0; y < 256; y++) {
                    for (int x = 0; x < 256; x++) {
                        x_widepix = U_get_coord_x_chip(chip, x);
                        z = U_get_pixel_data(pixel_type, thl, x_widepix, y);

                        widepix.U_add_pixel_2(z);
                        chip_info[chip]->U_add_pixel_2(z);

                        mask = U_get_mask(x_widepix, y);
                        in_roi = (x_widepix >= x_min) & (x_widepix < x_max) & (y >= y_min) & (y < y_max);
                        in_thl1_d = (thl >= thl_id_1) & (thl <= thl_id_2);
                        in_thl2_d = (thl >= thl_id_3) & (thl <= thl_id_4);
                        cnt0 = scan->U_get_data(thl, x_widepix, y, 0);
                        cnt1 = scan->U_get_data(thl, x_widepix, y, 1);
                        if (ff_enable) {
                            cnt0ff = ff->U_get_data(thl, x_widepix, y, 0);
                            cnt1ff = ff->U_get_data(thl, x_widepix, y, 1);
                        } else {
                            cnt0ff = 0;
                            cnt1ff = 0;
                        }
                        if (df_enable) {
                            cnt0df = df->U_get_data(thl, x_widepix, y, 0);
                            cnt1df = df->U_get_data(thl, x_widepix, y, 1);
                        } else {
                            cnt0df = 0;
                            cnt1df = 0;
                        }
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
                            if (in_thl1_d) {
                                roi_thlid1_thlid2.U_add_pixel_2(z);
                            }
                            if (in_thl2_d) {
                                roi_thlid3_thlid4.U_add_pixel_2(z);
                            }
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
        roi_thlid1_thlid2.U_finalize_2();
        roi_thlid3_thlid4.U_finalize_2();
    }

    emit US_chip_data(widepix);
    emit US_chip_data(roi);
    emit US_chip_data(masked);
    emit US_chip_data(unmasked);
    emit US_chip_data(not_overflow);
    emit US_chip_data(good_pixels_in_roi);
    emit US_chip_data(roi_thlid1_thlid2);
    emit US_chip_data(roi_thlid3_thlid4);
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
    int total = (x_max - x_min) * (y_max - y_min) * (thl_finish - thl_start);
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) continue;
            for (int thl = thl_start; thl <= thl_finish; thl++) {
                z = U_get_pixel_data(pixel_type, thl, x, y);
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

void UC_plot::U_generate_frame_distribution(int n_bins, double min, double max, int thl) {
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
            z = U_get_pixel_data(pixel_type, thl, x, y);
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

void UC_plot::U_generate_calibration(int chip) {
    int n = scan->U_get_n();
    int hist_thl_min = scan->U_get_thl_min();
    int hist_thl_max = scan->U_get_thl_max();
    double * thl_data = new double [n];
    double * data = new double [n];
    int x_min = chip * 256;
    int x_max = x_min + 256;
    int i = 0;
    for (int thl = hist_thl_min; thl < hist_thl_max; thl++) {
        thl_data[i] = thl;
//        for (int x = x_min; x < x_max; x++) {
//            for (int y = 0; y < 256; y++) {
//                if (!U_get_mask(x, y)) {
//                    data[i] += U_get_pixel_data_2(thl, x, y);
//                }
//            }
//        }
        this->x_max = x_max;
        this->x_min = x_min;
        data[i] = U_get_frame_data(frame_type, pixel_type, thl);
        emit US_calibration_chip_data(thl_data[i], data[i], false);
        if (qAbs(data[i]) > 1e-10) data[i] = qLn(data[i]);
        i++;
    }
    TGraph gr(n, thl_data, data);
    TF1 fit("fit", "[0] + [1] * x");
    gr.Fit("fit", "M", "", hist_thl_min + 1, hist_thl_max - 1);

    for (int thl = hist_thl_min; thl < hist_thl_max; thl++) {
        emit US_calibration_chip_data(thl, qExp(fit.Eval(thl)), true);
    }

    emit US_replot_calibration_chip();
}

void UC_plot::U_generate_calibration() {
    UC_data_container * scan_r = scan;
    int n_scans = scans_list.size();
    int total = n_scans * 15 + 14;
    int current_progress = 0;
    QVector<double> energy_chip;
    QVector<double> thl_chip[15];
    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
        scan = &(scans_list[scan_i]);
        if (!scan->U_get_calibration()) continue;
        int n = scan->U_get_n();
        int hist_thl_min = scan->U_get_thl_min();
        int hist_thl_max = scan->U_get_thl_max();
        for (int chip = 0; chip < 15; chip++) {
            int x_min = chip * 256;
            int x_max = x_min + 256;
            QVector<double> data_current(n);
            QVector<double> thl_current(n);
            int i = 0;
            double data;
            for (int thl = hist_thl_min; thl <= hist_thl_max; thl++) {
                thl_current[i] = thl;
//                for (int x = x_min; x < x_max; x++) {
//                    for (int y = 0; y < 256; y++) {
//                        if (!U_get_mask(x, y)) {
//                            data_current[i] += U_get_pixel_data_2(thl, x, y);
//                        }
//                    }
//                }
                this->x_max = x_max;
                this->x_min = x_min;
                data = U_get_frame_data(frame_type, pixel_type, thl);
                if (data > 1e-10) data_current[i] = qLn(data);
                    else data_current[i] = data;
                i++;
            }
            TGraph gr(n, thl_current.data(), data_current.data());
            TF1 fit("fit", "[0] + [1] * x");
            fit.SetParameters(29, -0.1);
            gr.Fit("fit", "M", "", hist_thl_min + 1, hist_thl_max - 1);
            double a = fit.GetParameter(1);
            double b = fit.GetParameter(0);
            double thl_value = (qLn(threshold_level) - b) / a;
            thl_chip[chip] << thl_value;
            emit US_renew_progress_bar(current_progress, total);
            current_progress++;
        }
        energy_chip << scans_list[scan_i].U_get_energy();
    }
    int n = thl_chip->size();
    for (int chip = 0; chip < 15; chip++) {
        for (int i = 0; i < n; i++) {
            US_calibration_data(thl_chip[chip][i], energy_chip[i], chip, false);
        }
        TGraph gr(n, thl_chip[chip].data(), energy_chip.data());
        TF1 fit("fit", "[0] + [1] * x + [2] * x * x");
        //fit.SetParameters(-2.3, 0.26, 0.00018);
        gr.Fit("fit", "M", "", energy_chip[0], energy_chip[n - 1]);
        calibration_p2[chip] = fit.GetParameter(2);
        calibration_p1[chip] = fit.GetParameter(1);
        calibration_p0[chip] = fit.GetParameter(0);
        for (int i = 0; i < n; i++) {
            US_calibration_data(thl_chip[chip][i], fit.Eval(thl_chip[chip][i]), chip, true);
        }
        emit US_renew_progress_bar(current_progress, total);
        current_progress++;
    }

    emit US_replot_calibration();
    scan = scan_r;
    using_calibration = true;


//    UC_data_container * scan_r = scan;
//    int n_scans = scans_list.size();
//    int total = n_scans * 15 + 14;
//    int current_progress = 0;
//    QVector<double> energy_chip;
//    QVector<double> thl_chip[15];
//    for (int chip = 0; chip < 15; chip++) {
//        int x_min = chip * 256;
//        int x_max = x_min + 256;
//        for (int scan_i = 0; scan_i < n_scans; scan_i++) {
//            scan = &(scans_list[scan_i]);
//            if (!scan->U_get_calibration()) continue;
//            int hist_thl_min = scan->U_get_thl_min();
//            int hist_thl_max = scan->U_get_thl_max();
//            int thl_edge = 0;
//            for (int thl = hist_thl_min; thl <= hist_thl_max; thl++) {
//                int data_cnt = 0;
//                int pix_cnt = 0;
//                for (int x = x_min; x < x_max; x++) {
//                    for (int y = 0; y < 256; y++) {
//                        if (!U_get_mask(x, y)) {
//                            data_cnt += U_get_pixel_data_40(thl, x, y);
//                            pix_cnt ++;
//                        }
//                    }
//                }
//                if (data_cnt > (pix_cnt / 2)) {
//                    thl_edge = thl;
//                    break;
//                }
//            }
//            thl_chip[chip] << thl_edge;

//        }
//    }
//    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
//        scan = &(scans_list[scan_i]);
//        if (!scan->U_get_calibration()) continue;
//        energy_chip << scans_list[scan_i].U_get_energy();
//    }

//    int n = thl_chip->size();
//    for (int chip = 0; chip < 15; chip++) {
//        for (int i = 0; i < n; i++) {
//            US_calibration_data(thl_chip[chip][i], energy_chip[i], chip, false);
//        }
//        TGraph gr(n, thl_chip[chip].data(), energy_chip.data());
//        TF1 fit("fit", "[0] + [1] * x + [2] * x * x");
//        fit.SetParameters(-2.3, 0.26, 0.00018);
//        gr.Fit("fit", "M", "", energy_chip[0], energy_chip[n - 1]);
//        calibration_p2[chip] = fit.GetParameter(2);
//        calibration_p1[chip] = fit.GetParameter(1);
//        calibration_p0[chip] = fit.GetParameter(0);
//        for (int i = 0; i < n; i++) {
//            US_calibration_data(thl_chip[chip][i], fit.Eval(thl_chip[chip][i]), chip, true);
//        }
//        emit US_renew_progress_bar(current_progress, total);
//        current_progress++;
//    }

//    emit US_replot_calibration();
//    scan = scan_r;
//    using_calibration = true;
}


void UC_plot::U_generate_identification_roi() {
    switch (identification_type) {
        case UTE_IT_GA_method : {
            U_identification_roi_1();
            return;
        }
        case UTE_IT_linear_combination : {
            U_identification_roi_2();
            return;
        }
    }
}

void UC_plot::U_generate_additional_data() {
    int nx = static_cast<int>(ceil(static_cast<double>(this->x_max - this->x_min) / rebin_x));
    int ny = static_cast<int>(ceil(static_cast<double>(this->y_max - this->y_min) / rebin_y));
    int current = 0;
    int total = nx * ny;
    for (int x = 0; x < nx; x++) {
        for (int y = 0; y < ny; y++) {
            U_calculating_xmu(scan, x, y, rebin_x, rebin_y, rebin_thl);
            current++;
            emit US_renew_progress_bar(current, total);
        }
    }
}

void UC_plot::U_generate_identification_data() {
    QList<UC_data_container *> id_samples;
    QList<QString> elements;

    scan->U_clean_id_data(x_min, x_max, y_min, y_max);
    U_generate_additional_data();

    UC_data_container * scan_reserved = scan;
    for (int i = 0; i < scans_list.size(); i++) {
        if (scans_list[i].U_get_sample_of_element()) {
            id_samples << &(scans_list[i]);
            scan = &(scans_list[i]);
            U_generate_additional_data();
            elements << scan->U_get_name();
        }
    }
    scan = scan_reserved;

    U_calculating_id(scan, id_samples);
    emit US_identification_scan_list(elements);
}

void UC_plot::U_generate_identification_frame(int element_index) {
    double z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) {
                emit US_identification_frame_data(x, y, 0);
            } else {
                z = scan->U_get_id_data(x, y, element_index);
                emit US_identification_frame_data(x, y, z);
            }
        }
    }
    emit US_replot_identificaton_frame();
}


void UC_plot::U_generate_range(int thl) {
    double z, max = -9999999999999, min = 9999999999999999;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) continue;
            z = U_get_pixel_data(pixel_type, thl, x, y);
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
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl, x, y);
                if (z > max) max = z;
                if (z < min) min = z;
                i++;
                emit US_renew_progress_bar(i, total);
            }
        }
    }
    emit US_range_data(min, max);
}
///



///////////////////////////////////////////////////////////////////////////////////////////////////////


void UC_plot::U_set_frame_type(UTE_frame_type frame_type) {
    this->frame_type = frame_type;
}

void UC_plot::U_set_pixel_type(UTE_pixel_type pixel_type) {
    this->pixel_type = pixel_type;
}

void UC_plot::U_set_identification_type(UC_plot::UTE_identification_type identification_type) {
    this->identification_type = identification_type;
}

void UC_plot::U_set_roi(int x_min, int x_max, int y_min, int y_max) {
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
}

void UC_plot::U_set_rebin(int rebin_x, int rebin_y, int rebin_thl) {
    this->rebin_x   = rebin_x;
    this->rebin_y   = rebin_y;
    this->rebin_thl = rebin_thl;
}

void UC_plot::U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4) {
    this->thl_id_1 = thl_id_1;
    this->thl_id_2 = thl_id_2;
    this->thl_id_3 = thl_id_3;
    this->thl_id_4 = thl_id_4;
}

void UC_plot::U_set_threshold_range(int thl_start, int thl_finish) {
    this->thl_start = thl_start;
    this->thl_finish = thl_finish;
}

void UC_plot::U_set_data_enable(UC_pixels_info::UTStr_data_enable data_enable) {
    this->data_enable = data_enable;
}

void UC_plot::U_set_using_calibraion(bool enable) {
    using_calibration = enable;
}

void UC_plot::U_set_threshold_level(int level) {
    this->threshold_level = level;
}

void UC_plot::U_set_smoothing(int smoothing) {
    this->smoothing = smoothing;
}

void UC_plot::U_save_calibration(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int i = 0; i < 15; i++) {
        str << calibration_p0[i] << " ";
        str << calibration_p1[i] << " ";
        str << calibration_p2[i];
        str << endl;
    }
    file.close();
}

void UC_plot::U_load_calibration(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    for (int i = 0; i < 15; i++) {
        str >> calibration_p0[i];
        str >> calibration_p1[i];
        str >> calibration_p2[i];
    }
    file.close();
}
///////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_set_mask(bool mask, bool more, double value, bool in_roi, int thl) {
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
            data = U_get_pixel_data(pixel_type, thl, x, y);
            if (more) {
                if (data > value) {
                    U_set_mask(x, y, mask);
                }
            } else {
                if (data < value) U_set_mask(x, y, mask);
            }
        }
    }
}

void UC_plot::U_count_mask(bool more, double value, bool in_roi, int thl) {
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
            data = U_get_pixel_data(pixel_type, thl, x, y);
            if (more) {
                if (data > value) n++;
            } else {
                if (data < value) n++;
            }
        }
    }
    emit US_count_mask(n);
}

void UC_plot::U_set_mask_overflowed(bool in_roi, int thl) {
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

    int overflow = scan->U_get_count() * 4095;
    int overflow_ff = ff->U_get_count() * 4095;
    int overflow_df = df->U_get_count() * 4095;
    int cnt0, cnt1, cnt0ff, cnt1ff, cnt0df, cnt1df;
    bool overflowed = false;

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            cnt0 = scan->U_get_data(thl, x, y, 0);
            cnt1 = scan->U_get_data(thl, x, y, 1);
            if (ff_enable) {
                cnt0ff = ff->U_get_data(thl, x, y, 0);
                cnt1ff = ff->U_get_data(thl, x, y, 1);
            } else {
                cnt0ff = 0;
                cnt1ff = 0;
            }
            if (df_enable) {
                cnt0df = df->U_get_data(thl, x, y, 0);
                cnt1df = df->U_get_data(thl, x, y, 1);
            } else {
                cnt0df = 0;
                cnt1df = 0;
            }
            overflowed = (cnt0 == overflow) | (cnt1 == overflow);
            overflowed |= (cnt0ff == overflow_ff) | (cnt1ff == overflow_ff);
            overflowed |= (cnt0df == overflow_df) | (cnt1df == overflow_df);
            if (overflowed) U_set_mask(x, y, true);
        }
    }
}

void UC_plot::U_set_mask_overflowed(bool in_roi) {
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        U_set_mask_overflowed(in_roi, thl);
    }
}

void UC_plot::U_reset_mask() {
    for (int i = 0; i < (15 * 256 * 256); i++) {
        mask[i] = false;
    }
}

void UC_plot::U_mask_selected(int x_min, int x_max, int y_min, int y_max) {
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            U_set_mask(x, y, true);
        }
    }
}

void UC_plot::U_mask_selected_value(double value_min, double value_max, bool in_roi) {
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

    for (int thl = thl_start; thl < thl_finish; thl++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {

                data = U_get_pixel_data(pixel_type, thl, x, y);
                if ((data < value_max) && (value_min < data)) {
                    U_set_mask(x, y, true);
                }
            }
        }
    }
}

void UC_plot::U_mask_selected_value_thl(double value_min, double value_max, bool in_roi, int thl) {
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
            data = U_get_pixel_data(pixel_type, thl, x, y);
            if ((data < value_max) && (value_min < data)) {
                U_set_mask(x, y, true);
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////

//    element_vector.clear();
//    U_preoperating(scan);

//    int n_scans = scans_list.size();
//    int number_of_samples = 0;
//    UC_data_container * current_scan_i, * current_scan_j;
//    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
//        current_scan_i = &(scans_list[scan_i]);
//        if (current_scan_i->U_get_sample_of_element()) {
//            number_of_samples++;
//            U_preoperating(current_scan_i);
//            element_vector << current_scan_i->U_get_settings().element;
//    }}

//    TMatrixD eqv_matr(number_of_samples, number_of_samples);
//    TVectorD scan_thl_data(number_of_samples);

//    int index_scan_i = 0, index_scan_j = 0;

//    for (int x = x_min; x < x_max; x++) {
//        for (int y = y_min; y < y_max; y++) {
//            for (int scan_i = 0; scan_i < n_scans; scan_i++) {
//                current_scan_i = &(scans_list[scan_i]);
//                if (!current_scan_i->U_get_sample_of_element()) continue;
//                index_scan_j = 0;
//                int thl = current_scan_i->U_get_thl_sample();
//                for (int scan_j = 0; scan_j < n_scans; scan_j++) {
//                    current_scan_j = &(scans_list[scan_j]);
//                    if (!current_scan_j->U_get_sample_of_element()) continue;
//                    double data = current_scan_j->U_get_data_additional(thl, x, y);
//                    eqv_matr[index_scan_i][index_scan_j] = data;
//                    index_scan_j++;
//                }
//                double data = scan->U_get_data_additional(thl, x, y);
//                scan_thl_data[index_scan_i] = data;
//                index_scan_i++;
//            }

//            TDecompLU decomp(eqv_matr);
//            Bool_t ok;
//            TVectorD result = decomp.Solve(scan_thl_data, ok);
//            int nr = 0;
//            while (!ok) {
//                decomp.SetMatrix(eqv_matr);
//                decomp.SetTol(0.1 * decomp.GetTol());
//                if (nr++ > 10) break;
//                result = decomp.Solve(scan_thl_data, ok);
//            }
//            for (int i = 0; i < number_of_samples; i++) {
//                scan->U_set_id_data(x, y, result[i]);
//            }
//        }
//    }
//    for (int x = x_min; x < x_max; x++) {
//        for (int y = y_min; y < y_max; y++) {
//            for (int scan_i = 0; scan_i < n_scans; scan_i++) {
//                current_scan_i = &(scans_list[scan_i]);
//                if (!current_scan_i->U_get_sample_of_element()) continue;
//                index_scan_j = 0;
//                int thl = current_scan_i->U_get_thl_sample();
//                for (int scan_j = 0; scan_j < n_scans; scan_j++) {
//                    current_scan_j = &(scans_list[scan_j]);
//                    if (!current_scan_j->U_get_sample_of_element()) continue;
//                    double data = current_scan_j->U_get_data_additional(thl, x, y);
//                    eqv_matr[index_scan_i][index_scan_j] = data;
//                    index_scan_j++;
//                }
//                double data = scan->U_get_data_additional(thl, x, y);
//                scan_thl_data[index_scan_i] = data;
//                index_scan_i++;
//            }

//            TDecompLU decomp(eqv_matr);
//            Bool_t ok;
//            TVectorD result = decomp.Solve(scan_thl_data, ok);
//            int nr = 0;
//            while (!ok) {
//                decomp.SetMatrix(eqv_matr);
//                decomp.SetTol(0.1 * decomp.GetTol());
//                if (nr++ > 10) break;
//                result = decomp.Solve(scan_thl_data, ok);
//            }
//            for (int i = 0; i < number_of_samples; i++) {
//                scan->U_set_id_data(x, y, result[i]);
//            }
//        }
//    }

//    int n_scans = scans_list.size();
//    int number_of_samples = 0;
//    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
//        if (scans_list[scan_i].U_get_sample_of_element()) number_of_samples++;
//    }

//    UC_data_container * scan;
//    scan = this->scan;


//    QVector<double> data_y_scan(thl_finish - thl_start);
//    QVector<double> data_y_ff(thl_finish - thl_start);
//    QVector<double> data_thl(thl_finish - thl_start);

//    UC_data_container * ff = &scans_list[scan->U_get_settings().ff_int];
//    this->scan = ff;
//    for (int thl_index = thl_start; thl_index < thl_finish; thl_index++) {
//        data_y_ff[thl_index - thl_start] = U_get_frame_data(frame_type, pixel_type, thl_index);
//        data_thl[thl_index - thl_start] = thl_index;
//    }
//    data_y_ff = U_smooth(data_thl, data_y_ff);
//    data_y_ff = U_diff(data_thl, data_y_ff);
//    data_y_ff = U_vector_minus(data_y_ff);
//    data_y_ff = U_smooth(data_thl, data_y_ff);
//    this->scan = scan;
//    for (int thl_index = thl_start; thl_index < thl_finish; thl_index++) {
//        data_y_scan[thl_index - thl_start] = U_get_frame_data(frame_type, pixel_type, thl_index);
//    }
//    data_y_scan = U_smooth(data_thl, data_y_scan);
//    data_y_scan = U_diff(data_thl, data_y_scan);
//    data_y_scan = U_vector_minus(data_y_scan);
//    data_y_scan = U_smooth(data_thl, data_y_scan);
//    data_y_scan = U_vector_divide(data_y_ff, data_y_scan);
//    data_y_scan = U_smooth(data_thl, data_y_scan);
//    data_y_scan = U_vector_log(data_y_scan);

//    TMatrixD eqv_matr(number_of_samples, number_of_samples);
//    TVectorD scan_thl_data(number_of_samples);

//    QVector<double> * data_y_samples[number_of_samples];
//    int current_scan = 0;
//    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
//        scan = &(scans_list[scan_i]);
//        if (!scan->U_get_sample_of_element()) continue;
//        QVector<double> data_y_scan(thl_finish - thl_start);
//        QVector<double> data_y_ff(thl_finish - thl_start);
//        UC_data_container * ff = &scans_list[scan->U_get_settings().ff_int];
//        this->scan = ff;
//        for (int thl_index = thl_start; thl_index < thl_finish; thl_index++) {
//            data_y_ff[thl_index - thl_start] = U_get_frame_data(frame_type, pixel_type, thl_index);
//        }
//        data_y_ff = U_smooth(data_thl, data_y_ff);
//        data_y_ff = U_diff(data_thl, data_y_ff);
//        data_y_ff = U_vector_minus(data_y_ff);
//        data_y_ff = U_smooth(data_thl, data_y_ff);
//        this->scan = scan;
//        for (int thl_index = thl_start; thl_index < thl_finish; thl_index++) {
//            data_y_scan[thl_index - thl_start] = U_get_frame_data(frame_type, pixel_type, thl_index);
//        }
//        data_y_scan = U_smooth(data_thl, data_y_scan);
//        data_y_scan = U_diff(data_thl, data_y_scan);
//        data_y_scan = U_vector_minus(data_y_scan);
//        data_y_scan = U_smooth(data_thl, data_y_scan);
//        data_y_scan = U_vector_divide(data_y_ff, data_y_scan);
//        data_y_scan = U_smooth(data_thl, data_y_scan);
//        data_y_scan = U_vector_log(data_y_scan);
//        data_y_samples[current_scan] = new QVector<double>(data_y_scan);
//    }

//    int index_scan_i = 0, index_scan_j = 0;
//    UC_data_container * current_scan_i, * current_scan_j;
//    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
//        current_scan_i = &(scans_list[scan_i]);
//        if (!current_scan_i->U_get_sample_of_element()) continue;
//        index_scan_j = 0;
//        int index = data_thl.indexOf(current_scan_i->U_get_thl_sample());
//        for (int scan_j = 0; scan_j < n_scans; scan_j++) {
//            current_scan_j = &(scans_list[scan_j]);
//            if (!current_scan_j->U_get_sample_of_element()) continue;
//            double data = (data_y_samples[index_scan_j]->at(index));
//            eqv_matr[index_scan_i][index_scan_j] = data;
//            index_scan_j++;
//        }
//        double data = data_y_scan[index];
//        scan_thl_data[index_scan_i] = data;
//        index_scan_i++;
//    }
//    TDecompLU decomp(eqv_matr);
//    Bool_t ok;
//    TVectorD result = decomp.Solve(scan_thl_data, ok);
//    int nr = 0;
//    while (!ok) {
//        decomp.SetMatrix(eqv_matr);
//        decomp.SetTol(0.1 * decomp.GetTol());
//        if (nr++ > 10) break;
//        result = decomp.Solve(scan_thl_data, ok);
//    }
//    US_identification_data(result[0], result[1]);
//    this->scan = scan;

//void UC_plot::U_generate_muxes(int rebin_xy, int rebin_thl) {
////    //id.U_generate_muxes(scan, ff, rebin_xy, rebin_thl);
////    //U_preoperating();
////    element_vector.clear();
////    U_preoperating(scan);

////    int n_scans = scans_list.size();
////    int number_of_samples = 0;
////    UC_data_container * current_scan_i, * current_scan_j;
////    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
////        current_scan_i = &(scans_list[scan_i]);
////        if (current_scan_i->U_get_sample_of_element()) {
////            number_of_samples++;
////            U_preoperating(current_scan_i);
////            element_vector << current_scan_i->U_get_settings().element;
////        }
////    }

////    TMatrixD eqv_matr(number_of_samples, number_of_samples);
////    TVectorD scan_thl_data(number_of_samples);

////    int index_scan_i = 0, index_scan_j = 0;

////    for (int x = x_min; x < x_max; x++) {
////        for (int y = y_min; y < y_max; y++) {
////            if (U_get_mask(x, y)) continue;
////            for (int scan_i = 0; scan_i < n_scans; scan_i++) {
////                current_scan_i = &(scans_list[scan_i]);
////                if (!current_scan_i->U_get_sample_of_element()) continue;
////                index_scan_j = 0;
////                int thl = current_scan_i->U_get_thl_sample();
////                for (int scan_j = 0; scan_j < n_scans; scan_j++) {
////                    current_scan_j = &(scans_list[scan_j]);
////                    if (!current_scan_j->U_get_sample_of_element()) continue;
////                    double data = current_scan_j->U_get_data_additional(thl, x, y);
////                    eqv_matr[index_scan_i][index_scan_j] = data;
////                    index_scan_j++;
////                }
////                double data = scan->U_get_data_additional(thl, x, y);
////                scan_thl_data[index_scan_i] = data;
////                index_scan_i++;
////            }

////            TDecompLU decomp(eqv_matr);
////            Bool_t ok;
////            TVectorD result = decomp.Solve(scan_thl_data, ok);
////            int nr = 0;
////            while (!ok) {
////                decomp.SetMatrix(eqv_matr);
////                decomp.SetTol(0.1 * decomp.GetTol());
////                if (nr++ > 10) break;
////                result = decomp.Solve(scan_thl_data, ok);
////            }
////            for (int i = 0; i < number_of_samples; i++) {
////                scan->U_set_id_data(x, y, result[i]);
////            }
////        }
////    }
//}


//void UC_plot::U_generate_identification_frame(int element_index) {
////    int current_element_index = element_vector.indexOf(element_index);
////    double z;
////    for (int x = x_min; x < x_max; x++) {
////        for (int y = y_min; y < y_max; y++) {
////            if (U_get_mask(x, y)) {
////                emit US_identification_frame_data(x, y, 0);
////            } else {
////                z = scan->U_get_id_data(x, y, current_element_index);
////                emit US_identification_frame_data(x, y, z);
////            }
////        }
////    }
////    emit US_replot_identificaton_frame();
//}

////void UC_plot::U_generate_identification() {
////    if (!ff) return;
////    double data_y = 0;
////    int n = thl_finish - thl_start;
////    int total = n * 2;
////    int current = 1;
////    QVector<double> y_vector, y_vector_ff, x_vector;
////    for (int thl = thl_start; thl <= thl_finish; thl++) {
////        data_y = U_get_frame_data(frame_type, UTE_PT_cnt1, thl);
////        y_vector << data_y;
////        x_vector << thl;
////        //data_y = U_get_frame_data(frame_type, UTE_PT_cnt1ff, thl);
////        y_vector_ff << data_y;
////        data_y = U_get_frame_data(frame_type, UTE_PT_mu_diff, thl);
////        //data_y = U_get_frame_data(frame_type, UTE_PT_diff_cnt1ff, thl);
////        emit US_identification_data(thl, data_y, false);
////        emit US_renew_progress_bar(current++, total);
////    }
////    y_vector = U_smooth(x_vector, y_vector);
////    y_vector = U_diff(x_vector, y_vector);
////    y_vector = U_vector_minus(y_vector);
////    y_vector = U_smooth(x_vector, y_vector);
////    y_vector_ff = U_smooth(x_vector, y_vector_ff);
////    y_vector_ff = U_diff(x_vector, y_vector_ff);
////    y_vector_ff = U_vector_minus(y_vector_ff);
////    y_vector_ff = U_smooth(x_vector, y_vector_ff);
////    for (int i = 0; i < x_vector.size() - 1; i++) {
////        if (qAbs(y_vector[i]) > 1e-10) y_vector[i] = y_vector_ff[i] / y_vector[i];
////            else y_vector[i] = 0;
////    }
////    for (int i = 0; i < x_vector.size() - 1; i++) {
////        if (y_vector[i] > 1e-10) y_vector[i] = qLn(y_vector[i]);
////            else y_vector[i] = 0;
////    }
////    for (int i = 0; i < x_vector.size() - 1; i++) {
////        emit US_identification_data(x_vector[i], y_vector[i], true);
//////        emit US_identification_data(x_vector[i], y_vector_ff[i], true);
////        emit US_renew_progress_bar(current++, total);
////    }
////    emit US_replot_identificaton();
////    //delete h;
////}

////void UC_plot::U_generate_identification_frame(int thl) {
////    if (scan->U_get_settings().ff_int == -1) {
////        ff_enable = false;
////    } else {
////        ff_enable = true;
////        ff = &scans_list[scan->U_get_settings().ff_int];
////    }
////    if (scan->U_get_settings().df_int == -1) {
////        df_enable = false;
////    } else {
////        df_enable = true;
////        df = &scans_list[scan->U_get_settings().df_int];
////    }
////    double z;
////    for (int x = 0; x < (15 * 256); x++) {
////        for (int y = 0; y < 256; y++) {
////            if (U_get_mask(x, y)) {
////                emit US_identification_frame_data(x, y, 0);
////            } else {
////                //z = scan->U_get_muxes(thl, x, y);
////                emit US_identification_frame_data(x, y, z);
////            }
////        }
////    }
////    emit US_replot_identificaton_frame();
////}
