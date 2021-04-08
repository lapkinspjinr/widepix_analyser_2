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

    connect(this, SIGNAL(US_set_data(QString, int, double, bool)),                  plot, SLOT(U_set_data(QString, int, double, bool)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_data_df(QString, int, double, bool)),               plot, SLOT(U_set_data_df(QString, int, double, bool)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_data_ff(QString, int, double, bool)),               plot, SLOT(U_set_data_ff(QString, int, double, bool)), Qt::DirectConnection);
    connect(this, SIGNAL(US_reset_data()),                                          plot, SLOT(U_reset_data()), Qt::DirectConnection);

    connect(this, SIGNAL(US_generate_plot()),                                       plot, SLOT(U_generate_spectra()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_frame(int)),                                   plot, SLOT(U_generate_frame(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_table(int)),                                   plot, SLOT(U_generate_table(int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_table()),                                      plot, SLOT(U_generate_table()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_distribution(int, double, double)),            plot, SLOT(U_generate_distribution(int, double, double)), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_frame_distribution(int, double, double, int)), plot, SLOT(U_generate_frame_distribution(int, double, double, int)), Qt::DirectConnection);
//    connect(this, SIGNAL(US_generate_calibration()),                                plot, SLOT(U_generate_calibration()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_range()),                                      plot, SLOT(U_generate_range()), Qt::DirectConnection);
    connect(this, SIGNAL(US_generate_range(int)),                                   plot, SLOT(U_generate_range(int)), Qt::DirectConnection);

    connect(this, SIGNAL(US_set_frame_type(UC_plot::UTE_frame_type)),                   plot, SLOT(U_set_frame_type(UC_plot::UTE_frame_type)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_pixel_type(UC_plot::UTE_pixel_type)),                   plot, SLOT(U_set_pixel_type(UC_plot::UTE_pixel_type)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_roi(int, int, int, int)),                               plot, SLOT(U_set_roi(int, int, int, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_identification_type(UC_plot::UTE_identification_type)), plot, SLOT(U_set_identification_type(UC_plot::UTE_identification_type)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_thresholds(int, int, int, int)),                        plot, SLOT(U_set_thresholds(int, int, int, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_threshold_range(int, int)),                             plot, SLOT(U_set_threshold_range(int, int)), Qt::DirectConnection);

    connect(this, SIGNAL(US_set_mask(bool, bool, double, bool, int)),               plot, SLOT(U_set_mask(bool, bool, double, bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_count_mask(bool, double, bool, int)),                   plot, SLOT(U_count_mask(bool, double, bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_mask_overflowed(bool, int)),                        plot, SLOT(U_set_mask_overflowed(bool, int)), Qt::DirectConnection);
    connect(this, SIGNAL(US_set_mask_overflowed(bool)),                             plot, SLOT(U_set_mask_overflowed(bool)), Qt::DirectConnection);
    connect(this, SIGNAL(US_reset_mask()),                                          plot, SLOT(U_reset_mask()), Qt::DirectConnection);

    connect(plot, SIGNAL(US_plot_data(double, double)),                             this, SLOT(U_add_spectra_data(double, double)), Qt::DirectConnection);
    connect(plot, SIGNAL(US_frame_data(double, double, double)),                    this, SLOT(U_add_frame_data(double, double, double)), Qt::DirectConnection);
    connect(plot, SIGNAL(US_chip_data(UC_pixels_info)),                             this, SLOT(U_add_table_data(UC_pixels_info)), Qt::DirectConnection);
    connect(plot, SIGNAL(US_distribution_data(double, double)),                     this, SLOT(U_add_distibution_data(double, double)), Qt::DirectConnection);
//    connect(plot, SIGNAL(US_calibration_data(double, double)),                      this, SLOT(U_add_calibration_data(double, double)), Qt::DirectConnection);

    connect(plot, SIGNAL(US_replot_spectra()),                                      this, SLOT(U_replot_spectra()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_frame()),                                        this, SLOT(U_replot_frame()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_rewrite_table()),                                       this, SLOT(U_rewrite_table()), Qt::DirectConnection);
    connect(plot, SIGNAL(US_replot_distribution()),                                 this, SLOT(U_replot_distribution()), Qt::DirectConnection);
    //connect(plot, SIGNAL(US_replot_calibration()),                                  this, SLOT(U_replot_calibration()), Qt::DirectConnection);

    connect(plot, SIGNAL(US_new_thl(int)),                                          this, SLOT(U_new_thl(int)), Qt::DirectConnection);

    plot->moveToThread(thread);

    //thread->start();
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

    qcp->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectLegend);
    qcp->xAxis->setLabel("thl");
    connect(qcp->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_spectra_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_spectra_y_axis_range(QCPRange)), Qt::DirectConnection);

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

    connect(qcp_2->xAxis, SIGNAL(rangeChanged(QCPRange)),            this, SLOT(U_set_frame_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp_2->yAxis, SIGNAL(rangeChanged(QCPRange)),            this, SLOT(U_set_frame_y_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp_color_scale->axis(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_frame_z_axis_range(QCPRange)), Qt::DirectConnection);

    this->qcp_3 = qcp_3;
    qcp_3->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    connect(qcp_3->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_distribution_x_axis_range(QCPRange)), Qt::DirectConnection);
    connect(qcp_3->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(U_set_distribution_y_axis_range(QCPRange)), Qt::DirectConnection);
}

void UC_wai::U_set_table(QTableView * table) {
    this->table = table;

    model = new QStandardItemModel(12, 22);
    table->setModel(model);
    index = model->index(0, 0);
    model->setData(index, "tittle");
    index = model->index(1, 0);
    model->setData(index, "number of pixel");
    index = model->index(2, 0);
    model->setData(index, "min");
    index = model->index(3, 0);
    model->setData(index, "max");
    index = model->index(4, 0);
    model->setData(index, "sum");
    index = model->index(5, 0);
    model->setData(index, "mean");
    index = model->index(6, 0);
    model->setData(index, "median");
    index = model->index(7, 0);
    model->setData(index, "zeros");
    index = model->index(8, 0);
    model->setData(index, "overflows");
    index = model->index(9, 0);
    model->setData(index, "masked");
    index = model->index(10, 0);
    model->setData(index, "standart deviation");
    index = model->index(11, 0);
    model->setData(index, "signal to noise resolution");
    index = model->index(0, 1);
    model->setData(index, "chip 1");
    index = model->index(0, 2);
    model->setData(index, "chip 2");
    index = model->index(0, 3);
    model->setData(index, "chip 3");
    index = model->index(0, 4);
    model->setData(index, "chip 4");
    index = model->index(0, 5);
    model->setData(index, "chip 5");
    index = model->index(0, 6);
    model->setData(index, "chip 6");
    index = model->index(0, 7);
    model->setData(index, "chip 7");
    index = model->index(0, 8);
    model->setData(index, "chip 8");
    index = model->index(0, 9);
    model->setData(index, "chip 9");
    index = model->index(0, 10);
    model->setData(index, "chip 10");
    index = model->index(0, 11);
    model->setData(index, "chip 11");
    index = model->index(0, 12);
    model->setData(index, "chip 12");
    index = model->index(0, 13);
    model->setData(index, "chip 13");
    index = model->index(0, 14);
    model->setData(index, "chip 14");
    index = model->index(0, 15);
    model->setData(index, "chip 15");
    index = model->index(0, 16);
    model->setData(index, "widepix");
    index = model->index(0, 17);
    model->setData(index, "ROI");
    index = model->index(0, 18);
    model->setData(index, "masked");
    index = model->index(0, 19);
    model->setData(index, "unmasked");
    index = model->index(0, 20);
    model->setData(index, "not overflow");
    index = model->index(0, 21);
    model->setData(index, "good pixels in ROI");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_set_data(QString path, int count, double time, bool both_counters) {
    emit US_set_data(path, count, time, both_counters);
}

void UC_wai::U_set_data_df(QString path, int count, double time, bool both_counters) {
    emit US_set_data_df(path, count, time, both_counters);
}

void UC_wai::U_set_data_ff(QString path, int count, double time, bool both_counters) {
    emit US_set_data_ff(path, count, time, both_counters);
}

void UC_wai::U_reset_data() {
    emit US_reset_data();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_generate_plot(QString graph_name) {
    U_delete_mask_plots();
    qcp->addGraph();
    qcp->graph()->setName(graph_name);
    qcp->graph()->setPen(pens[qcp->graphCount() % 16]);
    qcp->graph()->setLineStyle(QCPGraph::lsStepCenter);

    emit US_generate_plot();
}

void UC_wai::U_delete_last_graph() {
    qcp->removeGraph(qcp->graphCount() - 1);
    qcp->replot(QCustomPlot::rpQueuedReplot);
}

void UC_wai::U_reset_plot() {
    qcp->clearGraphs();
    qcp->replot(QCustomPlot::rpQueuedReplot);
}
///
void UC_wai::U_generate_frame(int thl) {
    emit US_generate_frame(thl);
}

void UC_wai::U_generate_table(int thl) {
    emit US_generate_table(thl);
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

//void UC_wai::U_generate_calibration() {
//    qcp_4->clearGraphs();
//    qcp_4->addGraph();
//    qcp_4->graph()->setLineStyle(QCPGraph::lsStepCenter);

//    emit US_generate_calibration();
//}


void UC_wai::U_generate_range() {
    emit US_generate_range();
}

void UC_wai::U_generate_range(int thl_index) {
    emit US_generate_range(thl_index);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_set_frame_type(UC_plot::UTE_frame_type frame_type_in) {
    emit US_set_frame_type(frame_type_in);
}

void UC_wai::U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type_in) {
    emit US_set_pixel_type(pixel_type_in);
}

void UC_wai::U_set_roi(int x_min, int x_max, int y_min, int y_max) {
    emit US_set_roi(x_min, x_max, y_min, y_max);
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
}

void UC_wai::U_set_identification_type(UC_plot::UTE_identification_type identification_type) {
    emit US_set_identification_type(identification_type);
}

void UC_wai::U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4) {
    emit US_set_thresholds(thl_id_1, thl_id_2, thl_id_3, thl_id_4);
}

void UC_wai::U_set_threshold_range(int thl_min, int thl_max) {
    emit US_set_threshold_range(thl_min, thl_max);
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
////
void UC_wai::U_save_table_tsv(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int row = 0; row < (model->rowCount()); row++) {
        for (int column = 0; column < (model->columnCount() - 1); column++) {
            index = model->index(row, column);
            str << model->data(index).toString();
            str << QChar(9);
        }
        index = model->index(row, model->columnCount() - 1);
        str << model->data(index).toString();
        str << endl;
    }
    file.close();
}

void UC_wai::U_save_table_csv(QString file_name) {
    QFile file(file_name);
    file.open(QFile::Truncate | QFile::WriteOnly);
    QTextStream str(&file);
    for (int row = 0; row < (model->rowCount()); row++) {
        for (int column = 0; column < (model->columnCount() - 1); column++) {
            index = model->index(row, column);
            str << model->data(index).toString();
            str << ",";
        }
        index = model->index(row, model->columnCount() - 1);
        str << model->data(index).toString();
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



//////////////////////////////////////////////////////////////////////////////////////////
void UC_wai::U_set_mask(bool mask, bool more, double value, bool in_roi, int thl_index) {
    emit US_set_mask(mask, more, value, in_roi, thl_index);
}

void UC_wai::U_count_mask(bool more, double value, bool in_roi, int thl_index) {
    emit US_count_mask(more, value, in_roi, thl_index);
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

void UC_wai::U_set_mask_overflowed(bool in_roi, int thl_index) {
    emit US_set_mask_overflowed(in_roi, thl_index);
}

void UC_wai::U_set_mask_overflowed(bool in_roi) {
    emit US_set_mask_overflowed(in_roi);
}

void UC_wai::U_reset_mask() {
    emit US_reset_mask();
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
    index = model->index(1, column_index);
    model->setData(index, pixels_info.U_get_n());
    index = model->index(2, column_index);
    model->setData(index, pixels_info.U_get_min());
    index = model->index(3, column_index);
    model->setData(index, pixels_info.U_get_max());
    index = model->index(4, column_index);
    model->setData(index, pixels_info.U_get_sum());
    index = model->index(5, column_index);
    model->setData(index, pixels_info.U_get_mean());
    index = model->index(6, column_index);
    model->setData(index, pixels_info.U_get_median());
    index = model->index(7, column_index);
    model->setData(index, pixels_info.U_get_zeros());
    index = model->index(8, column_index);
    model->setData(index, pixels_info.U_get_overflows());
    index = model->index(9, column_index);
    model->setData(index, pixels_info.U_get_masked());
    index = model->index(10, column_index);
    model->setData(index, pixels_info.U_get_std_dev());
    index = model->index(11, column_index);
    model->setData(index, pixels_info.U_get_snr());
}

void UC_wai::U_add_distibution_data(double x, double y) {
    qcp_3->graph()->addData(x, y);
}

//void UC_wai::U_add_calibration_data(double x, double y) {
//    qcp_4->graph()->addData(x, y);
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UC_wai::U_replot_frame() {
    qcp_2->xAxis->setRange(x_min, x_max);
    qcp_2->yAxis->setRange(y_min, y_max);
    U_rescale_frame();
    qcp_2->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp_2, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_frame(QMouseEvent*)));
    emit US_set_distribution_range(qcp_color_scale->axis()->range().lower, qcp_color_scale->axis()->range().upper);
}

void UC_wai::U_replot_spectra() {
    qcp->legend->setVisible(true);
    qcp->rescaleAxes(true);
    qcp->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_spectra(QMouseEvent*)));
    emit US_set_distribution_range(qcp->yAxis->range().lower, qcp->yAxis->range().upper);
}

void UC_wai::U_rewrite_table() {

}

void UC_wai::U_replot_distribution() {
    qcp_3->legend->setVisible(true);
    qcp_3->rescaleAxes(true);
    qcp_3->replot(QCustomPlot::rpQueuedReplot);
    connect(qcp_3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(U_mouse_move_distribution(QMouseEvent*)));
}

//void UC_wai::U_replot_calibration() {
//    qcp_4->rescaleAxes(true);
//    qcp_4->replot(QCustomPlot::rpQueuedReplot);
//}
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
    str = str.arg(static_cast<int>(it->key)).arg(qcp_3->graph(0)->name()).arg(it->value);

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
void UC_wai::U_new_thl(int thl) {
    emit US_new_thl(thl);
}

