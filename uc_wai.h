#ifndef UC_WAI_H
#define UC_WAI_H

#include <QObject>

#include "qcustomplot.h"

#include "uc_plot.h"

class UC_wai : public QObject
{
    UC_plot * plot;

    QCustomPlot * qcp, * qcp_2, * qcp_3, * qcp_4;
    QList<QPen> pens;

    QCPColorMap * qcp_color_map;
    QCPColorScale * qcp_color_scale;
    QCPColorMapData * color_map_data;
    QCPColorGradient * gradient;
    QCPMarginGroup * margin_group;

    QTableView * table;
    QStandardItemModel * model;
    QModelIndex index;

    QThread * thread;

    int x_min, x_max, y_min, y_max;
    bool mask_tick;

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
    void U_set_table(QTableView * table);
////////////////////////////////////////////////////////////////
    void U_set_data(QString path, int count, double time, bool both_counters);
    void U_set_data_df(QString path, int count, double time, bool both_counters);
    void U_set_data_ff(QString path, int count, double time, bool both_counters);
    void U_reset_data();
////////////////////////////////////////////////////////////////////////
    void U_generate_plot(QString graph_name);
    void U_delete_last_graph();
    void U_reset_plot();
    ///
    void U_generate_frame(int thl);
    void U_generate_table(int thl);
    void U_generate_table();
    ///
    void U_generate_distribution(int n_bins, double min, double max, QString graph_name);
    void U_generate_frame_distribution(int n_bins, double min, double max, int thl_index, QString graph_name);
    void U_reset_distribution();
    void U_delete_last_distribution();
    void U_generate_range();
    void U_generate_range(int thl_index);
///////////////////////////////////////////////////////////////////////
    void U_set_frame_type(UC_plot::UTE_frame_type frame_type_in);
    void U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in);
    void U_set_roi(int x_min, int x_max, int y_min, int y_max);
    void U_set_identification_type(UC_plot::UTE_identification_type identification_type);
    void U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    void U_set_threshold_range(int thl_min, int thl_max);
////////////////////////////////////////////////////////////////////////
    void U_resize_spectra();
    void U_normalize_spectra();
    void U_resize_frame();
    void U_rescale_frame();
    void U_resize_distribution();
    ///////////////////////////////////////////////////////////////////
    void U_save_frame_txt(QString file_name);
    void U_save_frame(QString file_name, UTE_file_type file_type);
    ////
    void U_save_spectra_txt(QString file_name);
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
////////////////////////////////////////////////////////////////////////
    void U_set_mask(bool mask, bool more, double value, bool in_roi, int thl_index);
    void U_count_mask(bool more, double value, bool in_roi, int thl_index);
    void U_set_mask_plots(double value);
    void U_delete_mask_plots();
    void U_set_mask_overflowed(bool in_roi, int thl_index);
    void U_set_mask_overflowed(bool in_roi);
    void U_reset_mask();

signals:
    void US_set_data(QString path, int count, double time, bool both_counters);
    void US_set_data_df(QString path, int count, double time, bool both_counters);
    void US_set_data_ff(QString path, int count, double time, bool both_counters);
    void US_reset_data();
///////////////////////////////////////////////////////////////////////////
    void US_generate_plot();
    void US_generate_frame(int thl);
    void US_generate_table(int thl);
    void US_generate_table();
    void US_generate_distribution(int n_bins, double min, double max);
    void US_generate_frame_distribution(int n_bins, double min, double max, int thl_index);
    //void US_generate_calibration();
    void US_generate_range();
    void US_generate_range(int thl_index);
////////////////////////////////////////////////////////////////////////////
    void US_set_frame_type(UC_plot::UTE_frame_type frame_type_in);
    void US_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in);
    void US_set_roi(int x_min, int x_max, int y_min, int y_max);
    void US_set_identification_type(UC_plot::UTE_identification_type identification_type);
    void US_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    void US_set_threshold_range(int thl_min, int thl_max);
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
    void US_set_mask(bool mask, bool more, double value, bool in_roi, int thl_index);
    void US_count_mask(bool more, double value, bool in_roi, int thl_index);
    void US_set_mask_overflowed(bool in_roi, int thl_index);
    void US_set_mask_overflowed(bool in_roi);
    void US_reset_mask();
    /////////////////////////////////////////////////////////////////////////
    void US_mouse_data(QString);
    ////////////////////////////////////////////////////////
    void US_new_thl(int thl);
    ////////////////////////////////////////////////////////
    void US_set_distribution_range(double lower, double upper);


public slots:
    void U_add_spectra_data(double x, double y);
    void U_add_frame_data(double x, double y, double z);
    void U_add_table_data(UC_pixels_info pixels_info);
    void U_add_distibution_data(double x, double y);
//    void U_add_calibration_data(double x, double y);

//////////////////////////////////////////////////////////////////////
    void U_replot_spectra();
    void U_replot_frame();
    void U_rewrite_table();
    void U_replot_distribution();
//    void U_replot_calibration();
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
    //////////////////////////////////////////////////////////////////////
    void U_new_thl(int thl);

};

#endif // UC_WAI_H
