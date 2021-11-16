#include "uc_plot.h"


///////////////////////////////////////////////////////////////////////////////////////////////


UC_plot::UC_plot(QObject *parent) : QObject(parent) {
    /// Инициализация всех переменных класса
    /// RoI
    x_min = 0;
    x_max = 15 * 256;
    y_min = 0;
    y_max = 256;
    thl_index_min = 0;
    thl_index_max = 511;

    threshold_level = 10;   /// Стандартное значение Pixet'а при эквилизации

    stop = false;
    using_calibration = false;
    ff_minus_df_ready = false;
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
//    for (int i = 0; i < scans_list.size(); i++) {
//        scans_list[i].~UC_data_container();
//    }
//    scans_list.~QList();
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
        return p0 + p1 * thl + p2 * thl * thl;
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
        return p0 + p1 * thl + p2 * thl * thl;
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
void UC_plot::U_make_ff_minus_df_mean(int thl_index) {
    double ff_value = 0;
    double df_value = 0;
    int count = 0;
    mean_ff_minus_df = 0;
    for (int x = 0; x < 15 * 256; x++) {
        for (int y = 0; y < 256; y++) {
            if (U_get_mask(x, y)) continue;
            if (ff_enable) {
                ff_value = ff->U_get_data_scaled(thl_index, x, y, 1);
            }
            if (df_enable) {
                df_value = df->U_get_data_scaled(thl_index, x, y, 1);
            }

            mean_ff_minus_df += ff_value - df_value;
            count++;
        }
    }
    mean_ff_minus_df /= count;
    ff_minus_df_ready = true;
}

double UC_plot::U_get_ff_minus_df_mean(int thl_index) {
    if (!ff_minus_df_ready) U_make_ff_minus_df_mean(thl_index);
    return mean_ff_minus_df;
}

void UC_plot::U_reset_ff_minus_df_mean() {
    ff_minus_df_ready = false;
}
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

void UC_plot::U_add_pixel_table(int thl_index, int x, int y) {
    double z;
    int cnt0;
    int cnt1;
    int overflow;
    int n;
    bool mask;
    bool overflowed;

    overflow = scan->U_get_count() * 4095;
    cnt0 = scan->U_get_data(thl_index, x, y, 0);
    cnt1 = scan->U_get_data(thl_index, x, y, 1);
    overflowed = (cnt0 == overflow) | (cnt1 == overflow);
    mask = U_get_mask(x, y);
    z = U_get_pixel_data(pixel_type, thl_index, x, y);
    n = pixels_areas.size();

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_add_pixel(z, x, y, mask, overflowed);
    }
}

//

void UC_plot::U_get_calibration_chip_spectra(QVector<double> * x, QVector<double> * y, int chip) {
    int x_min_reserve = this->x_min;
    int x_max_reserve = this->x_max;
    int y_min_reserve = this->y_min;
    int y_max_reserve = this->y_max;
    int x_min = chip * 256;
    int x_max = x_min + 256;
    this->x_max = x_max;
    this->x_min = x_min;
    this->y_max = 256;
    this->y_min = 0;
    int n = scan->U_get_n();
    for (int thl_index = 0; thl_index <= n - 1; thl_index++) {
        *x << (*(scan->U_get_thl_vector()))[thl_index];
        *y << U_get_frame_data(frame_type, pixel_type, thl_index);
        U_renew_progress_bar();
        if (stop) break;
    }
    this->x_max = x_max_reserve;
    this->x_min = x_min_reserve;
    this->y_max = y_max_reserve;
    this->y_min = y_min_reserve;
}

TF1 UC_plot::U_get_calibration_fit(QVector<double> * x, QVector<double> * y) {
    *y = U_vector_log(*y);
    TGraph gr((*x).size(), (*x).data(), (*y).data());
    TF1 fit("fit", "[0] + [1] * x");
    double thl_min = (*x)[0];
    double thl_max = (*x)[(*x).size() - 1];
    gr.Fit("fit", "MQ", "", thl_min, thl_max);
    return fit;
}

void UC_plot::U_get_calibration_chip_fit(QVector<double> * x, QVector<double> * y) {
    TF1 fit = U_get_calibration_fit(x, y);

    QVector<double> vector_fit;
    int n = x->size();
    for (int i = 0; i < n; i++) {
        vector_fit << qExp(fit.Eval((*x)[i]));
        U_renew_progress_bar();
        if (stop) break;
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
//
void UC_plot::U_enable_ff_df() {
    if (scan_index < 0) {
        scan_enable = false;
        return;
    }
    if (scan_index >= scans_list.size()) {
        scan_enable = false;
        return;
    }
    scan = &(scans_list[scan_index]);
    int ff = scan->U_get_settings().ff_int;
    if (ff == -1) {
        ff_enable = false;
    } else {
        ff_enable = true;
        this->ff = &scans_list[ff];
    }
    int df = scan->U_get_settings().df_int;
    if (df == -1) {
        df_enable = false;
    } else {
        df_enable = true;
        this->df = &scans_list[df];
    }
}

//
double UC_plot::U_get_diff_data(UC_data_container * scan, int thl_index, int x, int y, int cnt) { //UTE_PT_diff_cnt1
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }
    double yin2 = scan->U_get_data(tim2, x, y, cnt);
    double yin1 = scan->U_get_data(tim1, x, y, cnt);
    double yi1 = scan->U_get_data(ti1, x, y, cnt);
    double yi2 = scan->U_get_data(ti2, x, y, cnt);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
}

double UC_plot::U_get_diff_data_scaled(UC_data_container * scan, int thl_index, int x, int y, int cnt) { //UTE_PT_diff_cnt1
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }

    double yin2 = scan->U_get_data_scaled(tim2, x, y, cnt);
    double yin1 = scan->U_get_data_scaled(tim1, x, y, cnt);
    double yi1 = scan->U_get_data_scaled(ti1, x, y, cnt);
    double yi2 = scan->U_get_data_scaled(ti2, x, y, cnt);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
}

double UC_plot::U_get_diff_data_corr(UC_data_container * scan, int thl_index, int x, int y) { //UTE_PT_diff_cnt1
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }

    double yin2 = scan->U_get_corrected_cnt1(tim2, x, y);
    double yin1 = scan->U_get_corrected_cnt1(tim1, x, y);
    double yi1 = scan->U_get_corrected_cnt1(ti1, x, y);
    double yi2 = scan->U_get_corrected_cnt1(ti2, x, y);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
}

double UC_plot::U_get_diff_data_corr_scaled(UC_data_container * scan, int thl_index, int x, int y) { //UTE_PT_diff_cnt1
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }

    double yin2 = scan->U_get_corrected_cnt1_scaled(tim2, x, y);
    double yin1 = scan->U_get_corrected_cnt1_scaled(tim1, x, y);
    double yi1 = scan->U_get_corrected_cnt1_scaled(ti1, x, y);
    double yi2 = scan->U_get_corrected_cnt1_scaled(ti2, x, y);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
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
        case UTE_PT_cnt1_divide_on_cnt0 : {
            return U_get_pixel_data_3(thl_index, x, y);
        }
        case UTE_PT_cnt0_subtr_cnt1 : {
            return U_get_pixel_data_4(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1 : {
            return U_get_pixel_data_5(thl_index, x, y);
        }
        case UTE_PT_cnt1_subtr_cnt1_max_thl : {
            return U_get_pixel_data_6(thl_index, x, y);
        }
        case UTE_PT_ffc : {
            return U_get_pixel_data_7(thl_index, x, y);
        }
        case UTE_PT_mu : {
            return U_get_pixel_data_8(thl_index, x, y);
        }
        case UTE_PT_diff_ffc : {
            return U_get_pixel_data_9(thl_index, x, y);
        }
        case UTE_PT_diff_mu : {
            return U_get_pixel_data_10(thl_index, x, y);
        }
        case UTE_PT_ffc_diff : {
            return U_get_pixel_data_11(thl_index, x, y);
        }
        case UTE_PT_mu_diff : {
            return U_get_pixel_data_12(thl_index, x, y);
        }
        ////
        case UTE_PT_cnt1_corr_cnt0 : {
            return U_get_pixel_data_13(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1_corr_cnt0 : {
            return U_get_pixel_data_14(thl_index, x, y);
        }
        case UTE_PT_ffc_corr_cnt0 : {
            return U_get_pixel_data_15(thl_index, x, y);
        }
        case UTE_PT_mu_corr_cnt0 : {
            return U_get_pixel_data_16(thl_index, x, y);
        }
        case UTE_PT_diff_ffc_corr_cnt0 : {
            return U_get_pixel_data_17(thl_index, x, y);
        }
        case UTE_PT_diff_mu_corr_cnt0 : {
            return U_get_pixel_data_18(thl_index, x, y);
        }
        case UTE_PT_ffc_diff_corr_cnt0 : {
            return U_get_pixel_data_19(thl_index, x, y);
        }
        case UTE_PT_mu_diff_corr_cnt0 : {
            return U_get_pixel_data_20(thl_index, x, y);
        }
        ////
        case UTE_PT_ffc_non_xray : {
            return U_get_pixel_data_21(thl_index, x, y);
        }
        case UTE_PT_cnt0_deviation : {
            return U_get_pixel_data_22(thl_index, x, y);
        }
        case UTE_PT_cnt1_act : {
            return U_get_pixel_data_23(thl_index, x, y);
        }
        case UTE_PT_cnt0_act : {
            return U_get_pixel_data_24(thl_index, x, y);
        }
        case UTE_PT_cnt1_rejected : {
            return U_get_pixel_data_25(thl_index, x, y);
        }
        case UTE_PT_diff_cnt1_rejected : {
            return U_get_pixel_data_26(thl_index, x, y);
        }
        case UTE_PT_cnt1ffc_div_cnt0ffc : {
            return U_get_pixel_data_27(thl_index, x, y);
        }
        case UTE_PT_GA_request : {
            return U_get_pixel_data_28(thl_index, x, y);
        }
        case UTE_PT_ffc_cnt0 : {
            return U_get_pixel_data_29(thl_index, x, y);
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
int UC_plot::U_get_pixel_data_1(int thl_index, int x, int y) { //UTE_PT_cnt0
    return static_cast<int>(scan->U_get_data(thl_index, x, y, 0));
}
/*!
 * \brief UC_plot::U_get_pixel_data_2
 * Первый счетчик
 * \param thl
 * \param x
 * \param y
 * \return
 */
int UC_plot::U_get_pixel_data_2(int thl_index, int x, int y) { //UTE_PT_cnt1
    return static_cast<int>(scan->U_get_data(thl_index, x, y, 1));
}

double UC_plot::U_get_pixel_data_3(int thl_index, int x, int y) { //UTE_PT_cnt1_divide_on_cnt0
    double denom = scan->U_get_data(thl_index, x, y, 0);
    if (qAbs(denom) < 1e-10) return 0;
    return scan->U_get_data(thl_index, x, y, 1) / denom;
}

int UC_plot::U_get_pixel_data_4(int thl_index, int x, int y) { //UTE_PT_cnt0_subtr_cnt1
    return static_cast<int>(scan->U_get_data(thl_index, x, y, 0) - scan->U_get_data(thl_index, x, y, 1));
}

double UC_plot::U_get_pixel_data_5(int thl_index, int x, int y) { //UTE_PT_diff_cnt1
    return -U_get_diff_data(scan, thl_index, x, y, 1);
}

int UC_plot::U_get_pixel_data_6(int thl_index, int x, int y) { //UTE_PT_cnt1_subtr_cnt1_max_thl
    int thl_data = static_cast<int>(scan->U_get_data(thl_index, x, y, 1));
    int thl_max_data = static_cast<int>(scan->U_get_data(thl_index_max + 1, x, y, 1));
    return thl_data - thl_max_data;
}

double UC_plot::U_get_pixel_data_7(int thl_index, int x, int y) { //UTE_PT_ffc
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl_index, x, y, 1);
    } else {
        cnt1df = 0;
    }
    double ffc = 0;
    double denom = ff->U_get_data_scaled(thl_index, x, y, 1) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    ffc = scan->U_get_data_scaled(thl_index, x, y, 1) - cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_8(int thl_index, int x, int y) { //UTE_PT_mu
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl_index, x, y, 1);
    } else {
        cnt1df = 0;
    }
    double mu = 0;
    double denom = scan->U_get_data_scaled(thl_index, x, y, 1) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    mu = ff->U_get_data_scaled(thl_index, x, y, 1) - cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);
}

double UC_plot::U_get_pixel_data_9(int thl_index, int x, int y) { //UTE_PT_diff_ffc
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }
    double yin2 = U_get_pixel_data_7(tim2, x, y);
    double yin1 = U_get_pixel_data_7(tim1, x, y);
    double yi1 = U_get_pixel_data_7(ti1, x, y);
    double yi2 = U_get_pixel_data_7(ti2, x, y);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
}

double UC_plot::U_get_pixel_data_10(int thl_index, int x, int y) { //UTE_PT_diff_mu
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }
    double yin2 = U_get_pixel_data_8(tim2, x, y);
    double yin1 = U_get_pixel_data_8(tim1, x, y);
    double yi1 = U_get_pixel_data_8(ti1, x, y);
    double yi2 = U_get_pixel_data_8(ti2, x, y);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
}

double UC_plot::U_get_pixel_data_11(int thl_index, int x, int y) { //UTE_PT_ffc_diff
    if (!ff_enable) return 0;
    double delta_cnt1df;
    if (df_enable) {
        delta_cnt1df = U_get_diff_data_scaled(df, thl_index, x, y, 1);
    } else {
        delta_cnt1df = 0;
    }
    double denom = U_get_diff_data_scaled(ff, thl_index, x, y, 1) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double ffc = U_get_diff_data_scaled(scan, thl_index, x, y, 1) - delta_cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_12(int thl_index, int x, int y) { //UTE_PT_mu_diff
    if (!ff_enable) return 0;
    double delta_cnt1df;
    if (df_enable) {
        delta_cnt1df = U_get_diff_data_scaled(df, thl_index, x, y, 1);
    } else {
        delta_cnt1df = 0;
    }
    double denom = U_get_diff_data_scaled(scan, thl_index, x, y, 1) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double mu = U_get_diff_data_scaled(ff, thl_index, x, y, 1) - delta_cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);
}
////

double UC_plot::U_get_pixel_data_13(int thl_index, int x, int y) { //UTE_PT_cnt1_corr_cnt0
    return scan->U_get_corrected_cnt1(thl_index, x, y);
}

double UC_plot::U_get_pixel_data_14(int thl_index, int x, int y) { //UTE_PT_diff_cnt1_corr_cnt0
    return -U_get_diff_data_corr(scan, thl_index, x, y);
}

double UC_plot::U_get_pixel_data_15(int thl_index, int x, int y) { //UTE_PT_ffc_corr_cnt0
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_corrected_cnt1_scaled(thl_index, x, y);
    } else {
        cnt1df = 0;
    }
    double denom = ff->U_get_corrected_cnt1_scaled(thl_index, x, y) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double ffc = scan->U_get_corrected_cnt1_scaled(thl_index, x, y) - cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_16(int thl_index, int x, int y) { //UTE_PT_mu_corr_cnt0
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_corrected_cnt1_scaled(thl_index, x, y);
    } else {
        cnt1df = 0;
    }
    double denom = scan->U_get_corrected_cnt1_scaled(thl_index, x, y) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double mu = ff->U_get_corrected_cnt1_scaled(thl_index, x, y) - cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);
}

double UC_plot::U_get_pixel_data_17(int thl_index, int x, int y) { //UTE_PT_diff_ffc_corr_cnt0
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }
    double yin2 = U_get_pixel_data_15(tim2, x, y);
    double yin1 = U_get_pixel_data_15(tim1, x, y);
    double yi1 = U_get_pixel_data_15(ti1, x, y);
    double yi2 = U_get_pixel_data_15(ti2, x, y);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
}

double UC_plot::U_get_pixel_data_18(int thl_index, int x, int y) { //UTE_PT_diff_mu_corr_cnt0
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }
    double yin2 = U_get_pixel_data_16(tim2, x, y);
    double yin1 = U_get_pixel_data_16(tim1, x, y);
    double yi1 = U_get_pixel_data_16(ti1, x, y);
    double yi2 = U_get_pixel_data_16(ti2, x, y);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
}

double UC_plot::U_get_pixel_data_19(int thl_index, int x, int y) { //UTE_PT_ffc_diff_corr_cnt0
    if (!ff_enable) return 0;
    double delta_cnt1df;
    if (df_enable) {
        delta_cnt1df = U_get_diff_data_corr_scaled(df, thl_index, x, y);
    } else {
        delta_cnt1df = 0;
    }
    double denom = U_get_diff_data_corr_scaled(ff, thl_index, x, y) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double ffc = U_get_diff_data_corr_scaled(scan, thl_index, x, y) - delta_cnt1df;
    ffc /= denom;
    return ffc;
}

double UC_plot::U_get_pixel_data_20(int thl_index, int x, int y) { //UTE_PT_mu_diff_corr_cnt0
    if (!ff_enable) return 0;
    double delta_cnt1df;
    if (df_enable) {
        delta_cnt1df = U_get_diff_data_corr_scaled(df, thl_index, x, y);
    } else {
        delta_cnt1df = 0;
    }
    double denom = U_get_diff_data_corr_scaled(scan, thl_index, x, y) - delta_cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    double mu = U_get_diff_data_corr_scaled(ff, thl_index, x, y) - delta_cnt1df;
    mu /= denom;
    if (mu < 1e-10) return 0;
    return qLn(mu);
}
///
double UC_plot::U_get_pixel_data_21(int thl_index, int x, int y) { //UTE_PT_ffc_non_xray
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl_index, x, y, 1);
    } else {
        cnt1df = 0;
    }
    double ffc = 0;
    double denom = ff->U_get_data_scaled(thl_index, x, y, 1) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    ffc = scan->U_get_data_scaled(thl_index, x, y, 1) - cnt1df;
    ffc /= denom;
    ffc *= U_get_ff_minus_df_mean(thl_index);
    return ffc;
}

double UC_plot::U_get_pixel_data_22(int thl_index, int x, int y) { //UTE_PT_cnt0_deviation
    double denom = scan->U_get_corrected_cnt1(thl_index, x, y);
    if (qAbs(denom) < 1e-10) return 0;
    return scan->U_get_data(thl_index, x, y, 1) / denom;
}

int UC_plot::U_get_pixel_data_23(int thl_index, int x, int y) { //UTE_PT_cnt1_act
    if (scan->U_get_data(thl_index, x, y, 1) > threshold_level) {
        return 1;
    } else {
        return 0;
    }
}

int UC_plot::U_get_pixel_data_24(int thl_index, int x, int y) { //UTE_PT_cnt0_act
    if (scan->U_get_data(thl_index, x, y, 0) > threshold_level) {
        return 1;
    } else {
        return 0;
    }
}

int UC_plot::U_get_pixel_data_25(int thl_index, int x, int y) { //UTE_PT_cnt1_rejected
    bool act0 = scan->U_get_data(thl_index, x, y, 0) > threshold_level;
    bool act1 = scan->U_get_data(thl_index, x, y, 1) > threshold_level;
    if (act0 && (!act1))  {
        return 1;
    } else {
        return 0;
    }
}

double UC_plot::U_get_pixel_data_26(int thl_index, int x, int y) { //UTE_PT_diff_cnt1_rejected
    int tim2 = thl_index - 2;
    int tim1 = thl_index - 1;
    int ti1 = thl_index + 1;
    int ti2 = thl_index + 2;
    if (thl_index == thl_index_min) {
        tim2 = thl_index_min;
        tim1 = thl_index_min;
    }
    if ((thl_index - 1) == thl_index_min) {
        tim2 = thl_index_min;
    }
    if (thl_index == thl_index_max) {
        ti1 = thl_index_max;
        ti2 = thl_index_max;
    }
    if ((thl_index + 1) == thl_index_max) {
        ti2 = thl_index_max;
    }
    double yin2 = U_get_pixel_data_25(tim2, x, y);
    double yin1 = U_get_pixel_data_25(tim1, x, y);
    double yi1 = U_get_pixel_data_25(ti1, x, y);
    double yi2 = U_get_pixel_data_25(ti2, x, y);
    return U_diff(yin2, yin1, yi1, yi2, (*thl_vector)[tim2], (*thl_vector)[tim1], (*thl_vector)[ti1], (*thl_vector)[ti2]);
}

double UC_plot::U_get_pixel_data_27(int thl_index, int x, int y) { //UTE_PT_cnt1ffc_div_cnt0ffc
    if (!ff_enable) return 0;
    double cnt0 = scan->U_get_data_scaled(thl_index, x, y, 0);
    double cnt1 = scan->U_get_data_scaled(thl_index, x, y, 1);
    double cnt0ff = ff->U_get_data_scaled(thl_index, x, y, 0);
    double cnt1ff = ff->U_get_data_scaled(thl_index, x, y, 1);
    double denom = cnt0 * cnt1ff;
    if (qAbs(denom) < 1e-10) return 0;
    double nom = cnt1 * cnt0ff;
    return nom / denom;
}

double UC_plot::U_get_pixel_data_28(int thl_index, int x, int y) { //UTE_PT_GA_request
    if (!ff_enable) return 0;
    double delta_cnt1df;
    double delta_cnt1df38;
    if (df_enable) {
        double cnt1df = df->U_get_data_scaled(thl_index, x, y, 1);
        double cnt1df_thl_max = df->U_get_data_scaled(thl_index_max + 1, x, y, 1);
        double cnt1df38 = df->U_get_data_scaled(38, x, y, 1);
        delta_cnt1df = cnt1df - cnt1df_thl_max;
        delta_cnt1df38 = cnt1df38 - cnt1df_thl_max;
    } else {
        delta_cnt1df = 0;
        delta_cnt1df38 = 0;
    }
    double cnt1_thl_max = scan->U_get_data_scaled(thl_index_max + 1, x, y, 1);
    double cnt1ff_thl_max = ff->U_get_data_scaled(thl_index_max + 1, x, y, 1);
    double cnt1 = scan->U_get_data_scaled(thl_index, x, y, 1);
    double cnt1ff = ff->U_get_data_scaled(thl_index, x, y, 1);
    double cnt138 = scan->U_get_data_scaled(38, x, y, 1);
    double cnt1ff38 = ff->U_get_data_scaled(38, x, y, 1);
    double delta_cnt1 = cnt1 - cnt1_thl_max;
    double delta_cnt138 = cnt138 - cnt1_thl_max;
    double delta_cnt1ff = cnt1ff - cnt1ff_thl_max;
    double delta_cnt1ff38 = cnt1ff38 - cnt1ff_thl_max;
    double denom = (delta_cnt1ff - delta_cnt1df);
    if (qAbs(denom) < 1e-10) return 0;
    double ffc = (delta_cnt1 - delta_cnt1df) / denom;
    if (qAbs(ffc) < 1e-10) return 0;
    denom = (delta_cnt1ff38 - delta_cnt1df38);
    if (qAbs(denom) < 1e-10) return 0;
    double ffc38 = (delta_cnt138 - delta_cnt1df38) / denom;
    double result = ffc38 / ffc;
    return result;
}

double UC_plot::U_get_pixel_data_29(int thl_index, int x, int y) { //UTE_PT_ffc_cnt0
    if (!ff_enable) return 0;
    double cnt1df;
    if (df_enable) {
        cnt1df = df->U_get_data_scaled(thl_index, x, y, 0);
    } else {
        cnt1df = 0;
    }
    double ffc = 0;
    double denom = ff->U_get_data_scaled(thl_index, x, y, 0) - cnt1df;
    if (qAbs(denom) < 1e-10) return 0;
    ffc = scan->U_get_data_scaled(thl_index, x, y, 0) - cnt1df;
    ffc /= denom;
    return ffc;
}
/////////////////////////////////////////////////////////////////////////////////
double UC_plot::U_get_frame_data(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, int thl_index) {
    U_reset_ff_minus_df_mean();
    QVector<double> data;
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (!U_get_mask(x, y)) {
                data << U_get_pixel_data(type_pixel, thl_index, x, y);
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
        case UTE_FT_max_density : {
            return U_get_frame_data_10(data);
        }
        case UTE_FT_variance : {
            return U_get_frame_data_11(data);
        }
    }
    return 0;
}

double UC_plot::U_get_frame_data_energy(UTE_frame_type type_spectra, QVector<double> data) {
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
        case UTE_FT_max_density : {
            return U_get_frame_data_10(data);
        }
        case UTE_FT_variance : {
            return U_get_frame_data_11(data);
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
    if (cnt == 0) return 0;
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

double UC_plot::U_get_frame_data_10(QVector<double> data) { //UTE_FT_max_density
    const int density = 100;
    double min = 1e300;
    double max = -1e300;
    double z = 0;
    int cnt = data.size();
    for (int i = 0; i < cnt; i++) {
        z = data[i];
        if (z < min) min = z;
        if (z > max) max = z;
    }
    int bins;
    if (cnt < 100) bins = 20;
        else bins = cnt / density;
    TH1D * h = new TH1D("h", "h", bins, min, max);
    for (int i = 0; i < cnt; i++) {
        h->Fill(data[i]);
    }
    z = h->GetBinCenter(h->GetMaximumBin());
    delete h;
    return z;
}

double UC_plot::U_get_frame_data_11(QVector<double> data) { //UTE_FT_variance
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
    return std_dev / (cnt - 1);
}

void UC_plot::U_set_data_1file(UC_data_container::UTStr_data_container_settings * settings_ptr, QStringList file_names) {
    UC_data_container::UTStr_data_container_settings settings = * settings_ptr;
    QString str;

    UC_data_container * data_container;
    data_container = new UC_data_container(settings);

    int adr_cnt0 = 1;
    int adr_cnt1 = 0;
    if (!settings_ptr->both_counters) {
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
    QVector<int> thl_vector;

    int thl_index = 0;
    for (int i = 0; i < file_names.size(); i++) {

        str = file_names[i];
        emit US_file_found(str);
        file.setFileName(settings.path + "/" + str);
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
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
        data_container->U_set_data(thl_index, frame_0, adr_cnt0);
        data_container->U_set_data(thl_index, frame_1, adr_cnt1);
        thl_vector << thl;
        emit US_new_thl(thl);
        thl_index++;

        file.close();
        U_renew_progress_bar();
    }

    if (stop) {
        emit US_list_scans(&scans_list, scan_index);
        emit US_ready();
        stop = false;
        return;
    }

    data_container->U_set_thl_vector(thl_vector);
    scans_list << *data_container;
    if (scan_enable == false) {
        scan_index = scans_list.length() - 1;
        scan_enable = true;
        U_enable_ff_df();
        this->thl_vector = &thl_vector;
        thl_index_min = 0;
        thl_index_max = thl_vector.size() - 1;
        n_thl = thl_vector.size();
        emit US_thl_vector(thl_vector);
    }
    U_set_scan(scan_index);
}

void UC_plot::U_set_data_2file(UC_data_container::UTStr_data_container_settings * settings_ptr, QStringList file_names) {
    UC_data_container::UTStr_data_container_settings settings = * settings_ptr;
    QString str;

    UC_data_container * data_container;
    data_container = new UC_data_container(settings);

    int adr_cnt0 = 0;
    int adr_cnt1 = 1;
    if (!settings_ptr->both_counters) {
        adr_cnt0 = 1;
        adr_cnt1 = 0;
    }

    QFile file;
    QTextStream txt_str;
    int cnt0 = 0;
    int cnt1 = 0;
    int thl = 0;
    qulonglong pixel_data = 0;
    UC_data_container::UTStr_frame frame_0;
    UC_data_container::UTStr_frame frame_1;
    QVector<int> thl_vector;

    int thl_index = 0;
    for (int i = 0; i < file_names.size(); i += 2) {

        str = file_names[i];
        emit US_file_found(str);
        file.setFileName(settings.path + "/" + str);
        file.open(QFile::ReadOnly);
        txt_str.setDevice(&file);
        thl = str.left(3).toInt();
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < (15 * 256); x++) {
                txt_str >> pixel_data;
                cnt0 = static_cast<int>(pixel_data);
                frame_0.pixel[x][y] = cnt0;
                if (stop) break;
            }
        }
        file.close();
        data_container->U_set_data(thl_index, frame_0, adr_cnt0);

        str = file_names[i + 1];
        emit US_file_found(str);
        file.setFileName(settings.path + "/" + str);
        file.open(QFile::ReadOnly);
        txt_str.setDevice(&file);

        if (settings_ptr->both_counters) {
            for (int y = 0; y < 256; y++) {
                for (int x = 0; x < (15 * 256); x++) {
                    txt_str >> pixel_data;
                    cnt1 = static_cast<int>(pixel_data);
                    frame_1.pixel[x][y] = cnt1;
                    if (stop) break;
                }
                if (stop) break;
            }
            file.close();
            data_container->U_set_data(thl_index, frame_1, adr_cnt1);
        }

        thl_vector << thl;
        emit US_new_thl(thl);
        thl_index++;

        if (stop) break;
        U_renew_progress_bar();
    }

    if (stop) {
        emit US_list_scans(&scans_list, scan_index);
        emit US_ready();
        stop = false;
        return;
    }

    data_container->U_set_thl_vector(thl_vector);
    scans_list << *data_container;
    if (scan_enable == false) {
        scan_index = scans_list.length() - 1;
        scan_enable = true;
        U_enable_ff_df();
        this->thl_vector = &thl_vector;
        thl_index_min = 0;
        thl_index_max = thl_vector.size() - 1;
        n_thl = thl_vector.size();
        emit US_thl_vector(thl_vector);
    }
    U_set_scan(scan_index);
}


////////////////////////////!!!!!!!!!!SLOTS!!!!!!!!!!!///////////////////////////////////////////////////////////////////////////

void UC_plot::U_set_data(UC_data_container::UTStr_data_container_settings * settings_ptr) {
    UC_data_container::UTStr_data_container_settings settings = * settings_ptr;
    QString path = settings.path;
    QDir dir(path);
    QStringList file_names = dir.entryList(QDir::Files, QDir::Name);
    bool file1 = true;

    QString str;
    QString name;

    int n = file_names.size();
    QRegExp rx("[012345][0123456789][0123456789].pmf");
    rx.setPatternSyntax(QRegExp::Wildcard);
    int i = 0;
    while (i < n) {
        if (rx.exactMatch(file_names[i])) {
            i++;
        } else {
            file_names.removeAt(i);
            n--;
        }
    }

    if (n == 0) {
        //return;
        file1 = false;
        file_names = dir.entryList(QDir::Files, QDir::Name);
        n = file_names.size();
        rx.setPattern("[012345][0123456789][0123456789]_THL[01].pmf");
        i = 0;
        while (i < n) {
            if (rx.exactMatch(file_names[i])) {
                i++;
            } else {
                file_names.removeAt(i);
                n--;
            }
        }
    }

    if (n != 0) {
        settings.thl_min = file_names[0].left(3).toInt();
        settings.thl_max = file_names[file_names.size() - 1].left(3).toInt();
    }

    settings.n_thl = file_names.size();
    if (file1) {
        U_set_total_progress_bar(settings.n_thl);
    } else {
        U_set_total_progress_bar(settings.n_thl / 2);
    }
    emit US_n_files(settings.n_thl);

    if (file1) {
        U_set_data_1file(&settings, file_names);
    } else {
        U_set_data_2file(&settings, file_names);
    }

    emit US_list_scans(&scans_list, scan_index);
    emit US_ready();

//    UC_data_container::UTStr_data_container_settings settings = * settings_ptr;
//    QString path = settings.path;
//    QDir dir(path);
//    QStringList file_names = dir.entryList(QDir::Files, QDir::Name);

//    QString str;
//    QString name;

//    int n = file_names.size();

//    int i = 0;
//    while (i < n) {
//        name = file_names[i];
//        str = name.right(4);
//        if (str == ".pmf") {
//            str = name.right(8);
//            if (str == "THL0.pmf") {
//                file_names.removeAt(i);
//                n--;
//                continue;
//            }
//            if (str == "THL1.pmf") {
//                file_names.removeAt(i);
//                n--;
//                continue;
//            }
//            i++;
//        } else {
//            file_names.removeAt(i);
//            n--;
//        }
//    }

//    if (n == 0) {
//        return;
//    }

//    if (n != 0) {
//        settings.thl_min = file_names[0].left(3).toInt();
//        settings.thl_max = file_names[file_names.size() - 1].left(3).toInt();
//    }

//    settings.n_thl = file_names.size();
//    U_set_total_progress_bar(settings.n_thl);
//    UC_data_container * data_container;
//    data_container = new UC_data_container(settings);
//    emit US_n_files(settings.n_thl);

//    int adr_cnt0 = 1;
//    int adr_cnt1 = 0;
//    if (!settings_ptr->both_counters) {
//        adr_cnt0 = 0;
//        adr_cnt1 = 1;
//    }

//    QFile file;
//    QTextStream txt_str;
//    int cnt0 = 0;
//    int cnt1 = 0;
//    int thl = 0;
//    qulonglong pixel_data = 0;
//    UC_data_container::UTStr_frame frame_0;
//    UC_data_container::UTStr_frame frame_1;
//    QVector<int> thl_vector;

//    int thl_index = 0;
//    for (int i = 0; i < file_names.size(); i++) {

//        str = file_names[i];
//        emit US_file_found(str);
//        file.setFileName(path + "/" + str);
//        file.open(QFile::ReadOnly);
//        txt_str.setDevice(&file);
//        thl = str.left(3).toInt();

//        for (int y = 0; y < 256; y++) {
//            for (int x = 0; x < (15 * 256); x++) {
//                txt_str >> pixel_data;
//                cnt1 = static_cast<int>(pixel_data & 0xffffffff);
//                cnt0 = static_cast<int>((pixel_data >> 32) & 0xffffffff);
//                frame_0.pixel[x][y] = cnt0;
//                frame_1.pixel[x][y] = cnt1;
//                if (stop) break;
//            }
//            if (stop) break;
//        }
//        if (stop) break;
//        data_container->U_set_data(thl_index, frame_0, adr_cnt0);
//        data_container->U_set_data(thl_index, frame_1, adr_cnt1);
//        thl_vector << thl;
//        emit US_new_thl(thl);
//        thl_index++;

//        file.close();
//        U_renew_progress_bar();
//    }

//    if (stop) {
//        emit US_list_scans(&scans_list, scan_index);
//        emit US_ready();
//        stop = false;
//        return;
//    }

//    if (settings_ptr->both_counters) {
//        data_container->U_calculate_mean();
//    }


//    data_container->U_set_thl_vector(thl_vector);
//    scans_list << *data_container;
//    if (scan_enable == false) {
//        scan_index = scans_list.length() - 1;
//        scan_enable = true;
//        U_enable_ff_df();
//        this->thl_vector = &thl_vector;
//        thl_index_min = 0;
//        thl_index_max = thl_vector.size() - 1;
//        n_thl = thl_vector.size();
//        emit US_thl_vector(thl_vector);
//    }
//    U_set_scan(scan_index);

//    emit US_list_scans(&scans_list, scan_index);
//    emit US_ready();
}

void UC_plot::U_reset_data() {
    scans_list.clear();

    scan_enable = false;
    ff_enable = false;
    df_enable = false;
    scan_index = -1;

    emit US_list_scans(&scans_list, scan_index);

    U_reset_mask();
}

void UC_plot::U_delete_scan(int index) {
    if (index < 0) {
        return;
    }
    if (index > scans_list.size()) {
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

    if (index == scan_index) {
        if (scans_list.size() == 1) {
            scan_enable = false;
            ff_enable = false;
            df_enable = false;
            scan_index = -1;
        } else {
            scan_index = 0;
            U_enable_ff_df();
            thl_vector = scans_list[scan_index].U_get_thl_vector();
            thl_index_min = 0;
            thl_index_max = thl_vector->size() - 1;
            n_thl = thl_vector->size();
        }
    }
    if (index < scan_index) {
        scan_index--;
    }
    scans_list.removeAt(index);
    U_enable_ff_df();
    emit US_list_scans(&scans_list, scan_index);
}

void UC_plot::U_set_scan(int index) {
    if (index < 0) {
        return;
    }
    if (index >= scans_list.size()) {
        return;
    }
    scan_index = index;
    scan = &(scans_list[scan_index]);
    U_enable_ff_df();
    thl_vector = scan->U_get_thl_vector();
    thl_index_min = 0;
    thl_index_max = thl_vector->size() - 1;
    n_thl = thl_vector->size();
    emit US_thl_vector(*thl_vector);
    if (using_calibration) U_get_energy_range((*thl_vector)[0], (*thl_vector)[thl_vector->size() - 1]);
    emit US_list_scans(&scans_list, scan_index);
}

void UC_plot::U_set_settings(int index, UC_data_container::UTStr_data_container_settings *settings) {
    if (index < 0) {
        return;
    }
    if (index >= scans_list.size()) {
        return;
    }

    bool emit_signal = false;
    emit_signal = (scans_list[index].U_get_name() != settings->name);

    scans_list[index].U_set_settings(*settings);
    U_enable_ff_df();
    if (emit_signal) emit US_list_scans(&scans_list, scan_index);
}

void UC_plot::U_get_settings(int index) {
    if (index < 0) {
        return;
    }
    if (index >= scans_list.size()) {
        return;
    }
    UC_data_container::UTStr_data_container_settings settings = scans_list[index].U_get_settings();
    emit US_scan_settings(&settings);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_plot::U_generate_spectra() {
    double x;
    double y;
    U_set_total_progress_bar(thl_index_max - thl_index_min);
    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        U_reset_ff_minus_df_mean();
        y = U_get_frame_data(frame_type, pixel_type, thl_index);
        x = (*thl_vector)[thl_index];
        emit US_spectra_data(x, y);
        U_renew_progress_bar();
        if (stop) break;
    }
    stop = false;
    emit US_replot_spectra();
    emit US_ready();
}

void UC_plot::U_generate_spectra(int n) {
    QVector<QVector<double>> dev(n);
    QVector<double> bounds(n);
    double e_delta = (energy_max - energy_min) / n;
    bounds[0] = energy_min + e_delta;
    for (int i = 1; i < n; i++) {
        bounds[i] =  bounds[i - 1] + e_delta;
    }
    U_set_total_progress_bar((x_max - x_min) * n_thl);
    for (int thl_index = 0; thl_index <= n_thl - 1; thl_index++) {
        U_reset_ff_minus_df_mean();
        for (int x = x_min; x < x_max; x++) {
            double energy = U_get_energy_from_thl(x, (*thl_vector)[thl_index]);
            U_renew_progress_bar();
            if (energy < energy_min) continue;
            if (energy > energy_max) continue;
            int i = 0;
            while (energy >= bounds[i]) {
                i++;
                if (i == n) {
                    i--;
                    break;
                }
            }
            for (int y = y_min; y < y_max; y++) {
                if (!U_get_mask(x, y)) {
                    dev[i] << U_get_pixel_data(pixel_type, thl_index, x, y);
                }
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }
    for (int i = 0; i < n; i++) {
        if (stop) break;
        emit US_spectra_data(bounds[i] - e_delta / 2, U_get_frame_data_energy(frame_type, dev[i]));
    }
    stop = false;
    emit US_replot_spectra();
    emit US_ready();
//    QVector<double> dev;
//    int chip_min = x_min / 256;
//    int chip_max = (x_max / 256) + 1;
//    if (chip_max > 14) chip_max = 14;
//    int thl_max = 0;
//    int thl_max_temp = 0;
//    int thl_min = 512;
//    int thl_min_temp = 0;
//    for (int chip = chip_min; chip <= chip_max; chip++) {
//        thl_max_temp = U_get_thl_from_energy_chip(chip, energy_max);
//        if (thl_max < thl_max_temp) thl_max = thl_max_temp;
//        thl_min_temp = U_get_thl_from_energy_chip(chip, energy_min);
//        if (thl_min > thl_min_temp) thl_min = thl_min_temp;
//    }
//    U_set_total_progress_bar(15 * (thl_max - thl_min));
//    for (int thl_index = thl_min; thl_index <= thl_max; thl_index++) {
//        U_reset_ff_minus_df_mean();
//        for (int chip = chip_min; chip <= chip_max; chip++) {
//            int x_max_current = x_max;
//            if ((x_max_current / 256) > chip) x_max_current = 256 * (chip + 1);
//            int x_min_current = x_min;
//            if ((x_min_current / 256) < chip) x_min_current = 256 * chip;
//            double energy = U_get_energy_from_thl_chip(chip, (*thl_vector)[thl_index]);
//            for (int x = x_min_current; x < x_max_current; x++) {
//                for (int y = y_min; y < y_max; y++) {
//                    if (!U_get_mask(x, y)) {
//                        dev << U_get_pixel_data(pixel_type, thl_index, x, y);
//                    }
//                    if (stop) break;
//                }
//                if (stop) break;
//            }
//            emit US_spectra_data(energy, U_get_frame_data_energy(frame_type, dev));
//            if (stop) break;
//        }
//        if (stop) break;
//    }
//    stop = false;
//    emit US_replot_spectra();
//    emit US_ready();
}

void UC_plot::U_generate_frame(int thl_index) {
    double z;
    U_set_total_progress_bar(15 * 256 * 256);
    U_reset_ff_minus_df_mean();
    for (int x = 0; x < (15 * 256); x++) {
        for (int y = 0; y < 256; y++) {
            if (U_get_mask(x, y)) {
                emit US_frame_data(x, y, 0);
            } else {
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                emit US_frame_data(x, y, z);
            }
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }
    stop = false;
    emit US_replot_frame();
    emit US_ready();
}

void UC_plot::U_generate_frame(double energy) {
    double z;
    U_set_total_progress_bar(15 * 256 * 256);
    U_reset_ff_minus_df_mean();
    for (int x = 0; x < (15 * 256); x++) {
        int thl_index = thl_vector->indexOf(U_get_thl_from_energy(x, energy));
        for (int y = 0; y < 256; y++) {
            if (U_get_mask(x, y)) {
                emit US_frame_data(x, y, 0);
            } else {
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                emit US_frame_data(x, y, z);
            }
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }
    stop = false;
    emit US_replot_frame();
    emit US_ready();
}

void UC_plot::U_generate_table(int thl_index) {
    U_reset_ff_minus_df_mean();
    const int n = pixels_areas.size();
    U_set_total_progress_bar(256 * 256 * 15 + n);

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_reset();
    }

    for (int x = 0; x < 3840; x++) {
        for (int y = 0; y < 256; y++) {
            U_add_pixel_table(thl_index, x, y);
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }

    for (int i = 0; i < n; i++) {
        if (stop) break;
        pixels_areas[i].U_finalize();
        emit US_chip_data(pixels_areas[i]);
        U_renew_progress_bar();
    }
    stop = false;
    emit US_rewrite_table();
    emit US_ready();
}

void UC_plot::U_generate_table(double energy) {
    const int n = pixels_areas.size();
    U_set_total_progress_bar(256 * 256 * 15 + n);

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_reset();
    }

    for (int x = 0; x < 3840; x++) {
        int thl_index = thl_vector->indexOf(U_get_thl_from_energy(x, energy));
        U_reset_ff_minus_df_mean();
        for (int y = 0; y < 256; y++) {
            U_add_pixel_table(thl_index, x, y);
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }

    for (int i = 0; i < n; i++) {
        if (stop) break;
        pixels_areas[i].U_finalize();
        emit US_chip_data(pixels_areas[i]);
        U_renew_progress_bar();
    }
    stop = false;
    emit US_rewrite_table();
    emit US_ready();
}

void UC_plot::U_generate_table() {
    const int n = pixels_areas.size();
    U_set_total_progress_bar(256 * 256 * 15 * (thl_index_max - thl_index_min + 1) + n);

    for (int i = 0; i < n; i++) {
        pixels_areas[i].U_reset();
    }

    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        U_reset_ff_minus_df_mean();
        for (int x = 0; x < 3840; x++) {
            for (int y = 0; y < 256; y++) {
                U_add_pixel_table(thl_index, x, y);
                U_renew_progress_bar();
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }

    for (int i = 0; i < n; i++) {
        if (stop) break;
        pixels_areas[i].U_finalize();
        emit US_chip_data(pixels_areas[i]);
        U_renew_progress_bar();
    }
    stop = false;
    U_reset_ff_minus_df_mean();
    emit US_rewrite_table();
    emit US_ready();
}

void UC_plot::U_generate_distribution(int n_bins, double min, double max) {
    double z;
    TH1D * hist = new TH1D("hist", "hist", n_bins, min, max);
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_index_max - thl_index_min + 1));

    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        U_reset_ff_minus_df_mean();
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                U_renew_progress_bar();
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (z < min) continue;
                if (z > max) continue;
                hist->Fill(z);
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }

    QVector<double> x;
    QVector<double> y;
    for (int i = 1; i <= n_bins; i++){
        x << hist->GetBinCenter(i);
        y << hist->GetBinContent(i);
    }
    x.insert(0, min);
    y.insert(0, y[0]);
    x << max;
    y << y[y.size() - 1];

    emit US_replot_distribution(x, y);
    delete hist;
    stop = false;
    emit US_ready();
}

void UC_plot::U_generate_frame_distribution(int n_bins, double min, double max, int thl_index) {
    double z;
    TH1D * hist = new TH1D("hist", "hist", n_bins, min, max);
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min));
    U_reset_ff_minus_df_mean();

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            U_renew_progress_bar();
            if (U_get_mask(x, y)) continue;
            z = U_get_pixel_data(pixel_type, thl_index, x, y);
            if (z < min) continue;
            if (z > max) continue;
            hist->Fill(z);
            if (stop) break;
        }
        if (stop) break;
    }

    QVector<double> x;
    QVector<double> y;
    for (int i = 1; i <= n_bins; i++){
        x << hist->GetBinCenter(i);
        y << hist->GetBinContent(i);
    }
    x.insert(0, min);
    y.insert(0, y[0]);
    x << max;
    y << y[y.size() - 1];

    emit US_replot_distribution(x, y);
    delete hist;
    stop = false;
    emit US_ready();
}

void UC_plot::U_generate_calibration(int chip) {
    QVector<double> x;
    QVector<double> y;
    U_reset_ff_minus_df_mean();

    bool fit_enable = (pixel_type == UTE_PT_cnt1)                   ||
                      (pixel_type == UTE_PT_cnt1_divide_on_cnt0)    ||
                      (pixel_type == UTE_PT_diff_cnt1)              ||
                      (pixel_type == UTE_PT_cnt1_corr_cnt0)         ||
                      (pixel_type == UTE_PT_diff_cnt1_corr_cnt0)    ||
                      (pixel_type == UTE_PT_ffc_non_xray);

    if (fit_enable) {
        U_set_total_progress_bar(scan->U_get_n() * 2);
    } else {
        U_set_total_progress_bar(scan->U_get_n());
    }

    U_get_calibration_chip_spectra(&x, &y, chip);
    US_replot_calibration_chip(x, y, false);

    if (!fit_enable) {
        emit US_ready();
        stop = false;
        return;
    }
    U_get_calibration_chip_fit(&x, &y);
    stop = false;
    emit US_ready();
}

void UC_plot::U_generate_calibration() {
    int scan_index_reserve = scan_index;
    QVector<double> fit_x[15];
    QVector<double> fit_y;
    int n_scans = scans_list.size();
    QList<int> calibration_list;
    int total = 0;
    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
        scan = &(scans_list[scan_i]);
        if (scan->U_get_calibration()) {
            calibration_list << scan_i;
            total += (scan->U_get_n()) * 15;
        }
    }
    U_reset_ff_minus_df_mean();
    n_scans = calibration_list.size();
    total += 15 * n_scans + 15;
    U_set_total_progress_bar(total);
    double thl_min = 512;
    double thl_max = 0;
    double data;
    for (int scan_i = 0; scan_i < n_scans; scan_i++) {
        U_set_scan(calibration_list[scan_i]);
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
            if (stop) break;
        }
        if (stop) break;
    }

    const int n = 10;
    QVector<double> fit_n_x;
    QVector<double> fit_n_y;
    double data_x = 0;
    const double delta_x = (thl_max - thl_min) / (n - 1);
    for (int chip = 0; chip < 15; chip++) {
        if (stop) break;
        emit US_replot_calibration(fit_x[chip], fit_y, chip, false);
        TGraph gr(n_scans, fit_x[chip].data(), fit_y.data());
        TF1 fit("fit", "[0] + [1] * x + [2] * x * x");
        gr.Fit("fit", "MQ", "", fit_y[0], fit_y[n_scans - 1]);
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

    using_calibration = !stop;
    U_set_scan(scan_index_reserve);
    if (!stop) U_get_energy_range((*thl_vector)[0], (*thl_vector)[thl_vector->size() - 1]);
    stop = false;
    emit US_ready();
}

void UC_plot::U_generate_spectra_2d() {
    double z;
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (this->thl_index_max - this->thl_index_min + 1));

    for (int thl_index = this->thl_index_min; thl_index <= this->thl_index_max; thl_index++) {
        U_reset_ff_minus_df_mean();
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                U_renew_progress_bar();
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                emit US_spectra_2d_data((*thl_vector)[thl_index], z);
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }

    emit US_replot_spectra_2d();
    stop = false;
    emit US_ready();
}

void UC_plot::U_generate_spectra_2d(double energy_min, double energy_max) {
    if (!using_calibration) return;
    double z;
    double energy;
    int total = (x_max - x_min);
    U_set_total_progress_bar(total);
    for (int x = x_min; x < x_max; x++) {
        int thl_min = U_get_thl_from_energy(x, energy_min);
        int thl_index_min = thl_vector->indexOf(thl_min);
        if (thl_index_min == -1) {
            thl_index_min = 0;
        } else {
            if (thl_index_min > 0) thl_index_min = thl_index_min - 1;
        }
        int thl_max = U_get_thl_from_energy(x, energy_max);
        int thl_index_max = thl_vector->indexOf(thl_max);
        if (thl_index_max == -1) {
            thl_index_max = (thl_vector->size() - 1);
        } else {
            if (thl_index_max < (thl_vector->size() - 2)) thl_index_min = thl_index_min + 1;
        }
        for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
            U_reset_ff_minus_df_mean();
            energy = U_get_energy_from_thl(x, (*thl_vector)[thl_index]);
            if (energy > energy_max) continue;
            if (energy < energy_min) continue;
            for (int y = y_min; y < y_max; y++) {
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                emit US_spectra_2d_data(energy, z);

                if (stop) break;
            }
            if (stop) break;
        }
        U_renew_progress_bar();
        if (stop) break;
    }

    emit US_replot_spectra_2d();
    stop = false;
    emit US_ready();
}

void UC_plot::U_generate_range(int thl_index) {
    double z;
    double max = -1e300;
    double min = 1e300;
    U_reset_ff_minus_df_mean();

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) continue;
            z = U_get_pixel_data(pixel_type, thl_index, x, y);
            if (z > max) max = z;
            if (z < min) min = z;
            if (stop) break;
        }
        if (stop) break;
    }
    if (max > 0) {
        max *= 1.0001;
    } else {
        max *= 0.9999;
    }
    if (min > 0) {
        min *= 0.9999;
    } else {
        min *= 1.0001;
    }
    stop = false;
    emit US_range_data(min, max);
    emit US_ready();
}

void UC_plot::U_generate_range() {
    double z;
    double max = -1e300;
    double min = 1e300;
    U_reset_ff_minus_df_mean();
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_index_max - thl_index_min));
    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (z > max) {
                    max = z;
                }
                if (z < min) {
                    min = z;
                }
                U_renew_progress_bar();
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }
    if (max > 0) {
        max *= 1.0001;
    } else {
        max *= 0.9999;
    }
    if (min > 0) {
        min *= 0.9999;
    } else {
        min *= 1.0001;
    }
    stop = false;
    emit US_range_data(min, max);
    emit US_ready();
}

void UC_plot::U_generate_range_spectra_2d(int thl_index_min, int thl_index_max) {
    double z;
    double max = -1e300;
    double min = 1e300;

    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_index_max - thl_index_min + 1));

    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        U_reset_ff_minus_df_mean();
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                U_renew_progress_bar();
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (z > max) max = z;
                if (z < min) min = z;
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }

    emit US_spectra_2d_range_data(min, max);
    stop = false;
    emit US_ready();
}

void UC_plot::U_generate_range_spectra_2d(double energy_min, double energy_max) {
    if (!using_calibration) return;
    double z;
    double max = -1e300;
    double min = 1e300;
    double energy;
    int total = (x_max - x_min);
    U_set_total_progress_bar(total);
    for (int x = x_min; x < x_max; x++) {
        int thl_min = U_get_thl_from_energy(x, energy_min);
        int thl_index_min = thl_vector->indexOf(thl_min);
        if (thl_index_min == -1) {
            thl_index_min = 0;
        } else {
            if (thl_index_min > 0) thl_index_min = thl_index_min - 1;
        }
        int thl_max = U_get_thl_from_energy(x, energy_max);
        int thl_index_max = thl_vector->indexOf(thl_max);
        if (thl_index_max == -1) {
            thl_index_max = (thl_vector->size() - 1);
        } else {
            if (thl_index_max < (thl_vector->size() - 2)) thl_index_min = thl_index_min + 1;
        }
        for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
            U_reset_ff_minus_df_mean();
            energy = U_get_energy_from_thl(x, (*thl_vector)[thl_index]);
            if (energy > energy_max) continue;
            if (energy < energy_min) continue;
            for (int y = y_min; y < y_max; y++) {
                if (U_get_mask(x, y)) continue;
                z = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (z > max) max = z;
                if (z < min) min = z;

                if (stop) break;
            }
            if (stop) break;
        }
        U_renew_progress_bar();
        if (stop) break;
    }

    emit US_spectra_2d_range_data(min, max);
    stop = false;
    emit US_ready();
}
///



///////////////////////////////////////////////////////////////////////////////////////////////////////


void UC_plot::U_set_frame_type(UTE_frame_type frame_type) {
    this->frame_type = frame_type;
}

void UC_plot::U_set_pixel_type(UTE_pixel_type pixel_type) {
    this->pixel_type = pixel_type;
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
    U_get_energy_range((*thl_vector)[0], (*thl_vector)[thl_vector->size() - 1]);
}

void UC_plot::U_set_threshold_range(int thl_index_min, int thl_index_max) {
    this->thl_index_min = thl_index_min;
    this->thl_index_max = thl_index_max;
}

void UC_plot::U_set_energy_range(double energy_min, double energy_max) {
    this->energy_min = energy_min;
    this->energy_max = energy_max;
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
    using_calibration = true;
    U_get_energy_range((*thl_vector)[0], (*thl_vector)[thl_vector->size() - 1]);
}
//
void UC_plot::U_get_thl_index_range(int thl_min, int thl_max) {
    int z = 0;
    int index_min = 0;
    int index_max = n_thl - 1;
    for (int i = 0; i < n_thl; i++) {
        z = (*thl_vector)[i];
        if (z >= thl_min) {
            index_min = i;
            break;
        }
    }
    for (int i = n_thl - 1; i >= 0; i--) {
        z = (*thl_vector)[i];
        if (z <= thl_max) {
            index_max = i;
            break;
        }
    }
    thl_index_min = index_min;
    thl_index_max = index_max;
    emit US_thl_range(index_min, index_max);
}

void UC_plot::U_get_energy_range(int thl_min, int thl_max) {
    if (!using_calibration) return;
    double max;
    double min;
    energy_max = 0;
    energy_min = 1e300;
    int chip_min = x_min / 256;
    int chip_max = (x_max - 1) / 256;
    for (int chip = chip_min; chip <= chip_max; chip++) {
        max = U_get_energy_from_thl_chip(chip, thl_max);
        min = U_get_energy_from_thl_chip(chip, thl_min);
        if (max > energy_max) energy_max = max;
        if (min < energy_min) energy_min = min;
    }
    emit US_energy_range(energy_min, energy_max);
}

void UC_plot::U_get_thl_vector() {
    emit US_thl_vector((*thl_vector));
}
///////////////////////////////////////////////////////////////////////////////////////
void UC_plot::U_set_mask(bool mask, bool more, double value, bool in_roi, int thl_vector) {
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

    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min));

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            data = U_get_pixel_data(pixel_type, thl_vector, x, y);
            if (more) {
                if (data > value) {
                    U_set_mask(x, y, mask);
                }
            } else {
                if (data < value) U_set_mask(x, y, mask);
            }
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
}

void UC_plot::U_set_mask(bool mask, bool more, double value, bool in_roi) {
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

    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_index_max - thl_index_min));

    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                data = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (more) {
                    if (data > value) {
                        U_set_mask(x, y, mask);
                    }
                } else {
                    if (data < value) U_set_mask(x, y, mask);
                }
                U_renew_progress_bar();
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
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
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min));

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            if (U_get_mask(x, y)) continue;
            data = U_get_pixel_data(pixel_type, thl_index, x, y);
            if (more) {
                if (data > value) n++;
            } else {
                if (data < value) n++;
            }
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) stop = false;
        else emit US_count_mask(n);
    emit US_ready();
}

void UC_plot::U_count_mask(bool more, double value, bool in_roi) {
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
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_index_max - thl_index_min));

    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                if (U_get_mask(x, y)) continue;
                data = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (more) {
                    if (data > value) n++;
                } else {
                    if (data < value) n++;
                }
                U_renew_progress_bar();
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) stop = false;
        else emit US_count_mask(n);
    emit US_ready();
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

    int cnt0 = 0;
    int cnt1 = 0;
    bool overflowed;
    int overflow = scan->U_get_count() * 4095;
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min));

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            cnt0 = scan->U_get_data(thl_index, x, y, 0);
            cnt1 = scan->U_get_data(thl_index, x, y, 1);
            overflowed = (cnt0 == overflow) | (cnt1 == overflow);
            if (overflowed) U_set_mask(x, y, true);
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
}

void UC_plot::U_set_mask_overflowed(bool in_roi) {
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
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_index_max - thl_index_min));

    for (int thl_index = thl_index_min; thl_index <= thl_index_max; thl_index++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                cnt0 = scan->U_get_data(thl_index, x, y, 0);
                cnt1 = scan->U_get_data(thl_index, x, y, 1);
                overflowed = (cnt0 == overflow) | (cnt1 == overflow);
                if (overflowed) U_set_mask(x, y, true);
                U_renew_progress_bar();
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
}

void UC_plot::U_reset_mask() {
    for (int i = 0; i < (15 * 256 * 256); i++) {
        mask[i] = false;
    }
}

void UC_plot::U_mask_selected(int x_min, int x_max, int y_min, int y_max) {
    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min));
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            U_set_mask(x, y, true);
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
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

    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min) * (thl_index_max - thl_index_min));

    for (int thl_index = thl_index_min; thl_index < thl_index_max; thl_index++) {
        for (int x = x_min; x < x_max; x++) {
            for (int y = y_min; y < y_max; y++) {
                data = U_get_pixel_data(pixel_type, thl_index, x, y);
                if ((data < value_max) && (value_min < data)) {
                    U_set_mask(x, y, true);
                }
                U_renew_progress_bar();
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
}

void UC_plot::U_mask_selected_value(double value_min, double value_max, bool in_roi, int thl_index) {
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

    U_set_total_progress_bar((x_max - x_min) * (y_max - y_min));

    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            data = U_get_pixel_data(pixel_type, thl_index, x, y);
            if ((data < value_max) && (value_min < data)) {
                U_set_mask(x, y, true);
            }
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
}

void UC_plot::U_set_mask_sd_from_mean(bool mask, bool more, double value, bool in_roi, int thl_index) {
    int x_min = 0;
    int x_max = 15 * 256;
    int y_min = 0;
    int y_max = 256;
    double data;
    if (!in_roi) {
        x_min = this->x_min;
        x_max = this->x_max;
        y_min = this->y_min;
        y_max = this->y_max;
        this->x_min = 0;
        this->x_max = 15 * 256;
        this->y_min = 0;
        this->y_max = 256;
    }

    double mean = U_get_frame_data(UTE_FT_average, pixel_type, thl_index);
    double sd = U_get_frame_data(UTE_FT_standart_deviation, pixel_type, thl_index);
    U_set_total_progress_bar((this->x_max - this->x_min) * (this->y_max - this->y_min));

    for (int x = this->x_min; x < this->x_max; x++) {
        for (int y = this->y_min; y < this->y_max; y++) {
            data = U_get_pixel_data(pixel_type, thl_index, x, y);
            if (more) {
                if (qAbs(data - mean) > (value * sd)) U_set_mask(x, y, mask);
            } else {
                if (qAbs(data - mean) < (value * sd)) U_set_mask(x, y, mask);
            }
            U_renew_progress_bar();
            if (stop) break;
        }
        if (stop) break;
    }
    if (!in_roi) {
        this->x_min = x_min;
        this->x_max = x_max;
        this->y_min = y_min;
        this->y_max = y_max;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
}

void UC_plot::U_set_mask_sd_from_mean(bool mask, bool more, double value, bool in_roi) {
    int x_min = 0;
    int x_max = 15 * 256;
    int y_min = 0;
    int y_max = 256;
    double data;
    if (!in_roi) {
        x_min = this->x_min;
        x_max = this->x_max;
        y_min = this->y_min;
        y_max = this->y_max;
        this->x_min = 0;
        this->x_max = 15 * 256;
        this->y_min = 0;
        this->y_max = 256;
    }

    U_set_total_progress_bar((this->x_max - this->x_min) * (this->y_max - this->y_min) * (thl_index_max - thl_index_min));

    for (int thl_index = thl_index_min; thl_index < thl_index_max; thl_index++) {
        double mean = U_get_frame_data(UTE_FT_average, pixel_type, thl_index);
        double sd = U_get_frame_data(UTE_FT_standart_deviation, pixel_type, thl_index);

        for (int x = this->x_min; x < this->x_max; x++) {
            for (int y = this->y_min; y < this->y_max; y++) {
                data = U_get_pixel_data(pixel_type, thl_index, x, y);
                if (more) {
                    if (qAbs(data - mean) > (value * sd)) U_set_mask(x, y, mask);
                } else {
                    if (qAbs(data - mean) < (value * sd)) U_set_mask(x, y, mask);
                }
                U_renew_progress_bar();
                if (stop) break;
            }
            if (stop) break;
        }
        if (stop) break;
    }

    if (!in_roi) {
        this->x_min = x_min;
        this->x_max = x_max;
        this->y_min = y_min;
        this->y_max = y_max;
    }
    if (stop) {
        U_reset_mask();
        stop = false;
    }
    emit US_ready();
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
//
void UC_plot::U_stop() {
    stop = true;
}


