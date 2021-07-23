/*!
    \file
    \brief Заголовочный файл с классами, обеспечивающими обработку данных в отдельном потоке

    Данный файл содержаит описание двух классов:
    Класс UC_pixels_info - класс - контейнер для расчета статистических данных разных областей детектора.
    Класс UC_plot - используется для непосредственного хранения и обработки данных.
*/

#ifndef UC_PLOT_H
#define UC_PLOT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QFile>
#include <QDir>
#include <QVector>
#include <QTextStream>
#include <QtMath>
#include <QMessageBox>

#include <TGraph.h>
#include <TF1.h>
#include <TH1D.h>

#include "uc_data_container.h"
#include "uc_pixels_info.h"
#include "common_functions.h"
#include "uc_identification.h"

/*!
 * \brief Класс, который хранит данные и выполняет расчеты.
 *
 * Основной клас который загружает и хранит данные и проводит с ними все манипуляции.
 * Должен работать в отдельтном потоке.
 */
class UC_plot : public QObject
{
    /// RoI
    int x_min;              ///< Левая граница области интереса.
    int x_max;              ///< Правая граница области интереса.
    int y_min;              ///< Нижняя граница области интереса.
    int y_max;              ///< Верхняя граница области интереса.
    int thl_min;            ///< Минимальный порог области интереса по порогам.
    int thl_max;            ///< Максимальный порог области интереса по порогам.
    double energy_min;      ///< Максимальная энергии диапазона по энергии
    double energy_max;      ///< Минимальная энергии диапазона по энергии

    int thl_id_1;           ///< Нижний порог левого оокна для идентификации веществ по методу Шелкова.
    int thl_id_2;           ///< Верхний порог левого оокна для идентификации веществ по методу Шелкова.
    int thl_id_3;           ///< Нижний порог правого оокна для идентификации веществ по методу Шелкова.
    int thl_id_4;           ///< Верхний порог правого оокна для идентификации веществ по методу Шелкова.

    double threshold_level;        ///< Уровень порога активности пикселя.
    int rebin_x;                ///< Грппировка (ребининг) пикселей по направлению x.
    int rebin_y;                ///< Грппировка (ребининг) пикселей по направлению y.
    int rebin_thl;              ///< Грппировка (ребининг) значений при соседних порогах.
    int smoothing;              ///< Показатель сглаживания.
    int total_progress_bar;     ///< Переменная, хранящая общее число шагов при расчетах.
    int current_progress_bar;   ///< Переменная, хранящая пройденное число шагов при расчетах.

    bool stop;
    bool using_calibration;     ///< Использование калибровки.
    bool ff_minus_df_ready;
    bool scan_enable;           ///< Наличие активного скана.
    bool ff_enable;             ///< Наличие активного скана плоского поля.
    bool df_enable;             ///< Наличие активного скана черного поля.
    bool mask[256 * 256 * 15];  ///< Массив масок. По одному значению на каждый пиксель.

    double calibration_p0[15];  ///< Нулевые коэффициенты калибровки по чипам.
    double calibration_p1[15];  ///< Первые коэффициенты калибровки по чипам.
    double calibration_p2[15];  ///< Вторые коэффициенты калибровки по чипам.

    double mean_ff_minus_df;  ///< Коэффициенты для полного FFC по чипам.

    QList<UC_data_container> scans_list;    ///< Список загруженных сканов.

    int scan_index;
    UC_data_container * scan;   ///< Указатель на активный скан.
    UC_data_container * ff;     ///< Указатель на активный скан плоского поля.
    UC_data_container * df;     ///< Указатель на активный скан черного поля.

    QVector<int> element_vector;    ///< Вектор индексов эелементов.

public :
    /// Методы расчета кадра.
    typedef enum {
        UTE_FT_average,                     ///< Среднее значение кадра.
        UTE_FT_sum,                         ///< Сумма значений в кадре.
        UTE_FT_median,                      ///< Медианное значение в кадре.
        UTE_FT_max,                         ///< Максимальное значение в кадре.
        UTE_FT_min,                         ///< Минимальное значние в кадре.
        UTE_FT_zeros,                       ///< Число нулевых значений в кадре.
        UTE_FT_overflowed,                  ///< Число переполнений в кадре.
        UTE_FT_standart_deviation,          ///< Стандартное отклонение значений в кадре.
        UTE_FT_signal_to_noise_resolution,  ///< Отношение сигнал - шум для значений в кадре.
    } UTE_frame_type;

    /// Методы идентификации веществ
    typedef enum {
        UTE_IT_GA_method,           ///< Простейший математический критерий отбора одного элемента.
        UTE_IT_linear_combination,  ///< Спектр рассматриваемого скана раскладывается как линейная комбинация на спектры элементов.
    } UTE_id_type;

    /// Методы калибровки
    typedef enum {
        UTE_CT_with_fit,        ///< Значение порога при данном скане для данного чипа рассчитывается с фитированием.
        UTE_CT_without_fit,     ///< Значение порога при данном скане для данного чипа рассчитывается без фитирования.
    } UTE_calibration_type;

    /// Методы расчета пикселя.
    typedef enum {
        UTE_PT_cnt0,                        ///< Значение нулевого счетчика при сканировании с образцом.
        UTE_PT_cnt1,                        ///< Значение первого счетчика при сканировании с образцом.
        UTE_PT_cnt1_divide_on_cnt0,         ///< Отношение значений первого счетчика ко значению нулевого счетчика, взятых при сканировании с образцом.
        UTE_PT_cnt0_subtr_cnt1,             ///< Разность значений нулевого и первого счетчиков, взятых при сканировании с образцом.
        UTE_PT_cnt1_subtr_cnt1_max_thl,     ///< Значение первого счетчика при сканировании с образцом с вычитанием кадра с максимальным thl.
        UTE_PT_diff_cnt1,                   ///< Приращение значения первого счетчика при понижении порога в сканировании с образцом.
        UTE_PT_ffc,                         ///< Коррекция плоского поля. Значения берутся из сырых данных.
        UTE_PT_mu,                          ///< Коэффициент поглощения. Значения берутся из сырых данных.
        UTE_PT_diff_ffc,                    ///< Коррекция плоского поля. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_diff_mu,                     ///< Коэффициент поглощения. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_ffc_diff,                    ///< Коррекция плоского поля. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_mu_diff,                     ///< Коэффициент поглощения. Значения берутся равными соответствующим приращениям при понижении порога.

        UTE_PT_cnt1_corr_cnt0,              ///< Значение первого счетчика, скореектированного по среднему значению нулевого счетчика, при сканировании с образцом.
        UTE_PT_diff_cnt1_corr_cnt0,         ///< Приращение значения первого счетчика, скореектированного по среднему значению нулевого счетчика, при понижении порога в сканировании с образцом.
        UTE_PT_ffc_corr_cnt0,               ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся из сырых данных.
        UTE_PT_mu_corr_cnt0,                ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся из сырых данных.
        UTE_PT_diff_ffc_corr_cnt0,          ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_diff_mu_corr_cnt0,           ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_ffc_diff_corr_cnt0,          ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_mu_diff_corr_cnt0,           ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.

        UTE_PT_ffc_non_xray,                ///< Коррекция плоского поля для нерентгеновских изображений.
        UTE_PT_cnt0_deviation,              ///< Коррекция плоского поля для нерентгеновских изображений.
        UTE_PT_cnt1_act,                    ///< Активность нулевого счетчика.
        UTE_PT_cnt0_act,                    ///< Активность первого счетчика.
        UTE_PT_cnt1_rejected,               ///< подавление активности пикселя порогом.
        UTE_PT_diff_cnt1_rejected,          ///< Порог подавления активности пикселя.
        UTE_PT_cnt1ffc_div_cnt0ffc,         ///< Отношение коррекции чистого поля 1-го счетчика к коррекции плоского поля для 2-го счетчика.
        UTE_PT_smoothing_mu_diff,             ///< Дополнительные данные.
        UTE_PT_GA_request,
        UTE_PT_ffc_cnt0
    } UTE_pixel_type;

    typedef struct {
        double id_data;
        int thl_1_window_left;
        int thl_1_window_right;
        int thl_2_window_left;
        int thl_2_window_right;
        double sum_1_window;
        double sum_2_window;
        UTE_frame_type frame_type;                      ///< Текущий метод расчета кадра.
        UTE_pixel_type pixel_type;
    } UTStr_id_GA_data;

    typedef struct {
        int number_of_samples;
        QList<QString> elements;
        QVector<double> id_values;
        QVector<int> thl_samples;
        QVector<double> data;
        QVector<QVector<double>> matrix;
        UTE_frame_type frame_type;                      ///< Текущий метод расчета кадра.
        UTE_pixel_type pixel_type;
    } UTStr_id_LC_data;

private :

    UTE_frame_type frame_type;                      ///< Текущий метод расчета кадра.
    UTE_pixel_type pixel_type;                      ///< Текущий метод расчета пикселей.
    UTE_id_type id_type;    ///< Текущий метод идентификации.
    UTE_calibration_type calibration_type;

    QList<UC_pixels_info> pixels_areas;

    Q_OBJECT
public:

    explicit UC_plot(QObject *parent = nullptr);
    ~UC_plot();

    /// Функции калибровки
    double U_get_energy_from_thl(int x, int thl);
    double U_get_energy_from_thl_chip(int chip, int thl);
    int U_get_thl_from_energy(int x, double energy);
    int U_get_thl_from_energy_chip(int chip, double energy);
//////////////////////////////////////////////////////////////////////////
    void U_make_ff_minus_df_mean(int thl);
    double U_get_ff_minus_df_mean(int thl_index);
    void U_reset_ff_minus_df_mean();
/////////////////////////////////////////////////////////////////////////////////
    void U_set_mask(int x, int y, bool value);
    bool U_get_mask(int x, int y);
    //
    void U_set_total_progress_bar(int total);
    void U_renew_progress_bar();
    //
    void U_add_pixel_table(int thl, int x, int y);
    //
    void U_get_calibration_chip_spectra(QVector<double> * x, QVector<double> * y, int chip);
    TF1 U_get_calibration_fit(QVector<double> * x, QVector<double> * y);
    void U_get_calibration_chip_fit(QVector<double> * x, QVector<double> * y);
    double U_get_calibration_data_1(QVector<double> * x, QVector<double> * y);
    double U_get_calibration_data_2(QVector<double> * x, QVector<double> * y);
    //
    void U_enable_ff_df();
    //
    double U_get_data_rebin(UC_data_container * scan, int thl, int x, int y, int cnt);
    double U_get_data_rebin_scaled(UC_data_container * scan, int thl, int x, int y, int cnt);
    double U_get_data_rebin_corr(UC_data_container * scan, int thl, int x, int y);
    double U_get_data_rebin_corr_scaled(UC_data_container * scan, int thl, int x, int y);
    //
    double U_get_diff_data(UC_data_container * scan, int thl, int x, int y, int cnt);
    double U_get_diff_data_scaled(UC_data_container * scan, int thl, int x, int y, int cnt);
    double U_get_diff_data_corr(UC_data_container * scan, int thl, int x, int y);
    double U_get_diff_data_corr_scaled(UC_data_container * scan, int thl, int x, int y);
///////////////////////////////////////////////////////////////////////////////
    double U_get_pixel_data(UTE_pixel_type type, int thl, int x, int y);
    int U_get_pixel_data_1(int thl, int x, int y); //UTE_PT_cnt0
    int U_get_pixel_data_2(int thl, int x, int y); //UTE_PT_cnt1
    double U_get_pixel_data_3(int thl, int x, int y); //UTE_PT_cnt1_divide_on_cnt0
    int U_get_pixel_data_4(int thl, int x, int y); //UTE_PT_cnt0_subtr_cnt1
    double U_get_pixel_data_5(int thl, int x, int y); //UTE_PT_diff_cnt1
    int U_get_pixel_data_6(int thl, int x, int y); //UTE_PT_cnt1_subtr_cnt1_max_thl
    double U_get_pixel_data_7(int thl, int x, int y); //UTE_PT_ffc
    double U_get_pixel_data_8(int thl, int x, int y); //UTE_PT_mu
    double U_get_pixel_data_9(int thl, int x, int y); //UTE_PT_diff_ffc
    double U_get_pixel_data_10(int thl, int x, int y); //UTE_PT_diff_mu
    double U_get_pixel_data_11(int thl, int x, int y); //UTE_PT_ffc_diff
    double U_get_pixel_data_12(int thl, int x, int y); //UTE_PT_mu_diff
    ///
    double U_get_pixel_data_13(int thl, int x, int y); //UTE_PT_cnt1_corr_cnt0
    double U_get_pixel_data_14(int thl, int x, int y); //UTE_PT_diff_cnt1_corr_cnt0
    double U_get_pixel_data_15(int thl, int x, int y); //UTE_PT_ffc_corr_cnt0
    double U_get_pixel_data_16(int thl, int x, int y); //UTE_PT_mu_corr_cnt0
    double U_get_pixel_data_17(int thl, int x, int y); //UTE_PT_diff_ffc_corr_cnt0
    double U_get_pixel_data_18(int thl, int x, int y); //UTE_PT_diff_mu_corr_cnt0
    double U_get_pixel_data_19(int thl, int x, int y); //UTE_PT_ffc_diff_corr_cnt0
    double U_get_pixel_data_20(int thl, int x, int y); //UTE_PT_mu_diff_corr_cnt0
    ///
    double U_get_pixel_data_21(int thl, int x, int y); //UTE_PT_ffc_non_xray
    double U_get_pixel_data_22(int thl, int x, int y); //UTE_PT_cnt0_deviation
    //
    int U_get_pixel_data_23(int thl, int x, int y); //UTE_PT_cnt1_act
    int U_get_pixel_data_24(int thl, int x, int y); //UTE_PT_cnt0_act
    int U_get_pixel_data_25(int thl, int x, int y); //UTE_PT_cnt1_rejected
    int U_get_pixel_data_26(int thl, int x, int y); //UTE_PT_diff_cnt1_rejected
    //
    double U_get_pixel_data_27(int thl, int x, int y); //UTE_PT_cnt1ffc_div_cnt0ffc
    //
    double U_get_pixel_data_28(int thl, int x, int y); //UTE_PT_smoothing_mu_diff
    //
    double U_get_pixel_data_29(int thl, int x, int y); //UTE_PT_GA_request
    double U_get_pixel_data_30(int thl, int x, int y); //UTE_PT_ffc_cnt0
////////////////////////////////////////////////////////////////////////////////
    double U_get_frame_data(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, int thl);
    double U_get_frame_data_energy(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, double e_min, double e_max);
    double U_get_frame_data_1(QVector<double> data); //UTE_FT_average
    double U_get_frame_data_2(QVector<double> data); //UTE_FT_sum
    double U_get_frame_data_3(QVector<double> data); //UTE_FT_median
    double U_get_frame_data_4(QVector<double> data); //UTE_FT_max
    double U_get_frame_data_5(QVector<double> data); //UTE_FT_min
    int U_get_frame_data_6(QVector<double> data); //UTE_FT_zeros
    int U_get_frame_data_7(QVector<double> data); //UTE_FT_overflowed
    double U_get_frame_data_8(QVector<double> data); //UTE_FT_standart_deviation
    double U_get_frame_data_9(QVector<double> data); //UTE_FT_signal_to_noise_resolution
////////////////////////////////////////////////////////////////////////////////////
    void U_id_roi_1();

    QVector<double> U_calculating_samples_value(UC_data_container * scan, QVector<int> thl_samples);
    void U_id_roi_2();
    //
    void U_calculating_xmu(UC_data_container * scan, int i, int j, int rebin_x, int rebin_y, int rebin_thl);
    void U_calculating_id(UC_data_container * scan, QList<UC_data_container *> id_samples);

signals:
    void US_spectra_data(double x, double y);
    void US_frame_data(double x, double y, double z);
    void US_chip_data(UC_pixels_info pixels_info);
    void US_distribution_data(double x, double y);
    void US_range_data(double lower, double upper);
    void US_calibration_chip_data(double x, double y, bool is_fit);
    void US_calibration_data(double x, double y, int chip, bool is_fit);
    void US_spectra_2d_data(double x, double y);
    void US_spectra_2d_range_data(double lower, double upper);
    void US_id_roi_GA_data(UC_plot::UTStr_id_GA_data data);
    void US_id_roi_LC_data(UC_plot::UTStr_id_LC_data data);
    void US_id_data();
    void US_id_frame_data(double x, double y, double z);
    void US_id_data(int x, int y, double value);
    void US_id_data(double x, double y);
    ///////////////////////////////////////////////////////////////////////////////////

    void US_replot_spectra(QVector<double> x, QVector<double> y);
    void US_replot_spectra();
    void US_replot_frame();
    void US_rewrite_table();
    void US_replot_distribution(QVector<double> x, QVector<double> y);
    void US_replot_calibration(QVector<double> x, QVector<double> y, int chip, bool fit);
    void US_replot_calibration_chip(QVector<double> x, QVector<double> y, bool fit);
    void US_replot_spectra_2d();
    void US_replot_id();
    void US_replot_id_frame();
    /////////////////////////////////////////////////////////////////////////////
    void US_new_thl(int thl);
    void US_count_mask(int n);
    //
    void US_thl_range(int thl_min, int thl_max);
    void US_energy_range(double energy_min, double energy_max);
    ////////////////////////////////////////////////////////////////////////
    void US_file_found(QString file_name);
    void US_n_files(int n);
    void US_renew_progress_bar(double current, double total);
    //
    void US_list_scans(QList<UC_data_container> * scans_list, int active_index);
    void US_scan_settings(UC_data_container::UTStr_data_container_settings * settings);
    //
    void US_id_scan_list(QList<QString> list);
    void US_set_roi_range(int x_min, int x_max, int y_min, int y_max);
    //
    void US_ready();


public slots:
    void U_set_data(UC_data_container::UTStr_data_container_settings * settings_ptr);
    void U_reset_data();
    void U_delete_scan(int index);
    void U_set_scan(int index);
    void U_set_settings(int index, UC_data_container::UTStr_data_container_settings * settings);
    void U_get_settings(int index);

//////////////////////////////////////////////////////////////////////////////////
    void U_generate_spectra();
    void U_generate_spectra(int n);
    void U_generate_frame(int thl);
    void U_generate_frame(double energy);
    void U_generate_table(int thl);
    void U_generate_table(double energy);
    void U_generate_table();
    void U_generate_distribution(int n_bins, double min, double max);
    void U_generate_frame_distribution(int n_bins, double min, double max, int thl);
    void U_generate_calibration(int chip);
    void U_generate_calibration();
    void U_generate_spectra_2d(int thl_min, int thl_max);
    void U_generate_spectra_2d(double energy_min, double energy_max);
    void U_generate_id_roi();
    void U_generate_additional_data();
    void U_generate_id_data();
    void U_generate_id_frame(int element_index);
    void U_generate_range(int thl);
    void U_generate_range();
    void U_generate_range_spectra_2d(int thl_min, int thl_max);
    void U_generate_range_spectra_2d(double energy_min, double energy_max);
/////////////////////////////////////////////////////////////////
    void U_set_frame_type(UC_plot::UTE_frame_type frame_type);
    void U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type);
    void U_set_id_type(UC_plot::UTE_id_type id_type);
    void U_set_calibration_type(UC_plot::UTE_calibration_type calibration_type);
    ///
    void U_set_roi(int x_min, int x_max, int y_min, int y_max);
    void U_set_rebin(int rebin_x, int rebin_y, int rebin_thl);
    void U_set_id_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    void U_set_threshold_range(int thl_start, int thl_finish);
    //
    void U_set_using_calibraion(bool enable);
    void U_set_threshold_level(double level);
    void U_set_smoothing(int smoothing);
    //
    void U_save_calibration(QString file_name);
    void U_load_calibration(QString file_name);
    //
    void U_get_max_thl_range();
    void U_get_common_thl_range();
    void U_get_max_energy_range();
    void U_get_common_energy_range();
//////////////////////////////////////////////////////////////////////
    void U_set_mask(bool mask, bool more, double value, bool in_roi, int thl);
    void U_set_mask(bool mask, bool more, double value, bool in_roi);
    void U_count_mask(bool more, double value, bool in_roi, int thl);
    void U_count_mask(bool more, double value, bool in_roi);
    void U_set_mask_overflowed(bool in_roi, int thl);
    void U_set_mask_overflowed(bool in_roi);
    void U_reset_mask();
    void U_mask_selected(int x_min, int x_max, int y_min, int y_max);
    void U_mask_selected_value(double value_min, double value_max, bool in_roi);
    void U_mask_selected_value(double value_min, double value_max, bool in_roi, int thl);
    void U_set_mask_sd_from_mean(bool mask, bool more, double value, bool in_roi, int thl);
    void U_set_mask_sd_from_mean(bool mask, bool more, double value, bool in_roi);
    void U_save_mask(QString);
    void U_load_mask(QString);
    //
    void U_add_roi(UC_roi roi);
    void U_delete_roi(int index);
    //
    void U_stop();
};




#endif // UC_PLOT_H
//
