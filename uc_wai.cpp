#include "uc_wai.h"

UC_wai::UC_wai(QObject *parent) : QObject(parent)
{
    plot = new UC_plot();
    thread = new QThread();

    x_min = 0;
    x_max = 15 * 256;
    y_min = 0;
    y_max = 256;
    mask_tick = false;
    n_scans = 0;
    scan_index = -1;
    ff_index = -1;
    df_index = -1;
    renew_renges = true;

    plot->moveToThread(thread);

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

    connect(this, SIGNAL(US_set_data(UC_data_container::UTStr_data_container_settings)),            plot, SLOT(U_set_data(UC_data_container::UTStr_data_container_settings)),           Qt::DirectConnection);
    connect(this, SIGNAL(US_reset_data()),                                                          plot, SLOT(U_reset_data()),                                                         Qt::DirectConnection);
    connect(this, SIGNAL(US_delete_scan(int)),                                                      plot, SLOT(U_delete_scan(int)),                                                     Qt::DirectConnection);
    connect(this, SIGNAL(US_set_scan(int)),                                                         plot, SLOT(U_set_scan(int)),                                                        Qt::DirectConnection);
    connect(this, SIGNAL(US_get_settings(int)),                                                     plot, SLOT(U_get_settings(int)),                                                    Qt::DirectConnection);
    connect(this, SIGNAL(US_set_settings(int, UC_data_container::UTStr_data_container_settings)),   plot, SLOT(U_set_settings(int, UC_data_container::UTStr_data_container_settings)),  Qt::DirectConnection);

    connect(this, SIGNAL(US_generate_spectra()),                                    plot, SLOT(U_generate_spectra()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_spectra(double, double, int)),                 plot, SLOT(U_generate_spectra(double, double, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_frame(int)),                                   plot, SLOT(U_generate_frame(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_frame(double)),                                plot, SLOT(U_generate_frame(double)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_table(int)),                                   plot, SLOT(U_generate_table(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_table(double)),                                plot, SLOT(U_generate_table(double)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_table()),                                      plot, SLOT(U_generate_table()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_distribution(int, double, double)),            plot, SLOT(U_generate_distribution(int, double, double)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_frame_distribution(int, double, double, int)), plot, SLOT(U_generate_frame_distribution(int, double, double, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_calibration(int)),                             plot, SLOT(U_generate_calibration(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_calibration()),                                plot, SLOT(U_generate_calibration()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_identification_roi()),                         plot, SLOT(U_generate_identification_roi()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_additional_data()),                            plot, SLOT(U_generate_additional_data()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_identification_data()),                        plot, SLOT(U_generate_identification_data()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_identification_frame(int)),                    plot, SLOT(U_generate_identification_frame(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_range()),                                      plot, SLOT(U_generate_range()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_range(int)),                                   plot, SLOT(U_generate_range(int)), Qt::DirectConnection);
    //connect(this, SIGNAL(US_generate_identification(int, int)),                     plot, SLOT(U_generate_identification(int, int)), Qt::DirectConnection);

    connect(this, SIGNAL(US_set_frame_type(UC_plot::UTE_frame_type)),                   plot, SLOT(U_set_frame_type(UC_plot::UTE_frame_type)),                      Qt::DirectConnection);
    connect(this, SIGNAL(US_set_pixel_type(UC_plot::UTE_pixel_type)),                   plot, SLOT(U_set_pixel_type(UC_plot::UTE_pixel_type)),                      Qt::DirectConnection);
    connect(this, SIGNAL(US_set_roi(int, int, int, int)),                               plot, SLOT(U_set_roi(int, int, int, int)),                                  Qt::DirectConnection);
    connect(this, SIGNAL(US_set_rebin(int, int, int)),                                  plot, SLOT(U_set_rebin(int, int, int)),                                     Qt::DirectConnection);
    connect(this, SIGNAL(US_set_identification_type(UC_plot::UTE_identification_type)), plot, SLOT(U_set_identification_type(UC_plot::UTE_identification_type)),    Qt::DirectConnection);
    connect(this, SIGNAL(US_set_thresholds(int, int, int, int)),                        plot, SLOT(U_set_thresholds(int, int, int, int)),                           Qt::DirectConnection);
    connect(this, SIGNAL(US_set_threshold_range(int, int)),                             plot, SLOT(U_set_threshold_range(int, int)),                                Qt::DirectConnection);
    connect(this, SIGNAL(US_set_data_enable(UC_pixels_info::UTStr_data_enable)),        plot, SLOT(U_set_data_enable(UC_pixels_info::UTStr_data_enable)),           Qt::DirectConnection);

    connect(this, SIGNAL(US_set_mask(bool, bool, double, bool, int)),               plot, SLOT(U_set_mask(bool, bool, double, bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_count_mask(bool, double, bool, int)),                   plot, SLOT(U_count_mask(bool, double, bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_mask_overflowed(bool, int)),                        plot, SLOT(U_set_mask_overflowed(bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_mask_overflowed(bool)),                             plot, SLOT(U_set_mask_overflowed(bool)), Qt::DirectConnection);
    connect(this, SIGNAL(US_reset_mask()),                                          plot, SLOT(U_reset_mask()), Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected(int, int, int, int)),                     plot, SLOT(U_mask_selected(int, int, int, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected_value(double, double, bool)),             plot, SLOT(U_mask_selected_value(double, double, bool)), Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected_value_thl(double, double, bool, int)),    plot, SLOT(U_mask_selected_value_thl(double, double, bool, int)), Qt::DirectConnection);

    connect(plot, SIGNAL(US_spectra_data(double, double)),                      this, SLOT(U_add_spectra_data(double, double)),                         Qt::DirectConnection);
    connect(plot, SIGNAL(US_frame_data(double, double, double)),                this, SLOT(U_add_frame_data(double, double, double)),                   Qt::DirectConnection);
    connect(plot, SIGNAL(US_chip_data(UC_pixels_info)),                         this, SLOT(U_add_table_data(UC_pixels_info)),                           Qt::DirectConnection);
    connect(plot, SIGNAL(US_distribution_data(double, double)),                 this, SLOT(U_add_distibution_data(double, double)),                     Qt::DirectConnection);
    connect(plot, SIGNAL(US_calibration_chip_data(double, double, bool)),       this, SLOT(U_add_calibration_chip_data(double, double, bool)),          Qt::DirectConnection);
    connect(plot, SIGNAL(US_calibration_data(double, double, int, bool)),       this, SLOT(U_add_calibration_data(double, double, int, bool)),          Qt::DirectConnection);
    connect(plot, SIGNAL(US_id_roi_GA_data(UC_plot::UTStr_id_GA_data)),         this, SLOT(U_add_id_roi_GA_data(UC_plot::UTStr_id_GA_data)),            Qt::DirectConnection);
    connect(plot, SIGNAL(US_id_roi_LC_data(UC_plot::UTStr_id_LC_data)),         this, SLOT(U_add_id_roi_LC_data(UC_plot::UTStr_id_LC_data)),            Qt::DirectConnection);
    connect(plot, SIGNAL(US_identification_frame_data(double, double, double)), this, SLOT(U_add_identification_frame_data(double, double, double)),    Qt::DirectConnection);

    connect(plot, SIGNAL(US_replot_spectra()),                                      this, SLOT(U_replot_spectra()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_frame()),                                        this, SLOT(U_replot_frame()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_rewrite_table()),                                       this, SLOT(U_rewrite_table()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_distribution()),                                 this, SLOT(U_replot_distribution()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_calibration_chip()),                             this, SLOT(U_replot_calibration_chip()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_calibration()),                                  this, SLOT(U_replot_calibration()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_identificaton()),                                this, SLOT(U_replot_identification()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_identificaton_frame()),                          this, SLOT(U_replot_identification_frame()), Qt::DirectConnection);

    connect(plot, SIGNAL(US_new_thl(int)),                                          this, SLOT(U_new_thl(int)), Qt::DirectConnection);
    connect(plot, SIGNAL(US_new_thl(int)),                                          this, SLOT(U_new_thl(int)), Qt::DirectConnection);

    connect(this, SIGNAL(US_generate_muxes(int, int)),                              plot, SLOT(U_generate_muxes(int, int)), Qt::DirectConnection);

    connect(plot, SIGNAL(US_scan_settings(UC_data_container::UTStr_data_container_settings)),                              this, SLOT(U_renew_list_model(UC_data_container::UTStr_data_container_settings)), Qt::DirectConnection);

    connect(this, SIGNAL(US_set_smoothing(int)),                              plot, SLOT(U_set_smoothing(int)), Qt::DirectConnection);


    plot->moveToThread(thread);

    //thread->start();

    select = false;
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
void UC_wai::U_set_qcp(QCustomPlot * qcp, QCustomPlot * qcp_2, QCustomPlot * qcp_3) {
    this->qcp = qcp;

    qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    qcp->yAxis->setLabel("value");
    qcp_sr_spectra = new QCPSelectionRect(qcp);
    qcp->setSelectionRect(qcp_sr_spectra);
    connect(qcp->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_spectra_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_spectra_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp, SIGNAL(mouseRelease(QMouseEvent*)),          this, SLOT(U_mouse_select_spectra(QMouseEvent*)));

    this->qcp_2 = qcp_2;

    qcp_2->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    qcp_2->axisRect()->setupFullAxesBox(true);
    qcp_2->xAxis->setLabel("x");
    qcp_2->yAxis->setLabel("y");

    qcp_color_map = new QCPColorMap(qcp_2->xAxis, qcp_2->yAxis);
    color_map_data = new QCPColorMapData(3840, 256, QCPRange(0, 3839), QCPRange(0, 255));
    qcp_color_map->setData(color_map_data, true);

    qcp_color_scale = new QCPColorScale(qcp_2);
    qcp_2->plotLayout()->addElement(0, 1, qcp_color_scale);
    qcp_color_scale->setType(QCPAxis::atRight);
    qcp_color_map->setColorScale(qcp_color_scale);
    qcp_color_scale->axis()->setLabel("count");

    gradient = new QCPColorGradient(QCPColorGradient::gpThermal);
    qcp_color_map->setGradient(*gradient);
    qcp_color_map->setInterpolate(false);

    margin_group = new QCPMarginGroup(qcp_2);
    qcp_2->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group);
    qcp_color_scale->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group);

    qcp_color_map->rescaleDataRange(true);
    qcp_color_scale->rescaleDataRange(true);
    qcp_2->rescaleAxes();
    qcp_2->replot(QCustomPlot::rpQueuedReplot);

    qcp_sr_frame = new QCPSelectionRect(qcp_2);
    qcp_2->setSelectionRect(qcp_sr_frame);

    connect(qcp_2->xAxis, SIGNAL(rangeChanged(QCPRange)),            this, SLOT(U_set_frame_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp_2->yAxis, SIGNAL(rangeChanged(QCPRange)),            this, SLOT(U_set_frame_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp_color_scale->axis(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_frame_z_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp_2, SIGNAL(mouseRelease(QMouseEvent*)),          this, SLOT(U_mouse_select_frame(QMouseEvent*)));


    this->qcp_3 = qcp_3;
    qcp->xAxis->setLabel("value");
    qcp->yAxis->setLabel("pixels");
    qcp_3->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    qcp_sr_distribution = new QCPSelectionRect(qcp_3);
    qcp_3->setSelectionRect(qcp_sr_distribution);

    connect(qcp_3->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_distribution_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp_3->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_distribution_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp_3, SIGNAL(mouseRelease(QMouseEvent*)),          this, SLOT(U_mouse_select_distribution(QMouseEvent*)));
}

void UC_wai::U_set_colibration_qcp(QCustomPlot * chip_fit_qcp, QCustomPlot * calibration_qcp) {
    this->chip_fit_qcp = chip_fit_qcp;

    chip_fit_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    chip_fit_qcp->xAxis->setLabel("thl");
    chip_fit_qcp->yAxis->setLabel("value");
    chip_fit_qcp_sr = new QCPSelectionRect(chip_fit_qcp);
    chip_fit_qcp->setSelectionRect(chip_fit_qcp_sr);
    //chip_fit_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
//    connect(qcp->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_spectra_x_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(qcp->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_spectra_y_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(qcp, SIGNAL(mouseRelease(QMouseEvent*)),          this, SLOT(U_mouse_select_spectra(QMouseEvent*)));

    this->calibration_qcp = calibration_qcp;

    calibration_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    calibration_qcp->xAxis->setLabel("thl");
    calibration_qcp->yAxis->setLabel("energy");
    calibration_qcp_sr = new QCPSelectionRect(calibration_qcp);
    calibration_qcp->setSelectionRect(calibration_qcp_sr);
    //chip_fit_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
//    connect(qcp->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_spectra_x_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(qcp->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_spectra_y_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(qcp, SIGNAL(mouseRelease(QMouseEvent*)),          this, SLOT(U_mouse_select_spectra(QMouseEvent*)));

}

void UC_wai::U_set_identification_qcp(QCustomPlot * id_frame_qcp) {
    this->id_frame_qcp = id_frame_qcp;

    id_frame_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    id_frame_qcp->axisRect()->setupFullAxesBox(true);
    id_frame_qcp->xAxis->setLabel("x");
    id_frame_qcp->yAxis->setLabel("y");

    id_qcp_color_map = new QCPColorMap(id_frame_qcp->xAxis, id_frame_qcp->yAxis);
    id_color_map_data = new QCPColorMapData(3840, 256, QCPRange(0, 3839), QCPRange(0, 255));
    id_qcp_color_map->setData(id_color_map_data, true);

    id_qcp_color_scale = new QCPColorScale(id_frame_qcp);
    id_frame_qcp->plotLayout()->addElement(0, 1, id_qcp_color_scale);
    id_qcp_color_scale->setType(QCPAxis::atRight);
    id_qcp_color_map->setColorScale(id_qcp_color_scale);
    id_qcp_color_scale->axis()->setLabel("count");

    gradient = new QCPColorGradient(QCPColorGradient::gpThermal);
    id_qcp_color_map->setGradient(*gradient);
    id_qcp_color_map->setInterpolate(false);

    id_margin_group = new QCPMarginGroup(id_frame_qcp);
    id_frame_qcp->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, id_margin_group);
    id_qcp_color_scale->setMarginGroup(QCP::msBottom|QCP::msTop, id_margin_group);

    id_qcp_color_map->rescaleDataRange(true);
    id_qcp_color_scale->rescaleDataRange(true);
    id_frame_qcp->rescaleAxes();
    id_frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_table(QTableView * table) {
    this->table = table;

    table_model = new QStandardItemModel(13, 24);
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
    table_index = table_model->index(12, 0);
    table_model->setData(table_index, "identification 1");
    table_index = table_model->index(0, 1);
    table_model->setData(table_index, "chip 1");
    table_index = table_model->index(0, 2);
    table_model->setData(table_index, "chip 2");
    table_index = table_model->index(0, 3);
    table_model->setData(table_index, "chip 3");
    table_index = table_model->index(0, 4);
    table_model->setData(table_index, "chip 4");
    table_index = table_model->index(0, 5);
    table_model->setData(table_index, "chip 5");
    table_index = table_model->index(0, 6);
    table_model->setData(table_index, "chip 6");
    table_index = table_model->index(0, 7);
    table_model->setData(table_index, "chip 7");
    table_index = table_model->index(0, 8);
    table_model->setData(table_index, "chip 8");
    table_index = table_model->index(0, 9);
    table_model->setData(table_index, "chip 9");
    table_index = table_model->index(0, 10);
    table_model->setData(table_index, "chip 10");
    table_index = table_model->index(0, 11);
    table_model->setData(table_index, "chip 11");
    table_index = table_model->index(0, 12);
    table_model->setData(table_index, "chip 12");
    table_index = table_model->index(0, 13);
    table_model->setData(table_index, "chip 13");
    table_index = table_model->index(0, 14);
    table_model->setData(table_index, "chip 14");
    table_index = table_model->index(0, 15);
    table_model->setData(table_index, "chip 15");
    table_index = table_model->index(0, 16);
    table_model->setData(table_index, "widepix");
    table_index = table_model->index(0, 17);
    table_model->setData(table_index, "ROI");
    table_index = table_model->index(0, 18);
    table_model->setData(table_index, "masked");
    table_index = table_model->index(0, 19);
    table_model->setData(table_index, "unmasked");
    table_index = table_model->index(0, 20);
    table_model->setData(table_index, "not overflow");
    table_index = table_model->index(0, 21);
    table_model->setData(table_index, "good pixels in ROI");
    table_index = table_model->index(0, 22);
    table_model->setData(table_index, "roi thl id1 - thl id2");
    table_index = table_model->index(0, 23);
    table_model->setData(table_index, "roi thl id3 - thl id4");
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
    qcp->xAxis->setLabel("thl");
    qcp->addGraph();
    qcp->graph()->setName(graph_name);
    qcp->graph()->setPen(pens[qcp->graphCount() % 16]);
    qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    emit US_generate_spectra();
}

void UC_wai::U_generate_spectra(QString graph_name, double e_min, double e_max, int n) {
    U_delete_mask_plots();
    qcp->xAxis->setLabel("energy");
    qcp->addGraph();
    qcp->graph()->setName(graph_name);
    qcp->graph()->setPen(pens[qcp->graphCount() % 16]);
    qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    emit US_generate_spectra(e_min, e_max, n);
}

void UC_wai::U_delete_last_graph() {
    qcp->removeGraph(qcp->graphCount() - 1);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_reset_spectra() {
    qcp->clearGraphs();
    qcp->replot(QCustomPlot::rpQueuedReplot);
}
///
void UC_wai::U_generate_frame(int thl) {
    emit US_generate_frame(thl);
}

void UC_wai::U_generate_frame(double energy) {
    emit US_generate_frame(energy);
}

void UC_wai::U_generate_table(int thl) {
    emit US_generate_table(thl);
}

void UC_wai::U_generate_table(double energy) {
    emit US_generate_table(energy);
}

void UC_wai::U_generate_table() {
    emit US_generate_table();
}
///
void UC_wai::U_generate_distribution(int n_bins, double min, double max, QString graph_name) {
    U_delete_mask_plots();
    qcp_3->addGraph();
    qcp_3->graph()->setName(graph_name);
    qcp_3->graph()->setPen(pens[qcp_3->graphCount() % 16]);
    qcp_3->graph()->setLineStyle(QCPGraph::lsStepLeft);

    emit US_generate_distribution(n_bins, min, max);
}

void UC_wai::U_generate_frame_distribution(int n_bins, double min, double max, int thl_index, QString graph_name) {
    qcp_3->addGraph();
    qcp_3->graph()->setName(graph_name);
    qcp_3->graph()->setPen(pens[qcp_3->graphCount() % 16]);
    qcp_3->graph()->setLineStyle(QCPGraph::lsStepLeft);

    emit US_generate_frame_distribution(n_bins, min, max, thl_index);
}

void UC_wai::U_delete_last_distribution() {
    qcp_3->removeGraph(qcp_3->graphCount() - 1);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_reset_distribution() {
    qcp_3->clearGraphs();
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
}
///

void UC_wai::U_generate_range() {
    emit US_generate_range();
}

void UC_wai::U_generate_range(int thl_index) {
    emit US_generate_range(thl_index);
}

//

void UC_wai::U_generate_calibration_chip(int chip) {

    chip_fit_qcp->addGraph();
    chip_fit_qcp->graph()->setName(QString("Counter 1 chip %1").arg(chip));
    chip_fit_qcp->graph()->setPen(pens[chip_fit_qcp->graphCount() % 16]);
    chip_fit_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    chip_fit_qcp->addGraph();
    chip_fit_qcp->graph()->setName(QString("Fit chip %1").arg(chip));
    chip_fit_qcp->graph()->setPen(pens[chip_fit_qcp->graphCount() % 16]);
    chip_fit_qcp->graph()->setLineStyle(QCPGraph::lsLine);

    emit US_generate_calibration(chip);
}

void UC_wai::U_reset_calibration_chip() {
    chip_fit_qcp->clearGraphs();
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_generate_calibration() {
    calibration_qcp->clearGraphs();
    QVector<QCPScatterStyle::ScatterShape> shapes;
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

void UC_wai::U_toggle_calibration() {
    calibration_qcp->legend->setVisible(!calibration_qcp->legend->visible());
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}

//

void UC_wai::U_generate_identification_roi(QString id_GA_element, UC_plot::UTE_identification_type type) {
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

    emit US_generate_identification_roi();
}

void UC_wai::U_reset_identification_roi(UC_plot::UTE_identification_type type) {
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

void UC_wai::U_generate_identification_data() {
    emit US_generate_identification_data();
}

void UC_wai::U_generate_identification_frame(int element_index) {
    emit US_generate_identification_frame(element_index);
}

//void UC_wai::U_generate_identification(QString graph_name) {

//    id_qcp->addGraph();
//    id_qcp->graph()->setName(graph_name);
//    id_qcp->graph()->setPen(pens[id_qcp->graphCount() % 16]);
//    id_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

//    id_qcp->addGraph();
//    id_qcp->graph()->setName(QString("Fit ") + graph_name);
//    id_qcp->graph()->setPen(pens[id_qcp->graphCount() % 16]);
//    id_qcp->graph()->setLineStyle(QCPGraph::lsLine);

//    emit US_generate_identification();
//}

//void UC_wai::U_reset_identification() {
//    id_qcp->clearGraphs();
//    id_qcp->replot(QCustomPlot::rpQueuedReplot);
//}





////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_set_frame_type(UC_plot::UTE_frame_type frame_type_in) {
    emit US_set_frame_type(frame_type_in);
}

void UC_wai::U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in) {
    emit US_set_pixel_type(pixel_type_in);
}

void UC_wai::U_set_identification_type(UC_plot::UTE_identification_type identification_type) {
    emit US_set_identification_type(identification_type);
}

void UC_wai::U_set_roi(int x_min, int x_max, int y_min, int y_max) {
    emit US_set_roi(x_min, x_max, y_min, y_max);
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
}

void UC_wai::U_set_rebin(int rebin_x, int rebin_y, int rebin_thl) {
    emit US_set_rebin(rebin_x, rebin_y, rebin_thl);
}

void UC_wai::U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4) {
    emit US_set_thresholds(thl_id_1, thl_id_2, thl_id_3, thl_id_4);
}

void UC_wai::U_set_threshold_range(int thl_min, int thl_max) {
    thl_start = thl_min;
    thl_finish = thl_max;
    emit US_set_threshold_range(thl_min, thl_max);
}

void UC_wai::U_set_data_enable(UC_pixels_info::UTStr_data_enable data_enable) {
    emit US_set_data_enable(data_enable);
}

void UC_wai::U_set_interaction_mode(bool select) {
    this->select = select;
    if (select) {
        qcp->setInteractions(static_cast<QCP::Interaction>(0));
        qcp_2->setInteractions(static_cast<QCP::Interaction>(0));
        qcp_3->setInteractions(static_cast<QCP::Interaction>(0));
        qcp->setSelectionRectMode(QCP::srmCustom);
        qcp_2->setSelectionRectMode(QCP::srmCustom);
        qcp_3->setSelectionRectMode(QCP::srmCustom);
    } else {
        qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        qcp_2->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        qcp_3->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        qcp->setSelectionRectMode(QCP::srmNone);
        qcp_2->setSelectionRectMode(QCP::srmNone);
        qcp_3->setSelectionRectMode(QCP::srmNone);
    }
}

void UC_wai::U_set_x_axis_type(bool view) {
    if (view) {
        qcp->xAxis->setScaleType(QCPAxis::stLinear);
        qcp_3->xAxis->setScaleType(QCPAxis::stLinear);
        chip_fit_qcp->xAxis->setScaleType(QCPAxis::stLinear);
    } else {
        qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
        qcp_3->xAxis->setScaleType(QCPAxis::stLogarithmic);
        chip_fit_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
    }
    qcp->replot(QCustomPlot::rpQueuedReplot);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_y_axis_type(bool view) {
    if (view) {
        qcp->yAxis->setScaleType(QCPAxis::stLinear);
        qcp_3->yAxis->setScaleType(QCPAxis::stLinear);
        chip_fit_qcp->yAxis->setScaleType(QCPAxis::stLinear);
    } else {
        qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
        qcp_3->yAxis->setScaleType(QCPAxis::stLogarithmic);
        chip_fit_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
    }
    qcp->replot(QCustomPlot::rpQueuedReplot);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_renew_ranges(bool enable) {
    renew_renges = enable;
}

void UC_wai::U_set_smoothing(int smoothing) {
    this->smoothing = smoothing;
    US_set_smoothing(smoothing);
}

///////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_resize_spectra() {
    qcp->rescaleAxes(true);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_normalize_spectra() {
    if (qcp->graphCount() == 0) return;
    double x, y, y_max;

    int n_graph = qcp->graphCount();
    for (int i = 0; i < n_graph; i++) {
        int n = qcp->graph(i)->dataCount();
        y_max = 0;
        for (int j = 0; j < n; j++) {
            y = qcp->graph(i)->data()->at(j)->value;
            if (qAbs(y) > y_max) y_max = qAbs(y);
        }

        for (int j = 0; j < n; j++) {
            y = (qcp->graph(i)->data()->at(j)->value / y_max);
            x = qcp->graph(i)->data()->at(j)->key;
            qcp->graph(i)->data()->remove(x);
            qcp->graph(i)->addData(x, y);
        }
    }
    qcp->rescaleAxes(true);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_diff_spectra() {
    if (qcp->graphCount() == 0) return;

    int n_graph = qcp->graphCount();
    for (int i = 0; i < n_graph; i++) {
        int n = qcp->graph(i)->dataCount();
        if (n <= 1) continue;
        QVector<double> x(n), y(n);
        for (int j = 0; j < n; j++) {
            y[j] = qcp->graph(i)->data()->at(j)->value;
            x[j] = qcp->graph(i)->data()->at(j)->key;
        }
        y = U_diff(x, y);

        qcp->graph(i)->data().clear();
        qcp->graph(i)->setData(x, y);
    }
    if (renew_renges) qcp->rescaleAxes(true);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_smooth_spectra() {
    if (qcp->graphCount() == 0) return;

    int n_graph = qcp->graphCount();
    for (int i = 0; i < n_graph; i++) {
        int n = qcp->graph(i)->dataCount();
        if (n <= 1) continue;
        QVector<double> x(n), y(n);
        for (int j = 0; j < n; j++) {
            y[j] = qcp->graph(i)->data()->at(j)->value;
            x[j] = qcp->graph(i)->data()->at(j)->key;
        }
        y = U_smooth(x, y, smoothing);

        qcp->graph(i)->data().clear();
        qcp->graph(i)->setData(x, y);
    }
    if (renew_renges) qcp->rescaleAxes(true);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_resize_frame() {
    qcp_2->xAxis->setRange(x_min, x_max);
    qcp_2->yAxis->setRange(y_min, y_max);
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_rescale_frame() {
    QCPRange xrange = qcp_2->xAxis->range();
    QCPRange yrange = qcp_2->yAxis->range();
    double min = 99999999999999;
    double max = -99999999999;
    double z = 0;
    for (int x = static_cast<int>(xrange.lower); x < xrange.upper; x++) {
        for (int y = static_cast<int>(yrange.lower); y < yrange.upper; y++) {
            z = qcp_color_map->data()->cell(x, y);
            if (z < min) min = z;
            if (z > max) max = z;
        }
    }
    qcp_color_scale->axis()->setRange(min, max);
}

void UC_wai::U_resize_distribution() {
    qcp_3->rescaleAxes(true);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_resize_calibration_chip() {
    chip_fit_qcp->rescaleAxes(true);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_resize_calibration() {
    calibration_qcp->rescaleAxes(true);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
}
///////////////////////////////////////////////////////////////////
void UC_wai::U_save_frame_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int y = 0; y < 255; y++) {
        for (int x = 0; x < 3839; x++) {
            str << qcp_color_map->data()->data(x, y) << " ";
        }
        str << qcp_color_map->data()->data(3839, y) << endl;
    }
    for (int x = 0; x < 3839; x++) {
        str << qcp_color_map->data()->data(x, 255) << " ";
    }
    str << qcp_color_map->data()->data(3839, 255);
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
            qcp_2->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            qcp_2->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            qcp_2->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            qcp_2->saveBmp(file_name + ".bmp");
            break;
        }
    }
}
////
void UC_wai::U_save_spectra_txt(QString file_name) {
    if (qcp->graphCount() == 0) return;
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    int n_graph = qcp->graphCount();
    str << n_graph << endl;
    for (int i = 0; i < n_graph; i++) {
        str << qcp->graph(i)->name() << endl;
        int n = qcp->graph(i)->dataCount();
        str << n << endl;
        for (int j = 0; j < n; j++) {
            str << qcp->graph(i)->data()->at(j)->key << " ";
        }
        str << endl;
        for (int j = 0; j < n; j++) {
            str << qcp->graph(i)->data()->at(j)->value << " ";
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
            qcp->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            qcp->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            qcp->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            qcp->saveBmp(file_name + ".bmp");
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
    if (qcp->graphCount() == 0) return;
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    int n_graph = qcp_3->graphCount();
    str << n_graph << endl;
    for (int i = 0; i < n_graph; i++) {
        int n = qcp_3->graph(i)->dataCount();
        str << n << endl;
        for (int j = 0; j < n; j++) {
            str << qcp_3->graph(i)->data()->at(j)->key << " ";
        }
        str << endl;
        for (int j = 0; j < n; j++) {
            str << qcp_3->graph(i)->data()->at(j)->value << " ";
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
            qcp_3->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            qcp_3->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            qcp_3->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            qcp_3->saveBmp(file_name + ".bmp");
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
        qcp->addGraph();
        qcp->graph()->setName(graph_name);
        qcp->graph()->setPen(pens[qcp->graphCount() % 16]);
        qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
        qcp->graph()->setData(x, y);
    }
    qcp->legend->setVisible(true);
    qcp->rescaleAxes(true);
    qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_spectra(QMouseEvent*)));
    emit US_set_distribution_range(qcp->yAxis->range().lower, qcp->yAxis->range().upper);
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
            qcp_color_map->data()->setData(x, y, z);
        }
    }
    qcp_2->xAxis->setRange(x_min, x_max);
    qcp_2->yAxis->setRange(y_min, y_max);
    U_rescale_frame();
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp_2, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_frame(QMouseEvent*)));
    emit US_set_distribution_range(qcp_color_scale->axis()->range().lower, qcp_color_scale->axis()->range().upper);
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
        qcp_3->addGraph();
        qcp_3->graph()->setName(graph_name);
        qcp_3->graph()->setPen(pens[qcp_3->graphCount() % 16]);
        qcp_3->graph()->setLineStyle(QCPGraph::lsStepLeft);
        qcp_3->graph()->setData(x, y);
    }
    qcp_3->legend->setVisible(true);
    qcp_3->rescaleAxes(true);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp_3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_distribution(QMouseEvent*)));
    file.close();
}
//

void UC_wai::U_save_calibration_chip(QString file_name, UTE_file_type file_type) {
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_FT_txt : {
            U_save_calibration_chip_txt(file_name + ".txt");
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

void UC_wai::U_save_calibration_chip_txt(QString file_name) {
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
//
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

//////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_set_mask(bool mask, bool more, double value, bool in_roi, int thl) {
    emit US_set_mask(mask, more, value, in_roi, thl);
}

void UC_wai::U_count_mask(bool more, double value, bool in_roi, int thl) {
    emit US_count_mask(more, value, in_roi, thl);
}

void UC_wai::U_set_mask_plots(double value) {
    if (mask_tick) return;
    qcp_3->addGraph();
    qcp_3->graph()->setPen(QPen(Qt::black));
    qcp_3->graph()->setLineStyle(QCPGraph::lsImpulse);
    qcp_3->graph()->addData(value, qcp_3->yAxis->range().upper);
    qcp_3->graph()->setName("Mask");
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
    qcp->addGraph();
    qcp->graph()->setPen(QPen(Qt::black));
    qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
    qcp->graph()->addData(qcp->xAxis->range().lower, value);
    qcp->graph()->addData(qcp->xAxis->range().upper, value);
    qcp->graph()->setName("Mask");
    qcp->replot(QCustomPlot::rpQueuedReplot);
    mask_tick = true;
}

void UC_wai::U_delete_mask_plots() {
    if (!mask_tick) return;
    qcp_3->removeGraph(qcp_3->graphCount() - 1);
    qcp->removeGraph(qcp->graphCount() - 1);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
    qcp->replot(QCustomPlot::rpQueuedReplot);
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
//
//

void UC_wai::U_add_roi(int x_min, int x_max, int y_min, int y_max) {
    list_roi.append(QRect(x_min, y_min, x_max - x_min, y_max - y_min));
}

QRect UC_wai::U_get_roi(int index) {
    return list_roi[index];
}

void UC_wai::U_delete_roi(int index) {
    list_roi.removeAt(index);
}

//

void UC_wai::U_calculating_spectra(UTE_calculating_spectras calc_type, int index1, int index2) {
    int n_graph = qcp->graphCount();
    if (n_graph < 2) return;
    int n1 = qcp->graph(index1)->dataCount();
    QVector<double> x1(n1), y1(n1);
    for (int i = 0; i < n1; i++) {
        y1[i] = qcp->graph(index1)->data()->at(i)->value;
        x1[i] = qcp->graph(index1)->data()->at(i)->key;
    }
    int n2 = qcp->graph(index2)->dataCount();
    QVector<double> x2(n2), y2(n2);
    for (int i = 0; i < n2; i++) {
        y2[i] = qcp->graph(index2)->data()->at(i)->value;
        x2[i] = qcp->graph(index2)->data()->at(i)->key;
    }

    QVector<double> x, y;
    for (double thl = thl_start; thl <= thl_finish; thl++) {
        int i1 = x1.indexOf(thl);
        int i2 = x2.indexOf(thl);
        double data1 = 0;
        if (i1 != -1) data1 = y1[i1];
        double data2 = 0;
        if (i2 != -1) data2 = y2[i2];
        x << thl;
        double data;
        switch (calc_type) {
            case UTE_CS_add : {
                data = U_calculating_spectra_1(data1, data2);
                break;
            }
            case UTE_CS_multiplay : {
                data = U_calculating_spectra_2(data1, data2);
                break;
            }
            case UTE_CS_subtract : {
                data = U_calculating_spectra_3(data1, data2);
                break;
            }
            case UTE_CS_divide : {
                data = U_calculating_spectra_4(data1, data2);
                break;
            }
        }
        y << data;
    }

    qcp->addGraph();
    QString name = qcp->graph(index1)->name();
    switch (calc_type) {
        case UTE_CS_add : {
            name += " + ";
            break;
        }
        case UTE_CS_multiplay : {
            name += " * ";
            break;
        }
        case UTE_CS_subtract : {
            name += " - ";
            break;
        }
        case UTE_CS_divide : {
            name += " / ";
            break;
        }
    }
    name += qcp->graph(index2)->name();
    emit US_new_spectra(name);
    qcp->graph()->setName(name);
    qcp->graph()->setPen(pens[qcp->graphCount() % 16]);
    qcp->graph()->setData(x, y);

    if (renew_renges) qcp->rescaleAxes(true);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

double UC_wai::U_calculating_spectra_1(double data1, double data2) {
    return data1 + data2;
}

double UC_wai::U_calculating_spectra_2(double data1, double data2) {
    return data1 * data2;
}

double UC_wai::U_calculating_spectra_3(double data1, double data2) {
    return data1 - data2;
}

double UC_wai::U_calculating_spectra_4(double data1, double data2) {
    if (qAbs(data2) < 1e-10) return 0;
        else return data1 / data2;
}


//////////////////////////!!!!!!!!!!!SLOTS!!!!!!!!!!!!!!!!!!!!!!!///////////////////////////////////////
void UC_wai::U_add_spectra_data(double x, double y) {
    qcp->graph()->addData(x, y);
}

void UC_wai::U_add_frame_data(double x, double y, double z) {
    qcp_color_map->data()->setData(x, y, z);
}

void UC_wai::U_add_table_data(UC_pixels_info pixels_info) {
    int column_index = static_cast<int>(pixels_info.U_get_pixel_area()) + 1;
    table_index = table_model->index(1, column_index);
    table_model->setData(table_index, pixels_info.U_get_n());
    table_index = table_model->index(2, column_index);
    table_model->setData(table_index, pixels_info.U_get_min());
    table_index = table_model->index(3, column_index);
    table_model->setData(table_index, pixels_info.U_get_max());
    table_index = table_model->index(4, column_index);
    table_model->setData(table_index, pixels_info.U_get_sum());
    table_index = table_model->index(5, column_index);
    table_model->setData(table_index, pixels_info.U_get_mean());
    table_index = table_model->index(6, column_index);
    table_model->setData(table_index, pixels_info.U_get_median());
    table_index = table_model->index(7, column_index);
    table_model->setData(table_index, pixels_info.U_get_zeros());
    table_index = table_model->index(8, column_index);
    table_model->setData(table_index, pixels_info.U_get_overflows());
    table_index = table_model->index(9, column_index);
    table_model->setData(table_index, pixels_info.U_get_masked());
    table_index = table_model->index(10, column_index);
    table_model->setData(table_index, pixels_info.U_get_std_dev());
    table_index = table_model->index(11, column_index);
    table_model->setData(table_index, pixels_info.U_get_snr());
    table_index = table_model->index(12, column_index);
    table_model->setData(table_index, pixels_info.U_get_id_1_data());
}

void UC_wai::U_add_distibution_data(double x, double y) {
    qcp_3->graph()->addData(x, y);
}

void UC_wai::U_add_calibration_chip_data(double x, double y, bool is_fit) {
    if (is_fit) {
        chip_fit_qcp->graph(chip_fit_qcp->graphCount() - 1)->addData(x, y);
    } else {
        chip_fit_qcp->graph(chip_fit_qcp->graphCount() - 2)->addData(x, y);
    }
}

void UC_wai::U_add_calibration_data(double x, double y, int chip, bool is_fit) {
    if (is_fit) {
        calibration_qcp->graph(chip * 2 + 1)->addData(x, y);
    } else {
        calibration_qcp->graph(chip * 2)->addData(x, y);
    }
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

void UC_wai::U_add_identification_frame_data(double x, double y, double z) {
    id_qcp_color_map->data()->setData(x, y, z);
}

//
//void UC_wai::U_add_identification_data(double x, double y, bool is_fit) {
//    if (is_fit) {
//        id_qcp->graph(id_qcp->graphCount() - 1)->addData(x, y);
//    } else {
//        id_qcp->graph(id_qcp->graphCount() - 2)->addData(x, y);
//    }
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_replot_frame() {
    if (renew_renges) qcp_2->xAxis->setRange(x_min, x_max);
    if (renew_renges) qcp_2->yAxis->setRange(y_min, y_max);
    if (renew_renges) U_rescale_frame();
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp_2, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_frame(QMouseEvent*)));
    emit US_set_distribution_range(qcp_color_scale->axis()->range().lower, qcp_color_scale->axis()->range().upper);
}

void UC_wai::U_replot_spectra() {
    qcp->legend->setVisible(true);
    if (renew_renges) qcp->rescaleAxes(true);
    qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_spectra(QMouseEvent*)));
    emit US_set_distribution_range(qcp->yAxis->range().lower, qcp->yAxis->range().upper);
}

void UC_wai::U_rewrite_table() {

}

void UC_wai::U_replot_distribution() {
    qcp_3->legend->setVisible(true);
    if (renew_renges) qcp_3->rescaleAxes(true);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp_3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_distribution(QMouseEvent*)));
}

void UC_wai::U_replot_calibration_chip() {
    chip_fit_qcp->legend->setVisible(true);
    if (renew_renges) chip_fit_qcp->rescaleAxes(true);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
    //connect(qcp_3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_distribution(QMouseEvent*)));
}

void UC_wai::U_replot_calibration() {
    calibration_qcp->legend->setVisible(true);
    if (renew_renges) calibration_qcp->rescaleAxes(true);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
    //connect(qcp_3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_distribution(QMouseEvent*)));
}

void UC_wai::U_replot_identification_frame() {
    if (renew_renges) id_frame_qcp->xAxis->setRange(x_min, x_max);
    if (renew_renges) id_frame_qcp->yAxis->setRange(y_min, y_max);
    //if (renew_renges) U_rescale_frame();
    id_frame_qcp->replot(QCustomPlot::rpQueuedReplot);
//    connect(qcp_2, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_frame(QMouseEvent*)));
//    emit US_set_distribution_range(qcp_color_scale->axis()->range().lower, qcp_color_scale->axis()->range().upper);
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
    qcp->xAxis->setRange(qcp->xAxis->range().lower, value);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_min_x(double value) {
    qcp->xAxis->setRange(value, qcp->xAxis->range().upper);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_max_y(double value) {
    qcp->yAxis->setRange(qcp->yAxis->range().lower, value);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_min_y(double value) {
    qcp->yAxis->setRange(value, qcp->yAxis->range().upper);
    qcp->replot(QCustomPlot::rpQueuedReplot);
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
    qcp_2->xAxis->setRange(qcp_2->xAxis->range().lower, value);
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_min_x(double value) {
    qcp_2->xAxis->setRange(value, qcp_2->xAxis->range().upper);
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_max_y(double value) {
    qcp_2->yAxis->setRange(qcp_2->yAxis->range().lower, value);
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_min_y(double value) {
    qcp_2->yAxis->setRange(value, qcp_2->yAxis->range().upper);
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_max_z(double value) {
    qcp_color_scale->axis()->setRange(qcp_color_scale->axis()->range().lower, value);
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_frame_min_z(double value) {
    qcp_color_scale->axis()->setRange(value, qcp_color_scale->axis()->range().upper);
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
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
    qcp_3->xAxis->setRange(qcp_3->xAxis->range().lower, value);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_distribution_min_x(double value) {
    qcp_3->xAxis->setRange(value, qcp_3->xAxis->range().upper);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_distribution_max_y(double value) {
    qcp_3->yAxis->setRange(qcp_3->yAxis->range().lower, value);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_distribution_min_y(double value) {
    qcp_3->yAxis->setRange(value, qcp_3->yAxis->range().upper);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_mouse_move_spectra(QMouseEvent * event) {
    int n = qcp->graphCount();
    if (n == 0) return;

    QCPGraphDataContainer::const_iterator it = qcp->graph(0)->data()->constEnd();
    QVariant details;
    double data = qcp->graph(0)->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) it = qcp->graph(0)->data()->at(dataPoints.dataRange().begin());
    }
    QString str = "thl = %1;" + QString("\n") + "%2 = %3;";
    str = str.arg(static_cast<int>(it->key)).arg(qcp->graph(0)->name()).arg(it->value);

    for (int i = 1; i < n; i++) {
        QCPGraphDataContainer::const_iterator it = qcp->graph(i)->data()->constEnd();
        QVariant details;
        double data = qcp->graph(i)->selectTest(event->pos(), false, &details);
        if (data > 0) {
            QCPDataSelection dataPoints = details.value<QCPDataSelection>();
            if (dataPoints.dataPointCount() > 0) it = qcp->graph(i)->data()->at(dataPoints.dataRange().begin());
        }
        str += "\n" + QString("%1 = %2;").arg(qcp->graph(i)->name()).arg(it->value);

    };
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, qcp);
}

void UC_wai::U_mouse_move_frame(QMouseEvent * event) {
    QVariant details;
    double data = qcp_color_map->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) {
            double x = qcp_2->xAxis->pixelToCoord(event->pos().x());
            double y = qcp_2->yAxis->pixelToCoord(event->pos().y());
            int cell_x, cell_y;
            qcp_color_map->data()->coordToCell(x, y, &cell_x, &cell_y);
            double z = qcp_color_map->data()->cell(cell_x, cell_y);
            QString str = "x = %1; y = %2; z = %3";
            US_mouse_data(str.arg(x).arg(y).arg(z));
            QToolTip::showText(event->globalPos(), str.arg(x).arg(y).arg(z), qcp_2);
        }
    }
}

void UC_wai::U_mouse_move_distribution(QMouseEvent * event) {
    int n = qcp_3->graphCount();
    if (n == 0) return;

    QCPGraphDataContainer::const_iterator it = qcp_3->graph(0)->data()->constEnd();
    QVariant details;
    double data = qcp_3->graph(0)->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) it = qcp_3->graph(0)->data()->at(dataPoints.dataRange().begin());
    }
    QString str = "bin = %1;" + QString("\n") + "%2 = %3;";
    str = str.arg(it->key).arg(qcp_3->graph(0)->name()).arg(it->value);

    for (int i = 1; i < n; i++) {
        QCPGraphDataContainer::const_iterator it = qcp_3->graph(i)->data()->constEnd();
        QVariant details;
        double data = qcp_3->graph(i)->selectTest(event->pos(), false, &details);
        if (data > 0) {
            QCPDataSelection dataPoints = details.value<QCPDataSelection>();
            if (dataPoints.dataPointCount() > 0) it = qcp_3->graph(i)->data()->at(dataPoints.dataRange().begin());
        }
        str += "\n" + QString("%1 = %2;").arg(qcp_3->graph(i)->name()).arg(it->value);

    };
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, qcp_3);
}
////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_mouse_select_spectra(QMouseEvent * event) {
    if (!select) return;
    int xmin, xmax, ymin, ymax;
    xmax = static_cast<int>(std::round(qcp_sr_spectra->range(qcp->xAxis).upper));
    xmin = static_cast<int>(std::round(qcp_sr_spectra->range(qcp->xAxis).lower));
    value_max = qcp_sr_spectra->range(qcp->yAxis).upper;
    value_min = qcp_sr_spectra->range(qcp->yAxis).lower;
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
    xmax = static_cast<int>(std::round(qcp_sr_frame->range(qcp_2->xAxis).upper));
    xmin = static_cast<int>(std::round(qcp_sr_frame->range(qcp_2->xAxis).lower));
    ymax = static_cast<int>(std::round(qcp_sr_frame->range(qcp_2->yAxis).upper));
    ymin = static_cast<int>(std::round(qcp_sr_frame->range(qcp_2->yAxis).lower));
    emit US_set_roi_range(xmin, xmax, ymin, ymax);
}

void UC_wai::U_mouse_select_distribution(QMouseEvent * event) {
    if (!select) return;
    double xmin, xmax;
    xmax = qcp_sr_distribution->range(qcp_3->xAxis).upper;
    xmin = qcp_sr_distribution->range(qcp_3->xAxis).lower;
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
