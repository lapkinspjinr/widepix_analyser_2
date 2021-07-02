#include "uc_wai.h"

UC_wai::UC_wai(QObject *parent) : QObject(parent)
{
    x_min = 0;
    x_max = 15 * 256;
    y_min = 0;
    y_max = 256;
    thl_start = 0;
    thl_finish = 512;

    n_scans = 0;
    scan_index = -1;
    ff_index = -1;
    df_index = -1;

    smoothing = 1;

    select = false;
    legend_enable = true;
    renew_renges = true;
    renew_roi_renges = true;
    mask_tick = false;

    plot = new UC_plot();
    thread = new QThread();

    pens << QPen(Qt::black);
    pens << QPen(Qt::red);
    pens << QPen(Qt::darkRed);
    pens << QPen(Qt::green);
    pens << QPen(Qt::darkGreen);
    pens << QPen(Qt::blue);
    pens << QPen(Qt::darkBlue);
    pens << QPen(Qt::cyan);
    pens << QPen(Qt::darkCyan);
    pens << QPen(Qt::magenta);
    pens << QPen(Qt::darkMagenta);
    pens << QPen(Qt::yellow);
    pens << QPen(Qt::darkYellow);
    pens << QPen(Qt::gray);
    pens << QPen(Qt::darkGray);
    pens << QPen(Qt::lightGray);

    shapes << QCPScatterStyle::ssCross;
    shapes << QCPScatterStyle::ssPlus;
    shapes << QCPScatterStyle::ssCircle;
    shapes << QCPScatterStyle::ssDisc;
    shapes << QCPScatterStyle::ssSquare;
    shapes << QCPScatterStyle::ssDiamond;
    shapes << QCPScatterStyle::ssStar;
    shapes << QCPScatterStyle::ssTriangle;
    shapes << QCPScatterStyle::ssTriangleInverted;
    shapes << QCPScatterStyle::ssCrossSquare;
    shapes << QCPScatterStyle::ssPlusSquare;
    shapes << QCPScatterStyle::ssCrossCircle;
    shapes << QCPScatterStyle::ssPlusCircle;
    shapes << QCPScatterStyle::ssPeace;
    shapes << QCPScatterStyle::ssCustom;

    list_roi << UC_roi("widepix", 0, 0, 3840, 256);
    QString str = "chip %1";
    for (int i = 0; i < 15; i++) {
        list_roi << UC_roi(str.arg(i + 1), 256 * i, 0, 256 * (i + 1), 256);
    }

    plot->moveToThread(thread);

    connect(this, SIGNAL(US_set_data(UC_data_container::UTStr_data_container_settings)),            plot, SLOT(U_set_data(UC_data_container::UTStr_data_container_settings)),           Qt::DirectConnection);
    connect(this, SIGNAL(US_reset_data()),                                                          plot, SLOT(U_reset_data()),                                                         Qt::DirectConnection);
    connect(this, SIGNAL(US_delete_scan(int)),                                                      plot, SLOT(U_delete_scan(int)),                                                     Qt::DirectConnection);
    connect(this, SIGNAL(US_set_scan(int)),                                                         plot, SLOT(U_set_scan(int)),                                                        Qt::DirectConnection);
    connect(this, SIGNAL(US_get_settings(int)),                                                     plot, SLOT(U_get_settings(int)),                                                    Qt::DirectConnection);
    connect(this, SIGNAL(US_set_settings(int, UC_data_container::UTStr_data_container_settings)),   plot, SLOT(U_set_settings(int, UC_data_container::UTStr_data_container_settings)),  Qt::DirectConnection);

    connect(this, SIGNAL(US_generate_spectra()),                                    plot, SLOT(U_generate_spectra()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_spectra(int)),                                 plot, SLOT(U_generate_spectra(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_frame(int)),                                   plot, SLOT(U_generate_frame(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_frame(double)),                                plot, SLOT(U_generate_frame(double)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_table(int)),                                   plot, SLOT(U_generate_table(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_table(double)),                                plot, SLOT(U_generate_table(double)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_table()),                                      plot, SLOT(U_generate_table()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_distribution(int, double, double)),            plot, SLOT(U_generate_distribution(int, double, double)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_distribution(int, double, double, int)),       plot, SLOT(U_generate_frame_distribution(int, double, double, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_chip_fit(int)),                                plot, SLOT(U_generate_calibration(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_calibration()),                                plot, SLOT(U_generate_calibration()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_id_roi()),                                     plot, SLOT(U_generate_id_roi()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_additional_data()),                            plot, SLOT(U_generate_additional_data()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_id_data()),                                    plot, SLOT(U_generate_id_data()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_id_frame(int)),                                plot, SLOT(U_generate_id_frame(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_range()),                                      plot, SLOT(U_generate_range()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_range(int)),                                   plot, SLOT(U_generate_range(int)), Qt::DirectConnection);

    connect(this, SIGNAL(US_set_frame_type(UC_plot::UTE_frame_type)),                   plot, SLOT(U_set_frame_type(UC_plot::UTE_frame_type)),                      Qt::DirectConnection);
    connect(this, SIGNAL(US_set_pixel_type(UC_plot::UTE_pixel_type)),                   plot, SLOT(U_set_pixel_type(UC_plot::UTE_pixel_type)),                      Qt::DirectConnection);
    connect(this, SIGNAL(US_set_roi(int, int, int, int)),                               plot, SLOT(U_set_roi(int, int, int, int)),                                  Qt::DirectConnection);
    connect(this, SIGNAL(US_set_rebin(int, int, int)),                                  plot, SLOT(U_set_rebin(int, int, int)),                                     Qt::DirectConnection);
    connect(this, SIGNAL(US_set_id_type(UC_plot::UTE_id_type)),                         plot, SLOT(U_set_id_type(UC_plot::UTE_id_type)),    Qt::DirectConnection);
    connect(this, SIGNAL(US_set_thresholds(int, int, int, int)),                        plot, SLOT(U_set_id_thresholds(int, int, int, int)),                           Qt::DirectConnection);
    connect(this, SIGNAL(US_set_threshold_range(int, int)),                             plot, SLOT(U_set_threshold_range(int, int)),                                Qt::DirectConnection);

    connect(this, SIGNAL(US_set_mask(bool, bool, double, bool, int)),               plot, SLOT(U_set_mask(bool, bool, double, bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_count_mask(bool, double, bool, int)),                   plot, SLOT(U_count_mask(bool, double, bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_mask_overflowed(bool, int)),                        plot, SLOT(U_set_mask_overflowed(bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_mask_overflowed(bool)),                             plot, SLOT(U_set_mask_overflowed(bool)), Qt::DirectConnection);
    connect(this, SIGNAL(US_reset_mask()),                                          plot, SLOT(U_reset_mask()), Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected(int, int, int, int)),                     plot, SLOT(U_mask_selected(int, int, int, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected_value(double, double, bool)),             plot, SLOT(U_mask_selected_value(double, double, bool)), Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected_value_thl(double, double, bool, int)),    plot, SLOT(U_mask_selected_value_thl(double, double, bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_save_mask(QString)),    plot, SLOT(U_save_mask(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(US_load_mask(QString)),    plot, SLOT(U_load_mask(QString)), Qt::DirectConnection);

    connect(plot, SIGNAL(US_frame_data(double, double, double)),                this, SLOT(U_add_frame_data(double, double, double)),                   Qt::DirectConnection);
    connect(plot, SIGNAL(US_chip_data(UC_pixels_info)),                         this, SLOT(U_add_table_data(UC_pixels_info)),                           Qt::DirectConnection);
    connect(plot, SIGNAL(US_id_roi_GA_data(UC_plot::UTStr_id_GA_data)),         this, SLOT(U_add_id_roi_GA_data(UC_plot::UTStr_id_GA_data)),            Qt::DirectConnection);
    connect(plot, SIGNAL(US_id_roi_LC_data(UC_plot::UTStr_id_LC_data)),         this, SLOT(U_add_id_roi_LC_data(UC_plot::UTStr_id_LC_data)),            Qt::DirectConnection);
    connect(plot, SIGNAL(US_id_frame_data(double, double, double)), this,       SLOT(U_add_id_frame_data(double, double, double)),    Qt::DirectConnection);

    connect(plot, SIGNAL(US_replot_spectra(QVector<double>, QVector<double>)),                  this, SLOT(U_replot_spectra(QVector<double>, QVector<double>)),                 Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_frame()),                                                    this, SLOT(U_replot_frame()),                                                   Qt::DirectConnection);
    connect(plot, SIGNAL(US_rewrite_table()),                                                   this, SLOT(U_rewrite_table()),                                                  Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_distribution(QVector<double>, QVector<double>)),             this, SLOT(U_replot_distribution(QVector<double>, QVector<double>)),            Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_calibration_chip(QVector<double>, QVector<double>, bool)),   this, SLOT(U_replot_chip_fit(QVector<double>, QVector<double>, bool)),  Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_calibration(QVector<double>, QVector<double>, int, bool)),   this, SLOT(U_replot_calibration(QVector<double>, QVector<double>, int, bool)),  Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_id_frame()),                                      this, SLOT(U_replot_id_frame()),                                    Qt::DirectConnection);

    connect(plot, SIGNAL(US_new_thl(int)),                                          this, SLOT(U_new_thl(int)), Qt::DirectConnection);

    connect(plot, SIGNAL(US_scan_settings(UC_data_container::UTStr_data_container_settings)),                              this, SLOT(U_renew_list_model(UC_data_container::UTStr_data_container_settings)), Qt::DirectConnection);

    connect(this, SIGNAL(US_set_smoothing(int)),                              plot, SLOT(U_set_smoothing(int)), Qt::DirectConnection);

    connect(this, SIGNAL(US_add_roi(UC_roi)),         plot, SLOT(U_add_roi(UC_roi)), Qt::DirectConnection);
    connect(this, SIGNAL(US_delete_roi(int)),                              plot, SLOT(U_delete_roi(int)), Qt::DirectConnection);


    plot->moveToThread(thread);
}

UC_wai::~UC_wai()
{
    thread->exit();
    if (thread->wait(3000)) {
        thread->terminate();
    }
    delete plot;
    delete gradient;
    delete thread;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_start() {
    thread->start();
}

UC_plot* UC_wai::U_get_plot() {
    return plot;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_set_spectra_qcp(QCustomPlot * spectra_qcp) {
    this->spectra_qcp = spectra_qcp;
    spectra_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    spectra_qcp->xAxis->setLabel("thl");
    spectra_qcp->yAxis->setLabel("value");

    sr_spectra_qcp = new QCPSelectionRect(spectra_qcp);
    spectra_qcp->setSelectionRect(sr_spectra_qcp);

    connect(spectra_qcp->xAxis, SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(spectra_qcp->yAxis, SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(spectra_qcp,        SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_spectra(QMouseEvent*)));
}

void UC_wai::U_set_frame_qcp(QCustomPlot * frame_qcp) {
    this->frame_qcp = frame_qcp;

    frame_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    frame_qcp->axisRect()->setupFullAxesBox(true);
    frame_qcp->xAxis->setLabel("x");
    frame_qcp->yAxis->setLabel("y");

    color_map_frame_qcp = new QCPColorMap(frame_qcp->xAxis, frame_qcp->yAxis);
    color_map_data_frame_qcp = new QCPColorMapData(3840, 256, QCPRange(0, 3839), QCPRange(0, 255));
    color_map_frame_qcp->setData(color_map_data_frame_qcp, true);

    color_scale_frame_qcp = new QCPColorScale(frame_qcp);
    frame_qcp->plotLayout()->addElement(0, 1, color_scale_frame_qcp);
    color_scale_frame_qcp->setType(QCPAxis::atRight);
    color_map_frame_qcp->setColorScale(color_scale_frame_qcp);
    color_scale_frame_qcp->axis()->setLabel("value");

    gradient = new QCPColorGradient(QCPColorGradient::gpThermal);
    color_map_frame_qcp->setGradient(*gradient);
    color_map_frame_qcp->setInterpolate(false);

    margin_group_frame_qcp = new QCPMarginGroup(frame_qcp);
    frame_qcp->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_frame_qcp);
    color_scale_frame_qcp->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_frame_qcp);

    color_map_frame_qcp->rescaleDataRange(true);
    color_scale_frame_qcp->rescaleDataRange(true);
    frame_qcp->rescaleAxes();
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);

    sr_frame_qcp = new QCPSelectionRect(frame_qcp);
    frame_qcp->setSelectionRect(sr_frame_qcp);

    connect(frame_qcp->xAxis,               SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_frame_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(frame_qcp->yAxis,               SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_frame_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(color_scale_frame_qcp->axis(),  SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_frame_z_axis_range(QCPRange)), Qt::DirectConnection);
    connect(frame_qcp,                      SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_frame(QMouseEvent*)));
}

void UC_wai::U_set_distribution_qcp(QCustomPlot * distribution_qcp) {
    this->distribution_qcp = distribution_qcp;
    distribution_qcp->xAxis->setLabel("value");
    distribution_qcp->yAxis->setLabel("pixels");
    distribution_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    sr_distribution_qcp = new QCPSelectionRect(distribution_qcp);
    distribution_qcp->setSelectionRect(sr_distribution_qcp);

    connect(distribution_qcp->xAxis,   SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_distribution_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(distribution_qcp->yAxis,   SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_distribution_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(distribution_qcp,          SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_distribution(QMouseEvent*)));
}

void UC_wai::U_set_chip_fit_qcp(QCustomPlot * chip_fit_qcp) {
    this->chip_fit_qcp = chip_fit_qcp;
    chip_fit_qcp->xAxis->setLabel("thl");
    chip_fit_qcp->yAxis->setLabel("value");
    chip_fit_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    sr_chip_fit_qcp = new QCPSelectionRect(chip_fit_qcp);
    chip_fit_qcp->setSelectionRect(sr_chip_fit_qcp);

    connect(chip_fit_qcp->xAxis,    SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_chip_fit_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(chip_fit_qcp->yAxis,    SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_chip_fit_y_axis_range(QCPRange)), Qt::DirectConnection);
    //connect(chip_fit_qcp,           SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_move_chip_fit(QMouseEvent*)));
}

void UC_wai::U_set_calibration_qcp(QCustomPlot * calibration_qcp) {
    this->calibration_qcp = calibration_qcp;

    calibration_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    calibration_qcp->xAxis->setLabel("thl");
    calibration_qcp->yAxis->setLabel("energy");
    sr_calibration_qcp = new QCPSelectionRect(calibration_qcp);
    calibration_qcp->setSelectionRect(sr_calibration_qcp);

    connect(calibration_qcp->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_calibration_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(calibration_qcp->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_calibration_y_axis_range(QCPRange)), Qt::DirectConnection);
    //connect(calibration_qcp, SIGNAL(mouseRelease(QMouseEvent*)),          this, SLOT(U_mouse_move_calibration(QMouseEvent*)));
}

void UC_wai::U_set_id_frame_qcp(QCustomPlot * id_frame_qcp) {
    this->id_frame_qcp = id_frame_qcp;

    id_frame_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    id_frame_qcp->axisRect()->setupFullAxesBox(true);
    id_frame_qcp->xAxis->setLabel("x");
    id_frame_qcp->yAxis->setLabel("y");

    color_map_id_frame_qcp = new QCPColorMap(id_frame_qcp->xAxis, id_frame_qcp->yAxis);
    color_map_data_id_frame_qcp = new QCPColorMapData(3840, 256, QCPRange(0, 3839), QCPRange(0, 255));
    color_map_id_frame_qcp->setData(color_map_data_id_frame_qcp, true);

    color_scale_id_frame_qcp = new QCPColorScale(id_frame_qcp);
    id_frame_qcp->plotLayout()->addElement(0, 1, color_scale_id_frame_qcp);
    color_scale_id_frame_qcp->setType(QCPAxis::atRight);
    color_map_id_frame_qcp->setColorScale(color_scale_id_frame_qcp);
    color_scale_id_frame_qcp->axis()->setLabel("value");

    gradient = new QCPColorGradient(QCPColorGradient::gpThermal);
    color_map_id_frame_qcp->setGradient(*gradient);
    color_map_id_frame_qcp->setInterpolate(false);

    margin_group_id_frame_qcp = new QCPMarginGroup(id_frame_qcp);
    id_frame_qcp->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_id_frame_qcp);
    color_scale_id_frame_qcp->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_id_frame_qcp);

    color_map_id_frame_qcp->rescaleDataRange(true);
    color_scale_id_frame_qcp->rescaleDataRange(true);
    id_frame_qcp->rescaleAxes();
    id_frame_qcp->replot(QCustomPlot::rpQueuedReplot);

    sr_id_frame_qcp = new QCPSelectionRect(id_frame_qcp);
    id_frame_qcp->setSelectionRect(sr_id_frame_qcp);

//    connect(frame_qcp->xAxis,               SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_frame_x_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(frame_qcp->yAxis,               SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_frame_y_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(color_scale_frame_qcp->axis(),  SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_frame_z_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(frame_qcp,                      SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_frame(QMouseEvent*)));
}

void UC_wai::U_set_table(QTableView * table) {
    this->table = table;

    table_model = new QStandardItemModel(12, 1);
    table->setModel(table_model);
    table_index = table_model->index(0, 0);
    table_model->setData(table_index, "tittle");
    table_index = table_model->index(1, 0);
    table_model->setData(table_index, "number of pixel");
    table_index = table_model->index(2, 0);
    table_model->setData(table_index, "min");
    table_index = table_model->index(3, 0);
    table_model->setData(table_index, "max");
    table_index = table_model->index(4, 0);
    table_model->setData(table_index, "sum");
    table_index = table_model->index(5, 0);
    table_model->setData(table_index, "mean");
    table_index = table_model->index(6, 0);
    table_model->setData(table_index, "median");
    table_index = table_model->index(7, 0);
    table_model->setData(table_index, "zeros");
    table_index = table_model->index(8, 0);
    table_model->setData(table_index, "overflows");
    table_index = table_model->index(9, 0);
    table_model->setData(table_index, "masked");
    table_index = table_model->index(10, 0);
    table_model->setData(table_index, "standart deviation");
    table_index = table_model->index(11, 0);
    table_model->setData(table_index, "signal to noise resolution");
}

void UC_wai::U_set_id_table(QTableView * id_table) {
    this->id_table = id_table;

    id_GA_table_model = new QStandardItemModel(1, 10);
    id_table_index = id_GA_table_model->index(0, 0);
    id_GA_table_model->setData(id_table_index, "element");
    id_table_index = id_GA_table_model->index(0, 1);
    id_GA_table_model->setData(id_table_index, "value");
    id_table_index = id_GA_table_model->index(0, 2);
    id_GA_table_model->setData(id_table_index, "first window left");
    id_table_index = id_GA_table_model->index(0, 3);
    id_GA_table_model->setData(id_table_index, "first window right");
    id_table_index = id_GA_table_model->index(0, 4);
    id_GA_table_model->setData(id_table_index, "second window left");
    id_table_index = id_GA_table_model->index(0, 5);
    id_GA_table_model->setData(id_table_index, "second window right");
    id_table_index = id_GA_table_model->index(0, 6);
    id_GA_table_model->setData(id_table_index, "first window sum");
    id_table_index = id_GA_table_model->index(0, 7);
    id_GA_table_model->setData(id_table_index, "second window sum");
    id_table_index = id_GA_table_model->index(0, 8);
    id_GA_table_model->setData(id_table_index, "frame type");
    id_table_index = id_GA_table_model->index(0, 9);
    id_GA_table_model->setData(id_table_index, "pixel type");

    id_LC_table_model = new QStandardItemModel(1, 4);
    id_table_index = id_LC_table_model->index(0, 0);
    id_LC_table_model->setData(id_table_index, "element");
    id_table_index = id_LC_table_model->index(0, 0);
    id_LC_table_model->setData(id_table_index, "value");
    id_table_index = id_LC_table_model->index(0, 1);
    id_LC_table_model->setData(id_table_index, "scan name");
    id_table_index = id_LC_table_model->index(0, 2);
    id_LC_table_model->setData(id_table_index, "thl sample");
    id_table_index = id_LC_table_model->index(0, 3);
    id_LC_table_model->setData(id_table_index, "sample data");
}

void UC_wai::U_set_list(QListView * list) {
    this->list = list;
    list_model = new QStandardItemModel();
    list->setModel(list_model);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_set_data(UC_data_container::UTStr_data_container_settings settings, QString str) {
    list_model->appendRow(new QStandardItem(str));
    emit US_set_data(settings);
    if (n_scans == 0) {
        scan_index = 0;
        emit US_set_scan(0);
        emit US_get_settings(0);
    }
    n_scans++;

}

void UC_wai::U_reset_data() {
    emit US_reset_data();
    scan_index = -1;
    n_scans = 0;
    list_model->clear();
}

void UC_wai::U_delete_scan(int index) {
    if (index == scan_index) {
        scan_index = 0;
    }
    list_model->removeRows(index, 1);
    if (scan_index > index) scan_index--;
    n_scans--;
    emit US_delete_scan(index);
}

void UC_wai::U_set_scan(int index) {
    scan_index = index;
    emit US_set_scan(index);
    emit US_get_settings(index);
}

void UC_wai::U_change_scan(int index) {
    emit US_get_settings(index);
}

void UC_wai::U_change_scan_setting(int index, UC_data_container::UTStr_data_container_settings settings) {
    emit US_set_settings(index, settings);
    emit US_renew_scan_settings(settings);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_generate_spectra(QString graph_name) {
    U_delete_mask_plots();
    spectra_qcp->xAxis->setLabel("thl");
    spectra_qcp->addGraph();
    spectra_qcp->graph()->setName(graph_name);
    spectra_qcp->graph()->setPen(pens[spectra_qcp->graphCount() % 16]);
    spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    emit US_generate_spectra();
}

void UC_wai::U_generate_spectra(QString graph_name, int n) {
    U_delete_mask_plots();
    spectra_qcp->xAxis->setLabel("energy");
    spectra_qcp->addGraph();
    spectra_qcp->graph()->setName(graph_name);
    spectra_qcp->graph()->setPen(pens[spectra_qcp->graphCount() % 16]);
    spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
    emit US_generate_spectra(n);
}

void UC_wai::U_delete_last_spectra() {
    spectra_qcp->removeGraph(spectra_qcp->graphCount() - 1);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_reset_spectra() {
    spectra_qcp->clearGraphs();
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}
///
void UC_wai::U_generate_frame(int thl) {
    emit US_generate_frame(thl);
}

void UC_wai::U_generate_frame(double energy) {
    emit US_generate_frame(energy);
}
//
void UC_wai::U_generate_table(int thl) {
    table_model->removeColumns(1, table_model->columnCount() - 1);
    emit US_generate_table(thl);
}

void UC_wai::U_generate_table(double energy) {
    table_model->removeColumns(1, table_model->columnCount() - 1);
    emit US_generate_table(energy);
}

void UC_wai::U_generate_table() {
    table_model->removeColumns(1, table_model->columnCount() - 1);
    emit US_generate_table();
}
///
void UC_wai::U_generate_distribution(int n_bins, double min, double max, QString graph_name) {
    U_delete_mask_plots();
    distribution_qcp->addGraph();
    distribution_qcp->graph()->setName(graph_name);
    distribution_qcp->graph()->setPen(pens[distribution_qcp->graphCount() % 16]);
    distribution_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    emit US_generate_distribution(n_bins, min, max);
}

void UC_wai::U_generate_distribution(int n_bins, double min, double max, int thl_index, QString graph_name) {
    distribution_qcp->addGraph();
    distribution_qcp->graph()->setName(graph_name);
    distribution_qcp->graph()->setPen(pens[distribution_qcp->graphCount() % 16]);
    distribution_qcp->graph()->setLineStyle(QCPGraph::lsStepLeft);

    emit US_generate_distribution(n_bins, min, max, thl_index);
}

void UC_wai::U_delete_last_distribution() {
    distribution_qcp->removeGraph(distribution_qcp->graphCount() - 1);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_reset_distribution() {
    distribution_qcp->clearGraphs();
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
}
///

void UC_wai::U_generate_range() {
    emit US_generate_range();
}

void UC_wai::U_generate_range(int thl_index) {
    emit US_generate_range(thl_index);
}

//

void UC_wai::U_generate_chip_fit(int chip) {
    chip_fit_qcp->addGraph();
    chip_fit_qcp->graph()->setName(QString("Counter 1 chip %1").arg(chip));
    chip_fit_qcp->graph()->setPen(pens[chip_fit_qcp->graphCount() % 16]);
    chip_fit_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    chip_fit_qcp->addGraph();
    chip_fit_qcp->graph()->setName(QString("Fit chip %1").arg(chip));
    chip_fit_qcp->graph()->setPen(pens[chip_fit_qcp->graphCount() % 16]);
    chip_fit_qcp->graph()->setLineStyle(QCPGraph::lsLine);

    emit US_generate_chip_fit(chip);
}

void UC_wai::U_reset_chip_fit() {
    chip_fit_qcp->clearGraphs();
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

//

void UC_wai::U_generate_calibration() {
    calibration_qcp->clearGraphs();
    for (int chip = 0; chip < 15; chip++) {
        calibration_qcp->addGraph();
        calibration_qcp->graph()->setName(QString("Calibration chip %1").arg(chip + 1));
        calibration_qcp->graph()->setPen(pens[chip]);
        calibration_qcp->graph()->setLineStyle(QCPGraph::lsNone);
        calibration_qcp->graph()->setScatterStyle(QCPScatterStyle(shapes[chip]));
        calibration_qcp->graph()->removeFromLegend();

        calibration_qcp->addGraph();
        calibration_qcp->graph()->setName(QString("Calibration fit chip %1").arg(chip + 1));
        calibration_qcp->graph()->setPen(pens[chip]);
        calibration_qcp->graph()->setLineStyle(QCPGraph::lsLine);
        calibration_qcp->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }
    emit US_generate_calibration();
}

void UC_wai::U_reset_calibration() {
    calibration_qcp->clearGraphs();
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}

//

void UC_wai::U_generate_id_roi(QString id_GA_element, UC_plot::UTE_id_type type) {
    this->id_GA_element = id_GA_element;
    if (type == UC_plot::UTE_IT_linear_combination) {
        int n = id_LC_table_model->rowCount();
        id_LC_table_model->removeRows(1, n - 1);
        n = id_LC_table_model->columnCount();
        id_LC_table_model->removeColumns(4, n - 4);
    }
    switch (type) {
        case UC_plot::UTE_IT_GA_method : {
            id_table->setModel(id_GA_table_model);
            break;
        }
        case UC_plot::UTE_IT_linear_combination : {
            id_table->setModel(id_LC_table_model);
            break;
        }
    }

    emit US_generate_id_roi();
}

void UC_wai::U_reset_id_roi(UC_plot::UTE_id_type type) {
    switch (type) {
        case UC_plot::UTE_IT_GA_method : {
            int n = id_GA_table_model->rowCount();
            id_GA_table_model->removeRows(2, n - 1);
            return;
        }
        case UC_plot::UTE_IT_linear_combination : {
            int n = id_LC_table_model->rowCount();
            id_LC_table_model->removeRows(2, n - 1);
            n = id_LC_table_model->columnCount();
            id_LC_table_model->removeColumns(4, n - 3);
            return;
        }
    }
}

void UC_wai::U_generate_additional_data() {
    emit US_generate_additional_data();
}

void UC_wai::U_generate_id_data() {
    emit US_generate_id_data();
}

void UC_wai::U_generate_id_frame(int element_index) {
    emit US_generate_id_frame(element_index);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_set_frame_type(UC_plot::UTE_frame_type frame_type_in) {
    emit US_set_frame_type(frame_type_in);
}

void UC_wai::U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in) {
    emit US_set_pixel_type(pixel_type_in);
}

void UC_wai::U_set_id_type(UC_plot::UTE_id_type id_type) {
    emit US_set_id_type(id_type);
}

//

void UC_wai::U_set_roi(int x_min, int x_max, int y_min, int y_max) {
    emit US_set_roi(x_min, x_max, y_min, y_max);
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
}

//

void UC_wai::U_set_threshold_range(int thl_min, int thl_max) {
    thl_start = thl_min;
    thl_finish = thl_max;
    emit US_set_threshold_range(thl_min, thl_max);
}
//

void UC_wai::U_set_rebin(int rebin_x, int rebin_y, int rebin_thl) {
    emit US_set_rebin(rebin_x, rebin_y, rebin_thl);
}

void UC_wai::U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4) {
    emit US_set_thresholds(thl_id_1, thl_id_2, thl_id_3, thl_id_4);
}

//

void UC_wai::U_set_interaction_mode(bool select) {
    this->select = select;
    if (select) {
        spectra_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        frame_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        distribution_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        chip_fit_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        calibration_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        id_frame_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        spectra_qcp->setSelectionRectMode(QCP::srmCustom);
        frame_qcp->setSelectionRectMode(QCP::srmCustom);
        distribution_qcp->setSelectionRectMode(QCP::srmCustom);
        chip_fit_qcp->setSelectionRectMode(QCP::srmCustom);
        calibration_qcp->setSelectionRectMode(QCP::srmCustom);
        id_frame_qcp->setSelectionRectMode(QCP::srmCustom);
    } else {
        spectra_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        frame_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        distribution_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        chip_fit_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        calibration_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        id_frame_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        spectra_qcp->setSelectionRectMode(QCP::srmNone);
        frame_qcp->setSelectionRectMode(QCP::srmNone);
        distribution_qcp->setSelectionRectMode(QCP::srmNone);
        chip_fit_qcp->setSelectionRectMode(QCP::srmNone);
        calibration_qcp->setSelectionRectMode(QCP::srmNone);
        id_frame_qcp->setSelectionRectMode(QCP::srmNone);
    }
}

void UC_wai::U_set_x_axis_type(bool view) {
    if (view) {
        spectra_qcp->xAxis->setScaleType(QCPAxis::stLinear);
        distribution_qcp->xAxis->setScaleType(QCPAxis::stLinear);
        chip_fit_qcp->xAxis->setScaleType(QCPAxis::stLinear);
        calibration_qcp->xAxis->setScaleType(QCPAxis::stLinear);
    } else {
        spectra_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
        distribution_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
        chip_fit_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
        calibration_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
    }
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_y_axis_type(bool view) {
    if (view) {
        spectra_qcp->yAxis->setScaleType(QCPAxis::stLinear);
        distribution_qcp->yAxis->setScaleType(QCPAxis::stLinear);
        chip_fit_qcp->yAxis->setScaleType(QCPAxis::stLinear);
        calibration_qcp->xAxis->setScaleType(QCPAxis::stLinear);
    } else {
        spectra_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
        distribution_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
        chip_fit_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
        calibration_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
    }
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_legend_enable(bool enable) {
    legend_enable = enable;

    spectra_qcp->legend->setVisible(enable);
    distribution_qcp->legend->setVisible(enable);
    chip_fit_qcp->legend->setVisible(enable);
    calibration_qcp->legend->setVisible(enable);

    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}
//
void UC_wai::U_set_renew_ranges(bool enable) {
    renew_renges = enable;
}

void UC_wai::U_set_renew_roi_ranges(bool enable) {
    renew_roi_renges = enable;
}
//
void UC_wai::U_set_smoothing(int smoothing) {
    this->smoothing = smoothing;
    US_set_smoothing(smoothing);
}

///////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_resize_spectra() {
    spectra_qcp->rescaleAxes(true);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_normalize_spectra() {
    if (spectra_qcp->graphCount() == 0) return;
    double x, y, y_max;

    int n_graph = spectra_qcp->graphCount();
    for (int i = 0; i < n_graph; i++) {
        int n = spectra_qcp->graph(i)->dataCount();
        y_max = 0;
        for (int j = 0; j < n; j++) {
            y = spectra_qcp->graph(i)->data()->at(j)->value;
            if (qAbs(y) > y_max) y_max = qAbs(y);
        }

        for (int j = 0; j < n; j++) {
            y = (spectra_qcp->graph(i)->data()->at(j)->value / y_max);
            x = spectra_qcp->graph(i)->data()->at(j)->key;
            spectra_qcp->graph(i)->data()->remove(x);
            spectra_qcp->graph(i)->addData(x, y);
        }
    }
    spectra_qcp->rescaleAxes(true);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}
//
void UC_wai::U_resize_frame() {
    frame_qcp->xAxis->setRange(x_min, x_max);
    frame_qcp->yAxis->setRange(y_min, y_max);
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_rescale_frame() {
    QCPRange xrange = frame_qcp->xAxis->range();
    QCPRange yrange = frame_qcp->yAxis->range();
    double min = 1e300;
    double max = -1e300;
    double z = 0;
    for (int x = static_cast<int>(xrange.lower); x < xrange.upper; x++) {
        for (int y = static_cast<int>(yrange.lower); y < yrange.upper; y++) {
            z = color_map_frame_qcp->data()->cell(x, y);
            if (z < min) min = z;
            if (z > max) max = z;
        }
    }
    color_scale_frame_qcp->axis()->setRange(min, max);
}
//
void UC_wai::U_resize_distribution() {
    distribution_qcp->rescaleAxes(true);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
}
//
void UC_wai::U_resize_chip_fit() {
    chip_fit_qcp->rescaleAxes(true);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_resize_calibration() {
    calibration_qcp->rescaleAxes(true);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}
///////////////////////////////////////////////////////////////////

void UC_wai::U_save_spectra_txt(QString file_name) {
    if (spectra_qcp->graphCount() == 0) return;
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    int n_graph = spectra_qcp->graphCount();
    str << n_graph << endl;
    for (int i = 0; i < n_graph; i++) {
        str << spectra_qcp->graph(i)->name() << endl;
        int n = spectra_qcp->graph(i)->dataCount();
        str << n << endl;
        for (int j = 0; j < n; j++) {
            str << spectra_qcp->graph(i)->data()->at(j)->key << " ";
        }
        str << endl;
        for (int j = 0; j < n; j++) {
            str << spectra_qcp->graph(i)->data()->at(j)->value << " ";
        }
        str << endl;
    }
    file.close();
}

void UC_wai::U_save_spectra(QString file_name, UTE_file_type file_type) {
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_FT_txt : {
            U_save_spectra_txt(file_name + ".txt");
            break;
        }
        case UTE_FT_pdf : {
            spectra_qcp->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            spectra_qcp->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            spectra_qcp->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            spectra_qcp->saveBmp(file_name + ".bmp");
            break;
        }
    }
}

void UC_wai::U_automatic_save_spectra(QString file_name, UTE_file_type file_type) {
    if (file_type == UTE_FT_txt) {
        QString str = file_name + "_chip%1";
        for (int chip = 0; chip < 15; chip++) {
            emit US_set_roi(chip * 256, chip * 256 + 256, 0, 255);
            U_generate_spectra(QString(str).arg(chip + 1));
        }
        U_save_spectra(QString(str).arg("s_all"), file_type);
    } else {
        QString str = file_name + "_chip%1";
        for (int chip = 0; chip < 15; chip++) {
            emit US_set_roi(chip * 256, chip * 256 + 256, 0, 255);
            U_generate_spectra(QString(str).arg(chip + 1));
            U_save_spectra(QString(str).arg(chip + 1), file_type);
            U_reset_spectra();
        }
    }
}
////
void UC_wai::U_save_frame_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int y = 0; y < 255; y++) {
        for (int x = 0; x < 3839; x++) {
            str << color_map_frame_qcp->data()->data(x, y) << " ";
        }
        str << color_map_frame_qcp->data()->data(3839, y) << endl;
    }
    for (int x = 0; x < 3839; x++) {
        str << color_map_frame_qcp->data()->data(x, 255) << " ";
    }
    str << color_map_frame_qcp->data()->data(3839, 255);
    file.close();
}

void UC_wai::U_save_frame(QString file_name, UTE_file_type file_type) {
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_FT_txt : {
            U_save_frame_txt(file_name + ".txt");
            break;
        }
        case UTE_FT_pdf : {
            frame_qcp->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            frame_qcp->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            frame_qcp->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            frame_qcp->saveBmp(file_name + ".bmp");
            break;
        }
    }
}
//
void UC_wai::U_save_table_tsv(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int row = 0; row < (table_model->rowCount()); row++) {
        for (int column = 0; column < (table_model->columnCount() - 1); column++) {
            table_index = table_model->index(row, column);
            str << table_model->data(table_index).toString();
            str << QChar(9);
        }
        table_index = table_model->index(row, table_model->columnCount() - 1);
        str << table_model->data(table_index).toString();
        str << endl;
    }
    file.close();
}

void UC_wai::U_save_table_csv(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int row = 0; row < (table_model->rowCount()); row++) {
        for (int column = 0; column < (table_model->columnCount() - 1); column++) {
            table_index = table_model->index(row, column);
            str << table_model->data(table_index).toString();
            str << ",";
        }
        table_index = table_model->index(row, table_model->columnCount() - 1);
        str << table_model->data(table_index).toString();
        str << endl;
    }
    file.close();
}

void UC_wai::U_save_table(QString file_name, UTE_table_file_type file_type) {
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_TFT_tsv : {
            U_save_table_tsv(file_name + ".tsv");
            break;
        }
        case UTE_TFT_csv : {
            U_save_table_csv(file_name + ".csv");
            break;
        }
    }
}
////

void UC_wai::U_save_distribution_txt(QString file_name) {
    if (distribution_qcp->graphCount() == 0) return;
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    int n_graph = distribution_qcp->graphCount();
    str << n_graph << endl;
    for (int i = 0; i < n_graph; i++) {
        int n = distribution_qcp->graph(i)->dataCount();
        str << n << endl;
        for (int j = 0; j < n; j++) {
            str << distribution_qcp->graph(i)->data()->at(j)->key << " ";
        }
        str << endl;
        for (int j = 0; j < n; j++) {
            str << distribution_qcp->graph(i)->data()->at(j)->value << " ";
        }
        str << endl;
    }
    file.close();
}

void UC_wai::U_save_distribution(QString file_name, UTE_file_type file_type) {
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_FT_txt : {
            U_save_distribution_txt(file_name + ".txt");
            break;
        }
        case UTE_FT_pdf : {
            distribution_qcp->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            distribution_qcp->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            distribution_qcp->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            distribution_qcp->saveBmp(file_name + ".bmp");
            break;
        }
    }
}
//
void UC_wai::U_save_chip_fit_txt(QString file_name) {
    if (chip_fit_qcp->graphCount() == 0) return;
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    int n_graph = chip_fit_qcp->graphCount();
    str << n_graph << endl;
    for (int i = 0; i < n_graph; i++) {
        str << chip_fit_qcp->graph(i)->name() << endl;
        int n = chip_fit_qcp->graph(i)->dataCount();
        str << n << endl;
        for (int j = 0; j < n; j++) {
            str << chip_fit_qcp->graph(i)->data()->at(j)->key << " ";
        }
        str << endl;
        for (int j = 0; j < n; j++) {
            str << chip_fit_qcp->graph(i)->data()->at(j)->value << " ";
        }
        str << endl;
    }
    file.close();
}

void UC_wai::U_save_chip_fit(QString file_name, UTE_file_type file_type) {
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_FT_txt : {
            U_save_chip_fit_txt(file_name + ".txt");
            break;
        }
        case UTE_FT_pdf : {
            chip_fit_qcp->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            chip_fit_qcp->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            chip_fit_qcp->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            chip_fit_qcp->saveBmp(file_name + ".bmp");
            break;
        }
    }
}
//
void UC_wai::U_save_calibration_txt(QString file_name) {
    if (calibration_qcp->graphCount() == 0) return;
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    int n_graph = calibration_qcp->graphCount();
    str << n_graph << endl;
    for (int i = 0; i < n_graph; i++) {
        str << calibration_qcp->graph(i)->name() << endl;
        int n = calibration_qcp->graph(i)->dataCount();
        str << n << endl;
        for (int j = 0; j < n; j++) {
            str << calibration_qcp->graph(i)->data()->at(j)->key << " ";
        }
        str << endl;
        for (int j = 0; j < n; j++) {
            str << calibration_qcp->graph(i)->data()->at(j)->value << " ";
        }
        str << endl;
    }
    file.close();
}

void UC_wai::U_save_calibration(QString file_name, UTE_file_type file_type) {
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_FT_txt : {
            U_save_calibration_txt(file_name + ".txt");
            break;
        }
        case UTE_FT_pdf : {
            calibration_qcp->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            calibration_qcp->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            calibration_qcp->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            calibration_qcp->saveBmp(file_name + ".bmp");
            break;
        }
    }
}
//
void UC_wai::U_save_id_frame_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int y = 0; y < 255; y++) {
        for (int x = 0; x < 3839; x++) {
            str << color_map_id_frame_qcp->data()->data(x, y) << " ";
        }
        str << color_map_id_frame_qcp->data()->data(3839, y) << endl;
    }
    for (int x = 0; x < 3839; x++) {
        str << color_map_id_frame_qcp->data()->data(x, 255) << " ";
    }
    str << color_map_id_frame_qcp->data()->data(3839, 255);
    file.close();
}

void UC_wai::U_save_id_frame(QString file_name, UTE_file_type file_type) {
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_FT_txt : {
            U_save_id_frame_txt(file_name + ".txt");
            break;
        }
        case UTE_FT_pdf : {
            id_frame_qcp->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            id_frame_qcp->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            id_frame_qcp->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            id_frame_qcp->saveBmp(file_name + ".bmp");
            break;
        }
    }
}

////
void UC_wai::U_load_spectra_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    int n, n_graph;
    double z;
    QString graph_name;
    str >> n_graph;
    for (int i = 0; i < n_graph; i++) {
        graph_name = str.readLine();
        graph_name = str.readLine();
        str >> n;
        QVector<double> x(n), y(n);
        for (int j = 0; j < n; j++) {
            str >> z;
            x << z;
        }
        for (int j = 0; j < n; j++) {
            str >> z;
            y << z;
        }
        spectra_qcp->addGraph();
        spectra_qcp->graph()->setName(graph_name);
        spectra_qcp->graph()->setPen(pens[spectra_qcp->graphCount() % 16]);
        spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
        spectra_qcp->graph()->setData(x, y);
    }
    spectra_qcp->legend->setVisible(legend_enable);
    if (renew_renges) spectra_qcp->rescaleAxes();
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(spectra_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_spectra(QMouseEvent*)));
    emit US_set_distribution_range(spectra_qcp->yAxis->range().lower, spectra_qcp->yAxis->range().upper);
    file.close();
}

void UC_wai::U_load_frame_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    double z;
    for (int y = 0; y < 256; y ++) {
        for (int x = 0; x < (15 * 256); x++) {
            str >> z;
            color_map_frame_qcp->data()->setData(x, y, z);
        }
    }
    if (renew_roi_renges) frame_qcp->xAxis->setRange(x_min, x_max);
    if (renew_roi_renges) frame_qcp->yAxis->setRange(y_min, y_max);
    if (renew_renges) U_rescale_frame();
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(frame_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_frame(QMouseEvent*)));
    emit US_set_distribution_range(color_scale_frame_qcp->axis()->range().lower, color_scale_frame_qcp->axis()->range().upper);
    file.close();
}

void UC_wai::U_load_distribution_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    int n, n_graph;
    double z;
    QString graph_name;
    str >> n_graph;
    for (int i = 0; i < n_graph; i++) {
        graph_name = str.readLine();
        graph_name = str.readLine();
        str >> n;
        QVector<double> x(n), y(n);
        for (int j = 0; j < n; j++) {
            str >> z;
            x << z;
        }
        for (int j = 0; j < n; j++) {
            str >> z;
            y << z;
        }
        distribution_qcp->addGraph();
        distribution_qcp->graph()->setName(graph_name);
        distribution_qcp->graph()->setPen(pens[distribution_qcp->graphCount() % 16]);
        distribution_qcp->graph()->setLineStyle(QCPGraph::lsStepLeft);
        distribution_qcp->graph()->setData(x, y);
    }
    distribution_qcp->legend->setVisible(legend_enable);
    if (renew_renges) distribution_qcp->rescaleAxes();
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(distribution_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_distribution(QMouseEvent*)));
    emit US_set_distribution_range(distribution_qcp->yAxis->range().lower, distribution_qcp->yAxis->range().upper);
    file.close();
}

void UC_wai::U_load_calibr_chip_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    int n, n_graph;
    double z;
    QString graph_name;
    str >> n_graph;
    for (int i = 0; i < n_graph; i++) {
        graph_name = str.readLine();
        graph_name = str.readLine();
        str >> n;
        QVector<double> x(n), y(n);
        for (int j = 0; j < n; j++) {
            str >> z;
            x << z;
        }
        for (int j = 0; j < n; j++) {
            str >> z;
            y << z;
        }
        chip_fit_qcp->addGraph();
        chip_fit_qcp->graph()->setName(graph_name);
        chip_fit_qcp->graph()->setPen(pens[chip_fit_qcp->graphCount() % 16]);
        chip_fit_qcp->graph()->setLineStyle(QCPGraph::lsStepLeft);
        chip_fit_qcp->graph()->setData(x, y);
    }
    chip_fit_qcp->legend->setVisible(legend_enable);
    if (renew_renges) chip_fit_qcp->rescaleAxes();
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(chip_fit_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_chip_fit(QMouseEvent*)));
    file.close();
}

void UC_wai::U_load_calibr_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    int n, n_graph;
    double z;
    QString graph_name;
    str >> n_graph;
    for (int i = 0; i < n_graph; i++) {
        graph_name = str.readLine();
        graph_name = str.readLine();
        str >> n;
        QVector<double> x(n), y(n);
        for (int j = 0; j < n; j++) {
            str >> z;
            x << z;
        }
        for (int j = 0; j < n; j++) {
            str >> z;
            y << z;
        }
        calibration_qcp->addGraph();
        calibration_qcp->graph()->setName(graph_name);
        calibration_qcp->graph()->setPen(pens[calibration_qcp->graphCount() % 16]);
        calibration_qcp->graph()->setLineStyle(QCPGraph::lsStepLeft);
        calibration_qcp->graph()->setData(x, y);
    }
    calibration_qcp->legend->setVisible(legend_enable);
    if (renew_renges) calibration_qcp->rescaleAxes();
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(calibration_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_calibration(QMouseEvent*)));
    file.close();
}

void UC_wai::U_load_id_frame_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    double z;
    for (int y = 0; y < 256; y ++) {
        for (int x = 0; x < (15 * 256); x++) {
            str >> z;
            color_map_id_frame_qcp->data()->setData(x, y, z);
        }
    }
    if (renew_roi_renges) id_frame_qcp->xAxis->setRange(x_min, x_max);
    if (renew_roi_renges) id_frame_qcp->yAxis->setRange(y_min, y_max);
    if (renew_renges) U_rescale_frame();
    id_frame_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(id_frame_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_frame(QMouseEvent*)));
    emit US_set_distribution_range(color_scale_id_frame_qcp->axis()->range().lower, color_scale_id_frame_qcp->axis()->range().upper);
    file.close();
}
//


//////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_set_mask(bool mask, bool more, double value, bool in_roi, int thl) {
    emit US_set_mask(mask, more, value, in_roi, thl);
}

void UC_wai::U_count_mask(bool more, double value, bool in_roi, int thl) {
    emit US_count_mask(more, value, in_roi, thl);
}

void UC_wai::U_set_mask_plots(double value) {
    if (mask_tick) return;
    distribution_qcp->addGraph();
    distribution_qcp->graph()->setPen(QPen(Qt::black));
    distribution_qcp->graph()->setLineStyle(QCPGraph::lsImpulse);
    distribution_qcp->graph()->addData(value, distribution_qcp->yAxis->range().upper);
    distribution_qcp->graph()->setName("Mask");
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    spectra_qcp->addGraph();
    spectra_qcp->graph()->setPen(QPen(Qt::black));
    spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
    spectra_qcp->graph()->addData(spectra_qcp->xAxis->range().lower, value);
    spectra_qcp->graph()->addData(spectra_qcp->xAxis->range().upper, value);
    spectra_qcp->graph()->setName("Mask");
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    mask_tick = true;
}

void UC_wai::U_delete_mask_plots() {
    if (!mask_tick) return;
    distribution_qcp->removeGraph(distribution_qcp->graphCount() - 1);
    spectra_qcp->removeGraph(spectra_qcp->graphCount() - 1);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    mask_tick = false;
}

void UC_wai::U_set_mask_overflowed(bool in_roi, int thl) {
    emit US_set_mask_overflowed(in_roi, thl);
}

void UC_wai::U_set_mask_overflowed(bool in_roi) {
    emit US_set_mask_overflowed(in_roi);
}

void UC_wai::U_reset_mask() {
    emit US_reset_mask();
}

void UC_wai::U_mask_selected(int x_min, int x_max, int y_min, int y_max) {
    emit US_mask_selected(x_min, x_max, y_min, y_max);
}

void UC_wai::U_mask_selected_value_thl(bool in_roi, int thl) {
    emit US_mask_selected_value_thl(value_min, value_max, in_roi, thl);
}

void UC_wai::U_mask_selected_value(bool in_roi) {
    emit US_mask_selected_value(value_min, value_max, in_roi);
}

void UC_wai::U_save_mask(QString file_name) {
    emit US_save_mask(file_name);
}

void UC_wai::U_load_mask(QString file_name) {
    emit US_load_mask(file_name);
}
//
//

void UC_wai::U_add_roi(QString name, int x_min, int x_max, int y_min, int y_max) {
    UC_roi roi(name, x_min, y_min, x_max, y_max);
    list_roi.append(roi);
    emit US_add_roi(roi);
}

UC_roi UC_wai::U_get_roi(int index) {
    return list_roi[index];
}

void UC_wai::U_delete_roi(int index) {
    list_roi.removeAt(index);
    emit US_delete_roi(index);
}

void UC_wai::U_save_roi(QString file_name) {
    int n = list_roi.size();
    if (n == 16) return;
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    str << n - 16;
    str << endl;
    for (int i = 16; i < n; i++) {
        str << list_roi[i].U_get_name();
        str << endl;
        str << list_roi[i].U_get_x_min() << " ";
        str << list_roi[i].U_get_y_min() << " ";
        str << list_roi[i].U_get_x_max() << " ";
        str << list_roi[i].U_get_y_max();
        str << endl;
    }
    file.close();
}

void UC_wai::U_load_roi(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    int n;
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    QString name;
    str >> n;
    for (int i = 0; i < n; i++) {
        name = str.readLine();
        name = str.readLine();
        str >> x_min;
        str >> y_min;
        str >> x_max;
        str >> y_max;
        UC_roi roi(name, x_min, y_min, x_max, y_max);
        list_roi << roi;
        emit US_add_roi(roi.U_get_name());
        emit US_add_roi(roi);
    }
    file.close();
}

//

void UC_wai::U_calculating_spectra(UTE_calculating_spectras calc_type, int index1, int index2) {
    int n_graph = spectra_qcp->graphCount();
    if (n_graph == 0) return;
    int n1 = spectra_qcp->graph(index1)->dataCount();
    QVector<double> x1(n1), y1(n1);
    for (int i = 0; i < n1; i++) {
        y1[i] = spectra_qcp->graph(index1)->data()->at(i)->value;
        x1[i] = spectra_qcp->graph(index1)->data()->at(i)->key;
    }
    int n2 = spectra_qcp->graph(index2)->dataCount();
    QVector<double> x2(n2), y2(n2);
    for (int i = 0; i < n2; i++) {
        y2[i] = spectra_qcp->graph(index2)->data()->at(i)->value;
        x2[i] = spectra_qcp->graph(index2)->data()->at(i)->key;
    }

    switch (calc_type) {
        case UTE_CS_add : {
            U_calculating_spectra_1(x1, y1, x2, y2);
            break;
        }
        case UTE_CS_multiplay : {
            U_calculating_spectra_2(x1, y1, x2, y2);
            break;
        }
        case UTE_CS_subtract : {
            U_calculating_spectra_3(x1, y1, x2, y2);
            break;
        }
        case UTE_CS_divide : {
            U_calculating_spectra_4(x1, y1, x2, y2);
            break;
        }
        case UTE_CS_smooth : {
            U_calculating_spectra_5(x1, y1);
            break;
        }
        case UTE_CS_diff : {
            U_calculating_spectra_6(x1, y1);
            break;
        }
    }

    spectra_qcp->addGraph();
    QString name1 = spectra_qcp->graph(index1)->name();
    QString name2 = spectra_qcp->graph(index2)->name();
    QString name;
    switch (calc_type) {
        case UTE_CS_add : {
            name = U_calculating_name_1(name1, name2);
            break;
        }
        case UTE_CS_multiplay : {
            name = U_calculating_name_2(name1, name2);
            break;
        }
        case UTE_CS_subtract : {
            name = U_calculating_name_3(name1, name2);
            break;
        }
        case UTE_CS_divide : {
            name = U_calculating_name_4(name1, name2);
            break;
        }
        case UTE_CS_smooth : {
            name = U_calculating_name_5(name1);
            break;
        }
        case UTE_CS_diff : {
            name = U_calculating_name_6(name1);
            break;
        }
    }

    emit US_new_spectra(name);
    spectra_qcp->graph()->setName(name);
    spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
    spectra_qcp->graph()->setPen(pens[spectra_qcp->graphCount() % 16]);
    spectra_qcp->graph()->setData(x1, y1);

    if (renew_renges) spectra_qcp->rescaleAxes(true);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_calculating_spectra_1(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2) {
    QVector<double> x, y;
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        int i1 = x1.indexOf(thl);
        int i2 = x2.indexOf(thl);
        double data1 = 0;
        if (i1 != -1) data1 = y1[i1];
        double data2 = 0;
        if (i2 != -1) data2 = y2[i2];
        x << thl;
        double data;
        data = data1 + data2;
        y << data;
    }
    x1 = x;
    y1 = y;
}

void UC_wai::U_calculating_spectra_2(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2) {
    QVector<double> x, y;
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        int i1 = x1.indexOf(thl);
        int i2 = x2.indexOf(thl);
        double data1 = 0;
        if (i1 != -1) data1 = y1[i1];
        double data2 = 0;
        if (i2 != -1) data2 = y2[i2];
        x << thl;
        double data;
        data = data1 * data2;
        y << data;
    }
    x1 = x;
    y1 = y;
}

void UC_wai::U_calculating_spectra_3(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2) {
    QVector<double> x, y;
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        int i1 = x1.indexOf(thl);
        int i2 = x2.indexOf(thl);
        double data1 = 0;
        if (i1 != -1) data1 = y1[i1];
        double data2 = 0;
        if (i2 != -1) data2 = y2[i2];
        x << thl;
        double data;
        data = data1 - data2;
        y << data;
    }
    x1 = x;
    y1 = y;
}

void UC_wai::U_calculating_spectra_4(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2) {
    QVector<double> x, y;
    for (int thl = thl_start; thl <= thl_finish; thl++) {
        int i1 = x1.indexOf(thl);
        int i2 = x2.indexOf(thl);
        double data1 = 0;
        if (i1 != -1) data1 = y1[i1];
        double data2 = 0;
        if (i2 != -1) data2 = y2[i2];
        x << thl;
        double data;
        if (qAbs(data2) < 1e-10) data = 0;
            else data = data1 / data2;
        y << data;
    }
    x1 = x;
    y1 = y;
}

void UC_wai::U_calculating_spectra_5(QVector<double> &x1, QVector<double> &y1) {
    y1 = U_smooth(x1, y1, smoothing);
}

void UC_wai::U_calculating_spectra_6(QVector<double> &x1, QVector<double> &y1) {
    y1 = U_diff(x1, y1);
}

QString UC_wai::U_calculating_name_1(QString name1, QString name2) {
    QString name = name1;
    name += " + ";
    name += name2;
    return name;
}

QString UC_wai::U_calculating_name_2(QString name1, QString name2) {
    QString name = name1;
    name += " * ";
    name += name2;
    return name;
}

QString UC_wai::U_calculating_name_3(QString name1, QString name2) {
    QString name = name1;
    name += " - ";
    name += name2;
    return name;
}

QString UC_wai::U_calculating_name_4(QString name1, QString name2) {
    QString name = name1;
    name += " / ";
    name += name2;
    return name;
}

QString UC_wai::U_calculating_name_5(QString name1) {
    QString name = name1;
    name += " smoothed";
    return name;
}

QString UC_wai::U_calculating_name_6(QString name1) {
    QString name = name1;
    name += " diffed";
    return name;
}
//////////////////////////!!!!!!!!!!!SLOTS!!!!!!!!!!!!!!!!!!!!!!!///////////////////////////////////////
void UC_wai::U_add_frame_data(double x, double y, double z) {
    color_map_frame_qcp->data()->setData(x, y, z);
}

void UC_wai::U_add_table_data(UC_pixels_info pixels_info) {
    QList<QStandardItem *> list;
    QStandardItem * item;

    list.clear();
    item = new QStandardItem(pixels_info.U_get_name());
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_n()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_min()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_max()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_sum()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_mean()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_median()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_zeros()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_overflows()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_masked()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_std_dev()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_snr()));
    list << item;
    table_model->appendColumn(list);
}

void UC_wai::U_add_id_roi_GA_data(UC_plot::UTStr_id_GA_data data) {
    QStandardItem * item;
    QList<QStandardItem *> list;

    item = new QStandardItem(id_GA_element);
    list << item;
    item = new QStandardItem(QString("%1").arg(data.id_data));
    list << item;
    item = new QStandardItem(QString("%1").arg(data.thl_1_window_left));
    list << item;
    item = new QStandardItem(QString("%1").arg(data.thl_1_window_right));
    list << item;
    item = new QStandardItem(QString("%1").arg(data.thl_2_window_left));
    list << item;
    item = new QStandardItem(QString("%1").arg(data.thl_2_window_right));
    list << item;
    item = new QStandardItem(QString("%1").arg(data.sum_1_window));
    list << item;
    item = new QStandardItem(QString("%1").arg(data.sum_2_window));
    list << item;
    item = new QStandardItem(QString("%1").arg(data.frame_type));
    list << item;
    item = new QStandardItem(QString("%1").arg(data.pixel_type));
    list << item;

    id_GA_table_model->appendRow(list);
}

void UC_wai::U_add_id_roi_LC_data(UC_plot::UTStr_id_LC_data data) {
    int n = data.number_of_samples;
    QList<QStandardItem *> list;
    QStandardItem * item;
    for (int i = 0; i < n; i++) {
        list.clear();
        item = new QStandardItem(data.elements[i]);
        list << item;
        id_LC_table_model->appendColumn(list);
    }
    list.clear();
    item = new QStandardItem("Frame count type");
    list << item;
    id_LC_table_model->appendColumn(list);
    list.clear();
    item = new QStandardItem("Pixel count type");
    list << item;
    id_LC_table_model->appendColumn(list);

    for (int i = 0; i < n; i++) {
        list.clear();
        item = new QStandardItem(QString("%1").arg(data.id_values[i]));
        list << item;
        item = new QStandardItem(data.elements[i]);
        list << item;
        item = new QStandardItem(QString("%1").arg(data.thl_samples[i]));
        list << item;
        item = new QStandardItem(QString("%1").arg(data.data[i]));
        list << item;
        for (int j = 0; j < n; j++) {
            item = new QStandardItem(QString("%1").arg(data.matrix[i][j]));
            list << item;
        }
        item = new QStandardItem(QString("%1").arg(data.frame_type));
        list << item;
        item = new QStandardItem(QString("%1").arg(data.pixel_type));
        list << item;
        id_LC_table_model->appendRow(list);
    }
}

void UC_wai::U_add_id_frame_data(double x, double y, double z) {
    color_map_id_frame_qcp->data()->setData(x, y, z);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_replot_spectra(QVector<double> x, QVector<double> y) {
    spectra_qcp->graph()->setData(x, y);
    spectra_qcp->legend->setVisible(legend_enable);
    if (renew_renges) spectra_qcp->rescaleAxes();
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(spectra_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_spectra(QMouseEvent*)));
    emit US_set_distribution_range(spectra_qcp->yAxis->range().lower, spectra_qcp->yAxis->range().upper);
}

void UC_wai::U_replot_frame() {
    if (renew_roi_renges) frame_qcp->xAxis->setRange(x_min, x_max);
    if (renew_roi_renges) frame_qcp->yAxis->setRange(y_min, y_max);
    if (renew_renges) U_rescale_frame();
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(frame_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_frame(QMouseEvent*)));
    emit US_set_distribution_range(color_scale_frame_qcp->axis()->range().lower, color_scale_frame_qcp->axis()->range().upper);
}

void UC_wai::U_rewrite_table() {

}

void UC_wai::U_replot_distribution(QVector<double> x, QVector<double> y) {
    distribution_qcp->graph()->setData(x, y);
    distribution_qcp->legend->setVisible(legend_enable);
    if (renew_renges) distribution_qcp->rescaleAxes();
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(distribution_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_distribution(QMouseEvent*)));
    emit US_set_distribution_range(distribution_qcp->yAxis->range().lower, distribution_qcp->yAxis->range().upper);
}

void UC_wai::U_replot_chip_fit(QVector<double> x, QVector<double> y, bool fit) {
    if (fit) {
        chip_fit_qcp->graph(chip_fit_qcp->graphCount() - 1)->setData(x, y);
    } else {
        chip_fit_qcp->graph(chip_fit_qcp->graphCount() - 2)->setData(x, y);
    }
    chip_fit_qcp->legend->setVisible(legend_enable);
    if (renew_renges) chip_fit_qcp->rescaleAxes();
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(chip_fit_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_chip_fit(QMouseEvent*)));
}

void UC_wai::U_replot_calibration(QVector<double> x, QVector<double> y, int chip, bool fit) {
    if (fit) {
        calibration_qcp->graph(chip * 2 + 1)->setData(x, y);
    } else {
        calibration_qcp->graph(chip * 2)->setData(x, y);
    }
    calibration_qcp->legend->setVisible(legend_enable);
    if (renew_renges) calibration_qcp->rescaleAxes();
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(calibration_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_calibration(QMouseEvent*)));
}

void UC_wai::U_replot_id_frame() {
    if (renew_roi_renges) id_frame_qcp->xAxis->setRange(x_min, x_max);
    if (renew_roi_renges) id_frame_qcp->yAxis->setRange(y_min, y_max);
    if (renew_renges) U_rescale_frame();
    id_frame_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(id_frame_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_frame(QMouseEvent*)));
}

///////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_set_spectra_x_axis_range(QCPRange range) {
    emit US_set_spectra_max_x(range.upper);
    emit US_set_spectra_min_x(range.lower);
}

void UC_wai::U_set_spectra_y_axis_range(QCPRange range) {
    emit US_set_spectra_max_y(range.upper);
    emit US_set_spectra_min_y(range.lower);
}
////

void UC_wai::U_set_spectra_max_x(double value) {
    spectra_qcp->xAxis->setRange(spectra_qcp->xAxis->range().lower, value);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_min_x(double value) {
    spectra_qcp->xAxis->setRange(value, spectra_qcp->xAxis->range().upper);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_max_y(double value) {
    spectra_qcp->yAxis->setRange(spectra_qcp->yAxis->range().lower, value);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_min_y(double value) {
    spectra_qcp->yAxis->setRange(value, spectra_qcp->yAxis->range().upper);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}

///////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_set_frame_x_axis_range(QCPRange range) {
    emit US_set_frame_max_x(range.upper);
    emit US_set_frame_min_x(range.lower);
}

void UC_wai::U_set_frame_y_axis_range(QCPRange range) {
    emit US_set_frame_max_y(range.upper);
    emit US_set_frame_min_y(range.lower);
}

void UC_wai::U_set_frame_z_axis_range(QCPRange range) {
    emit US_set_frame_max_z(range.upper);
    emit US_set_frame_min_z(range.lower);
}
////
void UC_wai::U_set_frame_max_x(double value) {
    frame_qcp->xAxis->setRange(frame_qcp->xAxis->range().lower, value);
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_min_x(double value) {
    frame_qcp->xAxis->setRange(value, frame_qcp->xAxis->range().upper);
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_max_y(double value) {
    frame_qcp->yAxis->setRange(frame_qcp->yAxis->range().lower, value);
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_min_y(double value) {
    frame_qcp->yAxis->setRange(value, frame_qcp->yAxis->range().upper);
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_max_z(double value) {
    color_scale_frame_qcp->axis()->setRange(color_scale_frame_qcp->axis()->range().lower, value);
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_min_z(double value) {
    color_scale_frame_qcp->axis()->setRange(value, color_scale_frame_qcp->axis()->range().upper);
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}
////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_set_distribution_x_axis_range(QCPRange range) {
    emit US_set_distribution_max_x(range.upper);
    emit US_set_distribution_min_x(range.lower);
}

void UC_wai::U_set_distribution_y_axis_range(QCPRange range) {
    emit US_set_distribution_max_y(range.upper);
    emit US_set_distribution_min_y(range.lower);
}
////

void UC_wai::U_set_distribution_max_x(double value) {
    distribution_qcp->xAxis->setRange(distribution_qcp->xAxis->range().lower, value);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_distribution_min_x(double value) {
    distribution_qcp->xAxis->setRange(value, distribution_qcp->xAxis->range().upper);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_distribution_max_y(double value) {
    distribution_qcp->yAxis->setRange(distribution_qcp->yAxis->range().lower, value);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_distribution_min_y(double value) {
    distribution_qcp->yAxis->setRange(value, distribution_qcp->yAxis->range().upper);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
}
//
void UC_wai::U_set_chip_fit_x_axis_range(QCPRange range) {
    emit US_set_chip_fit_max_x(range.upper);
    emit US_set_chip_fit_min_x(range.lower);
}

void UC_wai::U_set_chip_fit_y_axis_range(QCPRange range) {
    emit US_set_chip_fit_max_y(range.upper);
    emit US_set_chip_fit_min_y(range.lower);
}
////

void UC_wai::U_set_chip_fit_max_x(double value) {
    chip_fit_qcp->xAxis->setRange(chip_fit_qcp->xAxis->range().lower, value);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_chip_fit_min_x(double value) {
    chip_fit_qcp->xAxis->setRange(value, chip_fit_qcp->xAxis->range().upper);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_chip_fit_max_y(double value) {
    chip_fit_qcp->yAxis->setRange(chip_fit_qcp->yAxis->range().lower, value);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_chip_fit_min_y(double value) {
    chip_fit_qcp->yAxis->setRange(value, chip_fit_qcp->yAxis->range().upper);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

//
void UC_wai::U_set_calibration_x_axis_range(QCPRange range) {
    emit US_set_calibration_max_x(range.upper);
    emit US_set_calibration_min_x(range.lower);
}

void UC_wai::U_set_calibration_y_axis_range(QCPRange range) {
    emit US_set_calibration_max_y(range.upper);
    emit US_set_calibration_min_y(range.lower);
}
////

void UC_wai::U_set_calibration_max_x(double value) {
    calibration_qcp->xAxis->setRange(calibration_qcp->xAxis->range().lower, value);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_calibration_min_x(double value) {
    calibration_qcp->xAxis->setRange(value, calibration_qcp->xAxis->range().upper);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_calibration_max_y(double value) {
    calibration_qcp->yAxis->setRange(calibration_qcp->yAxis->range().lower, value);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_calibration_min_y(double value) {
    calibration_qcp->yAxis->setRange(value, calibration_qcp->yAxis->range().upper);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_mouse_move_spectra(QMouseEvent * event) {
    QString str;
    int n = spectra_qcp->graphCount();
    if (n == 0) return;

    QCPGraphDataContainer::const_iterator it = spectra_qcp->graph(0)->data()->constEnd();
    QVariant details;
    double data = spectra_qcp->graph(0)->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) it = spectra_qcp->graph(0)->data()->at(dataPoints.dataRange().begin());
    }
    str = "%1 = %2;" + QString("\n") + "%3 = %4;";
    str = str.arg(spectra_qcp->xAxis->label()).arg(static_cast<int>(it->key));
    str = str.arg(spectra_qcp->graph(0)->name()).arg(it->value);

    for (int i = 0; i < n; i++) {
        QCPGraphDataContainer::const_iterator it = spectra_qcp->graph(i)->data()->constEnd();
        QVariant details;
        double data = spectra_qcp->graph(i)->selectTest(event->pos(), false, &details);
        if (data > 0) {
            QCPDataSelection dataPoints = details.value<QCPDataSelection>();
            if (dataPoints.dataPointCount() > 0) it = spectra_qcp->graph(i)->data()->at(dataPoints.dataRange().begin());
        }
        str += "\n" + QString("%1 = %2;").arg(spectra_qcp->graph(i)->name()).arg(it->value);

    };
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, spectra_qcp);
}

void UC_wai::U_mouse_move_frame(QMouseEvent * event) {
    QVariant details;
    double data = color_map_frame_qcp->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) {
            double x = frame_qcp->xAxis->pixelToCoord(event->pos().x());
            double y = frame_qcp->yAxis->pixelToCoord(event->pos().y());
            int cell_x, cell_y;
            color_map_frame_qcp->data()->coordToCell(x, y, &cell_x, &cell_y);
            double z = color_map_frame_qcp->data()->cell(cell_x, cell_y);
            QString str = "x = %1; y = %2; z = %3";
            str = str.arg(x).arg(y).arg(z);
            US_mouse_data(str);
            QToolTip::showText(event->globalPos(), str, frame_qcp);
        }
    }
}

void UC_wai::U_mouse_move_distribution(QMouseEvent * event) {
    int n = distribution_qcp->graphCount();
    if (n == 0) return;

    QCPGraphDataContainer::const_iterator it = distribution_qcp->graph(0)->data()->constEnd();
    QVariant details;
    double data = distribution_qcp->graph(0)->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) it = distribution_qcp->graph(0)->data()->at(dataPoints.dataRange().begin());
    }
    QString str = "bin = %1;" + QString("\n") + "%2 = %3;";
    str = str.arg(it->key).arg(distribution_qcp->graph(0)->name()).arg(it->value);

    for (int i = 1; i < n; i++) {
        QCPGraphDataContainer::const_iterator it = distribution_qcp->graph(i)->data()->constEnd();
        QVariant details;
        double data = distribution_qcp->graph(i)->selectTest(event->pos(), false, &details);
        if (data > 0) {
            QCPDataSelection dataPoints = details.value<QCPDataSelection>();
            if (dataPoints.dataPointCount() > 0) it = distribution_qcp->graph(i)->data()->at(dataPoints.dataRange().begin());
        }
        str += "\n" + QString("%1 = %2;").arg(distribution_qcp->graph(i)->name()).arg(it->value);
    };
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, distribution_qcp);
}

void UC_wai::U_mouse_move_chip_fit(QMouseEvent * event) {
    QString str;
    int n = chip_fit_qcp->graphCount();
    if (n == 0) return;

    QCPGraphDataContainer::const_iterator it = chip_fit_qcp->graph(0)->data()->constEnd();
    QVariant details;
    double data = chip_fit_qcp->graph(0)->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) it = chip_fit_qcp->graph(0)->data()->at(dataPoints.dataRange().begin());
    }
    str = "%1 = %2;" + QString("\n") + "%3 = %4;";
    str = str.arg(chip_fit_qcp->xAxis->label()).arg(static_cast<int>(it->key));
    str = str.arg(chip_fit_qcp->graph(0)->name()).arg(it->value);

    for (int i = 0; i < n; i++) {
        QCPGraphDataContainer::const_iterator it = chip_fit_qcp->graph(i)->data()->constEnd();
        QVariant details;
        double data = chip_fit_qcp->graph(i)->selectTest(event->pos(), false, &details);
        if (data > 0) {
            QCPDataSelection dataPoints = details.value<QCPDataSelection>();
            if (dataPoints.dataPointCount() > 0) it = chip_fit_qcp->graph(i)->data()->at(dataPoints.dataRange().begin());
        }
        str += "\n" + QString("%1 = %2;").arg(chip_fit_qcp->graph(i)->name()).arg(it->value);

    };
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, chip_fit_qcp);
}

void UC_wai::U_mouse_move_calibration(QMouseEvent * event) {
    QString str;
    int n = calibration_qcp->graphCount();
    if (n == 0) return;

    QCPGraphDataContainer::const_iterator it = calibration_qcp->graph(0)->data()->constEnd();
    QVariant details;
    double data = calibration_qcp->graph(0)->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) it = calibration_qcp->graph(0)->data()->at(dataPoints.dataRange().begin());
    }
    str = "%1 = %2;" + QString("\n") + "%3 = %4;";
    str = str.arg(calibration_qcp->xAxis->label()).arg(static_cast<int>(it->key));
    str = str.arg(calibration_qcp->graph(0)->name()).arg(it->value);

    for (int i = 0; i < n; i++) {
        QCPGraphDataContainer::const_iterator it = calibration_qcp->graph(i)->data()->constEnd();
        QVariant details;
        double data = calibration_qcp->graph(i)->selectTest(event->pos(), false, &details);
        if (data > 0) {
            QCPDataSelection dataPoints = details.value<QCPDataSelection>();
            if (dataPoints.dataPointCount() > 0) it = calibration_qcp->graph(i)->data()->at(dataPoints.dataRange().begin());
        }
        str += "\n" + QString("%1 = %2;").arg(calibration_qcp->graph(i)->name()).arg(it->value);

    };
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, calibration_qcp);
}
////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_mouse_select_spectra(QMouseEvent * event) {
    if (!select) return;
    int xmin, xmax, ymin, ymax;
    xmax = static_cast<int>(std::round(sr_spectra_qcp->range(spectra_qcp->xAxis).upper));
    xmin = static_cast<int>(std::round(sr_spectra_qcp->range(spectra_qcp->xAxis).lower));
    value_max = sr_spectra_qcp->range(spectra_qcp->yAxis).upper;
    value_min = sr_spectra_qcp->range(spectra_qcp->yAxis).lower;
    ymin = static_cast<int>(std::round(value_min));
    ymax = static_cast<int>(std::round(value_max));
    emit US_set_thl_range(xmin, xmax);
    thl_start = xmin;
    thl_finish = xmax;
    emit US_set_distribution_range(ymin, ymax);
}

void UC_wai::U_mouse_select_frame(QMouseEvent * event) {
    if (!select) return;
    int xmin, xmax, ymin, ymax;
    xmax = static_cast<int>(std::round(sr_frame_qcp->range(frame_qcp->xAxis).upper));
    xmin = static_cast<int>(std::round(sr_frame_qcp->range(frame_qcp->xAxis).lower));
    ymax = static_cast<int>(std::round(sr_frame_qcp->range(frame_qcp->yAxis).upper));
    ymin = static_cast<int>(std::round(sr_frame_qcp->range(frame_qcp->yAxis).lower));
    emit US_set_roi_range(xmin, xmax, ymin, ymax);
}

void UC_wai::U_mouse_select_distribution(QMouseEvent * event) {
    if (!select) return;
    double xmin, xmax;
    xmax = sr_distribution_qcp->range(distribution_qcp->xAxis).upper;
    xmin = sr_distribution_qcp->range(distribution_qcp->xAxis).lower;
    value_min = xmin;
    value_max = xmax;
    emit US_set_distribution_range(xmin, xmax);
}
//


void UC_wai::U_new_thl(int thl) {
    emit US_new_thl(thl);
}

void UC_wai::U_renew_list_model(UC_data_container::UTStr_data_container_settings settings) {
    for (int i = 0; i < n_scans; i++) {
        list_model->item(i)->setBackground(QBrush(Qt::white));
    }
    if (scan_index != -1) list_model->item(scan_index)->setBackground(QBrush(Qt::green));
    if (settings.ff_int != -1) list_model->item(settings.ff_int)->setBackground(QBrush(Qt::red));
    if (settings.df_int != -1) list_model->item(settings.df_int)->setBackground(QBrush(Qt::blue));
    emit US_renew_scan_settings(settings);
}
