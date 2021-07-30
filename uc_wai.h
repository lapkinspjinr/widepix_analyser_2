#ifndef UC_WAI_H
#define UC_WAI_H

#include <QObject>

#include <TFile.h>
#include <TH2D.h>

#include "qcustomplot.h"

#include "uc_plot.h"
#include "common_functions.h"
#include "uc_pixels_info.h"

class UC_wai : public QObject
{
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int thl_start;
    int thl_finish;

    int smoothing;

    bool select;
    bool legend_enable;
    bool renew_renges;
    bool renew_roi_renges;
    bool mask_tick;

    double value_min;
    double value_max;

    QString id_GA_element;

    UC_plot * plot;
    QThread * thread;

    QList<QPen> pens;
    QList<QCPScatterStyle::ScatterShape> shapes;
    QList<UC_roi> list_roi;
    QCPColorGradient * gradient;

    QCustomPlot *       spectra_qcp;
    QCPSelectionRect *  sr_spectra_qcp;

    QCustomPlot *       frame_qcp;
    QCPColorMap *       color_map_frame_qcp;
    QCPColorScale *     color_scale_frame_qcp;
    QCPColorMapData *   color_map_data_frame_qcp;
    QCPMarginGroup *    margin_group_frame_qcp;
    QCPSelectionRect *  sr_frame_qcp;

    QCustomPlot *       distribution_qcp;
    QCPSelectionRect *  sr_distribution_qcp;

    QCustomPlot *       chip_fit_qcp;
    QCPSelectionRect *  sr_chip_fit_qcp;

    QCustomPlot *       calibration_qcp;
    QCPSelectionRect *  sr_calibration_qcp;

    QCustomPlot *       id_frame_qcp;
    QCPColorMap *       color_map_id_frame_qcp;
    QCPColorScale *     color_scale_id_frame_qcp;
    QCPColorMapData *   color_map_data_id_frame_qcp;
    QCPMarginGroup *    margin_group_id_frame_qcp;
    QCPSelectionRect *  sr_id_frame_qcp;

    QTableView *            table;
    QStandardItemModel *    table_model;
    QModelIndex             table_index;

    QCustomPlot *       spectra_2d_qcp;
    QCPColorMap *       color_map_spectra_2d_qcp;
    QCPColorScale *     color_scale_spectra_2d_qcp;
    QCPColorMapData *   color_map_data_spectra_2d_qcp;
    QCPMarginGroup *    margin_group_spectra_2d_qcp;
    QCPSelectionRect *  sr_spectra_2d_qcp;

    QTableView *            id_table;
    QStandardItemModel *    id_GA_table_model;
    QStandardItemModel *    id_LC_table_model;
    QModelIndex             id_table_index;

    QListView *             list;
    QStandardItemModel *    list_model;
    QModelIndex             list_index;

public :
    typedef enum {
        UTE_FT_txt,
        UTE_FT_pdf,
        UTE_FT_jpg,
        UTE_FT_png,
        UTE_FT_bmp,
        UTE_FT_root
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
        UTE_CS_smooth,
        UTE_CS_diff,
    } UTE_calculating_spectras;

    typedef struct {
        int bins_x;
        int bins_y;
        double x_max;
        double x_min;
        double y_min;
        double y_max;
    } UStr_spectra_2d_settings;

private :

    UStr_spectra_2d_settings spectra_2d_settings;

    Q_OBJECT
public:
    explicit UC_wai(QObject *parent = nullptr);
    ~UC_wai();
    ///////////////////////////////////////////
    void U_start();
    UC_plot* U_get_plot();
////////////////////////////////////////////////////////////////////
    void U_set_spectra_qcp(QCustomPlot * frame_qcp);
    void U_set_frame_qcp(QCustomPlot * spectra_qcp);
    void U_set_distribution_qcp(QCustomPlot * distribution_qcp);
    void U_set_chip_fit_qcp(QCustomPlot * chip_fit_qcp);
    void U_set_calibration_qcp(QCustomPlot * calibration_qcp);
    void U_set_spectra_2d_qcp(QCustomPlot * spectra_2d_qcp);
    void U_set_id_frame_qcp(QCustomPlot *id_frame_qcp);
    void U_set_table(QTableView * table);
    void U_set_id_table(QTableView * id_table);
    void U_set_list(QListView * list);
////////////////////////////////////////////////////////////////
    void U_reset_data();
////////////////////////////////////////////////////////////////////////
    void U_generate_spectra(QString graph_name);
    void U_generate_spectra(QString graph_name, int n);
    void U_delete_last_spectra();
    void U_reset_spectra();
    ///
    void U_generate_frame(int thl);
    void U_generate_frame(double energy);
    //
    void U_generate_table(int thl);
    void U_generate_table(double energy);
    void U_generate_table();
    ///
    void U_generate_distribution(int n_bins, double min, double max, QString graph_name);
    void U_generate_distribution(int n_bins, double min, double max, int thl_index, QString graph_name);
    void U_reset_distribution();
    void U_delete_last_distribution();
    void U_generate_range();
    void U_generate_range(int thl_index);
    //
    void U_generate_chip_fit(int chip, QString graph_name);
    void U_reset_chip_fit();
    //
    void U_generate_calibration();
    void U_reset_calibration();
    //
    void U_generate_spectra_2d(UStr_spectra_2d_settings settings);
    void U_generate_spectra_2d_energy(UStr_spectra_2d_settings settings);
    void U_generate_spectra_2d_range(int thl_min, int thl_max);
    void U_generate_spectra_2d_range_energy(double energy_min, double energy_max);
    //
    void U_generate_id_roi(QString id_GA_element, UC_plot::UTE_id_type type);
    void U_reset_id_roi(UC_plot::UTE_id_type type);
    //
    void U_generate_additional_data();
    void U_generate_id_data();
    void U_generate_id_frame(int element_index);
///////////////////////////////////////////////////////////////////////
    void U_set_frame_type(UC_plot::UTE_frame_type frame_type_in);
    void U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in);
    void U_set_id_type(UC_plot::UTE_id_type id_type);
    //
    void U_set_roi(int x_min, int x_max, int y_min, int y_max);
    //
    void U_set_threshold_range(int thl_min, int thl_max);
    //
    void U_set_rebin(int rebin_x, int rebin_y, int rebin_thl);
    void U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    //

    void U_set_interaction_mode(bool select);
    void U_set_x_axis_type(bool view);
    void U_set_y_axis_type(bool view);
    void U_set_z_axis_type(bool view);
    void U_set_spectra_2d_z_axis_type(bool view);
    void U_set_legend_enable(bool enable);
    //
    void U_set_renew_ranges(bool enable);
    void U_set_renew_roi_ranges(bool enable);
    //
    void U_set_smoothing(int smoothing);
////////////////////////////////////////////////////////////////////////
    void U_resize_spectra();
    void U_normalize_spectra();
    //
    void U_resize_frame();
    void U_rescale_frame();
    //
    void U_resize_distribution();
    //
    void U_resize_chip_fit();
    void U_resize_calibration();
    //
    void U_resize_spectra_2d();
    void U_rescale_spectra_2d();
    ///////////////////////////////////////////////////////////////////
    void U_save_spectra_txt(QString file_name);
    void U_save_spectra_root(QString file_name);
    void U_save_spectra(QString file_name, UTE_file_type file_type);
    void U_automatic_save_spectra(QString file_name, QString name, UTE_file_type file_type);
    //
    void U_save_frame_txt(QString file_name);
    void U_save_frame_root(QString file_name);
    void U_save_frame(QString file_name, UTE_file_type file_type);
    //
    void U_save_table_tsv(QString file_name);
    void U_save_table_csv(QString file_name);
    void U_save_table(QString file_name, UTE_table_file_type file_type);
    //
    void U_save_distribution_txt(QString file_name);
    void U_save_distribution_root(QString file_name);
    void U_save_distribution(QString file_name, UTE_file_type file_type);
    void U_automatic_save_distribution(QString file_name, QString name, UTE_file_type file_type, int n_bins, double min, double max, int thl);
    void U_automatic_save_distribution(QString file_name, QString name, UTE_file_type file_type, int n_bins, double min, double max);
    //
    void U_save_chip_fit_txt(QString file_name);
    void U_save_chip_fit_root(QString file_name);
    void U_save_chip_fit(QString file_name, UTE_file_type file_type);
    //
    void U_save_calibration_txt(QString file_name);
    void U_save_calibration_root(QString file_name);
    void U_save_calibration(QString file_name, UTE_file_type file_type);
    //
    void U_save_spectra_2d_txt(QString file_name);
    void U_save_spectra_2d_root(QString file_name);
    void U_save_spectra_2d(QString file_name, UTE_file_type file_type);
    void U_automatic_save_spectra_2d(QString file_name, UTE_file_type file_type, UStr_spectra_2d_settings settings);
    //
    void U_save_id_frame_txt(QString file_name);
    void U_save_id_frame(QString file_name, UTE_file_type file_type);
    ////
    void U_load_spectra_txt(QString file_name);
    void U_load_frame_txt(QString file_name);
    void U_load_distribution_txt(QString file_name);
    void U_load_calibr_chip_txt(QString file_name);
    void U_load_calibr_txt(QString file_name);
    void U_load_spectra_2d_txt(QString file_name);
    void U_load_id_frame_txt(QString file_name);
    void U_load_table_tsv(QString file_name);
    void U_load_table_csv(QString file_name);
    void U_load_table(QString file_name);
////////////////////////////////////////////////////////////////////////
    void U_set_mask(bool mask, bool more, double value, bool in_roi, int thl);
    void U_count_mask(bool more, double value, bool in_roi, int thl);
    void U_set_mask_plots(double value);
    void U_delete_mask_plots();
    void U_set_mask_overflowed(bool in_roi, int thl);
    void U_set_mask_overflowed(bool in_roi);
    void U_reset_mask();
    void U_mask_selected(int x_min, int x_max, int y_min, int y_max);
    void U_mask_selected_value(bool in_roi, int thl);
    void U_mask_selected_value(bool in_roi);
    void U_save_mask(QString file_name);
    void U_load_mask(QString file_name);
    //
    void U_add_roi(QString name, int x_min, int x_max, int y_min, int y_max);
    UC_roi U_get_roi(int index);
    void U_delete_roi(int index);
    void U_save_roi(QString file_name);
    void U_load_roi(QString file_name);
    QString U_get_name_roi(int x_min, int x_max, int y_min, int y_max);
    //
    void U_calculating_spectra(UTE_calculating_spectras calc_type, int index1, int index2);
    void U_calculating_spectra_1(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2); //UTE_CS_add
    void U_calculating_spectra_2(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2); //UTE_CS_multiplay
    void U_calculating_spectra_3(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2); //UTE_CS_subtract
    void U_calculating_spectra_4(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2); //UTE_CS_divide
    void U_calculating_spectra_5(QVector<double> &x1, QVector<double> &y1); //UTE_CS_smooth
    void U_calculating_spectra_6(QVector<double> &x1, QVector<double> &y1); //UTE_CS_diff
    QString U_calculating_name_1(QString name1, QString name2); //UTE_CS_add
    QString U_calculating_name_2(QString name1, QString name2); //UTE_CS_multiplay
    QString U_calculating_name_3(QString name1, QString name2); //UTE_CS_subtract
    QString U_calculating_name_4(QString name1, QString name2); //UTE_CS_divide
    QString U_calculating_name_5(QString name1); //UTE_CS_smooth
    QString U_calculating_name_6(QString name1); //UTE_CS_diff
    //
    void U_set_gradient(int n); //UTE_CS_diff
    void U_thick_up();
    void U_thick_down();

signals:
    void US_set_data(UC_data_container::UTStr_data_container_settings * settings);
    void US_reset_data();
    void US_delete_scan(int index);
    void US_set_scan(int index);
    void US_get_settings(int index);
    void US_set_settings(int index, UC_data_container::UTStr_data_container_settings settings);
    void US_renew_scan_settings(UC_data_container::UTStr_data_container_settings settings);
///////////////////////////////////////////////////////////////////////////
    void US_generate_spectra();
    void US_generate_spectra_direct();
    void US_generate_spectra(int n);
    void US_generate_frame(int thl);
    void US_generate_frame(double energy);
    void US_generate_table(int thl);
    void US_generate_table(double energy);
    void US_generate_table();
    void US_generate_distribution_direct(int n_bins, double min, double max);
    void US_generate_distribution(int n_bins, double min, double max);
    void US_generate_distribution_direct(int n_bins, double min, double max, int thl);
    void US_generate_distribution(int n_bins, double min, double max, int thl);
    void US_generate_range();
    void US_generate_range(int thl_index);
    void US_generate_chip_fit(int chip);
    void US_generate_calibration();
    void US_generate_spectra_2d(int thl_min, int thl_max);
    void US_generate_spectra_2d_direct(int thl_min, int thl_max);
    void US_generate_spectra_2d(double energy_min, double energy_max);
    void US_generate_spectra_2d_range(int thl_min, int thl_max);
    void US_generate_spectra_2d_range(double energy_min, double energy_max);
    void US_generate_id_roi();
    void US_generate_additional_data();
    void US_generate_id_data();
    void US_generate_id_frame(int thl);
//
////////////////////////////////////////////////////////////////////////////
    void US_set_frame_type(UC_plot::UTE_frame_type frame_type_in);
    void US_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in);
    void US_set_roi(int x_min, int x_max, int y_min, int y_max);
    void US_set_rebin(int rebin_x, int rebin_y, int rebin_thl);
    void US_set_id_type(UC_plot::UTE_id_type id_type);
    void US_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    //void US_set_threshold_range(int thl_min, int thl_max);
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
    //
    void US_set_chip_fit_max_x(double value);
    void US_set_chip_fit_min_x(double value);
    void US_set_chip_fit_max_y(double value);
    void US_set_chip_fit_min_y(double value);
    //
    void US_set_calibration_max_x(double value);
    void US_set_calibration_min_x(double value);
    void US_set_calibration_max_y(double value);
    void US_set_calibration_min_y(double value);
    //
    void US_set_spectra_2d_max_x(double value);
    void US_set_spectra_2d_min_x(double value);
    void US_set_spectra_2d_max_y(double value);
    void US_set_spectra_2d_min_y(double value);
    void US_set_spectra_2d_max_z(double value);
    void US_set_spectra_2d_min_z(double value);
    /////////////////////////////////////////////////////////////////////////
    void US_set_mask(bool mask, bool more, double value, bool in_roi, int thl);
    void US_count_mask(bool more, double value, bool in_roi, int thl);
    void US_set_mask_overflowed(bool in_roi, int thl);
    void US_set_mask_overflowed(bool in_roi);
    void US_reset_mask();
    void US_mask_selected(int x_min, int x_max, int y_min, int y_max);
    void US_mask_selected_value(double value_min, double value_max,  bool in_roi, int thl);
    void US_mask_selected_value(double value_min, double value_max,  bool in_roi);
    void US_save_mask(QString file_name);
    void US_load_mask(QString file_name);
    /////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    void US_set_distribution_range(double lower, double upper);
    void US_set_spectra_2d_range(double lower, double upper);
    void US_set_thl_range(int lower, int upper);
    void US_mouse_data(QString);
    void US_set_roi_range(int x_min, int x_max, int y_min, int y_max);
    ////////////////////////////////////////////////////////
    //
    void US_new_spectra(QString name);
    //
    void US_add_roi(UC_roi);
    void US_add_roi(QString);
    void US_delete_roi(int index);
    void US_get_roi(int index);



public slots:
    void U_add_spectra_data(double x, double y);
    void U_add_frame_data(double x, double y, double z);
    void U_add_table_data(UC_pixels_info pixels_info);
    void U_add_id_roi_GA_data(UC_plot::UTStr_id_GA_data data);
    void U_add_id_roi_LC_data(UC_plot::UTStr_id_LC_data data);
    void U_add_id_frame_data(double x, double y, double z);
    void U_add_spectra_2d_data(double x, double y);
//
//////////////////////////////////////////////////////////////////////
    void U_replot_spectra(QVector<double> x, QVector<double> y);
    void U_replot_spectra();
    void U_replot_frame();
    void U_rewrite_table();
    void U_replot_distribution(QVector<double> x, QVector<double> y);
    void U_replot_chip_fit(QVector<double> x, QVector<double> y, bool fit);
    void U_replot_calibration(QVector<double> x, QVector<double> y, int chip, bool fit);
    void U_replot_id_frame();
    void U_replot_spectra_2d();
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
    //
    void U_set_chip_fit_x_axis_range(QCPRange range);
    void U_set_chip_fit_y_axis_range(QCPRange range);
    ///
    void U_set_chip_fit_max_x(double value);
    void U_set_chip_fit_min_x(double value);
    void U_set_chip_fit_max_y(double value);
    void U_set_chip_fit_min_y(double value);
    //
    void U_set_calibration_x_axis_range(QCPRange range);
    void U_set_calibration_y_axis_range(QCPRange range);
    ///
    void U_set_calibration_max_x(double value);
    void U_set_calibration_min_x(double value);
    void U_set_calibration_max_y(double value);
    void U_set_calibration_min_y(double value);
    //
    void U_set_spectra_2d_x_axis_range(QCPRange range);
    void U_set_spectra_2d_y_axis_range(QCPRange range);
    void U_set_spectra_2d_z_axis_range(QCPRange range);
    ////
    void U_set_spectra_2d_max_x(double value);
    void U_set_spectra_2d_min_x(double value);
    void U_set_spectra_2d_max_y(double value);
    void U_set_spectra_2d_min_y(double value);
    void U_set_spectra_2d_max_z(double value);
    void U_set_spectra_2d_min_z(double value);
///////////////////////////////////////////////////////////////////////
    void U_mouse_move_spectra(QMouseEvent * event);
    void U_mouse_move_frame(QMouseEvent * event);
    void U_mouse_move_distribution(QMouseEvent * event);
    void U_mouse_move_chip_fit(QMouseEvent * event);
    void U_mouse_move_calibration(QMouseEvent * event);
    void U_mouse_move_spectra_2d(QMouseEvent * event);
//    void U_mouse_move_id_frame(QMouseEvent * event);
    //
    void U_mouse_select_spectra(QMouseEvent * event);
    void U_mouse_select_frame(QMouseEvent * event);
    void U_mouse_select_distribution(QMouseEvent * event);
    void U_mouse_select_chip_fit(QMouseEvent * event);
    void U_mouse_select_calibration(QMouseEvent * event);
    void U_mouse_select_spectra_2d(QMouseEvent * event);
    //////////////////////////////////////////////////////////////////////
    void U_renew_scans(QList<UC_data_container> * list_scans_ptr, int active_index);
};

#endif // UC_WAI_H
