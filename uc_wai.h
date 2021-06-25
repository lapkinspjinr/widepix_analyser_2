#ifndef UC_WAI_H
#define UC_WAI_H

#include <QObject>

#include "qcustomplot.h"

#include "uc_plot.h"
#include "common_functions.h"

class UC_wai : public QObject
{
    UC_plot * plot;

    QCustomPlot * qcp, * qcp_2, * qcp_3;
    QCustomPlot * chip_fit_qcp, * calibration_qcp, * id_frame_qcp;
    QCPSelectionRect * qcp_sr_spectra, * qcp_sr_frame, * qcp_sr_distribution;
    QCPSelectionRect * chip_fit_qcp_sr, * calibration_qcp_sr;
    QList<QPen> pens;

    QCPColorMap * qcp_color_map;
    QCPColorScale * qcp_color_scale;
    QCPColorMapData * color_map_data;
    QCPColorGradient * gradient;
    QCPMarginGroup * margin_group;

    QCPColorMap * id_qcp_color_map;
    QCPColorScale * id_qcp_color_scale;
    QCPColorMapData * id_color_map_data;
    QCPMarginGroup * id_margin_group;

    QTableView * table;
    QStandardItemModel * table_model;
    QModelIndex table_index;

    QTableView * id_table;
    QStandardItemModel * id_GA_table_model;
    QStandardItemModel * id_LC_table_model;
    QModelIndex id_table_index;

    QListView * list;
    QStandardItemModel * list_model;
    QModelIndex list_index;

    QThread * thread;

    int x_min, x_max, y_min, y_max;
    bool mask_tick;
    int scan_index, ff_index, df_index, n_scans;
    bool select;
    bool renew_renges;
    bool renew_roi_renges;
    int smoothing;

    double value_min;
    double value_max;

    double thl_start;
    double thl_finish;

    QList<QRect> list_roi;

    QString id_GA_element;

public :
    typedef enum {
        UTE_FT_txt,
        UTE_FT_pdf,
        UTE_FT_jpg,
        UTE_FT_png,
        UTE_FT_bmp,
    } UTE_file_type;

    typedef enum {
        UTE_TFT_tsv,
        UTE_TFT_csv,
    } UTE_table_file_type;

    typedef enum {
        UTE_CS_add,
        UTE_CS_multiplay,
        UTE_CS_subtract,
        UTE_CS_divide,
    } UTE_calculating_spectras;

private :

    Q_OBJECT
public:
    explicit UC_wai(QObject *parent = nullptr);
    ~UC_wai();
    ///////////////////////////////////////////
    void U_start();
    UC_plot* U_get_plot();
////////////////////////////////////////////////////////////////////
    void U_set_qcp(QCustomPlot * qcp, QCustomPlot *qcp_2, QCustomPlot *qcp_3);
    void U_set_colibration_qcp(QCustomPlot * chip_fit_qcp, QCustomPlot * calibration_qcp);
    void U_set_identification_qcp(QCustomPlot *id_frame_qcp);
    void U_set_table(QTableView * table);
    void U_set_id_table(QTableView * id_table);
    void U_set_list(QListView * list);
////////////////////////////////////////////////////////////////
    void U_set_data(UC_data_container::UTStr_data_container_settings settings, QString str);
    void U_reset_data();
    void U_delete_scan(int index);
    void U_set_scan(int index);
    void U_change_scan(int index);
    void U_change_scan_setting(int index, UC_data_container::UTStr_data_container_settings settings);

////////////////////////////////////////////////////////////////////////
    void U_generate_spectra(QString graph_name);
    void U_generate_spectra(QString graph_name, double e_min, double e_max, int n);
    void U_delete_last_graph();
    void U_reset_spectra();
    ///
    void U_generate_frame(int thl);
    void U_generate_frame(double energy);
    void U_generate_table(int thl);
    void U_generate_table(double energy);
    void U_generate_table();
    ///
    void U_generate_distribution(int n_bins, double min, double max, QString graph_name);
    void U_generate_frame_distribution(int n_bins, double min, double max, int thl_index, QString graph_name);
    void U_reset_distribution();
    void U_delete_last_distribution();
    void U_generate_range();
    void U_generate_range(int thl_index);
    //
    void U_generate_calibration_chip(int chip);
    void U_reset_calibration_chip();
    void U_generate_calibration();
    void U_reset_calibration();
    //
//    void U_generate_identification(QString graph_name);
//    void U_reset_identification();
    //

    void U_generate_identification_roi(QString id_GA_element, UC_plot::UTE_identification_type type);
    void U_reset_identification_roi(UC_plot::UTE_identification_type type);
    //
    void U_generate_additional_data();
    void U_generate_identification_data();
    void U_generate_identification_frame(int element_index);
///////////////////////////////////////////////////////////////////////
    void U_set_frame_type(UC_plot::UTE_frame_type frame_type_in);
    void U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in);
    void U_set_identification_type(UC_plot::UTE_identification_type identification_type);
    void U_set_roi(int x_min, int x_max, int y_min, int y_max);
    void U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    void U_set_rebin(int rebin_x, int rebin_y, int rebin_thl);
    void U_set_threshold_range(int thl_min, int thl_max);
    void U_set_data_enable(UC_pixels_info::UTStr_data_enable data_enable);
    void U_set_interaction_mode(bool select);
    void U_set_x_axis_type(bool view);
    void U_set_y_axis_type(bool view);
    void U_set_renew_ranges(bool enable);
    void U_set_renew_roi_ranges(bool enable);
    void U_set_smoothing(int smoothing);
////////////////////////////////////////////////////////////////////////
    void U_resize_spectra();
    void U_normalize_spectra();
    void U_diff_spectra();
    void U_smooth_spectra();
    void U_resize_frame();
    void U_rescale_frame();
    void U_resize_distribution();
    void U_resize_calibration_chip();
    void U_resize_calibration();
    ///////////////////////////////////////////////////////////////////
    void U_save_frame_txt(QString file_name);
    void U_save_frame(QString file_name, UTE_file_type file_type);
    ////
    void U_save_spectra_txt(QString file_name);
    void U_automatic_save_spectra(QString file_name, UTE_file_type file_type);
    void U_save_spectra(QString file_name, UTE_file_type file_type);
    ////
    void U_save_table_tsv(QString file_name);
    void U_save_table_csv(QString file_name);
    void U_save_table(QString file_name, UTE_table_file_type file_type);
    ////
    void U_save_distribution_txt(QString file_name);
    void U_save_distribution(QString file_name, UTE_file_type file_type);
    ////
    void U_load_spectra_txt(QString file_name);
    void U_load_frame_txt(QString file_name);
    void U_load_distribution_txt(QString file_name);
    //
    void U_save_calibration_chip(QString file_name, UTE_file_type file_type);
    void U_save_calibration_chip_txt(QString file_name);
    //
    void U_save_calibration(QString file_name, UTE_file_type file_type);
    void U_save_calibration_txt(QString file_name);
////////////////////////////////////////////////////////////////////////
    void U_set_mask(bool mask, bool more, double value, bool in_roi, int thl);
    void U_count_mask(bool more, double value, bool in_roi, int thl);
    void U_set_mask_plots(double value);
    void U_delete_mask_plots();
    void U_set_mask_overflowed(bool in_roi, int thl);
    void U_set_mask_overflowed(bool in_roi);
    void U_reset_mask();
    void U_mask_selected(int x_min, int x_max, int y_min, int y_max);
    void U_mask_selected_value_thl(bool in_roi, int thl);
    void U_mask_selected_value(bool in_roi);
    //
    void U_add_roi(int x_min, int x_max, int y_min, int y_max);
    QRect U_get_roi(int index);
    void U_delete_roi(int index);
    //
    void U_calculating_spectra(UTE_calculating_spectras calc_type, int index1, int index2);
    double U_calculating_spectra_1(double data1, double data2); //UTE_CS_add
    double U_calculating_spectra_2(double data1, double data2); //UTE_CS_multiplay
    double U_calculating_spectra_3(double data1, double data2); //UTE_CS_subtract
    double U_calculating_spectra_4(double data1, double data2); //UTE_CS_divide

signals:
    void US_set_data(UC_data_container::UTStr_data_container_settings settings);
    void US_reset_data();
    void US_delete_scan(int index);
    void US_set_scan(int index);
    void US_get_settings(int index);
    void US_set_settings(int index, UC_data_container::UTStr_data_container_settings settings);
    void US_renew_scan_settings(UC_data_container::UTStr_data_container_settings settings);
///////////////////////////////////////////////////////////////////////////
    void US_generate_spectra();
    void US_generate_spectra(double e_min, double e_max, int n);
    void US_generate_frame(int thl);
    void US_generate_frame(double energy);
    void US_generate_table(int thl);
    void US_generate_table(double energy);
    void US_generate_table();
    void US_generate_distribution(int n_bins, double min, double max);
    void US_generate_frame_distribution(int n_bins, double min, double max, int thl_index);
    void US_generate_range();
    void US_generate_range(int thl_index);
    void US_generate_calibration(int chip);
    void US_generate_calibration();
    void US_generate_identification_roi();
    void US_generate_additional_data();
    void US_generate_identification_data();
    void US_generate_identification_frame(int thl);
//    void US_generate_identification(int x, int y);
//
////////////////////////////////////////////////////////////////////////////
    void US_set_frame_type(UC_plot::UTE_frame_type frame_type_in);
    void US_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in);
    void US_set_roi(int x_min, int x_max, int y_min, int y_max);
    void US_set_rebin(int rebin_x, int rebin_y, int rebin_thl);
    void US_set_identification_type(UC_plot::UTE_identification_type identification_type);
    void US_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    void US_set_threshold_range(int thl_min, int thl_max);
    void US_set_data_enable(UC_pixels_info::UTStr_data_enable data_enable);
    void US_set_smoothing(int smoothing);
    /////////////////////////////////////////////////////////////////
    void US_set_spectra_max_x(double value);
    void US_set_spectra_min_x(double value);
    void US_set_spectra_max_y(double value);
    void US_set_spectra_min_y(double value);
    ///////////////////////////////////////////////////////////////////
    void US_set_frame_max_x(double value);
    void US_set_frame_min_x(double value);
    void US_set_frame_max_y(double value);
    void US_set_frame_min_y(double value);
    void US_set_frame_max_z(double value);
    void US_set_frame_min_z(double value);
    /////////////////////////////////////////////////////////////////////////
    void US_set_distribution_max_x(double value);
    void US_set_distribution_min_x(double value);
    void US_set_distribution_max_y(double value);
    void US_set_distribution_min_y(double value);
    /////////////////////////////////////////////////////////////////////////
    void US_set_mask(bool mask, bool more, double value, bool in_roi, int thl);
    void US_count_mask(bool more, double value, bool in_roi, int thl);
    void US_set_mask_overflowed(bool in_roi, int thl);
    void US_set_mask_overflowed(bool in_roi);
    void US_reset_mask();
    void US_mask_selected(int x_min, int x_max, int y_min, int y_max);
    void US_mask_selected_value_thl(double value_min, double value_max,  bool in_roi, int thl);
    void US_mask_selected_value(double value_min, double value_max,  bool in_roi);
    /////////////////////////////////////////////////////////////////////////
    void US_mouse_data(QString);
    ////////////////////////////////////////////////////////
    void US_new_thl(int thl);
    ////////////////////////////////////////////////////////
    void US_set_distribution_range(double lower, double upper);
    //
    void US_set_thl_range(double lower, double upper);
    ////////////////////////////////////////////////////////
    void US_set_roi_range(int x_min, int x_max, int y_min, int y_max);
    void US_generate_muxes(int rebin_xy, int rebin_thl);
    //
    void US_new_spectra(QString name);


public slots:
    void U_add_spectra_data(double x, double y);
    void U_add_frame_data(double x, double y, double z);
    void U_add_table_data(UC_pixels_info pixels_info);
    void U_add_distibution_data(double x, double y);
    void U_add_calibration_chip_data(double x, double y, bool is_fit);
    void U_add_calibration_data(double x, double y, int chip, bool is_fit);
    void U_add_id_roi_GA_data(UC_plot::UTStr_id_GA_data data);
    void U_add_id_roi_LC_data(UC_plot::UTStr_id_LC_data data);
    void U_add_identification_frame_data(double x, double y, double z);

//    void U_add_identification_data(double x, double y, bool is_fit);
//
//////////////////////////////////////////////////////////////////////
    void U_replot_spectra(QVector<double> x, QVector<double> y);
    void U_replot_frame();
    void U_rewrite_table();
    void U_replot_distribution(QVector<double> x, QVector<double> y);
    void U_replot_calibration_chip(QVector<double> x, QVector<double> y, bool fit);
    void U_replot_calibration(QVector<double> x, QVector<double> y, int chip, bool fit);
    void U_replot_identification_frame();
    /////////////////////////////////////////////////
    void U_set_spectra_x_axis_range(QCPRange range);
    void U_set_spectra_y_axis_range(QCPRange range);
    ///
    void U_set_spectra_max_x(double value);
    void U_set_spectra_min_x(double value);
    void U_set_spectra_max_y(double value);
    void U_set_spectra_min_y(double value);
    /////////////////////////////////////////////////
    void U_set_frame_x_axis_range(QCPRange range);
    void U_set_frame_y_axis_range(QCPRange range);
    void U_set_frame_z_axis_range(QCPRange range);
    ////
    void U_set_frame_max_x(double value);
    void U_set_frame_min_x(double value);
    void U_set_frame_max_y(double value);
    void U_set_frame_min_y(double value);
    void U_set_frame_max_z(double value);
    void U_set_frame_min_z(double value);
    //////////////////////////////////////////////////////////
    void U_set_distribution_x_axis_range(QCPRange range);
    void U_set_distribution_y_axis_range(QCPRange range);
    ///
    void U_set_distribution_max_x(double value);
    void U_set_distribution_min_x(double value);
    void U_set_distribution_max_y(double value);
    void U_set_distribution_min_y(double value);
///////////////////////////////////////////////////////////////////////
    void U_mouse_move_spectra(QMouseEvent * event);
    void U_mouse_move_frame(QMouseEvent * event);
    void U_mouse_move_distribution(QMouseEvent * event);
    //
    void U_mouse_select_spectra(QMouseEvent * event);
    void U_mouse_select_frame(QMouseEvent * event);
    void U_mouse_select_distribution(QMouseEvent * event);
    //////////////////////////////////////////////////////////////////////
    void U_new_thl(int thl);
    ///
    void U_toggle_calibration();
    //
    void U_renew_list_model(UC_data_container::UTStr_data_container_settings settings);


};

#endif // UC_WAI_H
