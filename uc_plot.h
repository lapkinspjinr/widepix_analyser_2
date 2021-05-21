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

#include <TGraph.h>
#include <TF1.h>
#include <TH1D.h>

#include "uc_data_container.h"
#include "uc_pixels_info.h"
#include "common_functions.h"

/*!
 * \brief Класс, который хранит данные и выполняет расчеты.
 *
 * Основной клас который загружает и хранит данные и проводит с ними все манипуляции.
 * Должен работать в отдельтном потоке.
 */
class UC_plot : public QObject
{
    int x_min;                  ///< Левая граница области интереса.
    int x_max;                  ///< Правая граница области интереса.
    int y_min;                  ///< Нижняя граница области интереса.
    int y_max;                  ///< Верхняя граница области интереса.
    int thl_id_1, thl_id_2, thl_id_3, thl_id_4;
    int thl_start, thl_finish;

    double calibration_p0[15];
    double calibration_p1[15];
    double calibration_p2[15];
    bool using_calibration;


    /// Массивы вспомогательных данных.
    bool mask[256 * 256 * 15];                  ///< Массив масок. По одно значению на каждый пиксель
//    double data_cnt0_mean[256 * 256 * 15];      ///< Массив средних значений нулевого счетчика для сканирования с образцом.
//    double data_df_cnt0_mean[256 * 256 * 15];   ///< Массив средних значений нулевого счетчика для сканирования с выключенным источником.
//    double data_ff_cnt0_mean[256 * 256 * 15];   ///< Массив средних значений нулевого счетчика для сканирования без образца.
//    double * ff_minus_df_mean;                  ///< Динамический массив средних значений обоих счетчиков для сканирования без образца.

//    /// Динамические массивы основных данных
//    int * data;                 ///< Динамический массив значений обоих счетчиков для сканирования c образцом.
//    int * data_ff;              ///< Динамический массив значений обоих счетчиков для сканирования без образца.
//    int * data_df;              ///< Динамический массив значений обоих счетчиков для сканирования с выключенным источником.
//    int * data_x;               ///< Динамический массив значений порогов.
    QList<UC_data_container> scans_list;
    bool scan_enable;
    UC_data_container * scan;
    bool ff_enable;
    UC_data_container * ff;
    bool df_enable;
    UC_data_container * df;

    UC_pixels_info::UTStr_data_enable data_enable;


    int thl_min;
    int thl_max;

    double threshold_level;

public :
    ///Методы расчета кадра.
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

    typedef enum {
        UTE_IT_no_identification,
        UTE_IT_1,
    } UTE_identification_type;

    ///Методы расчета пикселя.
    typedef enum {
        UTE_PT_cnt0,                        ///< Значение нулевого счетчика при сканировании с образцом.
        UTE_PT_cnt1,                        ///< Значение первого счетчика при сканировании с образцом.
        UTE_PT_cnt0ff,                      ///< Значение нулевого счетчика при сканировании без образца.
        UTE_PT_cnt1ff,                      ///< Значение первого счетчика при сканировании без образца.
        UTE_PT_cnt0df,                      ///< Значение нулевого счетчика при сканировании с выключенным источником.
        UTE_PT_cnt1df,                      ///< Значение первого счетчика при сканировании с выключенным источником.
        UTE_PT_cnt1_divide_on_cnt0,         ///< Отношение значений первого счетчика ко значению нулевого счетчика, взятых при сканировании с образцом.
        UTE_PT_cnt0_subtr_cnt1,             ///< Разность значений нулевого и первого счетчиков, взятых при сканировании с образцом.
        UTE_PT_diff_cnt1,                   ///< Приращение значения первого счетчика при понижении порога в сканировании с образцом.
        UTE_PT_diff_cnt1ff,                 ///< Приращение значения первого счетчика при понижении порога в сканировании без образца.
        UTE_PT_diff_cnt1df,                 ///< Приращение значения первого счетчика при понижении порога в сканировании с выключенным источником.
        UTE_PT_ffc,                         ///< Коррекция плоского поля. Значения берутся из сырых данных.
        UTE_PT_mu,                          ///< Коэффициент поглощения. Значения берутся из сырых данных.
        UTE_PT_diff_ffc,                    ///< Коррекция плоского поля. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_diff_mu,                     ///< Коэффициент поглощения. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_ffc_diff,                    ///< Коррекция плоского поля. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_mu_diff,                     ///< Коэффициент поглощения. Значения берутся равными соответствующим приращениям при понижении порога.

        UTE_PT_cnt1_corr_cnt0,              ///< Значение первого счетчика, скореектированного по среднему значению нулевого счетчика, при сканировании с образцом.
        UTE_PT_cnt1ff_corr_cnt0,            ///< Значение первого счетчика, скореектированного по среднему значению нулевого счетчика, при сканировании без образца.
        UTE_PT_cnt1df_corr_cnt0,            ///< Значение первого счетчика, скореектированного по среднему значению нулевого счетчика, при сканировании с выключенным источником.
        UTE_PT_diff_cnt1_corr_cnt0,         ///< Приращение значения первого счетчика, скореектированного по среднему значению нулевого счетчика, при понижении порога в сканировании с образцом.
        UTE_PT_diff_cnt1ff_corr_cnt0,       ///< Приращение значения первого счетчика, скореектированного по среднему значению нулевого счетчика, при понижении порога в сканировании без образца.
        UTE_PT_diff_cnt1df_corr_cnt0,       ///< Приращение значения первого счетчика, скореектированного по среднему значению нулевого счетчика, при понижении порога в сканировании с выключенным источником.
        UTE_PT_ffc_corr_cnt0,               ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся из сырых данных.
        UTE_PT_mu_corr_cnt0,                ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся из сырых данных.
        UTE_PT_diff_ffc_corr_cnt0,          ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_diff_mu_corr_cnt0,           ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_ffc_diff_corr_cnt0,          ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_mu_diff_corr_cnt0,           ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.

        UTE_PT_mu_max,                      ///< Детектирование максимума коэффициента поглощения. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_ffc_min,                     ///< Детектирование минимума коррекции плоского поля. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_noise_edge,                  ///< Детектирование края шума.
        UTE_PT_ffc_non_xray,                ///< Коррекция плоского поля для нерентгеновских изображений.
        UTE_PT_cnt0_deviation,              ///< Коррекция плоского поля для нерентгеновских изображений.
        UTE_PT_cnt0ff_deviation,
        UTE_PT_cnt0df_deviation,
        UTE_PT_int2,
        UTE_PT_cnt1_act,
        UTE_PT_cnt0_act,
        UTE_PT_cnt1_rejected,
        UTE_PT_diff_cnt1_rejected,
    } UTE_pixel_type;


private :

    UTE_frame_type frame_type;  ///< Текущий метод расчета кадра.
    UTE_pixel_type pixel_type;  ///< Текущий метод расчета пикселей.
    UTE_identification_type identification_type;


    Q_OBJECT
public:

    /*!
     * Конструктор. Создает экземпляр класса и инициализирует его переменные. Подготаливает экземляр класса к использованию.
     * \brief UC_plot Конструктор. Создает экземпляр класса и инициализирует его переменные.
     * \param[in] parent Родительский объект.
     */
    explicit UC_plot(QObject *parent = nullptr);
    /*!
     * Деструктор. Уничтожает экземпляр класса. Освобождает память динамических массивов.
     * \brief ~UC_plot Деструктор. Уничтожает экземпляр класса.
     */
    ~UC_plot();

    /// Функции преобразования координат.

    /*!
     * Преобразование координаты x в определенном чипе и номера чипа в координату детектора.
     * \brief U_get_coord_x_chip Преобразование координаты x в определенном чипе и номера чипа в координату детектора.
     * \param[in] chip Номер чипа.
     * \param[in] x Координта х в чипе.
     * \return Координата х в детекторе.
     */
    int U_get_coord_x_chip(int chip, int x);
    /*!
     * Преобразование координаты x в номер соответствующего чипа.
     * \brief U_get_chip_coord_x Преобразование координаты x в номер соответствующего чипа.
     * \param[in] x Координата х в детекторе.
     * \return Номер чипа.
     */
    int U_get_chip_coord_x(int x);
    /*!
     * Преобразование координаты x в детекторе в координату х в чипе.
     * \brief U_get_coord_x_in_chip Преобразование координаты x в детекторе в координату х в чипе.
     * \param[in] x Координата х в детекторе.
     * \return Координта х в чипе.
     */
    int U_get_coord_x_in_chip(int x);
//
    double U_get_energy_from_thl(int x, int thl);
    double U_get_energy_from_thl_chip(int chip, int thl);
    int U_get_thl_from_energy(int x, double energy);
    int U_get_thl_from_energy_chip(int chip, double energy);
//////////////////////////////////////////////////////////////////////////
    void U_alloc_ff_minus_df_mean();
    void U_make_ff_minus_df_mean();
    double U_get_ff_minus_df_mean(int thl_index, int x);
/////////////////////////////////////////////////////////////////////////////////
    void U_set_mask(int x, int y, bool value);
    bool U_get_mask(int x, int y);
///////////////////////////////////////////////////////////////////////////////
    double U_get_pixel_data(UTE_pixel_type type, int thl, int x, int y);
    int U_get_pixel_data_1(int thl, int x, int y); //UTE_PT_cnt0
    int U_get_pixel_data_2(int thl, int x, int y); //UTE_PT_cnt1
    int U_get_pixel_data_3(int thl, int x, int y); //UTE_PT_cnt0ff
    int U_get_pixel_data_4(int thl, int x, int y); //UTE_PT_cnt1ff
    int U_get_pixel_data_5(int thl, int x, int y); //UTE_PT_cnt0df
    int U_get_pixel_data_6(int thl, int x, int y); //UTE_PT_cnt1df
    double U_get_pixel_data_7(int thl, int x, int y); //UTE_PT_cnt1_divide_on_cnt0
    int U_get_pixel_data_8(int thl, int x, int y); //UTE_PT_cnt0_subtr_cnt1
    double U_get_pixel_data_9(int thl, int x, int y); //UTE_PT_diff_cnt1
    double U_get_pixel_data_10(int thl, int x, int y); //UTE_PT_diff_cnt1ff
    double U_get_pixel_data_11(int thl, int x, int y); //UTE_PT_diff_cnt1df
    double U_get_pixel_data_12(int thl, int x, int y); //UTE_PT_ffc
    double U_get_pixel_data_13(int thl, int x, int y); //UTE_PT_mu
    double U_get_pixel_data_14(int thl, int x, int y); //UTE_PT_diff_ffc
    double U_get_pixel_data_15(int thl, int x, int y); //UTE_PT_diff_mu
    double U_get_pixel_data_16(int thl, int x, int y); //UTE_PT_ffc_diff
    double U_get_pixel_data_17(int thl, int x, int y); //UTE_PT_mu_diff
    ///
    double U_get_pixel_data_18(int thl, int x, int y); //UTE_PT_cnt1_corr_cnt0
    double U_get_pixel_data_19(int thl, int x, int y); //UTE_PT_cnt1ff_corr_cnt0
    double U_get_pixel_data_20(int thl, int x, int y); //UTE_PT_cnt1df_corr_cnt0
    double U_get_pixel_data_21(int thl, int x, int y); //UTE_PT_diff_cnt1_corr_cnt0
    double U_get_pixel_data_22(int thl, int x, int y); //UTE_PT_diff_cnt1ff_corr_cnt0
    double U_get_pixel_data_23(int thl, int x, int y); //UTE_PT_diff_cnt1df_corr_cnt0
    double U_get_pixel_data_24(int thl, int x, int y); //UTE_PT_ffc_corr_cnt0
    double U_get_pixel_data_25(int thl, int x, int y); //UTE_PT_mu_corr_cnt0
    double U_get_pixel_data_26(int thl, int x, int y); //UTE_PT_diff_ffc_corr_cnt0
    double U_get_pixel_data_27(int thl, int x, int y); //UTE_PT_diff_mu_corr_cnt0
    double U_get_pixel_data_28(int thl, int x, int y); //UTE_PT_ffc_diff_corr_cnt0
    double U_get_pixel_data_29(int thl, int x, int y); //UTE_PT_mu_diff_corr_cnt0
    ///
    int U_get_pixel_data_30(int thl, int x, int y); //UTE_PT_mu_max
    int U_get_pixel_data_31(int thl, int x, int y); //UTE_PT_ffc_min
    int U_get_pixel_data_32(int thl, int x, int y); //UTE_PT_noise_edge
    double U_get_pixel_data_33(int thl, int x, int y); //UTE_PT_ffc_non_xray
    double U_get_pixel_data_34(int thl, int x, int y); //UTE_PT_cnt0_deviation
    double U_get_pixel_data_35(int thl, int x, int y); //UTE_PT_cnt0ff_deviation
    double U_get_pixel_data_36(int thl, int x, int y); //UTE_PT_cnt0df_deviation
    ///
    double U_get_pixel_data_37(int thl, int x, int y); //UTE_PT_int2
    //
    int U_get_pixel_data_38(int thl, int x, int y); //UTE_PT_cnt1_act
    int U_get_pixel_data_39(int thl, int x, int y); //UTE_PT_cnt0_act
    int U_get_pixel_data_40(int thl, int x, int y); //UTE_PT_cnt1_rejected
    int U_get_pixel_data_41(int thl, int x, int y); //UTE_PT_diff_cnt1_rejected
////////////////////////////////////////////////////////////////////////////////
    double U_get_frame_data(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, int thl);
    double U_get_frame_data_1(UTE_pixel_type type, int thl); //UTE_FT_average
    double U_get_frame_data_2(UTE_pixel_type type, int thl); //UTE_FT_sum
    double U_get_frame_data_3(UTE_pixel_type type, int thl); //UTE_FT_median
    double U_get_frame_data_4(UTE_pixel_type type, int thl); //UTE_FT_max
    double U_get_frame_data_5(UTE_pixel_type type, int thl); //UTE_FT_min
    int U_get_frame_data_6(UTE_pixel_type type, int thl); //UTE_FT_zeros
    int U_get_frame_data_7(int thl); //UTE_FT_overflowed
    double U_get_frame_data_8(UTE_pixel_type type, int thl); //UTE_FT_standart_deviation
    double U_get_frame_data_9(UTE_pixel_type type, int thl); //UTE_FT_signal_to_noise_resolution
////////////////////////////////////////////////////////////////////////////////
    double U_get_frame_data_energy(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, double e_min, double e_max);
    double U_get_frame_data_energy_1(UTE_pixel_type type_pixel, double e_min, double e_max); //UTE_FT_average
    double U_get_frame_data_energy_2(UTE_pixel_type type_pixel, double e_min, double e_max); //UTE_FT_sum
    double U_get_frame_data_energy_3(UTE_pixel_type type_pixel, double e_min, double e_max); //UTE_FT_median
    double U_get_frame_data_energy_4(UTE_pixel_type type_pixel, double e_min, double e_max); //UTE_FT_max
    double U_get_frame_data_energy_5(UTE_pixel_type type_pixel, double e_min, double e_max); //UTE_FT_min
    int U_get_frame_data_energy_6(UTE_pixel_type type_pixel, double e_min, double e_max); //UTE_FT_zeros
    int U_get_frame_data_energy_7(double e_min, double e_max); //UTE_FT_overflowed
    double U_get_frame_data_energy_8(UTE_pixel_type type_pixel, double e_min, double e_max); //UTE_FT_standart_deviation
    double U_get_frame_data_energy_9(UTE_pixel_type type_pixel, double e_min, double e_max); //UTE_FT_signal_to_noise_resolution
////////////////////////////////////////////////////////////////////////////////////
    double U_get_identification_data(UTE_identification_type type, int thl, int x, int y);
    double U_get_identification_data_1(int x, int y); //UTE_PT_cnt0
    int U_get_identification_data_2(int x, int y); //UTE_PT_cnt1
///////////////////////////////////////////////////////////////////////////////////
    void U_find_thl_range();
    ///
    //Double_t U_fit_function(Double_t *x, Double_t *par);

signals:
    void US_spectra_data(double x, double y);
    void US_frame_data(double x, double y, double z);
    void US_chip_data(UC_pixels_info pixels_info);
    void US_distribution_data(double x, double y);
    void US_range_data(double lower, double upper);
    void US_calibration_chip_data(double x, double y, bool is_fit);
    void US_calibration_data(double x, double y, int chip, bool is_fit);
    void US_identification_data(double x, double y, bool is_fit);
    ///////////////////////////////////////////////////////////////////////////////////

    void US_replot_spectra();
    void US_replot_frame();
    void US_rewrite_table();
    void US_replot_distribution();
    void US_replot_calibration();
    void US_replot_calibration_chip();
    void US_replot_identificaton();
    /////////////////////////////////////////////////////////////////////////////
    void US_new_thl(int thl);
    void US_count_mask(int n);
    void US_thl_range(int thl_min, int thl_max);
    ////////////////////////////////////////////////////////////////////////
    void US_file_found(QString file_name);
    void US_n_files(int n);
    void US_renew_progress_bar(double current, double total);

public slots:
    void U_set_data(UC_data_container::UTStr_data_container_settings settings);
    void U_reset_data();
    void U_delete_scan(int index);
    void U_set_scan(int index);
    void U_set_ff(int index);
    void U_set_df(int index);
//////////////////////////////////////////////////////////////////////////////////
    void U_generate_spectra();
    void U_generate_spectra(double energy_min, double energy_max, int n);
    void U_generate_frame(int thl);
    void U_generate_frame(double energy);
    void U_generate_table(int thl);
    void U_generate_table(double energy);
    void U_generate_table();
    void U_generate_distribution(int n_bins, double min, double max);
    void U_generate_frame_distribution(int n_bins, double min, double max, int thl);
    void U_generate_calibration(int chip);
    void U_generate_calibration();
    void U_generate_identification();
    void U_generate_range(int thl);
    void U_generate_range();
/////////////////////////////////////////////////////////////////
    void U_set_frame_type(UC_plot::UTE_frame_type frame_type);
    void U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type);
    void U_set_identification_type(UC_plot::UTE_identification_type identification_type);
    ///
    void U_set_roi(int x_min, int x_max, int y_min, int y_max);
    void U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    void U_set_threshold_range(int thl_start, int thl_finish);
    void U_set_data_enable(UC_pixels_info::UTStr_data_enable data_enable);
    void U_set_using_calibraion(bool enable);
    void U_set_threshold_level(double level);
    //
    void U_save_calibration(QString file_name);
    void U_load_calibration(QString file_name);
//////////////////////////////////////////////////////////////////////
    void U_set_mask(bool mask, bool more, double value, bool in_roi, int thl);
    void U_count_mask(bool more, double value, bool in_roi, int thl);
    void U_set_mask_overflowed(bool in_roi, int thl);
    void U_set_mask_overflowed(bool in_roi);
    void U_reset_mask();
    void U_mask_selected(int x_min, int x_max, int y_min, int y_max);
    void U_mask_selected_value(double value_min, double value_max, bool in_roi);
    void U_mask_selected_value_thl(double value_min, double value_max, bool in_roi, int thl);
};




#endif // UC_PLOT_H
