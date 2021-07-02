#include "uc_plot.h"


///////////////////////////////////////////////////////////////////////////////////////////////


UC_plot::UC_plot(QObject *parent) : QObject(parent) {
    /// Инициализация всех переменных класса
    /// RoI
    x_min = 0;
    x_max = 15 * 256;
    y_min = 0;
    y_max = 256;
    thl_min = 0;
    thl_max = 511;

    threshold_level = 10;   /// Стандартное значение Pixet'а при эквилизации
    rebin_x = 1;
    rebin_y = 1;
    rebin_thl = 1;

    thl_id_1 = 0;
    thl_id_2 = 511;
    thl_id_3 = 0;
    thl_id_4 = 511;

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
    id_type = UTE_IT_GA_method;
    calibration_type = UTE_CT_with_fit;

    UC_pixels_info pixel_area("Widepix", 0, 0, 3840, 256);
    pixels_areas << pixel_area;
    for (int i = 1; i < 16; i++) {
        QString name;
        name = QString("Chip %1").arg(i);
        UC_pixels_info pixel_area(name, 256 * (i - 1), 0, 256 * i, 256);
        pixels_areas << pixel_area;
    }

    U_reset_data();
}

UC_plot::~UC_plot() {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief UC_plot::U_get_energy_from_thl
 * Возвращает значение энергии по координате и порогу
 * \param x
 * \param thl
 * \return
 */
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

/*!
 * \brief UC_plot::U_get_energy_from_thl_chip
 * Возвращает значение энегрии по номеру чипа и порогу
 * \param chip
 * \param thl
 * \return
 */
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

/*!
 * \brief UC_plot::U_get_thl_from_energy
 * Возвращает порог по энергии и координате
 * \param x
 * \param energy
 * \return
 */
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

/*!
 * \brief UC_plot::U_get_thl_from_energy_chip
 * Возвращает порог по энергии и номеру чипа
 * \param chip
 * \param energy
 * \return
 */
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
/*!
 * \brief UC_plot::U_set_mask
 * Установка маскирования на указанный пиксель
 * \param x
 * \param y
 * \param value
 */
void UC_plot::U_set_mask(int x, int y, bool value) {
    mask[y * 256 * 15 + x] = value;
}

/*!
 * \brief UC_plot::U_get_mask
 * Получить маскировку указанного пикселя
 * \param x
 * \param y
 * \return
 */
bool UC_plot::U_get_mask(int x, int y) {
    return mask[y * 256 * 15 + x];
}

//

void UC_plot::U_set_total_progress_bar(int total) {
    total_progress_bar = total;
    current_progress_bar = 0;
}

void UC_plot::U_renew_progress_bar() {
    current_progress_bar++;
    emit US_renew_progress_bar(current_progress_bar, total_progress_bar);
}

//

void UC_plot::U_add_pixel_table(int thl, int x, int y) {
    double z;
    int cnt0;
    int cnt1;
    int overflow;
    int n;
    bool mask;
    bool overflowed;

    overflow = scan->U_get_count() * 4095;
    cnt0 = scan->U_get_data(thl, x, y, 0);
    cnt1 = scan->U_get_data(thl, x, y, 1);
    overflowed = (cnt0 == overflow) | (cnt1 == overflow);
    mask = U_get_mask(x, y);
    z = U_get_pixel_data(pixel_type, thl, x, y);
    n = pixels_areas.size();

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_add_pixel(z, x, y, mask, overflowed);
    }
}

//

void UC_plot::U_get_calibration_chip_spectra(QVector<double> * x, QVector<double> * y, int chip) {
    int scan_thl_min = scan->U_get_thl_min();
    int scan_thl_max = scan->U_get_thl_max();
    int x_min_reserve = this->x_min;
    int x_max_reserve = this->x_max;
    int x_min = chip * 256;
    int x_max = x_min + 256;
    this->x_max = x_max;
    this->x_min = x_min;
    for (int thl = scan_thl_min; thl <= scan_thl_max; thl++) {
        *x << thl;
        *y << U_get_frame_data(frame_type, pixel_type, thl);
        U_renew_progress_bar();
    }
    this->x_max = x_max_reserve;
    this->x_min = x_min_reserve;
}

TF1 UC_plot::U_get_calibration_fit(QVector<double> * x, QVector<double> * y) {
    *y = U_vector_log(*y);
    TGraph gr((*x).size(), (*x).data(), (*y).data());
    TF1 fit("fit", "[0] + [1] * x");
    double thl_min = (*x)[0];
    double thl_max = (*x)[(*x).size() - 1];
    gr.Fit("fit", "M", "", thl_min, thl_max);
    return fit;
}

void UC_plot::U_get_calibration_chip_fit(QVector<double> * x, QVector<double> * y) {
    TF1 fit = U_get_calibration_fit(x, y);

    QVector<double> vector_fit;
    int n = x->size();
    for (int i = 0; i < n; i++) {
        vector_fit << qExp(fit.Eval((*x)[i]));
        U_renew_progress_bar();
    }

    emit US_replot_calibration_chip(*x, vector_fit, true);
}

double UC_plot::U_get_calibration_data_1(QVector<double> * x, QVector<double> * y) {
    TF1 fit = U_get_calibration_fit(x, y);
    double a = fit.GetParameter(1);
    double b = fit.GetParameter(0);
    return (qLn(threshold_level) - b) / a;
}

double UC_plot::U_get_calibration_data_2(QVector<double> * x, QVector<double> * y) {
    int n = x->size();
    bool more = (*y)[0] > threshold_level;
    bool more_current;
    int i = 0;
    while (i < n) {
        more_current = (*y)[i] > threshold_level;
        if (more != more_current) break;
        i++;
    }
    if (i == n) {
        return (*x)[n - 1];
    }
    double denom = ((*y)[i - 1] - (*y)[i]);
    return (((*x)[i - 1] - (*x)[i])  * (threshold_level - (*y)[i]) / denom) + (*x)[i];
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
        case UTE_PT_smoothing_mu_diff : {
            return U_get_pixel_data_27(thl, x, y);
        }
        case UTE_PT_GA_request : {
            return U_get_pixel_data_28(thl, x, y);
        }
    }
    return 0;
}

/*!
 * \brief UC_plot::U_get_pixel_data_1
 * Нулевй счетчик
 * \param thl
 * \param x
 * \param y
 * \return
 */
int UC_plot::U_get_pixel_data_1(int thl, int x, int y) { //UTE_PT_cnt0
    return scan->U_get_data(thl, x, y, 0);
}
/*!
 * \brief UC_plot::U_get_pixel_data_2
 * Первый счетчик
 * \param thl
 * \param x
 * \param y
 * \return
 */
int UC_plot::U_get_pixel_data_2(int thl, int x, int y) { //UTE_PT_cnt1
    return scan->U_get_data(thl, x, y, 1);
}

double UC_plot::U_get_pixel_data_3(int thl, int x, int y) { //UTE_PT_cnt1_divide_on_cnt0
    int denom = scan->U_get_data(thl, x, y, 1);
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
    if (mu < 1e-10) return 0;
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
    if (mu < 1e-10) return 0;
    double mu_1 = ff->U_get_corrected_cnt1_scaled(thl + 1, x, y) - cnt1df_1;
    mu_1 /= denom_1;
    if (mu_1 < 1e-10) return 0;
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
    QVector<double> data;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (!U_get_mask(x, y)) {
                data << U_get_pixel_data(type_pixel, thl, x, y);
            }
        }
    }
    switch (type_spectra) {
        case UTE_FT_average : {
            return U_get_frame_data_1(data);
        }
        case UTE_FT_sum : {
            return U_get_frame_data_2(data);
        }
        case UTE_FT_median : {
            return U_get_frame_data_3(data);
        }
        case UTE_FT_max : {
            return U_get_frame_data_4(data);
        }
        case UTE_FT_min : {
            return U_get_frame_data_5(data);
        }
        case UTE_FT_zeros : {
            return U_get_frame_data_6(data);
        }
        case UTE_FT_overflowed : {
            return U_get_frame_data_7(data);
        }
        case UTE_FT_standart_deviation : {
            return U_get_frame_data_8(data);
        }
        case UTE_FT_signal_to_noise_resolution : {
            return U_get_frame_data_9(data);
        }
    }
    return 0;
}

double UC_plot::U_get_frame_data_energy(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, double e_min, double e_max) {
    QVector<double> data;
    for (int x = x_min; x < x_max; x++) {
        thl_min = U_get_thl_from_energy(x, e_min);
        thl_max = U_get_thl_from_energy(x, e_max);
        for (int thl = thl_min; thl <= thl_max; thl++) {
            for (int y = y_min; y < y_max; y++) {
                if (!U_get_mask(x, y)) {
                    data << U_get_pixel_data(type_pixel, thl, x, y);
                }
            }
        }
    }
    switch (type_spectra) {
        case UTE_FT_average : {
            return U_get_frame_data_1(data);
        }
        case UTE_FT_sum : {
            return U_get_frame_data_2(data);
        }
        case UTE_FT_median : {
            return U_get_frame_data_3(data);
        }
        case UTE_FT_max : {
            return U_get_frame_data_4(data);
        }
        case UTE_FT_min : {
            return U_get_frame_data_5(data);
        }
        case UTE_FT_zeros : {
            return U_get_frame_data_6(data);
        }
        case UTE_FT_overflowed : {
            return U_get_frame_data_7(data);
        }
        case UTE_FT_standart_deviation : {
            return U_get_frame_data_8(data);
        }
        case UTE_FT_signal_to_noise_resolution : {
            return U_get_frame_data_9(data);
        }
    }
    return 0;
}

double UC_plot::U_get_frame_data_1(QVector<double> data) { // UTE_FT_average
    double sum = 0;
    int cnt = data.size();
    for (int i = 0; i < cnt; i++) {
        sum += data[i];
    }
    sum /= cnt;
    return sum;
}

double UC_plot::U_get_frame_data_2(QVector<double> data) { // UTE_FT_sum
    double sum = 0;
    int cnt = data.size();
    for (int i = 0; i < cnt; i++) {
        sum += data[i];
    }
    return sum;
}

double UC_plot::U_get_frame_data_3(QVector<double> data) { //UTE_FT_median
    std::vector<double> z = data.toStdVector();
    int cnt = data.size();
    std::sort(z.begin(), z.end());
    if ((cnt % 2) == 1) {
        cnt /= 2;
        return z[static_cast<unsigned long>(cnt)];
    } else {
        cnt /= 2;
        double z1 = z[static_cast<unsigned long>(cnt - 1)];
        double z2 = z[static_cast<unsigned long>(cnt)];
        return (z1 + z2) / 2;
    }
}

double UC_plot::U_get_frame_data_4(QVector<double> data) { //UTE_FT_max
    double max = -1e300;
    double z = 0;
    int cnt = data.size();
    for (int i = 0; i < cnt; i++) {
        z = data[i];
        if (z > max) max = z;
    }
    return max;
}

double UC_plot::U_get_frame_data_5(QVector<double> data) { //UTE_FT_min
    double min = 1e300;
    double z = 0;
    int cnt = data.size();
    for (int i = 0; i < cnt; i++) {
        z = data[i];
        if (z < min) min = z;
    }
    return min;
}

int UC_plot::U_get_frame_data_6(QVector<double> data) { //UTE_FT_zeros
    int n = 0;
    int cnt = data.size();
    for (int i = 0; i < cnt; i++) {
        if (qAbs(data[i]) < 1e-10) n++;
    }
    return n;
}

int UC_plot::U_get_frame_data_7(QVector<double> data) { //UTE_FT_overflowed
    int n = 0;
    int overflow = scan->U_get_count() * 4095;
    int cnt = data.size();
    for (int i = 0; i < cnt; i++) {
        if (qAbs(data[i] - overflow) < 1e-10) n++;
    }
    return n;
}

double UC_plot::U_get_frame_data_8(QVector<double> data) { //UTE_FT_standart_deviation
    int cnt = data.size();
    if (cnt == 0) return 0;
    if (cnt == 1) return 0;
    double z = 0;
    double std_dev = 0;
    double mean = U_get_frame_data_1(data);
    for (int i = 0; i < cnt; i++) {
        z = data[i];
        std_dev += (mean - z) * (mean - z);
    }
    return qSqrt(std_dev / (cnt - 1));
}

double UC_plot::U_get_frame_data_9(QVector<double> data) { //UTE_FT_signal_to_noise_resolution
    double mean = U_get_frame_data_1(data);
    if (mean < 0) return 0;
    double std_dev = U_get_frame_data_8(data);
    if (qAbs(std_dev) < 1e-10) return 0;
    return qSqrt(mean) / std_dev;
}

//
///////////////////////////////////////////////////////////////////////////////////////

//
void UC_plot::U_id_roi_1() {    //UTE_IT_GA_method
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

    QVector<double> data_y_scan(thl_max - thl_min + 1);
    QVector<double> data_y_ff(thl_max - thl_min + 1);
    QVector<double> data_thl(thl_max - thl_min + 1);
    QVector<double> result;

    this->scan = ff;
    for (int thl = thl_min; thl <= thl_max; thl++) {
        data_y_ff[thl - thl_min] = U_get_frame_data(frame_type, pixel_type, thl);
        data_thl[thl - thl_min] = thl;
    }
    data_y_ff = U_smooth(data_thl, data_y_ff, smoothing);
    data_y_ff = U_diff(data_thl, data_y_ff);
    data_y_ff = U_vector_minus(data_y_ff);
    data_y_ff = U_smooth(data_thl, data_y_ff, smoothing);
    this->scan = scan;
    for (int thl = thl_min; thl <= thl_max; thl++) {
        data_y_scan[thl - thl_min] = U_get_frame_data(frame_type, pixel_type, thl);
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

void UC_plot::U_id_roi_2() {    //UTE_IT_linear_combination
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
//
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

    int n = static_cast<int>(ceil(static_cast<double>(thl_max - thl_min + 1) / rebin_thl));

    double value;
    this->scan = ff;

    for (int index = 0; index < n; index++) {
        int thl_start = index * rebin_thl + this->thl_min;
        int thl_finish = thl_start + rebin_thl - 1;
        if (thl_finish > this->thl_max) {
            thl_finish = this->thl_max;
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
        int thl_start = index * rebin_thl + this->thl_min;
        int thl_finish = thl_start + rebin_thl - 1;
        if (thl_finish > this->thl_max) {
            thl_finish = this->thl_max;
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
                int thl_start = index * rebin_thl + this->thl_min;
                int thl_finish = thl_start + rebin_thl - 1;
                if (thl_finish > this->thl_max) {
                    thl_finish = this->thl_max;
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
    QDir dir(path);
    QStringList file_names = dir.entryList(QDir::Files, QDir::Name);

    QString str;
    QString name;

    int n = file_names.size();
    if (n == 0) {
        QMessageBox msgBox;
        msgBox.setText("Files of scan have not been found.");
        msgBox.exec();
        //return;
    }

    int i = 0;
    while (i < n) {
        name = file_names[i];
        str = name.right(4);
        if (str == ".pmf") {
            str = name.right(8);
            if (str == "THL0.pmf") {
                file_names.removeAt(i);
                n--;
                continue;
            }
            if (str == "THL1.pmf") {
                file_names.removeAt(i);
                n--;
                continue;
            }
            emit US_file_found(str);
            i++;
        } else {
            file_names.removeAt(i);
            n--;
        }
    }

    if (n != 0) {
        settings.thl_min = file_names[0].left(3).toInt();
        settings.thl_max = file_names[file_names.size() - 1].left(3).toInt();
    }

    settings.n_thl = file_names.size();
    U_set_total_progress_bar(settings.n_thl);
    UC_data_container * data_container;
    data_container = new UC_data_container(settings);
    emit US_n_files(settings.n_thl);

    int adr_cnt0 = 1;
    int adr_cnt1 = 0;
    if (!settings.both_counters) {
        adr_cnt0 = 0;
        adr_cnt1 = 1;
    }

    QFile file;
    QTextStream txt_str;
    int cnt0 = 0;
    int cnt1 = 0;
    int thl = 0;
    qulonglong pixel_data = 0;
    UC_data_container::UTStr_frame frame_0;
    UC_data_container::UTStr_frame frame_1;
    for (int i = 0; i < file_names.size(); i++) {

        str = file_names[i];
        file.setFileName(path + "/" + str);
        file.open(QFile::ReadOnly);
        txt_str.setDevice(&file);
        thl = str.left(3).toInt();

        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < (15 * 256); x++) {
                txt_str >> pixel_data;
                cnt1 = static_cast<int>(pixel_data & 0xffffffff);
                cnt0 = static_cast<int>((pixel_data >> 32) & 0xffffffff);
                frame_0.pixel[x][y] = cnt0;
                frame_1.pixel[x][y] = cnt1;
            }
        }
        data_container->U_set_data(thl, frame_0, adr_cnt0);
        data_container->U_set_data(thl, frame_1, adr_cnt1);
        emit US_new_thl(thl);

        file.close();
        U_renew_progress_bar();
    }

    if (settings.both_counters) {
        data_container->U_calculate_mean();
    }

    scans_list << *data_container;
    if (scan_enable == false) {
        scan = &(scans_list[scans_list.length() - 1]);
        scan_enable = true;
    }

    U_get_max_thl_range();
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
    U_get_max_thl_range();
}

void UC_plot::U_set_scan(int index) {
    if (index == -1) {
        scan_enable = false;
    } else {
        if (index >= scans_list.size()) return;
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
    QVector<double> x;
    QVector<double> y;
    U_set_total_progress_bar(thl_max - thl_min);
    for (int thl = thl_min; thl <= thl_max; thl++) {
        y << U_get_frame_data(frame_type, pixel_type, thl);
        x << thl;
        U_renew_progress_bar();
    }
    emit US_replot_spectra(x, y);
}

void UC_plot::U_generate_spectra(int n) {
    QVector<double> x;
    QVector<double> y;
    double e_delta = (energy_max - energy_min) / n;
    double e_max = 0;
    U_set_total_progress_bar(n);
    for (double e_min = energy_min; e_min < energy_max; e_min += e_delta) {
        e_max = e_min + e_delta;
        y << U_get_frame_data_energy(frame_type, pixel_type, e_min, e_max);
        x << e_min + e_delta / 2;
        U_renew_progress_bar();
    }
    emit US_replot_spectra(x, y);
}

void UC_plot::U_generate_frame(int thl) {
    double z;
    for (int x = 0; x < (15 * 256); x++) {
        for (int y = 0; y < 256; y++) {
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
    for (int x = 0; x < (15 * 256); x++) {
        int thl = U_get_thl_from_energy(x, energy);
        for (int y = 0; y < 256; y++) {
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
    const int n = pixels_areas.size();
    U_set_total_progress_bar(256 * 256 * 15);

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_reset();
    }

    for (int x = 0; x < 3840; x++) {
        for (int y = 0; y < 256; y++) {
            U_add_pixel_table(thl, x, y);
            U_renew_progress_bar();
        }
    }

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_finalize();
        emit US_chip_data(pixels_areas[i]);
    }
    emit US_rewrite_table();
}

void UC_plot::U_generate_table(double energy) {
    const int n = pixels_areas.size();
    U_set_total_progress_bar(256 * 256 * 15);

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_reset();
    }

    for (int x = 0; x < 3840; x++) {
        int thl = U_get_thl_from_energy(x, energy);
        for (int y = 0; y < 256; y++) {
            U_add_pixel_table(thl, x, y);
            U_renew_progress_bar();
        }
    }

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_finalize();
        emit US_chip_data(pixels_areas[i]);
    }
    emit US_rewrite_table();
}

void UC_plot::U_generate_table() {
    const int n = pixels_areas.size();
    U_set_total_progress_bar(256 * 256 * 15);

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_reset();
    }

    for (int thl = thl_min; thl <= thl_max; thl++) {
        for (int x = 0; x < 3840; x++) {
            for (int y = 0; y < 256; y++) {
                U_add_pixel_table(thl, x, y);
                U_renew_progress_bar();
            }
        }
    }

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_finalize();
        emit US_chip_data(pixels_areas[i]);
    }
    emit US_rewrite_table();
}

void UC_plot::U_generate_distribution(int n_bins, double min, double max) {
    double z;
    TH1D * hist = new TH1D("hist", "hist", n_bins, min, max);
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_max - thl_min));

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            U_renew_progress_bar();
            if (U_get_mask(x, y)) continue;
            for (int thl = thl_min; thl <= thl_max; thl++) {
                z = U_get_pixel_data(pixel_type, thl, x, y);
                hist->Fill(z);
            }
        }
    }

    QVector<double> x;
    QVector<double> y;
    for (int i = 0; i < n_bins; i++){
        x << hist->GetBinCenter(i);
        y << hist->GetBinContent(i);
    }

    emit US_replot_distribution(x, y);
    delete hist;
}

void UC_plot::U_generate_frame_distribution(int n_bins, double min, double max, int thl) {
    double z;
    TH1D * hist = new TH1D("hist", "hist", n_bins, min, max);
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min));

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            U_renew_progress_bar();
            if (U_get_mask(x, y)) continue;
            z = U_get_pixel_data(pixel_type, thl, x, y);
            hist->Fill(z);
        }
    }

    QVector<double> x;
    QVector<double> y;
    for (int i = 0; i < n_bins; i++){
        x << hist->GetBinCenter(i);
        y << hist->GetBinContent(i);
    }

    emit US_replot_distribution(x, y);
    delete hist;
}

void UC_plot::U_generate_calibration(int chip) {
    QVector<double> x;
    QVector<double> y;

    int scan_thl_min = scan->U_get_thl_min();
    int scan_thl_max = scan->U_get_thl_max();
    U_set_total_progress_bar((scan_thl_max - scan_thl_min) * 2);

    U_get_calibration_chip_spectra(&x, &y, chip);
    US_replot_calibration_chip(x, y, false);

    bool fit_enable = (pixel_type == UTE_PT_cnt1)                   ||
                      (pixel_type == UTE_PT_cnt1_divide_on_cnt0)    ||
                      (pixel_type == UTE_PT_diff_cnt1)              ||
                      (pixel_type == UTE_PT_cnt1_corr_cnt0)         ||
                      (pixel_type == UTE_PT_diff_cnt1_corr_cnt0)    ||
                      (pixel_type == UTE_PT_ffc_non_xray);

    if (!fit_enable) return;
    U_get_calibration_chip_fit(&x, &y);
}

void UC_plot::U_generate_calibration() {
    UC_data_container * scan_reserve = scan;
    QVector<double> fit_x[15];
    QVector<double> fit_y;
    int n_scans = scans_list.size();
    QList<UC_data_container *> calibration_list;
    int total = 0;
    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
        scan = &(scans_list[scan_i]);
        if (scan->U_get_calibration()) {
            calibration_list << scan;
            total += (scan->U_get_thl_max() - scan->U_get_thl_min()) * 15;
        }
    }
    n_scans = calibration_list.size();
    total += 15 * n_scans + 15;
    U_set_total_progress_bar(total);
    double thl_min = 512;
    double thl_max = 0;
    double data;
    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
        scan = (calibration_list[scan_i]);
        fit_y << scan->U_get_energy();
        for (int chip = 0; chip < 15; chip++) {
            QVector<double> x;
            QVector<double> y;
            U_get_calibration_chip_spectra(&x, &y, chip);
            switch (calibration_type) {
                case UTE_CT_with_fit : {
                    data = U_get_calibration_data_1(&x, &y);
                    break;
                }
                case UTE_CT_without_fit : {
                    data = U_get_calibration_data_2(&x, &y);
                    break;
                }
            }
            fit_x[chip] << data;
            if (data < thl_min) thl_min = data;
            if (data > thl_max) thl_max = data;
            U_renew_progress_bar();
        }
    }

    const int n = 10;
    QVector<double> fit_n_x;
    QVector<double> fit_n_y;
    double data_x = 0;
    const double delta_x = (thl_max - thl_min) / (n - 1);
    for (int chip = 0; chip < 15; chip++) {
        emit US_replot_calibration(fit_x[chip], fit_y, chip, false);
        TGraph gr(n_scans, fit_x[chip].data(), fit_y.data());
        TF1 fit("fit", "[0] + [1] * x + [2] * x * x");
        gr.Fit("fit", "M", "", fit_y[0], fit_y[n_scans - 1]);
        calibration_p2[chip] = fit.GetParameter(2);
        calibration_p1[chip] = fit.GetParameter(1);
        calibration_p0[chip] = fit.GetParameter(0);
        data_x = thl_min;
        fit_n_x.clear();
        fit_n_y.clear();
        for (int i = 0; i < n; i++) {
            fit_n_x << data_x;
            fit_n_y << fit.Eval(data_x);
            data_x += delta_x;
        }
        emit US_replot_calibration(fit_n_x, fit_n_y, chip, true);
        U_renew_progress_bar();
    }

    scan = scan_reserve;
    using_calibration = true;
}


void UC_plot::U_generate_id_roi() {
    switch (id_type) {
        case UTE_IT_GA_method : {
            U_id_roi_1();
            return;
        }
        case UTE_IT_linear_combination : {
            U_id_roi_2();
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

void UC_plot::U_generate_id_data() {
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
    emit US_id_scan_list(elements);
}

void UC_plot::U_generate_id_frame(int element_index) {
    double z;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) {
                emit US_id_frame_data(x, y, 0);
            } else {
                z = scan->U_get_id_data(x, y, element_index);
                emit US_id_frame_data(x, y, z);
            }
        }
    }
    emit US_replot_id_frame();
}


void UC_plot::U_generate_range(int thl) {
    double z;
    double max = -1e300;
    double min = 1e300;
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
    double z;
    double max = -1e300;
    double min = 1e300;
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_max - thl_min));
    for (int thl = thl_min; thl <= thl_max; thl++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl, x, y);
                if (z > max) max = z;
                if (z < min) min = z;
                U_renew_progress_bar();
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

void UC_plot::U_set_id_type(UC_plot::UTE_id_type id_type) {
    this->id_type = id_type;
}

void UC_plot::U_set_calibration_type(UC_plot::UTE_calibration_type calibration_type) {
    this->calibration_type = calibration_type;
}
//
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

void UC_plot::U_set_threshold_range(int thl_min, int thl_max) {
    this->thl_min = thl_min;
    this->thl_max = thl_max;
}

void UC_plot::U_set_id_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4) {
    this->thl_id_1 = thl_id_1;
    this->thl_id_2 = thl_id_2;
    this->thl_id_3 = thl_id_3;
    this->thl_id_4 = thl_id_4;
}
//
void UC_plot::U_set_using_calibraion(bool enable) {
    using_calibration = enable;
}

void UC_plot::U_set_threshold_level(double level) {
    this->threshold_level = level;
}

void UC_plot::U_set_smoothing(int smoothing) {
    this->smoothing = smoothing;
}
//
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
//
void UC_plot::U_get_max_thl_range() {
    int max;
    int min;
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

void UC_plot::U_get_common_thl_range() {
    int max;
    int min;
    thl_max = 512;
    thl_min = 0;
    for (int i = 0; i < scans_list.size(); i++) {
        max = scans_list[i].U_get_thl_max();
        min = scans_list[i].U_get_thl_min();
        if (max > thl_max) thl_max = max;
        if (min < thl_min) thl_min = min;
    }
    emit US_thl_range(thl_min, thl_max);
}

void UC_plot::U_get_max_energy_range() {
    if (!using_calibration) return;
    int thl_min = 512;
    int thl_max = 0;
    double max;
    double min;
    for (int i = 0; i < scans_list.size(); i++) {
        thl_max = scans_list[i].U_get_thl_max();
        thl_min = scans_list[i].U_get_thl_min();
        for (int chip = 0; chip < 15; chip++) {
            max = U_get_energy_from_thl_chip(chip, thl_max);
            min = U_get_energy_from_thl_chip(chip, thl_min);
            if (max < energy_max) energy_max = max;
            if (min > energy_min) energy_min = min;
        }
    }
    emit US_energy_range(energy_min, energy_max);
}

void UC_plot::U_get_common_energy_range() {
    if (!using_calibration) return;
    int thl_min = 0;
    int thl_max = 512;
    double max;
    double min;
    for (int i = 0; i < scans_list.size(); i++) {
        thl_max = scans_list[i].U_get_thl_max();
        thl_min = scans_list[i].U_get_thl_min();
        for (int chip = 0; chip < 15; chip++) {
            max = U_get_energy_from_thl_chip(chip, thl_max);
            min = U_get_energy_from_thl_chip(chip, thl_min);
            if (max > energy_max) energy_max = max;
            if (min < energy_min) energy_min = min;
        }
    }
    emit US_energy_range(energy_min, energy_max);
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

    int cnt0 = 0;
    int cnt1 = 0;
    bool overflowed;
    int overflow = scan->U_get_count() * 4095;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            cnt0 = scan->U_get_data(thl, x, y, 0);
            cnt1 = scan->U_get_data(thl, x, y, 1);
            overflowed = (cnt0 == overflow) | (cnt1 == overflow);
            if (overflowed) U_set_mask(x, y, true);
        }
    }

    if (ff_enable) {
        int overflow = ff->U_get_count() * 4095;
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                cnt0 = ff->U_get_data(thl, x, y, 0);
                cnt1 = ff->U_get_data(thl, x, y, 1);
                overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                if (overflowed) U_set_mask(x, y, true);
            }
        }
    }

    if (df_enable) {
        int overflow = df->U_get_count() * 4095;
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                cnt0 = df->U_get_data(thl, x, y, 0);
                cnt1 = df->U_get_data(thl, x, y, 1);
                overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                if (overflowed) U_set_mask(x, y, true);
            }
        }
    }
}

void UC_plot::U_set_mask_overflowed(bool in_roi) {
    for (int thl = thl_min; thl <= thl_max; thl++) {
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

    for (int thl = thl_min; thl < thl_max; thl++) {
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

void UC_plot::U_save_mask(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int y = 0; y < 255; y++) {
        for (int x = 0; x < 3839; x++) {
            if (U_get_mask(x, y)) {
                str << "1 ";
            } else {
                str << "0 ";
            }
        }
        if (U_get_mask(3839, y)) {
            str << "1";
        } else {
            str << "0";
        }
        str << endl;
    }
    for (int x = 0; x < 3839; x++) {
        if (U_get_mask(x, 255)) {
            str << "1 ";
        } else {
            str << "0 ";
        }
    }
    if (U_get_mask(3839, 255)) {
        str << "1";
    } else {
        str << "0";
    }
    file.close();
}

void UC_plot::U_load_mask(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    int z;
    for (int y = 0; y < 256; y ++) {
        for (int x = 0; x < (15 * 256); x++) {
            str >> z;
            U_set_mask(x, y, (z == 1));
        }
    }
    file.close();
}
//////////////////////////////////////////////////////////////////////////////////////

void UC_plot::U_add_roi(UC_roi roi) {
    UC_pixels_info pixel_area(roi);
    pixels_areas << pixel_area;
}

void UC_plot::U_delete_roi(int index) {
    pixels_areas.removeAt(index);
}


