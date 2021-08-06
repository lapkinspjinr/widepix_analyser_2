#include "uc_wai.h"

UC_wai::UC_wai(QObject *parent) : QObject(parent)
{
    x_min = 0;
    x_max = 15 * 256;
    y_min = 0;
    y_max = 256;
    thl_start = 0;
    thl_finish = 512;

    smoothing = 1;

    select = false;
    legend_enable = true;
    renew_renges = true;
    renew_roi_renges = true;
    mask_tick = false;
    energy_spectra = false;

    plot = new UC_plot();
    thread = new QThread();

    gradient = new QCPColorGradient(QCPColorGradient::gpThermal);

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

   connect(plot, SIGNAL(US_list_scans(QList<UC_data_container> *, int)),                           this, SLOT(U_renew_scans(QList<UC_data_container> *, int))/*,                         Qt::DirectConnection*/);


    connect(this, SIGNAL(US_generate_spectra()),                                    plot, SLOT(U_generate_spectra())/*,                                       Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_spectra_direct()),                             plot, SLOT(U_generate_spectra()),                                       Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_spectra(int)),                                 plot, SLOT(U_generate_spectra(int))/*,                                    Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_frame(int)),                                   plot, SLOT(U_generate_frame(int))/*,                                      Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_frame(double)),                                plot, SLOT(U_generate_frame(double))/*,                                   Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_table(int)),                                   plot, SLOT(U_generate_table(int))/*,                                      Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_table(double)),                                plot, SLOT(U_generate_table(double))/*,                                   Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_table()),                                      plot, SLOT(U_generate_table())/*,                                         Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_distribution(int, double, double)),            plot, SLOT(U_generate_distribution(int, double, double))/*,               Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_distribution(int, double, double, int)),       plot, SLOT(U_generate_frame_distribution(int, double, double, int))/*,    Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_distribution_direct(int, double, double)),     plot, SLOT(U_generate_distribution(int, double, double)),               Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_distribution_direct(int, double, double, int)),plot, SLOT(U_generate_frame_distribution(int, double, double, int)),    Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_chip_fit(int)),                                plot, SLOT(U_generate_calibration(int))/*,                                Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_calibration()),                                plot, SLOT(U_generate_calibration())/*,                                   Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_spectra_2d()),                                 plot, SLOT(U_generate_spectra_2d())/*,                                      Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_spectra_2d_direct()),                          plot, SLOT(U_generate_spectra_2d()),                            Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_spectra_2d(double, double)),                   plot, SLOT(U_generate_spectra_2d(double, double))/*,                                      Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_id_roi()),                                     plot, SLOT(U_generate_id_roi()),                                        Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_additional_data()),                            plot, SLOT(U_generate_additional_data()),                               Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_id_data()),                                    plot, SLOT(U_generate_id_data()),                                       Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_id_frame(int)),                                plot, SLOT(U_generate_id_frame(int)),                                   Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_range()),                                      plot, SLOT(U_generate_range())/*,                                         Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_range(int)),                                   plot, SLOT(U_generate_range(int))/*,                                      Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_spectra_2d_range(int, int)),                   plot, SLOT(U_generate_range_spectra_2d(int, int))/*,                                         Qt::DirectConnection*/);
    connect(this, SIGNAL(US_generate_spectra_2d_range(double, double)),             plot, SLOT(U_generate_range_spectra_2d(double, double))/*,                                      Qt::DirectConnection*/);

    connect(this, SIGNAL(US_set_frame_type(UC_plot::UTE_frame_type)),                   plot, SLOT(U_set_frame_type(UC_plot::UTE_frame_type)),  Qt::DirectConnection);
    connect(this, SIGNAL(US_set_pixel_type(UC_plot::UTE_pixel_type)),                   plot, SLOT(U_set_pixel_type(UC_plot::UTE_pixel_type)),  Qt::DirectConnection);
    connect(this, SIGNAL(US_set_roi(int, int, int, int)),                               plot, SLOT(U_set_roi(int, int, int, int)),              Qt::DirectConnection);
    connect(this, SIGNAL(US_set_rebin(int, int, int)),                                  plot, SLOT(U_set_rebin(int, int, int)),                 Qt::DirectConnection);
    connect(this, SIGNAL(US_set_id_type(UC_plot::UTE_id_type)),                         plot, SLOT(U_set_id_type(UC_plot::UTE_id_type)),        Qt::DirectConnection);
    connect(this, SIGNAL(US_set_thresholds(int, int, int, int)),                        plot, SLOT(U_set_id_thresholds(int, int, int, int)),    Qt::DirectConnection);
    connect(this, SIGNAL(US_set_thl_range(int, int)),                                   plot, SLOT(U_get_thl_index_range(int, int)),            Qt::DirectConnection);
    connect(this, SIGNAL(US_set_smoothing(int)),                                        plot, SLOT(U_set_smoothing(int)),                       Qt::DirectConnection);

    connect(this, SIGNAL(US_set_mask(bool, bool, double, bool, int)),               plot, SLOT(U_set_mask(bool, bool, double, bool, int)),              Qt::DirectConnection);
    connect(this, SIGNAL(US_count_mask(bool, double, bool, int)),                   plot, SLOT(U_count_mask(bool, double, bool, int)),                  Qt::DirectConnection);
    connect(this, SIGNAL(US_set_mask_overflowed(bool, int)),                        plot, SLOT(U_set_mask_overflowed(bool, int)),                       Qt::DirectConnection);
    connect(this, SIGNAL(US_set_mask_overflowed(bool)),                             plot, SLOT(U_set_mask_overflowed(bool)),                            Qt::DirectConnection);
    connect(this, SIGNAL(US_reset_mask()),                                          plot, SLOT(U_reset_mask()),                                         Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected(int, int, int, int)),                     plot, SLOT(U_mask_selected(int, int, int, int)),                    Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected_value(double, double, bool)),             plot, SLOT(U_mask_selected_value(double, double, bool)),            Qt::DirectConnection);
    connect(this, SIGNAL(US_mask_selected_value(double, double, bool, int)),        plot, SLOT(U_mask_selected_value(double, double, bool, int)),       Qt::DirectConnection);
    connect(this, SIGNAL(US_save_mask(QString)),                                    plot, SLOT(U_save_mask(QString)),                                   Qt::DirectConnection);
    connect(this, SIGNAL(US_load_mask(QString)),                                    plot, SLOT(U_load_mask(QString)),                                   Qt::DirectConnection);

    connect(plot, SIGNAL(US_spectra_data(double, double)),                      this, SLOT(U_add_spectra_data(double, double)),                 Qt::DirectConnection);
    connect(plot, SIGNAL(US_frame_data(double, double, double)),                this, SLOT(U_add_frame_data(double, double, double)),           Qt::DirectConnection);
    connect(plot, SIGNAL(US_chip_data(UC_pixels_info)),                         this, SLOT(U_add_table_data(UC_pixels_info)),                   Qt::DirectConnection);
    connect(plot, SIGNAL(US_spectra_2d_data(double, double)),                   this, SLOT(U_add_spectra_2d_data(double, double)),              Qt::DirectConnection);
    connect(plot, SIGNAL(US_id_roi_GA_data(UC_plot::UTStr_id_GA_data)),         this, SLOT(U_add_id_roi_GA_data(UC_plot::UTStr_id_GA_data)),    Qt::DirectConnection);
    connect(plot, SIGNAL(US_id_roi_LC_data(UC_plot::UTStr_id_LC_data)),         this, SLOT(U_add_id_roi_LC_data(UC_plot::UTStr_id_LC_data)),    Qt::DirectConnection);
    connect(plot, SIGNAL(US_id_frame_data(double, double, double)),             this, SLOT(U_add_id_frame_data(double, double, double)),        Qt::DirectConnection);

    connect(plot, SIGNAL(US_replot_spectra(QVector<double>, QVector<double>)),                  this, SLOT(U_replot_spectra(QVector<double>, QVector<double>)),                 Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_spectra()),                                                  this, SLOT(U_replot_spectra()),                                                 Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_frame()),                                                    this, SLOT(U_replot_frame()),                                                   Qt::DirectConnection);
    connect(plot, SIGNAL(US_rewrite_table()),                                                   this, SLOT(U_rewrite_table()),                                                  Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_distribution(QVector<double>, QVector<double>)),             this, SLOT(U_replot_distribution(QVector<double>, QVector<double>)),            Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_calibration_chip(QVector<double>, QVector<double>, bool)),   this, SLOT(U_replot_chip_fit(QVector<double>, QVector<double>, bool)),          Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_calibration(QVector<double>, QVector<double>, int, bool)),   this, SLOT(U_replot_calibration(QVector<double>, QVector<double>, int, bool)),  Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_spectra_2d()),                                               this, SLOT(U_replot_spectra_2d()),                                              Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_id_frame()),                                                 this, SLOT(U_replot_id_frame()),                                                Qt::DirectConnection);

    connect(this, SIGNAL(US_add_roi(UC_roi)),   plot, SLOT(U_add_roi(UC_roi)), Qt::DirectConnection);
    connect(this, SIGNAL(US_delete_roi(int)),   plot, SLOT(U_delete_roi(int)), Qt::DirectConnection);

    connect(this, SIGNAL(US_set_settings(int, UC_data_container::UTStr_data_container_settings *)), plot, SLOT(U_set_settings(int, UC_data_container::UTStr_data_container_settings *)),    Qt::DirectConnection);
    connect(this, SIGNAL(US_set_scan(int)),                                                         plot, SLOT(U_set_scan(int)),                                                            Qt::DirectConnection);
    connect(this, SIGNAL(US_delete_scan(int)),                                                      plot, SLOT(U_delete_scan(int)),                                                         Qt::DirectConnection);


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

    spectra_qcp->legend->setSelectedParts(QCPLegend::spItems);

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
    connect(chip_fit_qcp,           SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_chip_fit(QMouseEvent*)));
}

void UC_wai::U_set_calibration_qcp(QCustomPlot * calibration_qcp) {
    this->calibration_qcp = calibration_qcp;

    calibration_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    calibration_qcp->xAxis->setLabel("thl");
    calibration_qcp->yAxis->setLabel("energy");
    sr_calibration_qcp = new QCPSelectionRect(calibration_qcp);
    calibration_qcp->setSelectionRect(sr_calibration_qcp);

    connect(calibration_qcp->xAxis, SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_calibration_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(calibration_qcp->yAxis, SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_calibration_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(calibration_qcp,        SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_calibration(QMouseEvent*)));
}

void UC_wai::U_set_spectra_2d_qcp(QCustomPlot * spectra_2d_qcp) {
    this->spectra_2d_qcp = spectra_2d_qcp;

    spectra_2d_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    spectra_2d_qcp->axisRect()->setupFullAxesBox(true);

    sr_spectra_2d_qcp = new QCPSelectionRect(spectra_2d_qcp);
    spectra_2d_qcp->setSelectionRect(sr_spectra_2d_qcp);
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
}

void UC_wai::U_set_table(QTableView * table) {
    this->table = table;
    QStringList headers;

    table_model = new QStandardItemModel(0, 14);
    headers << "Name of area";
    headers << "Number of pixel";
    headers << "Minimum";
    headers << "Maximum";
    headers << "Sum";
    headers << "Mean";
    headers << "Median";
    headers << "Zeros";
    headers << "Overflows";
    headers << "Masked";
    headers << "Standard deviation";
    headers << "Signal to noise resolution";
    headers << "Maximum density";
    headers << "Variance";
    headers << "x min";
    headers << "x max";
    headers << "y min";
    headers << "y max";
    table_model->setHorizontalHeaderLabels(headers);
    table->setModel(table_model);

    table->resizeColumnsToContents();
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

void UC_wai::U_set_table_widget(QTableWidget * table_widget) {
    this->table_widget = table_widget;
    table_widget->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    table_widget->setHorizontalHeaderItem(1, new QTableWidgetItem("Path"));
    table_widget->setHorizontalHeaderItem(2, new QTableWidgetItem("Flat field"));
    table_widget->setHorizontalHeaderItem(3, new QTableWidgetItem("Dark_field"));
    table_widget->setHorizontalHeaderItem(4, new QTableWidgetItem("Count"));
    table_widget->setHorizontalHeaderItem(5, new QTableWidgetItem("Time"));
    table_widget->setHorizontalHeaderItem(6, new QTableWidgetItem("Counters"));
    table_widget->setHorizontalHeaderItem(7, new QTableWidgetItem("N files"));
    table_widget->setHorizontalHeaderItem(8, new QTableWidgetItem("Calibration"));
    table_widget->setHorizontalHeaderItem(9, new QTableWidgetItem("Energy"));
    table_widget->setHorizontalHeaderItem(10, new QTableWidgetItem("Identification"));
    table_widget->setHorizontalHeaderItem(11, new QTableWidgetItem("THL"));
    table_widget->setHorizontalHeaderItem(12, new QTableWidgetItem("Active"));
    table_widget->setHorizontalHeaderItem(13, new QTableWidgetItem("Delete"));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_reset_data() {
    int n = table_widget->rowCount();
    for (int i = 0; i < n; i++) {
        table_widget->removeRow(0);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_generate_spectra(QString graph_name) {
    if (energy_spectra) {
        spectra_qcp->clearGraphs();
        energy_spectra = false;
    }
    U_delete_mask_plots();
    spectra_qcp->xAxis->setLabel("thl");
    spectra_qcp->addGraph();
    spectra_qcp->graph()->setName(graph_name);
    spectra_qcp->graph()->setPen(pens[spectra_qcp->graphCount() % 16]);
    spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    emit US_generate_spectra();
}

void UC_wai::U_generate_spectra(QString graph_name, int n) {
    if (!energy_spectra) {
        spectra_qcp->clearGraphs();
        energy_spectra = true;
    }
    U_delete_mask_plots();
    spectra_qcp->xAxis->setLabel("energy");
    spectra_qcp->addGraph();
    spectra_qcp->graph()->setName(graph_name);
    spectra_qcp->graph()->setPen(pens[spectra_qcp->graphCount() % 16]);
    spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
    emit US_generate_spectra(n);
}

void UC_wai::U_delete_last_spectra() {
    bool deleted = false;
    int n = spectra_qcp->legend->itemCount();
    if (n == 0) return;
    QCPAbstractLegendItem * item;
    for (int i = 0; i < n; i++) {
        item = spectra_qcp->legend->item(i);
        if (item->selected()) {
            spectra_qcp->removeGraph(i);
            deleted = true;
            break;
        }
    }
    if (!deleted) {
        spectra_qcp->removeGraph(spectra_qcp->graphCount() - 1);
    }
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
    table_model->removeRows(0, table_model->rowCount());
    emit US_generate_table(thl);
}

void UC_wai::U_generate_table(double energy) {
    table_model->removeRows(0, table_model->rowCount());
    emit US_generate_table(energy);
}

void UC_wai::U_generate_table() {
    table_model->removeRows(0, table_model->rowCount());
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
    distribution_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    emit US_generate_distribution(n_bins, min, max, thl_index);
}

void UC_wai::U_delete_last_distribution() {
    bool deleted = false;
    int n = distribution_qcp->legend->itemCount();
    if (n == 0) return;
    QCPAbstractLegendItem * item;
    for (int i = 0; i < n; i++) {
        item = distribution_qcp->legend->item(i);
        if (item->selected()) {
            distribution_qcp->removeGraph(i);
            deleted = true;
            break;
        }
    }
    if (!deleted) {
        distribution_qcp->removeGraph(spectra_qcp->graphCount() - 1);
    }
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

void UC_wai::U_generate_chip_fit(int chip, QString graph_name) {
    chip_fit_qcp->addGraph();
    chip_fit_qcp->graph()->setName(graph_name + QString("chip %1").arg(chip + 1));
    chip_fit_qcp->graph()->setPen(pens[chip_fit_qcp->graphCount() % 16]);
    chip_fit_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    chip_fit_qcp->addGraph();
    chip_fit_qcp->graph()->setName(QString("Fit; ") + graph_name + QString("chip %1").arg(chip + 1));
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

void UC_wai::U_generate_spectra_2d(UStr_spectra_2d_settings settings) {
    spectra_2d_settings = settings;
    spectra_2d_qcp->clearPlottables();
    color_map_spectra_2d_qcp = new QCPColorMap(spectra_2d_qcp->xAxis, spectra_2d_qcp->yAxis);
    color_map_data_spectra_2d_qcp = new QCPColorMapData(settings.bins_x, settings.bins_y, QCPRange(settings.x_min, settings.x_max), QCPRange(settings.y_min, settings.y_max));
    color_map_spectra_2d_qcp->setData(color_map_data_spectra_2d_qcp, true);

    spectra_2d_qcp->xAxis->setLabel("thl");
    spectra_2d_qcp->yAxis->setLabel("value");

    color_scale_spectra_2d_qcp = new QCPColorScale(spectra_2d_qcp);
    spectra_2d_qcp->plotLayout()->addElement(0, 1, color_scale_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->setType(QCPAxis::atRight);
    color_map_spectra_2d_qcp->setColorScale(color_scale_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->axis()->setLabel("count");

    color_map_spectra_2d_qcp->setGradient(*gradient);
    color_map_spectra_2d_qcp->setInterpolate(false);

    margin_group_spectra_2d_qcp = new QCPMarginGroup(spectra_2d_qcp);
    spectra_2d_qcp->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_spectra_2d_qcp);

    spectra_2d_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming

    color_map_spectra_2d_qcp->rescaleDataRange(true);
    color_scale_spectra_2d_qcp->rescaleDataRange(true);
    spectra_2d_qcp->rescaleAxes();
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);

    connect(spectra_2d_qcp->xAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(spectra_2d_qcp->yAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(color_scale_spectra_2d_qcp->axis(), SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_z_axis_range(QCPRange)), Qt::DirectConnection);
    connect(spectra_2d_qcp,                     SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_spectra_2d(QMouseEvent*)));

    emit US_generate_spectra_2d();
}

void UC_wai::U_generate_spectra_2d_energy(UStr_spectra_2d_settings settings) {
    spectra_2d_settings = settings;
    spectra_2d_qcp->clearPlottables();
    color_map_spectra_2d_qcp = new QCPColorMap(spectra_2d_qcp->xAxis, spectra_2d_qcp->yAxis);
    color_map_data_spectra_2d_qcp = new QCPColorMapData(settings.bins_x, settings.bins_y, QCPRange(settings.x_min, settings.x_max), QCPRange(settings.y_min, settings.y_max));
    color_map_spectra_2d_qcp->setData(color_map_data_spectra_2d_qcp, true);

    spectra_2d_qcp->xAxis->setLabel("energy");
    spectra_2d_qcp->yAxis->setLabel("value");

    color_scale_spectra_2d_qcp = new QCPColorScale(spectra_2d_qcp);
    spectra_2d_qcp->plotLayout()->addElement(0, 1, color_scale_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->setType(QCPAxis::atRight);
    color_map_spectra_2d_qcp->setColorScale(color_scale_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->axis()->setLabel("count");

    color_map_spectra_2d_qcp->setGradient(*gradient);
    color_map_spectra_2d_qcp->setInterpolate(false);

    margin_group_spectra_2d_qcp = new QCPMarginGroup(spectra_2d_qcp);
    spectra_2d_qcp->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_spectra_2d_qcp);

    spectra_2d_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    color_map_spectra_2d_qcp->rescaleDataRange(true);
    color_scale_spectra_2d_qcp->rescaleDataRange(true);
    spectra_2d_qcp->rescaleAxes();
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);

    connect(spectra_2d_qcp->xAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(spectra_2d_qcp->yAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(color_scale_spectra_2d_qcp->axis(), SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_z_axis_range(QCPRange)), Qt::DirectConnection);
    connect(spectra_2d_qcp,                     SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_spectra_2d(QMouseEvent*)));

    emit US_generate_spectra_2d(settings.x_min, settings.x_max);
}

void UC_wai::U_generate_spectra_2d_range(int thl_min, int thl_max) {
    emit US_generate_spectra_2d_range(thl_min, thl_max);
}

void UC_wai::U_generate_spectra_2d_range_energy(double energy_min, double energy_max) {
    emit US_generate_spectra_2d_range(energy_min, energy_max);
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
//    emit US_set_threshold_range(thl_min, thl_max);
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
        spectra_qcp->setInteractions(QCP::iSelectLegend);
        frame_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        distribution_qcp->setInteractions(QCP::iSelectLegend);
        chip_fit_qcp->setInteractions(QCP::iSelectLegend);
        calibration_qcp->setInteractions(QCP::iSelectLegend);
        id_frame_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->setInteractions(static_cast<QCP::Interaction>(0));
        spectra_qcp->setSelectionRectMode(QCP::srmCustom);
        frame_qcp->setSelectionRectMode(QCP::srmCustom);
        distribution_qcp->setSelectionRectMode(QCP::srmCustom);
        chip_fit_qcp->setSelectionRectMode(QCP::srmCustom);
        calibration_qcp->setSelectionRectMode(QCP::srmCustom);
        id_frame_qcp->setSelectionRectMode(QCP::srmCustom);
        if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->setSelectionRectMode(QCP::srmCustom);
    } else {
        spectra_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectLegend);
        frame_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        distribution_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectLegend);
        chip_fit_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectLegend);
        calibration_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectLegend);
        id_frame_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
        spectra_qcp->setSelectionRectMode(QCP::srmNone);
        frame_qcp->setSelectionRectMode(QCP::srmNone);
        distribution_qcp->setSelectionRectMode(QCP::srmNone);
        chip_fit_qcp->setSelectionRectMode(QCP::srmNone);
        calibration_qcp->setSelectionRectMode(QCP::srmNone);
        id_frame_qcp->setSelectionRectMode(QCP::srmNone);
        if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->setSelectionRectMode(QCP::srmNone);
    }
}

void UC_wai::U_set_x_axis_type(bool view) {
    if (view) {
        spectra_qcp->xAxis->setScaleType(QCPAxis::stLinear);
        distribution_qcp->xAxis->setScaleType(QCPAxis::stLinear);
        chip_fit_qcp->xAxis->setScaleType(QCPAxis::stLinear);
        calibration_qcp->xAxis->setScaleType(QCPAxis::stLinear);
        if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->xAxis->setScaleType(QCPAxis::stLinear);
    } else {
        spectra_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
        distribution_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
        chip_fit_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
        calibration_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
        if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->xAxis->setScaleType(QCPAxis::stLogarithmic);
    }
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
    if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_y_axis_type(bool view) {
    if (view) {
        spectra_qcp->yAxis->setScaleType(QCPAxis::stLinear);
        distribution_qcp->yAxis->setScaleType(QCPAxis::stLinear);
        chip_fit_qcp->yAxis->setScaleType(QCPAxis::stLinear);
        calibration_qcp->yAxis->setScaleType(QCPAxis::stLinear);
        if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->yAxis->setScaleType(QCPAxis::stLinear);
    } else {
        spectra_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
        distribution_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
        chip_fit_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
        calibration_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
        if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->yAxis->setScaleType(QCPAxis::stLogarithmic);
    }
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
    distribution_qcp->replot(QCustomPlot::rpQueuedReplot);
    chip_fit_qcp->replot(QCustomPlot::rpQueuedReplot);
    calibration_qcp->replot(QCustomPlot::rpQueuedReplot);
    if (spectra_2d_qcp->plottableCount() > 0) spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_z_axis_type(bool view) {
    if (view) {
        color_map_frame_qcp->setDataScaleType(QCPAxis::stLinear);
    } else {
        color_map_frame_qcp->setDataScaleType(QCPAxis::stLogarithmic);
    }
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_2d_z_axis_type(bool view) {
    if (view) {
        color_map_spectra_2d_qcp->setDataScaleType(QCPAxis::stLinear);
    } else {
        color_map_spectra_2d_qcp->setDataScaleType(QCPAxis::stLogarithmic);
    }
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
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
//

void UC_wai::U_resize_spectra_2d() {
    spectra_2d_qcp->rescaleAxes(true);
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_rescale_spectra_2d() {
    QCPRange xrange = spectra_2d_qcp->xAxis->range();
    QCPRange yrange = spectra_2d_qcp->yAxis->range();
    double min = 1e300;
    double max = -1e300;
    double z = 0;
    double delta_x = (spectra_2d_settings.x_max - spectra_2d_settings.x_min) / (spectra_2d_settings.bins_x);
    double delta_y = (spectra_2d_settings.y_max - spectra_2d_settings.y_min) / (spectra_2d_settings.bins_y);
    int keyindex;
    int valueindex;
    for (double x = xrange.lower; x < xrange.upper; x += delta_x) {
        for (double y = yrange.lower; y < yrange.upper; y+= delta_y) {
            color_map_data_spectra_2d_qcp->coordToCell(x, y, &keyindex, &valueindex);
            z = color_map_spectra_2d_qcp->data()->cell(keyindex, valueindex);
            if (z < min) min = z;
            if (z > max) max = z;
        }
    }
    color_scale_spectra_2d_qcp->axis()->setRange(min, max);

    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
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

void UC_wai::U_save_spectra_root(QString file_name) {
    if (spectra_qcp->graphCount() == 0) return;
    TFile file(file_name.toStdString().c_str(), "new");
    int n_graph = spectra_qcp->graphCount();
    for (int i = 0; i < n_graph; i++) {
        QVector<double> x;
        QVector<double> y;
        int n = spectra_qcp->graph(i)->dataCount();
        for (int j = 0; j < n; j++) {
            x << spectra_qcp->graph(i)->data()->at(j)->key;
            y << spectra_qcp->graph(i)->data()->at(j)->value;
        }
        TGraph gr(n, x.data(), y.data());
        gr.SetName(spectra_qcp->graph(i)->name().toStdString().c_str());
        gr.Write(file_name.toStdString().c_str());
    }
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
        case UTE_FT_root : {
            U_save_spectra_root(file_name + ".root");
            break;
        }
    }
}

void UC_wai::U_automatic_save_spectra(QString file_name, QString name, UTE_file_type file_type) {
    int x_min = this->x_min;
    int y_min = this->y_min;
    int x_max = this->x_max;
    int y_max = this->y_max;
    int n = list_roi.size();
    UC_roi * roi;
    U_delete_mask_plots();
    spectra_qcp->xAxis->setLabel("thl");
    if ((file_type == UTE_FT_txt) || (file_type == UTE_FT_root)) {
        for (int i = 0; i < n; i++) {
            roi = &(list_roi[i]);
            emit US_set_roi(roi->U_get_x_min(), roi->U_get_x_max(), roi->U_get_y_min(), roi->U_get_y_max());
            spectra_qcp->addGraph();
            spectra_qcp->graph()->setName(name + "; " + roi->U_get_name());
            spectra_qcp->graph()->setPen(pens[spectra_qcp->graphCount() % 16]);
            spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
            emit US_generate_spectra_direct();
        }
        U_save_spectra(file_name, file_type);
    } else {
        for (int i = 0; i < n; i++) {
            roi = &(list_roi[i]);
            emit US_set_roi(roi->U_get_x_min(), roi->U_get_x_max(), roi->U_get_y_min(), roi->U_get_y_max());
            spectra_qcp->addGraph();
            spectra_qcp->graph()->setName(name + "; " + roi->U_get_name());
            spectra_qcp->graph()->setPen(pens[spectra_qcp->graphCount() % 16]);
            spectra_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
            emit US_generate_spectra_direct();
            U_save_spectra(file_name + "." + roi->U_get_name(), file_type);
            U_reset_spectra();
        }
    }
    this->x_min = x_min;
    this->y_min = y_min;
    this->x_max = x_max;
    this->y_max = y_max;
    emit US_set_roi(x_min, x_max, y_min, y_max);
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

void UC_wai::U_save_frame_root(QString file_name) {
    TFile file(file_name.toStdString().c_str(), "new");
    TH2D * frame = new TH2D("frame", "frame", color_map_data_frame_qcp->keySize(), color_map_data_frame_qcp->keyRange().lower, color_map_data_frame_qcp->keyRange().upper,
                                              color_map_data_frame_qcp->valueSize(), color_map_data_frame_qcp->valueRange().lower, color_map_data_frame_qcp->valueRange().upper);
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 3840; x++) {
            frame->Fill(x, y, color_map_frame_qcp->data()->data(x, y));
        }
    }
    frame->Write(file_name.toStdString().c_str());
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
        case UTE_FT_root : {
            U_save_frame_root(file_name + ".root");
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

void UC_wai::U_save_distribution_root(QString file_name) {
    if (distribution_qcp->graphCount() == 0) return;
    TFile file(file_name.toStdString().c_str(), "new");
    int n_graph = distribution_qcp->graphCount();
    for (int i = 0; i < n_graph; i++) {
        QVector<double> x;
        QVector<double> y;
        int n = distribution_qcp->graph(i)->dataCount();
        for (int j = 0; j < n; j++) {
            x << distribution_qcp->graph(i)->data()->at(j)->key;
            y << distribution_qcp->graph(i)->data()->at(j)->value;
        }
        TGraph gr(n, x.data(), y.data());
        gr.SetName(distribution_qcp->graph(i)->name().toStdString().c_str());
        gr.Write(file_name.toStdString().c_str());
    }
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
        case UTE_FT_root : {
            U_save_distribution_root(file_name + ".root");
            break;
        }
    }
}

void UC_wai::U_automatic_save_distribution(QString file_name, QString name, UTE_file_type file_type, int n_bins, double min, double max, int thl) {
    int x_min = this->x_min;
    int y_min = this->y_min;
    int x_max = this->x_max;
    int y_max = this->y_max;
    int n = list_roi.size();
    UC_roi * roi;
    U_delete_mask_plots();
    if (file_type == UTE_FT_txt) {
        for (int i = 0; i < n; i++) {
            roi = &(list_roi[i]);
            emit US_set_roi(roi->U_get_x_min(), roi->U_get_x_max(), roi->U_get_y_min(), roi->U_get_y_max());
            distribution_qcp->addGraph();
            distribution_qcp->graph()->setName(name + "; " + roi->U_get_name());
            distribution_qcp->graph()->setPen(pens[distribution_qcp->graphCount() % 16]);
            distribution_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
            emit US_generate_distribution_direct(n_bins, min, max, thl);
        }
        U_save_spectra(file_name, file_type);
    } else {
        for (int i = 0; i < n; i++) {
            roi = &(list_roi[i]);
            emit US_set_roi(roi->U_get_x_min(), roi->U_get_x_max(), roi->U_get_y_min(), roi->U_get_y_max());
            distribution_qcp->addGraph();
            distribution_qcp->graph()->setName(name + "; " + roi->U_get_name());
            distribution_qcp->graph()->setPen(pens[distribution_qcp->graphCount() % 16]);
            distribution_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
            emit US_generate_distribution_direct(n_bins, min, max, thl);
            U_save_distribution(file_name + "." + roi->U_get_name(), file_type);
            U_reset_distribution();
        }
    }
    this->x_min = x_min;
    this->y_min = y_min;
    this->x_max = x_max;
    this->y_max = y_max;
    emit US_set_roi(x_min, x_max, y_min, y_max);
}

void UC_wai::U_automatic_save_distribution(QString file_name, QString name, UTE_file_type file_type, int n_bins, double min, double max) {
    int x_min = this->x_min;
    int y_min = this->y_min;
    int x_max = this->x_max;
    int y_max = this->y_max;
    int n = list_roi.size();
    UC_roi * roi;
    U_delete_mask_plots();
    if (file_type == UTE_FT_txt) {
        for (int i = 0; i < n; i++) {
            roi = &(list_roi[i]);
            emit US_set_roi(roi->U_get_x_min(), roi->U_get_x_max(), roi->U_get_y_min(), roi->U_get_y_max());
            distribution_qcp->addGraph();
            distribution_qcp->graph()->setName(name + "; " + roi->U_get_name());
            distribution_qcp->graph()->setPen(pens[distribution_qcp->graphCount() % 16]);
            distribution_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
            emit US_generate_distribution_direct(n_bins, min, max);
        }
        U_save_spectra(file_name, file_type);
    } else {
        for (int i = 0; i < n; i++) {
            roi = &(list_roi[i]);
            emit US_set_roi(roi->U_get_x_min(), roi->U_get_x_max(), roi->U_get_y_min(), roi->U_get_y_max());
            distribution_qcp->addGraph();
            distribution_qcp->graph()->setName(name + "; " + roi->U_get_name());
            distribution_qcp->graph()->setPen(pens[distribution_qcp->graphCount() % 16]);
            distribution_qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);
            emit US_generate_distribution_direct(n_bins, min, max);
            U_save_distribution(file_name + "." + roi->U_get_name(), file_type);
            U_reset_distribution();
        }
    }
    this->x_min = x_min;
    this->y_min = y_min;
    this->x_max = x_max;
    this->y_max = y_max;
    emit US_set_roi(x_min, x_max, y_min, y_max);
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

void UC_wai::U_save_chip_fit_root(QString file_name) {
    if (chip_fit_qcp->graphCount() == 0) return;
    TFile file(file_name.toStdString().c_str(), "new");
    int n_graph = chip_fit_qcp->graphCount();
    for (int i = 0; i < n_graph; i++) {
        QVector<double> x;
        QVector<double> y;
        int n = chip_fit_qcp->graph(i)->dataCount();
        for (int j = 0; j < n; j++) {
            x << chip_fit_qcp->graph(i)->data()->at(j)->key;
            y << chip_fit_qcp->graph(i)->data()->at(j)->value;
        }
        TGraph gr(n, x.data(), y.data());
        gr.SetName(chip_fit_qcp->graph(i)->name().toStdString().c_str());
        gr.Write(file_name.toStdString().c_str());
    }
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
        case UTE_FT_root : {
            U_save_chip_fit_root(file_name + ".root");
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

void UC_wai::U_save_calibration_root(QString file_name) {
    if (calibration_qcp->graphCount() == 0) return;
    TFile file(file_name.toStdString().c_str(), "new");
    int n_graph = calibration_qcp->graphCount();
    for (int i = 0; i < n_graph; i++) {
        QVector<double> x;
        QVector<double> y;
        int n = calibration_qcp->graph(i)->dataCount();
        for (int j = 0; j < n; j++) {
            x << calibration_qcp->graph(i)->data()->at(j)->key;
            y << calibration_qcp->graph(i)->data()->at(j)->value;
        }
        TGraph gr(n, x.data(), y.data());
        gr.SetName(calibration_qcp->graph(i)->name().toStdString().c_str());
        gr.Write(file_name.toStdString().c_str());
    }
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
        case UTE_FT_root : {
            U_save_calibration_root(file_name + ".root");
            break;
        }
    }
}
//
void UC_wai::U_save_spectra_2d_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    str << spectra_2d_settings.bins_y << " ";
    str << spectra_2d_settings.bins_x << " ";
    str << spectra_2d_settings.x_max << " ";
    str << spectra_2d_settings.x_min << " ";
    str << spectra_2d_settings.y_min << " ";
    str << spectra_2d_settings.y_max << " ";
    for (int y = 0; y < spectra_2d_settings.bins_y - 1; y++) {
        for (int x = 0; x < spectra_2d_settings.bins_x - 1; x++) {
            str << color_map_spectra_2d_qcp->data()->data(x, y) << " ";
        }
        str << color_map_spectra_2d_qcp->data()->data(spectra_2d_settings.bins_x - 1, y) << endl;
    }
    for (int x = 0; x < spectra_2d_settings.bins_x - 1; x++) {
        str << color_map_spectra_2d_qcp->data()->data(x, spectra_2d_settings.bins_y - 1) << " ";
    }
    str << color_map_spectra_2d_qcp->data()->data(spectra_2d_settings.bins_x - 1, spectra_2d_settings.bins_y - 1);
    file.close();
}

void UC_wai::U_save_spectra_2d_root(QString file_name) {
    TFile file(file_name.toStdString().c_str(), "new");
    TH2D * frame = new TH2D("spectra_2d", "spectra_2d", color_map_data_spectra_2d_qcp->keySize(), color_map_data_spectra_2d_qcp->keyRange().lower, color_map_data_spectra_2d_qcp->keyRange().upper,
                                              color_map_data_spectra_2d_qcp->valueSize(), color_map_data_spectra_2d_qcp->valueRange().lower, color_map_data_spectra_2d_qcp->valueRange().upper);
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 3840; x++) {
            frame->Fill(x, y, color_map_spectra_2d_qcp->data()->data(x, y));
        }
    }
    frame->Write(file_name.toStdString().c_str());
}

void UC_wai::U_save_spectra_2d(QString file_name, UTE_file_type file_type) {
    if (spectra_2d_qcp->plottableCount() == 0) return;
    if (file_name.right(4).left(1) == ".") file_name = file_name.mid(0, file_name.length() - 4);
    switch (file_type) {
        case UTE_FT_txt : {
            U_save_spectra_2d_txt(file_name + ".txt");
            break;
        }
        case UTE_FT_pdf : {
            spectra_2d_qcp->savePdf(file_name + ".pdf");
            break;
        }
        case UTE_FT_jpg : {
            spectra_2d_qcp->saveJpg(file_name + ".jpg");
            break;
        }
        case UTE_FT_png : {
            spectra_2d_qcp->savePng(file_name + ".png");
            break;
        }
        case UTE_FT_bmp : {
            spectra_2d_qcp->saveBmp(file_name + ".bmp");
            break;
        }
        case UTE_FT_root : {
            U_save_spectra_2d_root(file_name + ".root");
            break;
        }
    }
}

void UC_wai::U_automatic_save_spectra_2d(QString file_name, UTE_file_type file_type, UStr_spectra_2d_settings settings) {
    int x_min = this->x_min;
    int y_min = this->y_min;
    int x_max = this->x_max;
    int y_max = this->y_max;
    int n = list_roi.size();
    UC_roi * roi;
    for (int i = 0; i < n; i++) {
        roi = &(list_roi[i]);
        emit US_set_roi(roi->U_get_x_min(), roi->U_get_x_max(), roi->U_get_y_min(), roi->U_get_y_max());

        spectra_2d_qcp->clearPlottables();
        color_map_spectra_2d_qcp = new QCPColorMap(spectra_2d_qcp->xAxis, spectra_2d_qcp->yAxis);
        color_map_data_spectra_2d_qcp = new QCPColorMapData(settings.bins_x, settings.bins_y, QCPRange(settings.x_min, settings.x_max), QCPRange(settings.y_min, settings.y_max));
        color_map_spectra_2d_qcp->setData(color_map_data_spectra_2d_qcp, true);

        spectra_2d_qcp->xAxis->setLabel("thl");
        spectra_2d_qcp->yAxis->setLabel("value");

        color_scale_spectra_2d_qcp = new QCPColorScale(spectra_2d_qcp);
        spectra_2d_qcp->plotLayout()->addElement(0, 1, color_scale_spectra_2d_qcp);
        color_scale_spectra_2d_qcp->setType(QCPAxis::atRight);
        color_map_spectra_2d_qcp->setColorScale(color_scale_spectra_2d_qcp);
        color_scale_spectra_2d_qcp->axis()->setLabel("count");

        color_map_spectra_2d_qcp->setGradient(*gradient);
        color_map_spectra_2d_qcp->setInterpolate(false);

        margin_group_spectra_2d_qcp = new QCPMarginGroup(spectra_2d_qcp);
        spectra_2d_qcp->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_spectra_2d_qcp);
        color_scale_spectra_2d_qcp->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_spectra_2d_qcp);

        color_map_spectra_2d_qcp->rescaleDataRange(true);
        color_scale_spectra_2d_qcp->rescaleDataRange(true);
        if (renew_renges) spectra_2d_qcp->rescaleAxes();
        spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);

        connect(spectra_2d_qcp->xAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_x_axis_range(QCPRange)), Qt::DirectConnection);
        connect(spectra_2d_qcp->yAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_y_axis_range(QCPRange)), Qt::DirectConnection);
        connect(color_scale_spectra_2d_qcp->axis(), SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_z_axis_range(QCPRange)), Qt::DirectConnection);
        connect(spectra_2d_qcp,                     SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_move_spectra_2d(QMouseEvent*)));

        emit US_generate_spectra_2d_direct();
        U_save_spectra_2d(file_name + "." + roi->U_get_name(), file_type);
    }
    U_save_spectra(file_name, file_type);
    this->x_min = x_min;
    this->y_min = y_min;
    this->x_max = x_max;
    this->y_max = y_max;
    emit US_set_roi(x_min, x_max, y_min, y_max);
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
        QVector<double> x, y;
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
        US_new_spectra(graph_name);
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

void UC_wai::U_load_spectra_2d_txt(QString file_name) {
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    str >> spectra_2d_settings.bins_y;
    str >> spectra_2d_settings.bins_x;
    str >> spectra_2d_settings.x_max;
    str >> spectra_2d_settings.x_min;
    str >> spectra_2d_settings.y_min;
    str >> spectra_2d_settings.y_max;

    spectra_2d_qcp->clearPlottables();
    color_map_spectra_2d_qcp = new QCPColorMap(spectra_2d_qcp->xAxis, spectra_2d_qcp->yAxis);
    color_map_data_spectra_2d_qcp = new QCPColorMapData(spectra_2d_settings.bins_x, spectra_2d_settings.bins_y, QCPRange(spectra_2d_settings.x_min, spectra_2d_settings.x_max), QCPRange(spectra_2d_settings.y_min, spectra_2d_settings.y_max));
    color_map_spectra_2d_qcp->setData(color_map_data_spectra_2d_qcp, true);

    color_scale_spectra_2d_qcp = new QCPColorScale(spectra_2d_qcp);
    spectra_2d_qcp->plotLayout()->addElement(0, 1, color_scale_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->setType(QCPAxis::atRight);
    color_map_spectra_2d_qcp->setColorScale(color_scale_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->axis()->setLabel("value");

    color_map_spectra_2d_qcp->setGradient(*gradient);
    color_map_spectra_2d_qcp->setInterpolate(false);

    margin_group_spectra_2d_qcp = new QCPMarginGroup(spectra_2d_qcp);
    spectra_2d_qcp->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_spectra_2d_qcp);
    color_scale_spectra_2d_qcp->setMarginGroup(QCP::msBottom|QCP::msTop, margin_group_spectra_2d_qcp);

    double z = 0;
    for (int y = 0; y < 256; y ++) {
        for (int x = 0; x < (15 * 256); x++) {
            str >> z;
            color_map_spectra_2d_qcp->data()->setData(x, y, z);
        }
    }

    color_map_spectra_2d_qcp->rescaleDataRange(true);
    color_scale_spectra_2d_qcp->rescaleDataRange(true);
    spectra_2d_qcp->rescaleAxes();
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);

//    connect(spectra_2d_qcp->xAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_x_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(spectra_2d_qcp->yAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_y_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(color_scale_spectra_2d_qcp->axis(), SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_z_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(spectra_2d_qcp,                     SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_move_spectra_2d(QMouseEvent*)));


//    if (renew_renges) spectra_2d_qcp->rescaleAxes();
//    if (renew_renges) U_rescale_spectra_2d();
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
//    connect(spectra_2d_qcp->xAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_x_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(spectra_2d_qcp->yAxis,              SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_y_axis_range(QCPRange)), Qt::DirectConnection);
//    connect(color_scale_spectra_2d_qcp->axis(), SIGNAL(rangeChanged(QCPRange)),     this, SLOT(U_set_spectra_2d_z_axis_range(QCPRange)), Qt::DirectConnection);
    connect(spectra_2d_qcp,                     SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(U_mouse_select_spectra_2d(QMouseEvent*)));
    connect(spectra_2d_qcp,                     SIGNAL(mouseMove(QMouseEvent*)),    this, SLOT(U_mouse_move_spectra_2d(QMouseEvent*)));
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


void UC_wai::U_load_table_tsv(QString file_name) {
    table_model->removeRows(0, table_model->rowCount());
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    QString line;
    QStringList list_str;
    QList<QStandardItem *> list;
    int n;
    QStandardItem * item;
    while (! str.atEnd()) {
        line = str.readLine();
        list_str = line.split('\t');
        n = list_str.size();
        list.clear();
        for (int i = 0; i < n; i++) {
            item = new QStandardItem(list_str[i]);
            list << item;
        }
        table_model->appendRow(list);
    }
    table->resizeColumnsToContents();
    file.close();
}


void UC_wai::U_load_table_csv(QString file_name) {
    table_model->removeRows(0, table_model->rowCount());
    QFile file(file_name);
    file.open(QFile::ReadOnly);
    QTextStream str(&file);
    QString line = str.readLine();
    QStringList list_str = line.split(',');
    QList<QStandardItem *> list;
    int n;
    QStandardItem * item;
    while (! str.atEnd()) {
        line = str.readLine();
        list_str = line.split('\t');
        n = list_str.size();
        list.clear();
        for (int i = 0; i < n; i++) {
            item = new QStandardItem(list_str[i]);
            list << item;
        }
        table_model->appendRow(list);
    }
    table->resizeColumnsToContents();
    file.close();
}

void UC_wai::U_load_table(QString file_name) {
    QString str = file_name.right(3);
    if (str == "csv") U_load_table_csv(file_name);
    if (str == "tsv") U_load_table_tsv(file_name);
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

void UC_wai::U_mask_selected_value(bool in_roi, int thl) {
    emit US_mask_selected_value(value_min, value_max, in_roi, thl);
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

QString UC_wai::U_get_name_roi(int x_min, int x_max, int y_min, int y_max) {
    QString str = "xmin = %1, xmax = %2, ymin = %3, ymax = %4";
    str = str.arg(x_min).arg(x_max).arg(y_min).arg(y_max);
    int n = list_roi.size();
    for (int i = 0; i < n; i++) {
        if ((x_min == list_roi[i].U_get_x_min()) &&
            (x_max == list_roi[i].U_get_x_max()) &&
            (y_min == list_roi[i].U_get_y_min()) &&
            (y_max == list_roi[i].U_get_y_max())) {

            str = list_roi[i].U_get_name();
        }
    }
    return str;
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
    int n_1 = x1.size();
    int n_2 = x2.size();
    x << x1;
    x << x2;
    int n = x.size();
    double z;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (x[i] > x[j]) {
                z = x[i];
                x[i] = x[j];
                x[j] = z;
            }
        }
    }
    for (int i = 0; i < n - 1; i++) {
        if (qAbs(x[i] - x[i + 1]) < 1e-10) {
            x.remove(i);
            i--;
            n--;
        }
    }
    for (int i = 0; i < n; i++) {
        double thl = x[i];
        double data_1 = 0;
        if ((thl >= x1[0]) && (thl <= x1[n_1 - 1])) {
            int index_1 = x1.indexOf(thl);
            if (index_1 == -1) {
                index_1 = 0;
                while (x1[index_1] < thl) {
                    index_1++;
                }
                data_1 = (y1[index_1] - y1[index_1 - 1]) * (thl - x1[index_1 - 1]);
                data_1 /= (x1[index_1] - x1[index_1 - 1]);
                data_1 += y1[index_1 - 1];
            } else {
                data_1 = y1[index_1];
            }
        } else {
            data_1 = 0;
        }
        double data_2;
        if ((thl >= x2[0]) && (thl <= x2[n_2 - 1])) {
            int index_2 = x2.indexOf(thl);
            if (index_2 == -1) {
                index_2 = 0;
                while (x2[index_2] < thl) {
                    index_2++;
                }
                data_2 = (y2[index_2] - y2[index_2 - 1]) * (thl - x2[index_2 - 1]);
                data_2 /= (x2[index_2] - x2[index_2 - 1]);
                data_2 += y2[index_2 - 1];
            } else {
                data_2 = y2[index_2];
            }
        } else {
           data_2 = 0;
        }
        x << thl;
        double data;
        data = data_1 + data_2;
        y << data;
    }
    x1 = x;
    y1 = y;

//    int thl_min = thl_start;
//    int thl_max = thl_finish;
//    int thl_min_1 = x1[0];

//    int thl_max_1 = x1[n_1 - 1];
//    int thl_min_2 = x2[0];

//    int thl_max_2 = x2[n_2 - 1];
//    if (thl_min)

//    for (int thl = thl_start; thl <= thl_finish; thl++) {

//        int i1 = 0;


//        int i1 = x1.indexOf(thl);
//        int i2 = x2.indexOf(thl);
//        double data1 = 0;
//        if (i1 != -1) data1 = y1[i1];
//        double data2 = 0;
//        if (i2 != -1) data2 = y2[i2];
//        x << thl;
//        double data;
//        data = data1 + data2;
//        y << data;
//    }
//    x1 = x;
//    y1 = y;
}

void UC_wai::U_calculating_spectra_2(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2) {
    QVector<double> x, y;
    int n_1 = x1.size();
    int n_2 = x2.size();
    x << x1;
    x << x2;
    int n = x.size();
    double z;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (x[i] > x[j]) {
                z = x[i];
                x[i] = x[j];
                x[j] = z;
            }
        }
    }
    for (int i = 0; i < n - 1; i++) {
        if (qAbs(x[i] - x[i + 1]) < 1e-10) {
            x.remove(i);
            i--;
            n--;
        }
    }
    for (int i = 0; i < n; i++) {
        double thl = x[i];
        double data_1 = 0;
        if ((thl >= x1[0]) && (thl <= x1[n_1 - 1])) {
            int index_1 = x1.indexOf(thl);
            if (index_1 == -1) {
                index_1 = 0;
                while (x1[index_1] < thl) {
                    index_1++;
                }
                data_1 = (y1[index_1] - y1[index_1 - 1]) * (thl - x1[index_1 - 1]);
                data_1 /= (x1[index_1] - x1[index_1 - 1]);
                data_1 += y1[index_1 - 1];
            } else {
                data_1 = y1[index_1];
            }
        } else {
            data_1 = 0;
        }
        double data_2;
        if ((thl >= x2[0]) && (thl <= x2[n_2 - 1])) {
            int index_2 = x2.indexOf(thl);
            if (index_2 == -1) {
                index_2 = 0;
                while (x2[index_2] < thl) {
                    index_2++;
                }
                data_2 = (y2[index_2] - y2[index_2 - 1]) * (thl - x2[index_2 - 1]);
                data_2 /= (x2[index_2] - x2[index_2 - 1]);
                data_2 += y2[index_2 - 1];
            } else {
                data_2 = y2[index_2];
            }
        } else {
           data_2 = 0;
        }
        x << thl;
        double data;
        data = data_1 * data_2;
        y << data;
    }
    x1 = x;
    y1 = y;

//    QVector<double> x, y;
//    for (int thl = thl_start; thl <= thl_finish; thl++) {
//        int i1 = x1.indexOf(thl);
//        int i2 = x2.indexOf(thl);
//        double data1 = 0;
//        if (i1 != -1) data1 = y1[i1];
//            else data1 =
//        double data2 = 0;
//        if (i2 != -1) data2 = y2[i2];
//        x << thl;
//        double data;
//        data = data1 * data2;
//        y << data;
//    }
//    x1 = x;
//    y1 = y;
}

void UC_wai::U_calculating_spectra_3(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2) {
    QVector<double> x, y;
    int n_1 = x1.size();
    int n_2 = x2.size();
    x << x1;
    x << x2;
    int n = x.size();
    double z;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (x[i] > x[j]) {
                z = x[i];
                x[i] = x[j];
                x[j] = z;
            }
        }
    }
    for (int i = 0; i < n - 1; i++) {
        if (qAbs(x[i] - x[i + 1]) < 1e-10) {
            x.remove(i);
            i--;
            n--;
        }
    }
    for (int i = 0; i < n; i++) {
        double thl = x[i];
        double data_1 = 0;
        if ((thl >= x1[0]) && (thl <= x1[n_1 - 1])) {
            int index_1 = x1.indexOf(thl);
            if (index_1 == -1) {
                index_1 = 0;
                while (x1[index_1] < thl) {
                    index_1++;
                }
                data_1 = (y1[index_1] - y1[index_1 - 1]) * (thl - x1[index_1 - 1]);
                data_1 /= (x1[index_1] - x1[index_1 - 1]);
                data_1 += y1[index_1 - 1];
            } else {
                data_1 = y1[index_1];
            }
        } else {
            data_1 = 0;
        }
        double data_2;
        if ((thl >= x2[0]) && (thl <= x2[n_2 - 1])) {
            int index_2 = x2.indexOf(thl);
            if (index_2 == -1) {
                index_2 = 0;
                while (x2[index_2] < thl) {
                    index_2++;
                }
                data_2 = (y2[index_2] - y2[index_2 - 1]) * (thl - x2[index_2 - 1]);
                data_2 /= (x2[index_2] - x2[index_2 - 1]);
                data_2 += y2[index_2 - 1];
            } else {
                data_2 = y2[index_2];
            }
        } else {
           data_2 = 0;
        }
        x << thl;
        double data;
        data = data_1 - data_2;
        y << data;
    }
    x1 = x;
    y1 = y;

//    QVector<double> x, y;
//    for (int thl = thl_start; thl <= thl_finish; thl++) {
//        int i1 = x1.indexOf(thl);
//        int i2 = x2.indexOf(thl);
//        double data1 = 0;
//        if (i1 != -1) data1 = y1[i1];
//        double data2 = 0;
//        if (i2 != -1) data2 = y2[i2];
//        x << thl;
//        double data;
//        data = data1 - data2;
//        y << data;
//    }
//    x1 = x;
//    y1 = y;
}

void UC_wai::U_calculating_spectra_4(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2) {
    QVector<double> x, y;
    int n_1 = x1.size();
    int n_2 = x2.size();
    x << x1;
    x << x2;
    int n = x.size();
    double z;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (x[i] > x[j]) {
                z = x[i];
                x[i] = x[j];
                x[j] = z;
            }
        }
    }
    for (int i = 0; i < n - 1; i++) {
        if (qAbs(x[i] - x[i + 1]) < 1e-10) {
            x.remove(i);
            i--;
            n--;
        }
    }
    for (int i = 0; i < n; i++) {
        double thl = x[i];
        double data_1 = 0;
        if ((thl >= x1[0]) && (thl <= x1[n_1 - 1])) {
            int index_1 = x1.indexOf(thl);
            if (index_1 == -1) {
                index_1 = 0;
                while (x1[index_1] < thl) {
                    index_1++;
                }
                data_1 = (y1[index_1] - y1[index_1 - 1]) * (thl - x1[index_1 - 1]);
                data_1 /= (x1[index_1] - x1[index_1 - 1]);
                data_1 += y1[index_1 - 1];
            } else {
                data_1 = y1[index_1];
            }
        } else {
            data_1 = 0;
        }
        double data_2;
        if ((thl >= x2[0]) && (thl <= x2[n_2 - 1])) {
            int index_2 = x2.indexOf(thl);
            if (index_2 == -1) {
                index_2 = 0;
                while (x2[index_2] < thl) {
                    index_2++;
                }
                data_2 = (y2[index_2] - y2[index_2 - 1]) * (thl - x2[index_2 - 1]);
                data_2 /= (x2[index_2] - x2[index_2 - 1]);
                data_2 += y2[index_2 - 1];
            } else {
                data_2 = y2[index_2];
            }
        } else {
           data_2 = 0;
        }
        x << thl;
        double data;
        if (qAbs(data_2) < 1e-10) data = 0;
            else data = data_1 / data_2;
        y << data;
    }
    x1 = x;
    y1 = y;

//    QVector<double> x, y;
//    for (int thl = thl_start; thl <= thl_finish; thl++) {
//        int i1 = x1.indexOf(thl);
//        int i2 = x2.indexOf(thl);
//        double data1 = 0;
//        if (i1 != -1) data1 = y1[i1];
//        double data2 = 0;
//        if (i2 != -1) data2 = y2[i2];
//        x << thl;
//        double data;
//        if (qAbs(data2) < 1e-10) data = 0;
//            else data = data1 / data2;
//        y << data;
//    }
//    x1 = x;
//    y1 = y;
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
//
void UC_wai::U_set_gradient(int n) {
    gradient->loadPreset(static_cast<QCPColorGradient::GradientPreset>(n));
    color_map_frame_qcp->setGradient(*gradient);
    color_map_id_frame_qcp->setGradient(*gradient);
    frame_qcp->replot(QCustomPlot::rpQueuedReplot);
    id_frame_qcp->replot(QCustomPlot::rpQueuedReplot);
    if (spectra_2d_qcp->plottableCount() > 0) {
        color_map_spectra_2d_qcp->setGradient(*gradient);
        spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
    }
}

void UC_wai::U_thick_up() {

    QList<QCustomPlot *> list_qcp;
    list_qcp << spectra_qcp;
    list_qcp << distribution_qcp;
    list_qcp << chip_fit_qcp;
    list_qcp << calibration_qcp;
    list_qcp << frame_qcp;
    list_qcp << spectra_2d_qcp;

    for (int i = 0; i < list_qcp.size(); i++) {
        for (int j = 0; j < list_qcp[i]->graphCount(); j++) {
            QPen pen = list_qcp[i]->graph(j)->pen();
            pen.setWidth(pen.width() + 1);
            list_qcp[i]->graph(j)->setPen(pen);
        }
        QFont font = list_qcp[i]->legend->font();
        font.setWeight(font.weight() + 1);
        list_qcp[i]->legend->setFont(font);

        font = list_qcp[i]->xAxis->labelFont();
        font.setWeight(font.weight() + 1);
        list_qcp[i]->xAxis->setLabelFont(font);
        font = list_qcp[i]->yAxis->labelFont();
        font.setWeight(font.weight() + 1);
        list_qcp[i]->yAxis->setLabelFont(font);
        list_qcp[i]->replot(QCustomPlot::rpQueuedReplot);
    }
}

void UC_wai::U_thick_down() {
    QList<QCustomPlot *> list_qcp;
    list_qcp << spectra_qcp;
    list_qcp << distribution_qcp;
    list_qcp << chip_fit_qcp;
    list_qcp << calibration_qcp;
    list_qcp << frame_qcp;
    list_qcp << spectra_2d_qcp;

    for (int i = 0; i < list_qcp.size(); i++) {
        for (int j = 0; j < list_qcp[i]->graphCount(); j++) {
            QPen pen = list_qcp[i]->graph(j)->pen();
            pen.setWidth(pen.width() - 1);
            list_qcp[i]->graph(j)->setPen(pen);
        }
        QFont font = list_qcp[i]->legend->font();
        font.setWeight(font.weight() - 1);
        list_qcp[i]->legend->setFont(font);

        font = list_qcp[i]->xAxis->labelFont();
        font.setWeight(font.weight() - 1);
        list_qcp[i]->xAxis->setLabelFont(font);
        font = list_qcp[i]->yAxis->labelFont();
        font.setWeight(font.weight() - 1);
        list_qcp[i]->yAxis->setLabelFont(font);
        list_qcp[i]->replot(QCustomPlot::rpQueuedReplot);
    }
}
//////////////////////////!!!!!!!!!!!SLOTS!!!!!!!!!!!!!!!!!!!!!!!///////////////////////////////////////
void UC_wai::U_add_spectra_data(double x, double y) {
    spectra_qcp->graph()->addData(x, y);
    spectra_qcp->replot(QCustomPlot::rpQueuedReplot);
}

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
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_max_density()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_variance()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_x_min()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_x_max()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_y_min()));
    list << item;
    item = new QStandardItem(QString("%1").arg(pixels_info.U_get_y_max()));
    list << item;
    table_model->appendRow(list);
    table->resizeColumnsToContents();
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

void UC_wai::U_add_spectra_2d_data(double x, double y) {
    color_map_spectra_2d_qcp->data()->setData(x, y, color_map_spectra_2d_qcp->data()->data(x, y) + 1);
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

void UC_wai::U_replot_spectra() {
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

void UC_wai::U_replot_spectra_2d() {
    if (renew_renges) U_resize_spectra_2d();
    if (renew_renges) U_rescale_spectra_2d();
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(spectra_2d_qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_spectra_2d(QMouseEvent*)));
    //emit US_set_distribution_range(color_scale_spectra_2d_qcp->axis()->range().lower, color_scale_spectra_2d_qcp->axis()->range().upper);
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
//
void UC_wai::U_set_spectra_2d_x_axis_range(QCPRange range) {
    emit US_set_spectra_2d_max_x(range.upper);
    emit US_set_spectra_2d_min_x(range.lower);
}

void UC_wai::U_set_spectra_2d_y_axis_range(QCPRange range) {
    emit US_set_spectra_2d_max_y(range.upper);
    emit US_set_spectra_2d_min_y(range.lower);
}

void UC_wai::U_set_spectra_2d_z_axis_range(QCPRange range) {
    emit US_set_spectra_2d_max_z(range.upper);
    emit US_set_spectra_2d_min_z(range.lower);
}
////
void UC_wai::U_set_spectra_2d_max_x(double value) {
    spectra_2d_qcp->xAxis->setRange(spectra_2d_qcp->xAxis->range().lower, value);
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_2d_min_x(double value) {
    spectra_2d_qcp->xAxis->setRange(value, spectra_2d_qcp->xAxis->range().upper);
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_2d_max_y(double value) {
    spectra_2d_qcp->yAxis->setRange(spectra_2d_qcp->yAxis->range().lower, value);
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_2d_min_y(double value) {
    spectra_2d_qcp->yAxis->setRange(value, spectra_2d_qcp->yAxis->range().upper);
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_2d_max_z(double value) {
    color_scale_spectra_2d_qcp->axis()->setRange(color_scale_spectra_2d_qcp->axis()->range().lower, value);
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_set_spectra_2d_min_z(double value) {
    color_scale_spectra_2d_qcp->axis()->setRange(value, color_scale_spectra_2d_qcp->axis()->range().upper);
    spectra_2d_qcp->replot(QCustomPlot::rpQueuedReplot);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_mouse_move_spectra(QMouseEvent * event) {
    QString str;
    int n = spectra_qcp->graphCount();
    if (n == 0) return;

    if (!energy_spectra) {

        str = "%1 = %2\n";

        double x, y;
        spectra_qcp->graph(0)->pixelsToCoords(event->pos().x(), event->pos().y(), x, y);
        str = str.arg(spectra_qcp->xAxis->label()).arg(static_cast<int>(x + 0.5));

        for (int i = 0; i < n; i++) {
            double x, y;
            spectra_qcp->graph(i)->pixelsToCoords(event->pos().x(), event->pos().y(), x, y);
            bool result;
            QCPRange x_range = spectra_qcp->graph(i)->getKeyRange(result);
            if (result == false) continue;
            if (x_range.contains(x)) {
                QCPGraphDataContainer::const_iterator it0 = spectra_qcp->graph(i)->data()->findBegin(x);
                QCPGraphDataContainer::const_iterator it1 = it0 + 1;
                double x0 = it0->key;
                double x1 = it1->key;
                double half = (x0 + x1) / 2;
                if (x < half) {
                    y = it0->value;
                } else {
                    if (x1 > x_range.upper) {
                        y = it0->value;
                    } else {
                        y = it1->value;
                    }

                }
                str += QString("%1 = %2\n").arg(spectra_qcp->graph(i)->name()).arg(y);
            } else {
                str += QString("%1 = %2\n").arg(spectra_qcp->graph(i)->name()).arg(0);
            }
        }

    } else {

        for (int i = 0; i < n; i++) {
            double x, y;
            spectra_qcp->graph(i)->pixelsToCoords(event->pos().x(), event->pos().y(), x, y);
            bool result;
            QCPRange x_range = spectra_qcp->graph(i)->getKeyRange(result);
            if (result == false) continue;

            if (x_range.contains(x)) {
                QCPGraphDataContainer::const_iterator it0 = spectra_qcp->graph(i)->data()->findBegin(x);
                QCPGraphDataContainer::const_iterator it1 = it0 + 1;
                double x0 = it0->key;
                double x1 = it1->key;
                double half = (x0 + x1) / 2;
                if (x < half) {
                    x = it0->key;
                    y = it0->value;
                } else {
                    x = it1->key;
                    y = it1->value;
                    if (x1 > x_range.upper) {
                        x = it0->key;
                        y = it0->value;
                    } else {
                        x = it1->key;
                        y = it1->value;
                    }
                }
                str += QString("%1: energy bin value = %2; count = %3.\n").arg(spectra_qcp->graph(i)->name()).arg(x).arg(y);
            } else {
                str += QString("%1: energy bin value = %2; count = %3.\n").arg(spectra_qcp->graph(i)->name()).arg(x).arg(0);
            }
        }
    }
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
    QString str;

    for (int i = 0; i < n; i++) {
        double x, y;
        distribution_qcp->graph(i)->pixelsToCoords(event->pos().x(), event->pos().y(), x, y);
        bool result;
        QCPRange x_range = distribution_qcp->graph(i)->getKeyRange(result);
        if (result == false) continue;

        if (x_range.contains(x)) {
            QCPGraphDataContainer::const_iterator it0 = distribution_qcp->graph(i)->data()->findBegin(x);
            QCPGraphDataContainer::const_iterator it1 = it0 + 1;
            double x0 = it0->key;
            double x1 = it1->key;
            double half = (x0 + x1) / 2;
            if (x < half) {
                x = it0->key;
                y = it0->value;
            } else {
                x = it1->key;
                y = it1->value;
                if (x1 > x_range.upper) {
                    x = it0->key;
                    y = it0->value;
                } else {
                    x = it1->key;
                    y = it1->value;
                }
            }
            str += QString("%1: bin value = %2; count = %3.\n").arg(distribution_qcp->graph(i)->name()).arg(x).arg(y);
        } else {
            str += QString("%1: bin value = %2; count = %3.\n").arg(distribution_qcp->graph(i)->name()).arg(x).arg(0);
        }
    }
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, distribution_qcp);
}

void UC_wai::U_mouse_move_chip_fit(QMouseEvent * event) {
    QString str;
    int n = chip_fit_qcp->graphCount();
    if (n == 0) return;

    str = "%1 = %2\n";

    double x, y;
    chip_fit_qcp->graph(0)->pixelsToCoords(event->pos().x(), event->pos().y(), x, y);
    str = str.arg(chip_fit_qcp->xAxis->label()).arg(static_cast<int>(x + 0.5));

    for (int i = 0; i < n; i++) {
        double x, y;
        chip_fit_qcp->graph(i)->pixelsToCoords(event->pos().x(), event->pos().y(), x, y);
        bool result;
        QCPRange x_range = chip_fit_qcp->graph(i)->getKeyRange(result);
        if (result == false) continue;
        if (x_range.contains(x)) {
            QCPGraphDataContainer::const_iterator it0 = chip_fit_qcp->graph(i)->data()->findBegin(x);
            QCPGraphDataContainer::const_iterator it1 = it0 + 1;
            double x0 = it0->key;
            double x1 = it1->key;
            double half = (x0 + x1) / 2;
            if (x < half) {
                y = it0->value;
            } else {
                if (x1 > x_range.upper) {
                    y = it0->value;
                } else {
                    y = it1->value;
                }

            }
            str += QString("%1 = %2\n").arg(chip_fit_qcp->graph(i)->name()).arg(y);
        } else {
            str += QString("%1 = %2\n").arg(chip_fit_qcp->graph(i)->name()).arg(0);
        }
    };
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, chip_fit_qcp);
}

void UC_wai::U_mouse_move_calibration(QMouseEvent * event) {
    QString str;
    int n = calibration_qcp->graphCount();
    if (n == 0) return;

    str = "%1 = %2\n";

    double x, y;
    calibration_qcp->graph(0)->pixelsToCoords(event->pos().x(), event->pos().y(), x, y);
    str = str.arg(calibration_qcp->xAxis->label()).arg(static_cast<int>(x + 0.5));

    for (int i = 0; i < n; i++) {
        double x, y;
        calibration_qcp->graph(i)->pixelsToCoords(event->pos().x(), event->pos().y(), x, y);
        bool result;
        QCPRange x_range = calibration_qcp->graph(i)->getKeyRange(result);
        if (result == false) continue;
        if (x_range.contains(x)) {
            QCPGraphDataContainer::const_iterator it0 = calibration_qcp->graph(i)->data()->findBegin(x);
            QCPGraphDataContainer::const_iterator it1 = it0 + 1;
            double x0 = it0->key;
            double x1 = it1->key;
            double half = (x0 + x1) / 2;
            if (x < half) {
                y = it0->value;
            } else {
                if (x1 > x_range.upper) {
                    y = it0->value;
                } else {
                    y = it1->value;
                }

            }
            str += QString("%1 = %2\n").arg(calibration_qcp->graph(i)->name()).arg(y);
        } else {
            str += QString("%1 = %2\n").arg(calibration_qcp->graph(i)->name()).arg(0);
        }
    };
    US_mouse_data(str);
    QToolTip::showText(event->globalPos(), str, calibration_qcp);
}

void UC_wai::U_mouse_move_spectra_2d(QMouseEvent * event) {
    if (spectra_2d_qcp->plottableCount() == 0) return;
    QVariant details;
    double data = color_map_spectra_2d_qcp->selectTest(event->pos(), false, &details);
    if (data > 0) {
        QCPDataSelection dataPoints = details.value<QCPDataSelection>();
        if (dataPoints.dataPointCount() > 0) {
            double x = spectra_2d_qcp->xAxis->pixelToCoord(event->pos().x());
            double y = spectra_2d_qcp->yAxis->pixelToCoord(event->pos().y());
            int cell_x, cell_y;
            color_map_spectra_2d_qcp->data()->coordToCell(x, y, &cell_x, &cell_y);
            double z = color_map_spectra_2d_qcp->data()->cell(cell_x, cell_y);
            QString str = "x = %1; y = %2; z = %3";
            str = str.arg(x).arg(y).arg(z);
            US_mouse_data(str);
            QToolTip::showText(event->globalPos(), str, spectra_2d_qcp);
        }
    }
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

void UC_wai::U_mouse_select_chip_fit(QMouseEvent * event) {
    if (!select) return;
    int xmin, xmax, ymin, ymax;
    xmax = static_cast<int>(std::round(sr_chip_fit_qcp->range(chip_fit_qcp->xAxis).upper));
    xmin = static_cast<int>(std::round(sr_chip_fit_qcp->range(chip_fit_qcp->xAxis).lower));
    value_max = sr_chip_fit_qcp->range(chip_fit_qcp->yAxis).upper;
    value_min = sr_chip_fit_qcp->range(chip_fit_qcp->yAxis).lower;
    ymin = static_cast<int>(std::round(value_min));
    ymax = static_cast<int>(std::round(value_max));
    emit US_set_thl_range(xmin, xmax);
    thl_start = xmin;
    thl_finish = xmax;
    emit US_set_spectra_2d_range(ymin, ymax);
}

void UC_wai::U_mouse_select_calibration(QMouseEvent * event) {
    if (!select) return;
    int xmin, xmax;
    xmax = static_cast<int>(std::round(sr_calibration_qcp->range(calibration_qcp->xAxis).upper));
    xmin = static_cast<int>(std::round(sr_calibration_qcp->range(calibration_qcp->xAxis).lower));
    value_max = sr_calibration_qcp->range(calibration_qcp->yAxis).upper;
    value_min = sr_calibration_qcp->range(calibration_qcp->yAxis).lower;
    emit US_set_thl_range(xmin, xmax);
    thl_start = xmin;
    thl_finish = xmax;
}

void UC_wai::U_mouse_select_spectra_2d(QMouseEvent * event) {
    if (spectra_2d_qcp->plottableCount() == 0) return;
    if (!select) return;
    int xmin, xmax, ymin, ymax;
    xmax = static_cast<int>(std::round(sr_spectra_2d_qcp->range(spectra_2d_qcp->xAxis).upper));
    xmin = static_cast<int>(std::round(sr_spectra_2d_qcp->range(spectra_2d_qcp->xAxis).lower));
    ymax = static_cast<int>(std::round(sr_spectra_2d_qcp->range(spectra_2d_qcp->yAxis).upper));
    ymin = static_cast<int>(std::round(sr_spectra_2d_qcp->range(spectra_2d_qcp->yAxis).lower));
    value_max = ymax;
    value_min = ymin;
    //emit US_set_roi_range(xmin, xmax, ymin, ymax);
    thl_start = xmin;
    thl_finish = xmax;
    emit US_set_distribution_range(ymin, ymax);
    emit US_set_thl_range(xmin, xmax);
}
//
void UC_wai::U_renew_scans(QList<UC_data_container> * list_scans_ptr, int active_index) {
    disconnect(table_widget, SIGNAL(cellChanged(int, int)), this, SLOT(U_table_widget_changed()));
    const int n = list_scans_ptr->size();
    const int row_count = table_widget->rowCount();

    QStringList name_list;
    name_list << "disable";
    for (int i = 0; i < n; i++) {
        name_list << (*list_scans_ptr)[i].U_get_name();
    }

    for (int i = 0; i < row_count; i++) {
        if (i >= n) {
            table_widget->removeRow(i);
            continue;
        }

        UC_data_container::UTStr_data_container_settings settings = (*list_scans_ptr)[i].U_get_settings();

        table_widget->item(i, 0)->setText(settings.name);
        table_widget->item(i, 1)->setText(settings.path);

        QComboBox * combo_box_ff = reinterpret_cast<QComboBox *>(table_widget->cellWidget(i, 2));
        disconnect(combo_box_ff, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));
        combo_box_ff->clear();
        combo_box_ff->addItems(name_list);
        combo_box_ff->setCurrentIndex(settings.ff_int + 1);
        connect(combo_box_ff, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));

        QComboBox * combo_box_df = reinterpret_cast<QComboBox *>(table_widget->cellWidget(i, 3));
        disconnect(combo_box_df, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));
        combo_box_df->clear();
        combo_box_df->addItems(name_list);
        combo_box_df->setCurrentIndex(settings.df_int + 1);
        connect(combo_box_df, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));

        QSpinBox * spin_box_count = reinterpret_cast<QSpinBox *>(table_widget->cellWidget(i, 4));
        disconnect(spin_box_count, SIGNAL(valueChanged(int)), this, SLOT(U_table_widget_changed()));
        spin_box_count->setValue(settings.count);
        connect(spin_box_count, SIGNAL(valueChanged(int)), this, SLOT(U_table_widget_changed()));

        QDoubleSpinBox * spin_box_time = reinterpret_cast<QDoubleSpinBox *>(table_widget->cellWidget(i, 5));
        disconnect(spin_box_time, SIGNAL(valueChanged(double)), this, SLOT(U_table_widget_changed()));
        spin_box_time->setValue(settings.time);
        connect(spin_box_time, SIGNAL(valueChanged(double)), this, SLOT(U_table_widget_changed()));

        if (settings.both_counters) {
            table_widget->item(i, 6)->setText("two counters");
        } else {
            table_widget->item(i, 6)->setText("one counter");
        }

        table_widget->item(i, 7)->setText(QString("%1").arg(settings.n_thl));

        QComboBox * combo_box_calibration = reinterpret_cast<QComboBox *>(table_widget->cellWidget(i, 8));
        disconnect(combo_box_calibration, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));
        if (settings.calibration) {
            combo_box_calibration->setCurrentIndex(0);
        } else {
            combo_box_calibration->setCurrentIndex(1);
        }
        connect(combo_box_calibration, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));

        QDoubleSpinBox * double_spin_box_energy = reinterpret_cast<QDoubleSpinBox *>(table_widget->cellWidget(i, 9));
        disconnect(double_spin_box_energy, SIGNAL(valueChanged(double)), this, SLOT(U_table_widget_changed()));
        double_spin_box_energy->setValue(settings.energy);
        connect(double_spin_box_energy, SIGNAL(valueChanged(double)), this, SLOT(U_table_widget_changed()));

        QComboBox * combo_box_identification = reinterpret_cast<QComboBox *>(table_widget->cellWidget(i, 10));
        disconnect(combo_box_identification, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));
        if (settings.sample_of_element) {
            combo_box_identification->setCurrentIndex(0);
        } else {
            combo_box_identification->setCurrentIndex(1);
        }
        connect(combo_box_identification, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));

        QSpinBox * spin_box_thl = reinterpret_cast<QSpinBox *>(table_widget->cellWidget(i, 11));
        disconnect(spin_box_thl, SIGNAL(valueChanged(int)), this, SLOT(U_table_widget_changed()));
        spin_box_thl->setMinimum(settings.thl_min);
        spin_box_thl->setMaximum(settings.thl_max);
        spin_box_thl->setValue(settings.thl_sample);
        connect(spin_box_thl, SIGNAL(valueChanged(int)), this, SLOT(U_table_widget_changed()));

        table_widget->item(i, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table_widget->item(i, 6)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table_widget->item(i, 7)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }

    for (int i = row_count; i < n; i++) {
        table_widget->insertRow(i);
        UC_data_container::UTStr_data_container_settings settings = (*list_scans_ptr)[i].U_get_settings();

        table_widget->setItem(i, 0, new QTableWidgetItem(settings.name));
        table_widget->setItem(i, 1, new QTableWidgetItem(settings.path));

        QComboBox * combo_box_ff = new QComboBox(table_widget);
        combo_box_ff->addItems(name_list);
        combo_box_ff->setCurrentIndex(settings.ff_int + 1);
        table_widget->setCellWidget(i, 2, combo_box_ff);
        connect(combo_box_ff, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));

        QComboBox * combo_box_df = new QComboBox(table_widget);
        combo_box_df->addItems(name_list);
        combo_box_df->setCurrentIndex(settings.df_int + 1);
        table_widget->setCellWidget(i, 3, combo_box_df);
        connect(combo_box_df, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));

        QSpinBox * spin_box_count = new QSpinBox(table_widget);
        spin_box_count->setRange(1, 999999999);
        spin_box_count->setValue(settings.count);
        table_widget->setCellWidget(i, 4, spin_box_count);
        connect(spin_box_count, SIGNAL(valueChanged(int)), this, SLOT(U_table_widget_changed()));

        QDoubleSpinBox * double_spin_box_time = new QDoubleSpinBox(table_widget);
        double_spin_box_time->setRange(1e-10, 1e10);
        double_spin_box_time->setValue(settings.time);
        table_widget->setCellWidget(i, 5, double_spin_box_time);
        connect(double_spin_box_time, SIGNAL(valueChanged(double)), this, SLOT(U_table_widget_changed()));

        if (settings.both_counters) {
            table_widget->setItem(i, 6, new QTableWidgetItem("two counters"));
        } else {
            table_widget->setItem(i, 6, new QTableWidgetItem("one counter"));
        }

        table_widget->setItem(i, 7, new QTableWidgetItem(QString("%1").arg(settings.n_thl)));

        QComboBox * combo_box_calibration = new QComboBox(table_widget);
        combo_box_calibration->addItem("enable");
        combo_box_calibration->addItem("disable");
        if (settings.calibration) {
            combo_box_calibration->setCurrentIndex(0);
        } else {
            combo_box_calibration->setCurrentIndex(1);
        }
        table_widget->setCellWidget(i, 8, combo_box_calibration);
        connect(combo_box_calibration, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));

        QDoubleSpinBox * double_spin_box_energy = new QDoubleSpinBox(table_widget);
        double_spin_box_energy->setRange(1, 1000);
        double_spin_box_energy->setValue(settings.energy);
        table_widget->setCellWidget(i, 9, double_spin_box_energy);
        connect(double_spin_box_energy, SIGNAL(valueChanged(double)), this, SLOT(U_table_widget_changed()));

        QComboBox * combo_box_identification = new QComboBox(table_widget);
        combo_box_identification->addItem("enable");
        combo_box_identification->addItem("disable");
        if (settings.sample_of_element) {
            combo_box_identification->setCurrentIndex(0);
        } else {
            combo_box_identification->setCurrentIndex(1);
        }
        table_widget->setCellWidget(i, 10, combo_box_identification);
        connect(combo_box_identification, SIGNAL(currentIndexChanged(int)), this, SLOT(U_table_widget_changed()));

        QSpinBox * spin_box_thl = new QSpinBox(table_widget);
        spin_box_thl->setRange(settings.thl_min, settings.thl_max);
        spin_box_thl->setValue(settings.thl_sample);
        table_widget->setCellWidget(i, 11, spin_box_thl);
        connect(spin_box_thl, SIGNAL(valueChanged(int)), this, SLOT(U_table_widget_changed()));

        QPushButton * push_button_active = new QPushButton(table_widget);
        push_button_active->setText("Active");
        table_widget->setCellWidget(i, 12, push_button_active);
        connect(push_button_active, SIGNAL(clicked()), this, SLOT(U_table_widget_active()));

        QPushButton * push_button_delete = new QPushButton(table_widget);
        push_button_delete->setText("Delete");
        table_widget->setCellWidget(i, 13, push_button_delete);
        connect(push_button_delete, SIGNAL(clicked()), this, SLOT(U_table_widget_delete()));

        table_widget->item(i, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table_widget->item(i, 6)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table_widget->item(i, 7)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }

    if (active_index == -1) return;
    if (active_index < table_widget->rowCount()) {
        for (int i = 0; i < table_widget->rowCount(); i++) {
            table_widget->item(i, 0)->setBackground(QBrush(Qt::white));
        }
        table_widget->item(active_index, 0)->setBackground(QBrush(Qt::green));
    }

    connect(table_widget, SIGNAL(cellChanged(int, int)), this, SLOT(U_table_widget_changed()));
    table_widget->resizeColumnsToContents();
}

void UC_wai::U_add_scan(UC_data_container::UTStr_data_container_settings settings) {
}

void UC_wai::U_table_widget_changed() {
    int row = table_widget->currentRow();
    if (row == -1) return;
    UC_data_container::UTStr_data_container_settings settings;
    settings.name = table_widget->item(row, 0)->text();
    QComboBox * combo_box_ff = reinterpret_cast<QComboBox *>(table_widget->cellWidget(row, 2));
    settings.ff_int = combo_box_ff->currentIndex() - 1;
    QComboBox * combo_box_df = reinterpret_cast<QComboBox *>(table_widget->cellWidget(row, 3));
    settings.df_int = combo_box_df->currentIndex() - 1;
    if (settings.ff_int == -2) return;
    if (settings.df_int == -2) return;
    QSpinBox * spin_box_count = reinterpret_cast<QSpinBox *>(table_widget->cellWidget(row, 4));
    settings.count = spin_box_count->value();
    settings.time = reinterpret_cast<QDoubleSpinBox*>(table_widget->cellWidget(row, 5))->value();
    int calibration = reinterpret_cast<QComboBox*>(table_widget->cellWidget(row, 8))->currentIndex();
    if (calibration == 0) {
        settings.calibration = true;
    } else {
        settings.calibration = false;
    }
    settings.energy = reinterpret_cast<QDoubleSpinBox*>(table_widget->cellWidget(row, 9))->value();
    int identification = reinterpret_cast<QComboBox*>(table_widget->cellWidget(row, 10))->currentIndex();
    if (identification == 0) {
        settings.sample_of_element = true;
    } else {
        settings.sample_of_element = false;
    }
    settings.thl_sample = reinterpret_cast<QSpinBox*>(table_widget->cellWidget(row, 11))->value();
    emit US_set_settings(row, &settings);
}

void UC_wai::U_table_widget_active() {
    int row = table_widget->currentRow();
    if (row == -1) return;
    for (int i = 0; i < table_widget->rowCount(); i++) {
        table_widget->item(i, 0)->setBackground(QBrush(Qt::white));
    }
    table_widget->item(row, 0)->setBackground(QBrush(Qt::green));
    emit US_set_scan(row);
}

void UC_wai::U_table_widget_delete() {
    int row = table_widget->currentRow();
    if (row == -1) return;
    //table_widget->removeRow(row);
    emit US_delete_scan(row);
}
