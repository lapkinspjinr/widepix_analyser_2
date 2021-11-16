#include "widepix_analyser_2.h"
#include "ui_widepix_analyser_2.h"

widepix_analyser_2::widepix_analyser_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::widepix_analyser_2)
{
    ui->setupUi(this);

    wai = new UC_wai();
    wai->setParent(this);

    wai->U_set_spectra_qcp(ui->widget);
    wai->U_set_frame_qcp(ui->widget_2);
    wai->U_set_distribution_qcp(ui->widget_3);
    wai->U_set_chip_fit_qcp(ui->widget_4);
    wai->U_set_calibration_qcp(ui->widget_5);
    wai->U_set_spectra_2d_qcp(ui->widget_6);
    wai->U_set_table(ui->tableView);
    wai->U_set_table_widget(ui->tableWidget);

    connect(ui->doubleSpinBox_4, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_max_x(double)));
    connect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_min_x(double)));
    connect(ui->doubleSpinBox,   SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_max_y(double)));
    connect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_min_y(double)));
    ////
    connect(ui->doubleSpinBox_8,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_max_x(double)));
    connect(ui->doubleSpinBox_7,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_min_x(double)));
    connect(ui->doubleSpinBox_5,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_max_y(double)));
    connect(ui->doubleSpinBox_6,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_min_y(double)));
    connect(ui->doubleSpinBox_9,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_max_z(double)));
    connect(ui->doubleSpinBox_10, SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_min_z(double)));
    ////
    connect(ui->doubleSpinBox_14, SIGNAL(valueChanged(double)), wai, SLOT(U_set_distribution_max_x(double)));
    connect(ui->doubleSpinBox_13, SIGNAL(valueChanged(double)), wai, SLOT(U_set_distribution_min_x(double)));
    connect(ui->doubleSpinBox_12, SIGNAL(valueChanged(double)), wai, SLOT(U_set_distribution_max_y(double)));
    connect(ui->doubleSpinBox_15, SIGNAL(valueChanged(double)), wai, SLOT(U_set_distribution_min_y(double)));
    //
    connect(ui->doubleSpinBox_24, SIGNAL(valueChanged(double)), wai, SLOT(U_set_chip_fit_max_x(double)));
    connect(ui->doubleSpinBox_25, SIGNAL(valueChanged(double)), wai, SLOT(U_set_chip_fit_min_x(double)));
    connect(ui->doubleSpinBox_23, SIGNAL(valueChanged(double)), wai, SLOT(U_set_chip_fit_max_y(double)));
    connect(ui->doubleSpinBox_26, SIGNAL(valueChanged(double)), wai, SLOT(U_set_chip_fit_min_y(double)));
    //
    connect(ui->doubleSpinBox_27, SIGNAL(valueChanged(double)), wai, SLOT(U_set_calibration_max_x(double)));
    connect(ui->doubleSpinBox_28, SIGNAL(valueChanged(double)), wai, SLOT(U_set_calibration_min_x(double)));
    connect(ui->doubleSpinBox_29, SIGNAL(valueChanged(double)), wai, SLOT(U_set_calibration_max_y(double)));
    connect(ui->doubleSpinBox_30, SIGNAL(valueChanged(double)), wai, SLOT(U_set_calibration_min_y(double)));
    ////
    connect(ui->doubleSpinBox_35, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_2d_max_x(double)));
    connect(ui->doubleSpinBox_34, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_2d_min_x(double)));
    connect(ui->doubleSpinBox_32, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_2d_max_y(double)));
    connect(ui->doubleSpinBox_33, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_2d_min_y(double)));
    connect(ui->doubleSpinBox_38, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_2d_max_z(double)));
    connect(ui->doubleSpinBox_39, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_2d_min_z(double)));

    on_checkBox_13_toggled(true);
    //
    connect(wai, SIGNAL(US_mouse_data(QString)),                                                    ui->statusBar,  SLOT(showMessage(QString)));
    connect(wai, SIGNAL(US_set_distribution_range(double, double)),                                 this,           SLOT(U_set_distribution_range(double, double)));
    connect(wai, SIGNAL(US_set_spectra_2d_range(double, double)),                                   this,           SLOT(U_set_spectra_2D_range(double, double)));
    connect(wai, SIGNAL(US_add_roi(QString)),                                                       this,           SLOT(U_add_roi(QString)));
    connect(wai, SIGNAL(US_set_roi_range(int, int, int, int)),                                      this,           SLOT(U_set_roi_range(int, int, int, int)));
    connect(wai, SIGNAL(US_new_spectra(QString)),                                                   this,           SLOT(U_new_spectra(QString)));

    connect(this, SIGNAL(US_set_threshold_level(double)),   wai->U_get_plot(), SLOT(U_set_threshold_level(double)));
    connect(this, SIGNAL(US_save_calibration(QString)),     wai->U_get_plot(), SLOT(U_save_calibration(QString)));
    connect(this, SIGNAL(US_load_calibration(QString)),     wai->U_get_plot(), SLOT(U_load_calibration(QString)));

    connect(this,               SIGNAL(US_set_data(UC_data_container::UTStr_data_container_settings *)),                    wai->U_get_plot(),  SLOT(U_set_data(UC_data_container::UTStr_data_container_settings *)), Qt::QueuedConnection);
    connect(this,               SIGNAL(US_set_scan(int)),                                                                   wai->U_get_plot(),  SLOT(U_set_scan(int)),                                                              Qt::DirectConnection);
    connect(this,               SIGNAL(US_change_scan_settings(int, UC_data_container::UTStr_data_container_settings *)),   wai->U_get_plot(),  SLOT(U_set_settings(int, UC_data_container::UTStr_data_container_settings*)),       Qt::DirectConnection);
    connect(this,               SIGNAL(US_delete_scan(int)),                                                                wai->U_get_plot(),  SLOT(U_delete_scan(int)),                                                           Qt::DirectConnection);
    connect(this,               SIGNAL(US_reset_data()),                                                                    wai->U_get_plot(),  SLOT(U_reset_data()),                                                               Qt::DirectConnection);
    connect(this,               SIGNAL(US_get_scan_settings(int)),                                                          wai->U_get_plot(),  SLOT(U_get_settings(int)),                                                          Qt::DirectConnection);
    connect(this,               SIGNAL(US_scan_changed()),                                                                  wai,                SLOT(U_table_widget_changed()),                                                     Qt::DirectConnection);
    connect(wai->U_get_plot(),  SIGNAL(US_list_scans(QList<UC_data_container> *, int)),                                     this,               SLOT(U_set_scans(QList<UC_data_container> *, int)),                                 Qt::DirectConnection);
    connect(wai->U_get_plot(),  SIGNAL(US_scan_settings(UC_data_container::UTStr_data_container_settings *)),               this,               SLOT(U_change_scan_settings(UC_data_container::UTStr_data_container_settings *)),   Qt::DirectConnection);

    connect(this, SIGNAL(US_set_mask(bool, bool, double, bool, int)),       wai->U_get_plot(),  SLOT(U_set_mask(bool, bool, double, bool, int))/*,                Qt::DirectConnection*/);
    connect(this, SIGNAL(US_set_mask(bool, bool, double, bool)),            wai->U_get_plot(),  SLOT(U_set_mask(bool, bool, double, bool))/*,                     Qt::DirectConnection*/);
    connect(this, SIGNAL(US_count_mask(bool, double, bool, int)),           wai->U_get_plot(),  SLOT(U_count_mask(bool, double, bool, int))/*,                    Qt::DirectConnection*/);
    connect(this, SIGNAL(US_count_mask(bool, double, bool)),                wai->U_get_plot(),  SLOT(U_count_mask(bool, double, bool))/*,                         Qt::DirectConnection*/);
    connect(this, SIGNAL(US_set_mask_overflowed(bool, int)),                wai->U_get_plot(),  SLOT(U_set_mask_overflowed(bool, int))/*,                         Qt::DirectConnection*/);
    connect(this, SIGNAL(US_set_mask_overflowed(bool)),                     wai->U_get_plot(),  SLOT(U_set_mask_overflowed(bool))/*,                              Qt::DirectConnection*/);
    connect(this, SIGNAL(US_set_mask_gauss(bool, bool, double, bool, int)), wai->U_get_plot(),  SLOT(U_set_mask_sd_from_mean(bool, bool, double, bool, int))/*,   Qt::DirectConnection*/);
    connect(this, SIGNAL(US_set_mask_gauss(bool, bool, double, bool)),      wai->U_get_plot(),  SLOT(U_set_mask_sd_from_mean(bool, bool, double, bool))/*,        Qt::DirectConnection*/);
    connect(this, SIGNAL(US_reset_mask()),                                  wai->U_get_plot(),  SLOT(U_reset_mask())/*,                                           Qt::DirectConnection*/);
    connect(this, SIGNAL(US_save_mask(QString)),                            wai->U_get_plot(),  SLOT(U_save_mask(QString))/*,                                     Qt::DirectConnection*/);
    connect(this, SIGNAL(US_load_mask(QString)),                            wai->U_get_plot(),  SLOT(U_load_mask(QString))/*,                                     Qt::DirectConnection*/);

    connect(this, SIGNAL(US_set_threshold_range(int, int)),     wai->U_get_plot(), SLOT(U_set_threshold_range(int, int)));
    connect(this, SIGNAL(US_set_energy_range(double, double)),  wai->U_get_plot(), SLOT(U_set_energy_range(double, double)));

    connect(wai->U_get_plot(), SIGNAL(US_thl_range(int, int)),                      this, SLOT(U_set_thl_range(int, int)));
    connect(wai->U_get_plot(), SIGNAL(US_thl_vector(QVector<int>)),                 this, SLOT(U_add_thl(QVector<int>)));
    connect(wai->U_get_plot(), SIGNAL(US_energy_range(double, double)),             this, SLOT(U_set_energy_range(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_file_found(QString)),                      this, SLOT(U_file_found(QString)),                              Qt::DirectConnection);
    connect(wai->U_get_plot(), SIGNAL(US_n_files(int)),                             this, SLOT(U_n_files(int)),                                     Qt::DirectConnection);
    connect(wai->U_get_plot(), SIGNAL(US_renew_progress_bar(double, double)),       this, SLOT(U_renew_progress_bar(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_range_data(double, double)),               this, SLOT(U_set_distribution_range(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_spectra_2d_range_data(double, double)),    this, SLOT(U_set_spectra_2D_range(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_count_mask(int)),                          this, SLOT(U_set_count_mask(int)));

    connect(wai->U_get_plot(),  SIGNAL(US_ready()), this,               SLOT(U_ready()),    Qt::DirectConnection);
    connect(this,               SIGNAL(US_stop()),  wai->U_get_plot(),  SLOT(U_stop()),     Qt::DirectConnection);

    wai->U_start();

    list_enabling << ui->pushButton_13;
    list_enabling << ui->pushButton_15;
    list_enabling << ui->pushButton_23;
    list_enabling << ui->pushButton_16;
    list_enabling << ui->pushButton_6;
    list_enabling << ui->pushButton_12;
    list_enabling << ui->pushButton_7;
    list_enabling << ui->pushButton_25;
    list_enabling << ui->pushButton_17;
    list_enabling << ui->pushButton_19;
    list_enabling << ui->pushButton_20;
    list_enabling << ui->pushButton_31;
    list_enabling << ui->pushButton_34;
    list_enabling << ui->pushButton_35;
    list_enabling << ui->pushButton_36;
    list_enabling << ui->pushButton_40;
    list_enabling << ui->pushButton_33;
    list_enabling << ui->pushButton_32;
    list_enabling << ui->pushButton_48;
    list_enabling << ui->pushButton_42;
    list_enabling << ui->pushButton_45;
    for (int i = 0; i < list_enabling.size(); i++) {
        list_enabling[i]->setEnabled(false);
    }

    list_enabling_busy << ui->pushButton;
    list_enabling_busy << ui->pushButton_8;
    list_enabling_busy << ui->pushButton_15;
    list_enabling_busy << ui->pushButton_13;
    list_enabling_busy << ui->pushButton_23;
    list_enabling_busy << ui->pushButton_44;
    list_enabling_busy << ui->pushButton_69;
    list_enabling_busy << ui->pushButton_46;
    list_enabling_busy << ui->pushButton_54;
    list_enabling_busy << ui->pushButton_31;
    list_enabling_busy << ui->pushButton_5;
    list_enabling_busy << ui->pushButton_6;
    list_enabling_busy << ui->pushButton_47;
    list_enabling_busy << ui->pushButton_7;
    list_enabling_busy << ui->pushButton_48;
    list_enabling_busy << ui->pushButton_40;
    list_enabling_busy << ui->pushButton_33;
    list_enabling_busy << ui->pushButton_32;
    list_enabling_busy << ui->pushButton_17;
    list_enabling_busy << ui->pushButton_20;
    list_enabling_busy << ui->pushButton_42;
    list_enabling_busy << ui->pushButton_53;
    list_enabling_busy << ui->pushButton_45;
    list_enabling_busy << ui->pushButton_26;
    list_enabling_busy << ui->pushButton_3;
    list_enabling_busy << ui->pushButton_27;
    list_enabling_busy << ui->pushButton_52;
    list_enabling_busy << ui->pushButton_28;
    list_enabling_busy << ui->pushButton_22;
    list_enabling_busy << ui->comboBox;
    list_enabling_busy << ui->comboBox_2;
    list_enabling_busy << ui->comboBox_3;
    list_enabling_busy << ui->comboBox_17;
    list_enabling_busy << ui->spinBox_12;
    list_enabling_busy << ui->doubleSpinBox_22;
    list_enabling_busy << ui->doubleSpinBox_20;
    list_enabling_busy << ui->doubleSpinBox_21;
    list_enabling_busy << ui->doubleSpinBox_11;
    list_enabling_busy << ui->radioButton_4;
    list_enabling_busy << ui->radioButton_3;


    roi = "all chips";
}

widepix_analyser_2::~widepix_analyser_2()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void widepix_analyser_2::U_add_thl(QVector<int> thl_vector)
{
    ui->comboBox->clear();
    ui->comboBox_21->clear();
    ui->comboBox_28->clear();
    QString str = QString("thl = %1");
    int n = thl_vector.size();
    for (int i = 0; i < n; i++) {
        ui->comboBox->addItem(str.arg(thl_vector[i]));
        ui->comboBox_21->addItem(str.arg(thl_vector[i]));
        ui->comboBox_28->addItem(str.arg(thl_vector[i]));
    }
    ui->comboBox_28->setCurrentIndex(ui->comboBox_28->count() - 1);
}

void widepix_analyser_2::U_set_thl_range(int thl_index_min, int thl_index_max)
{
    ui->comboBox_21->setCurrentIndex(thl_index_min);
    ui->comboBox_28->setCurrentIndex(thl_index_max);
    int index = ui->comboBox->currentIndex();
    if (index < thl_index_min) ui->comboBox->setCurrentIndex(thl_index_min);
    if (index > thl_index_max) ui->comboBox->setCurrentIndex(thl_index_max);

//    QString str = QString("thl = %1").arg(thl_min);

//    ui->comboBox->clear();
//    ui->comboBox_12->clear();
//    ui->comboBox_13->clear();
//    ui->comboBox_14->clear();
//    ui->comboBox_15->clear();
//    ui->spinBox_13->setRange(thl_min, thl_max);
//    ui->spinBox_14->setRange(thl_min, thl_max);
//    ui->spinBox_13->setValue(thl_min);
//    ui->spinBox_14->setValue(thl_max);
//    for (int thl = thl_min; thl <= thl_max; thl++) {
//        ui->comboBox->addItem(QString("thl = %1").arg(thl));
//        ui->comboBox_12->addItem(QString("thl = %1").arg(thl));
//        ui->comboBox_13->addItem(QString("thl = %1").arg(thl));
//        ui->comboBox_14->addItem(QString("thl = %1").arg(thl));
//        ui->comboBox_15->addItem(QString("thl = %1").arg(thl));
//    }
}

void widepix_analyser_2::U_set_energy_range(double energy_min, double energy_max)
{
    ui->doubleSpinBox_20->setRange(energy_min, energy_max);
    ui->doubleSpinBox_21->setRange(energy_min, energy_max);
    ui->doubleSpinBox_22->setRange(energy_min, energy_max);
    ui->doubleSpinBox_20->setValue(energy_min);
    ui->doubleSpinBox_21->setValue(energy_max);
}

void widepix_analyser_2::U_file_found(QString file_name)
{
    ui->label_6->setText(file_name + " found");
}

void widepix_analyser_2::U_n_files(int n)
{
    ui->label_7->setText(QString("%1 files have been found").arg(n));
}

void widepix_analyser_2::U_renew_progress_bar(double current, double total)
{
    ui->progressBar->setValue(static_cast<int>(100 * current / total));
}

void widepix_analyser_2::U_set_distribution_range(double lower, double upper)
{
    ui->doubleSpinBox_17->setValue(lower);
    ui->doubleSpinBox_16->setValue(upper);
}

void widepix_analyser_2::U_set_spectra_2D_range(double lower, double upper)
{
    ui->doubleSpinBox_36->setValue(lower);
    ui->doubleSpinBox_37->setValue(upper);
}

void widepix_analyser_2::U_set_count_mask(int n)
{
    ui->label_8->setText(QString("Number of pixel would be mask: %1").arg(n));
}

void widepix_analyser_2::U_set_roi_range(int x_min, int x_max, int y_min, int y_max)
{
    ui->spinBox_5->setValue(x_min);
    ui->spinBox_7->setValue(x_max);
    ui->spinBox_4->setValue(y_min);
    ui->spinBox_6->setValue(y_max);
}

void widepix_analyser_2::U_set_scans(QList<UC_data_container> * scans_list_ptr, int active_index)
{
    ui->comboBox_17->clear();
    ui->comboBox_22->clear();
    ui->comboBox_23->clear();
    const int n = scans_list_ptr->size();
    if (n == 0) return;
    for (int i = 0; i < n; i++) {
        ui->comboBox_17->addItem((*scans_list_ptr)[i].U_get_name());
        ui->comboBox_22->addItem((*scans_list_ptr)[i].U_get_name());
        ui->comboBox_23->addItem((*scans_list_ptr)[i].U_get_name());
    }
    ui->label->setText("Current scan: " + (*scans_list_ptr)[active_index].U_get_name());
    ui->comboBox_17->setCurrentIndex(active_index);
    for (int i = 0; i < list_enabling.size(); i++) {
        list_enabling[i]->setEnabled(true);
    }
}

void widepix_analyser_2::U_change_scan_settings(UC_data_container::UTStr_data_container_settings *settings)
{
    ui->lineEdit_3->setText(settings->path);
    ui->lineEdit_4->setText(settings->name);
    if (settings->calibration) {
        ui->checkBox_3->setCheckState(Qt::Checked);
    } else {
        ui->checkBox_3->setCheckState(Qt::Unchecked);
    }
    ui->doubleSpinBox_19->setValue(settings->energy);
    if (settings->both_counters) {
        ui->checkBox_2->setCheckState(Qt::Checked);
    } else {
        ui->checkBox_2->setCheckState(Qt::Unchecked);
    }
    ui->spinBox_11->setValue(settings->count);
    ui->doubleSpinBox_18->setValue(settings->time);
    if (settings->ff_int == -1) {
        ui->checkBox_15->setCheckState(Qt::Unchecked);
    } else {
        ui->checkBox_15->setCheckState(Qt::Checked);
        ui->comboBox_22->setCurrentIndex(settings->ff_int);
    }
    if (settings->df_int == -1) {
        ui->checkBox_16->setCheckState(Qt::Unchecked);
    } else {
        ui->checkBox_16->setCheckState(Qt::Checked);
        ui->comboBox_23->setCurrentIndex(settings->df_int);
    }
}

void widepix_analyser_2::U_new_spectra(QString name)
{
    ui->comboBox_25->addItem(name);
    ui->comboBox_26->addItem(name);
}

void widepix_analyser_2::U_add_roi(QString name)
{
    ui->comboBox_6->addItem(name);
}

void widepix_analyser_2::U_ready()
{
    for (int i = 0; i < list_enabling_busy.size(); i++) {
        list_enabling_busy[i]->setEnabled(true);
    }
    ui->pushButton_70->setEnabled(false);
}

void widepix_analyser_2::U_busy()
{
    ui->radioButton_3->setChecked(false);
    ui->radioButton_4->setChecked(true);
    wai->U_set_interaction_mode(false);
    ui->pushButton_70->setEnabled(true);
    for (int i = 0; i < list_enabling_busy.size(); i++) {
        list_enabling_busy[i]->setEnabled(false);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void widepix_analyser_2::on_pushButton_clicked() ///< Spectra. Add graph
{
    U_busy();
    QString graph_name = ui->lineEdit_5->text();
    if (graph_name == "") {
        graph_name += ui->comboBox_17->currentText() + "; ";
        graph_name += ui->comboBox_2->currentText() + "; ";
        graph_name += ui->comboBox_3->currentText() + "; ";
        graph_name += wai->U_get_name_roi(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
    }
    wai->U_generate_spectra(graph_name);
    ui->comboBox_25->addItem(graph_name);
    ui->comboBox_26->addItem(graph_name);
}

void widepix_analyser_2::on_pushButton_3_clicked() ///< Spectra. Create spectra
{
    wai->U_calculating_spectra(static_cast<UC_wai::UTE_calculating_spectras>(ui->comboBox_27->currentIndex()), ui->comboBox_25->currentIndex(), ui->comboBox_26->currentIndex());
}

void widepix_analyser_2::on_pushButton_4_clicked() ///< Source. Browse
{
    QString str = QFileDialog::getExistingDirectory(this, "Open scan direction", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (str != "") {
        ui->lineEdit_3->setText(str);
        ui->pushButton_31->setEnabled(true);
    }
}

void widepix_analyser_2::on_comboBox_2_currentIndexChanged(int index) ///< Frame count type
{
    wai->U_set_frame_type(static_cast<UC_plot::UTE_frame_type>(index));
}

void widepix_analyser_2::on_comboBox_3_currentIndexChanged(int index) ///< Pixel count type
{
    wai->U_set_pixel_type(static_cast<UC_plot::UTE_pixel_type>(index));
}

void widepix_analyser_2::on_pushButton_6_clicked() ///< Frame. Show frame
{
    U_busy();
    wai->U_generate_frame(ui->comboBox->currentIndex());
}

void widepix_analyser_2::on_pushButton_7_clicked() ///< Table. Reload table for 1 frame
{
    U_busy();
    wai->U_generate_table(ui->comboBox->currentIndex());
    ui->pushButton_25->setEnabled(true);
}

void widepix_analyser_2::on_pushButton_8_clicked() ///< Set ROI
{
    wai->U_set_roi(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
    int index = ui->comboBox_6->currentIndex();
    if (index < 16) {
        roi = ui->comboBox_6->currentText();
    } else {
        roi = "other";
    }
    ui->label_2->setNum(ui->spinBox_5->value());
    ui->label_20->setNum(ui->spinBox_4->value());
    ui->label_21->setNum(ui->spinBox_6->value());
    ui->label_22->setNum(ui->spinBox_7->value());
}

void widepix_analyser_2::on_pushButton_9_clicked() ///< Spectra. Resize
{
    wai->U_resize_spectra();
}

void widepix_analyser_2::on_pushButton_11_clicked() ///< Frame. Resize
{
    wai->U_resize_frame();
}

void widepix_analyser_2::on_pushButton_10_clicked() ///< Frame. Rescale
{
    wai->U_rescale_frame();
}

void widepix_analyser_2::on_pushButton_12_clicked() ///< Frame. Save
{
    QString str = QFileDialog::getSaveFileName(this, "Save frame", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        wai->U_save_frame(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_7->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_13_clicked() ///< Set mask
{
    U_busy();
    if (ui->checkBox_4->checkState() == Qt::Checked) {
        emit US_set_mask((ui->checkBox_5->checkState() == Qt::Checked), ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked));
    } else {
        emit US_set_mask((ui->checkBox_5->checkState() == Qt::Checked), ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
    }
}

void widepix_analyser_2::on_pushButton_15_clicked() ///< Reset mask
{
    emit US_reset_mask();
}

void widepix_analyser_2::on_pushButton_16_clicked() ///< Spectra. Save
{
    QString str = QFileDialog::getSaveFileName(this, "Save spectra", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        wai->U_save_spectra(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_8->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_20_clicked() ///< Distribution. Distribution all frames
{
    U_busy();
    QString graph_name = ui->lineEdit_6->text();
    if (graph_name == "") {
        graph_name += ui->comboBox_17->currentText() + "; ";
        graph_name += ui->comboBox_3->currentText() + "; ";
        graph_name += wai->U_get_name_roi(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
    }
    wai->U_generate_distribution(ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value(), graph_name);
}

void widepix_analyser_2::on_pushButton_17_clicked() ///< Distribution. Distribution 1 frame
{
    U_busy();
    QString graph_name = ui->lineEdit_6->text();
    if (graph_name == "") {
        graph_name += ui->comboBox_17->currentText() + "; ";
        graph_name += ui->comboBox_3->currentText() + "; ";
        graph_name += ui->comboBox->currentText() + "; ";
        graph_name += wai->U_get_name_roi(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
    }
    wai->U_generate_distribution(ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value(), ui->comboBox->currentIndex(), graph_name);
}

void widepix_analyser_2::on_pushButton_5_clicked() ///< Source. Reset
{
    wai->U_reset_data();
    emit US_reset_data();
    ui->lineEdit_3->setText("");
    ui->comboBox->clear();
    ui->comboBox_17->clear();

    ui->comboBox_22->clear();
    ui->comboBox_23->clear();

    for (int i = 0; i < list_enabling.size(); i++) {
        list_enabling[i]->setEnabled(false);
    }
}

void widepix_analyser_2::on_pushButton_22_clicked() ///< Spectra. Reset
{
    wai->U_reset_spectra();
    ui->comboBox_25->clear();
    ui->comboBox_26->clear();
}

void widepix_analyser_2::on_pushButton_18_clicked() ///< Distribution. Resize
{
    wai->U_resize_distribution();
}

void widepix_analyser_2::on_pushButton_23_clicked() ///< Mask overflow
{
    U_busy();
    if (ui->checkBox_4->checkState() == Qt::Checked) {
        US_set_mask_overflowed((ui->checkBox->checkState() == Qt::Checked));
    } else {
        US_set_mask_overflowed((ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
    }
}

void widepix_analyser_2::on_pushButton_19_clicked() ///< Distribution. Save
{
    QString str = QFileDialog::getSaveFileName(this, "Save distribution", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        wai->U_save_distribution(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_9->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_25_clicked() ///< Table. Save
{
    QString str = QFileDialog::getSaveFileName(this, "Save table", "", "Tables (*.csv *.tsv)");
    if (str != "") {
        wai->U_save_table(str, static_cast<UC_wai::UTE_table_file_type>(ui->comboBox_10->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_26_clicked() ///< Spectra. Delete last graph
{
    wai->U_delete_last_spectra();
    ui->comboBox_25->removeItem(ui->comboBox_25->count() - 1);
    ui->comboBox_26->removeItem(ui->comboBox_26->count() - 1);
}

void widepix_analyser_2::on_pushButton_27_clicked() ///< Distribution. Delete last graph
{
    wai->U_delete_last_distribution();
}

void widepix_analyser_2::on_pushButton_28_clicked() ///< Distribution. Reset
{
    wai->U_reset_distribution();
}

void widepix_analyser_2::on_pushButton_31_clicked() ///< Source. Add scan
{
    U_busy();
    UC_data_container::UTStr_data_container_settings settings;
    settings.path = ui->lineEdit_3->text();
    settings.count = ui->spinBox_11->value();
    settings.time = ui->doubleSpinBox_18->value();
    settings.both_counters = (ui->checkBox_2->checkState() == Qt::Checked);
    settings.calibration = (ui->checkBox_3->checkState() == Qt::Checked);
    settings.energy = ui->doubleSpinBox_19->value();
    if (ui->checkBox_15->checkState() == Qt::Checked) {
        settings.ff_int = ui->comboBox_22->currentIndex();
    } else {
        settings.ff_int = - 1;
    }
    if (ui->checkBox_16->checkState() == Qt::Checked) {
        settings.df_int = ui->comboBox_23->currentIndex();
    } else {
        settings.df_int = - 1;
    }
    QString str;
    str = ui->lineEdit_4->text();
    if (str == "") {
        str += settings.path + "; ";
        str += QString("count = %1").arg(settings.count) + "; ";
        str += QString("time = %1").arg(settings.time) + "; ";
        if (settings.both_counters) {
            str += "both counters;";
        } else {
            str += "one counter;";
        }
    }
    settings.name = str;

    emit US_set_data(&settings);


}

void widepix_analyser_2::on_pushButton_34_clicked() ///< Show mask
{
    wai->U_set_mask_plots(ui->doubleSpinBox_11->value());
}

void widepix_analyser_2::on_pushButton_35_clicked() ///< Clean mask
{
    wai->U_delete_mask_plots();
}

void widepix_analyser_2::on_pushButton_36_clicked() ///< Count pixels would be masked
{
    U_busy();
    if (ui->checkBox_4->checkState() == Qt::Checked) {
        emit US_count_mask(ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked));
    } else {
        emit US_count_mask(ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
    }
}

void widepix_analyser_2::on_radioButton_toggled(bool checked) ///< then value. More
{
    if (checked) ui->radioButton_2->setDown(false);
}

void widepix_analyser_2::on_radioButton_2_toggled(bool checked) ///< then value. Less
{
    if (checked) ui->radioButton->setDown(false);
}

void widepix_analyser_2::on_pushButton_33_clicked() ///< Distribution. Set range 1 frame
{
    U_busy();
    wai->U_generate_range(ui->comboBox->currentIndex());
}

void widepix_analyser_2::on_pushButton_32_clicked() ///< Distribution. Set range all frames
{
    U_busy();
    wai->U_generate_range();
}

void widepix_analyser_2::on_pushButton_37_clicked() ///< Spectra. Load txt
{
    QString str = QFileDialog::getOpenFileName(this, "Open spectra", "", "Text files (*.txt)");
    if (str != "") {
        wai->U_load_spectra_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_38_clicked() ///< Frame. Load txt
{
    QString str = QFileDialog::getOpenFileName(this, "Open frame", "", "Text files (*.txt)");
    if (str != "") {
        wai->U_load_frame_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_39_clicked() ///< Distribution. Load txt
{
    QString str = QFileDialog::getOpenFileName(this, "Open distribution", "", "Text files (*.txt)");
    if (str != "") {
        wai->U_load_distribution_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_40_clicked() ///< Table. Reload table all frames
{
    U_busy();
    wai->U_generate_table();
}

void widepix_analyser_2::on_pushButton_41_clicked() ///< Spectra. Normalize
{
    wai->U_normalize_spectra();
}

void widepix_analyser_2::on_radioButton_3_clicked(bool checked) ///< Graphs interaction mode. Select
{
    wai->U_set_interaction_mode(checked);
}

void widepix_analyser_2::on_radioButton_4_clicked(bool checked) ///< Graphs interaction mode. Drag
{
    wai->U_set_interaction_mode(!checked);
}

void widepix_analyser_2::on_pushButton_44_clicked() ///< Mask selected 1 frame
{
    U_busy();
    if (ui->checkBox_4->checkState() == Qt::Checked) {
        switch (ui->tabWidget->currentIndex()) {
            case 1 : {
                wai->U_mask_selected_value(ui->checkBox->checkState() == Qt::Checked);
                break;
            }
            case 2 : {
                wai->U_mask_selected(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
                break;
            }
            case 4 : {
                wai->U_mask_selected_value(ui->checkBox->checkState() == Qt::Checked);
                break;
            }
            case 5 : {
                wai->U_mask_selected_value(ui->checkBox->checkState() == Qt::Checked);
                break;
            }
        }
    } else {
        switch (ui->tabWidget->currentIndex()) {
            case 1 : {
                wai->U_mask_selected_value(ui->checkBox->checkState() == Qt::Checked, ui->comboBox->currentIndex());
                break;
            }
            case 2 : {
                wai->U_mask_selected(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
                break;
            }
            case 4 : {
                wai->U_mask_selected_value(ui->checkBox->checkState() == Qt::Checked, ui->comboBox->currentIndex());
                break;
            }
            case 5 : {
                wai->U_mask_selected_value(ui->checkBox->checkState() == Qt::Checked, ui->comboBox->currentIndex());
                break;
            }
        }
    }

}

void widepix_analyser_2::on_radioButton_5_clicked(bool checked) ///< x Axis. Linear
{
    wai->U_set_x_axis_type(checked);
}

void widepix_analyser_2::on_radioButton_6_clicked(bool checked) ///< x Axis. Logarithmical
{
    wai->U_set_x_axis_type(!checked);
}

void widepix_analyser_2::on_radioButton_7_clicked(bool checked) ///< y Axis. Linear
{
    wai->U_set_y_axis_type(checked);
}

void widepix_analyser_2::on_radioButton_8_clicked(bool checked) ///< y Axis. Logarithmical
{
    wai->U_set_y_axis_type(!checked);
}

void widepix_analyser_2::on_pushButton_42_clicked() ///< Calibration. Show fitting chip
{
    U_busy();
    QString graph_name = ui->lineEdit_6->text();
    if (graph_name == "") {
        graph_name += ui->comboBox_17->currentText() + "; ";
        graph_name += ui->comboBox_3->currentText() + "; ";
    }
    wai->U_generate_chip_fit(ui->comboBox_18->currentIndex(), graph_name);  
}

void widepix_analyser_2::on_pushButton_45_clicked() ///< Calibration. Generate calibration
{
    U_busy();
    wai->U_generate_calibration();
}

void widepix_analyser_2::on_pushButton_46_clicked() ///< Spectra. Add graph by energy
{
    U_busy();
    QString graph_name = ui->lineEdit_5->text();
    if (graph_name == "") {
        graph_name += ui->comboBox_17->currentText() + "; ";
        graph_name += ui->comboBox_2->currentText() + "; ";
        graph_name += ui->comboBox_3->currentText() + "; ";
        graph_name += wai->U_get_name_roi(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value()) + "; ";
        graph_name += QString("bins = %1").arg(ui->spinBox_2->value());
    }
    wai->U_generate_spectra(graph_name, ui->spinBox_2->value());

    ui->pushButton->setEnabled(false);
    ui->pushButton_46->setEnabled(false);
}

void widepix_analyser_2::on_pushButton_47_clicked() ///< Frame. Frame by energy
{
    U_busy();
    wai->U_generate_frame(ui->doubleSpinBox_22->value());
}

void widepix_analyser_2::on_pushButton_48_clicked() ///< Table. Reload table for energy
{
    U_busy();
    wai->U_generate_table(ui->doubleSpinBox_22->value());
}

void widepix_analyser_2::on_pushButton_49_clicked() ///< Calibration. Save // chip fit
{
    QString str = QFileDialog::getSaveFileName(this, "Save chip fit", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        wai->U_save_chip_fit(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_16->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_52_clicked() ///< Calibration. Reset // chip fit
{
    wai->U_reset_chip_fit();
}

void widepix_analyser_2::on_pushButton_54_clicked() ///< Spectra. Automatic save by chip
{
    QString str = QFileDialog::getSaveFileName(this, "Save spectra", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        QString graph_name = ui->lineEdit_5->text(); /// name
        if (graph_name == "") {
            graph_name += ui->comboBox_17->currentText() + "; "; /// scan
            graph_name += ui->comboBox_2->currentText() + "; "; /// frame count type
            graph_name += ui->comboBox_3->currentText(); /// pixel count type
        }
        wai->U_automatic_save_spectra(str, graph_name, static_cast<UC_wai::UTE_file_type>(ui->comboBox_8->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_57_clicked() ///< Calibration. Save
{
    QString str = QFileDialog::getSaveFileName(this, "Save calibration", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        wai->U_save_calibration(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_20->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_60_clicked() ///< Calibration. Reset
{
    wai->U_reset_calibration();
}

void widepix_analyser_2::on_checkBox_13_toggled(bool checked) ///< renew ranges
{
    wai->U_set_renew_ranges(checked);
    if (checked) {
        connect(wai, SIGNAL(US_set_spectra_max_x(double)), ui->doubleSpinBox_4, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_spectra_min_x(double)), ui->doubleSpinBox_3, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_spectra_max_y(double)), ui->doubleSpinBox,   SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_spectra_min_y(double)), ui->doubleSpinBox_2, SLOT(setValue(double)));

        connect(wai, SIGNAL(US_set_frame_max_x(double)), ui->doubleSpinBox_8,  SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_frame_min_x(double)), ui->doubleSpinBox_7,  SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_frame_max_y(double)), ui->doubleSpinBox_5,  SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_frame_min_y(double)), ui->doubleSpinBox_6,  SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_frame_max_z(double)), ui->doubleSpinBox_9,  SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_frame_min_z(double)), ui->doubleSpinBox_10, SLOT(setValue(double)));

        connect(wai, SIGNAL(US_set_distribution_max_x(double)), ui->doubleSpinBox_14, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_distribution_min_x(double)), ui->doubleSpinBox_13, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_distribution_max_y(double)), ui->doubleSpinBox_12, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_distribution_min_y(double)), ui->doubleSpinBox_15, SLOT(setValue(double)));

        connect(wai, SIGNAL(US_set_chip_fit_max_x(double)), ui->doubleSpinBox_24, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_chip_fit_min_x(double)), ui->doubleSpinBox_25, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_chip_fit_max_y(double)), ui->doubleSpinBox_23, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_chip_fit_min_y(double)), ui->doubleSpinBox_26, SLOT(setValue(double)));

        connect(wai, SIGNAL(US_set_calibration_max_x(double)), ui->doubleSpinBox_27, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_calibration_min_x(double)), ui->doubleSpinBox_28, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_calibration_max_y(double)), ui->doubleSpinBox_29, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_calibration_min_y(double)), ui->doubleSpinBox_30, SLOT(setValue(double)));

        connect(wai, SIGNAL(US_set_spectra_2d_max_x(double)), ui->doubleSpinBox_35, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_spectra_2d_min_x(double)), ui->doubleSpinBox_34, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_spectra_2d_max_y(double)), ui->doubleSpinBox_32, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_spectra_2d_min_y(double)), ui->doubleSpinBox_33, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_spectra_2d_max_z(double)), ui->doubleSpinBox_38, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_spectra_2d_min_z(double)), ui->doubleSpinBox_39, SLOT(setValue(double)));
    } else {
        disconnect(wai, SIGNAL(US_set_spectra_max_x(double)), ui->doubleSpinBox_4, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_spectra_min_x(double)), ui->doubleSpinBox_3, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_spectra_max_y(double)), ui->doubleSpinBox,   SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_spectra_min_y(double)), ui->doubleSpinBox_2, SLOT(setValue(double)));

        disconnect(wai, SIGNAL(US_set_frame_max_x(double)), ui->doubleSpinBox_8,  SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_frame_min_x(double)), ui->doubleSpinBox_7,  SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_frame_max_y(double)), ui->doubleSpinBox_5,  SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_frame_min_y(double)), ui->doubleSpinBox_6,  SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_frame_max_z(double)), ui->doubleSpinBox_9,  SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_frame_min_z(double)), ui->doubleSpinBox_10, SLOT(setValue(double)));

        disconnect(wai, SIGNAL(US_set_distribution_max_x(double)), ui->doubleSpinBox_14, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_distribution_min_x(double)), ui->doubleSpinBox_13, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_distribution_max_y(double)), ui->doubleSpinBox_12, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_distribution_min_y(double)), ui->doubleSpinBox_15, SLOT(setValue(double)));

        disconnect(wai, SIGNAL(US_set_chip_fit_max_x(double)), ui->doubleSpinBox_24, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_chip_fit_min_x(double)), ui->doubleSpinBox_25, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_chip_fit_max_y(double)), ui->doubleSpinBox_23, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_chip_fit_min_y(double)), ui->doubleSpinBox_26, SLOT(setValue(double)));

        disconnect(wai, SIGNAL(US_set_calibration_max_x(double)), ui->doubleSpinBox_27, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_calibration_min_x(double)), ui->doubleSpinBox_28, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_calibration_max_y(double)), ui->doubleSpinBox_29, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_calibration_min_y(double)), ui->doubleSpinBox_30, SLOT(setValue(double)));

        disconnect(wai, SIGNAL(US_set_spectra_2d_max_x(double)), ui->doubleSpinBox_35, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_spectra_2d_min_x(double)), ui->doubleSpinBox_34, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_spectra_2d_max_y(double)), ui->doubleSpinBox_32, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_spectra_2d_min_y(double)), ui->doubleSpinBox_33, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_spectra_2d_max_z(double)), ui->doubleSpinBox_38, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_spectra_2d_min_z(double)), ui->doubleSpinBox_39, SLOT(setValue(double)));
    }
}

void widepix_analyser_2::on_pushButton_50_clicked() ///< Calibration. resize // chip fit
{
    wai->U_resize_chip_fit();
}

void widepix_analyser_2::on_pushButton_53_clicked() ///< Calibration. Set threshold level
{
    emit US_set_threshold_level(ui->doubleSpinBox_31->value());
}

void widepix_analyser_2::on_pushButton_62_clicked() ///< Calibration. Save calibration
{
    QString str = QFileDialog::getSaveFileName(this, "Save calibration", "", "Text files (*.txt)");
    if (str != "") {
        emit US_save_calibration(str);
    }
}

void widepix_analyser_2::on_pushButton_61_clicked() ///< Calibration. Load calibration
{
    QString str = QFileDialog::getOpenFileName(this, "Load calibration", "", "Text files (*.txt)");
    if (str != "") {
        emit US_load_calibration(str);
    }
}

void widepix_analyser_2::on_pushButton_65_clicked() ///< Add RoI
{
    QString str = ui->lineEdit->text();
    if (str == "") {
        str = "xmin = %1; ymin = %2; ymax = %3; xmax=%4";
        str = str.arg(ui->spinBox_5->value());
        str = str.arg(ui->spinBox_4->value());
        str = str.arg(ui->spinBox_6->value());
        str = str.arg(ui->spinBox_7->value());
    }
    wai->U_add_roi(str, ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
    ui->comboBox_6->addItem(str);
}

void widepix_analyser_2::on_pushButton_66_clicked() ///< Delete RoI
{
    int index = ui->comboBox_6->currentIndex();
    wai->U_delete_roi(index);
    ui->comboBox_6->removeItem(index);
}

void widepix_analyser_2::on_listView_clicked(const QModelIndex &index) ///< Source. List widget
{
    emit US_get_scan_settings(index.row());
}

void widepix_analyser_2::on_spinBox_12_valueChanged(int arg1) ///< Smoothing
{
    wai->U_set_smoothing(arg1);
}

void widepix_analyser_2::on_checkBox_17_toggled(bool checked) ///< Renew roi ranges
{
    wai->U_set_renew_roi_ranges(checked);
}

void widepix_analyser_2::on_comboBox_6_activated(int index) ///< ROI combo box
{
    UC_roi roi = wai->U_get_roi(index);
    ui->spinBox_5->setValue(roi.U_get_x_min());
    ui->spinBox_4->setValue(roi.U_get_y_min());
    ui->spinBox_6->setValue(roi.U_get_y_max());
    ui->spinBox_7->setValue(roi.U_get_x_max());
    if (index < 16) {
        ui->pushButton_66->setEnabled(false);
    } else {
        ui->pushButton_66->setEnabled(true);
    }
}

void widepix_analyser_2::on_pushButton_2_clicked() ///< Save RoI
{
    QString str = QFileDialog::getSaveFileName(this, "Save RoIs", "", "Text files (*.txt)");
    if (str != "") {
        wai->U_save_roi(str);
    }
}

void widepix_analyser_2::on_pushButton_14_clicked() ///< Load RoI
{
    QString str = QFileDialog::getOpenFileName(this, "Load RoIs", "", "Text files (*.txt)");
    if (str != "") {
        wai->U_load_roi(str);
    }
}

void widepix_analyser_2::on_pushButton_30_clicked() ///< Save mask
{
    QString str = QFileDialog::getSaveFileName(this, "Save mask", "", "Text files (*.txt)");
    if (str != "") {
        emit US_save_mask(str);
//        wai->U_save_mask(str);
    }
}

void widepix_analyser_2::on_pushButton_69_clicked() ///< Load mask
{
    QString str = QFileDialog::getOpenFileName(this, "Load mask", "", "Text files (*.txt)");
    if (str != "") {
        emit US_load_mask(str);
        //wai->U_load_mask(str);
    }
}

void widepix_analyser_2::on_checkBox_18_toggled(bool checked) ///< Legend enable
{
    wai->U_set_legend_enable(checked);
}

void widepix_analyser_2::on_comboBox_5_activated(int index)
{
    wai->U_set_gradient(index);
}

void widepix_analyser_2::on_comboBox_17_activated(int index)
{
    if (index != -1)
    emit US_set_scan(index);
}

void widepix_analyser_2::on_pushButton_70_clicked()
{
    emit US_stop();
}

void widepix_analyser_2::on_pushButton_71_clicked()
{
    QString str = QFileDialog::getSaveFileName(this, "Save distribution", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        QString graph_name = ui->lineEdit_6->text(); /// name
        if (graph_name == "") {
            graph_name += ui->comboBox_17->currentText() + "; "; /// scan
            graph_name += ui->comboBox_3->currentText(); /// pixel count type
        }
        wai->U_automatic_save_distribution(str, graph_name, static_cast<UC_wai::UTE_file_type>(ui->comboBox_9->currentIndex()), ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value(), ui->comboBox->currentIndex());
        }
}

void widepix_analyser_2::on_pushButton_73_clicked()
{
    QString str = QFileDialog::getSaveFileName(this, "Save distribution", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        QString graph_name = ui->lineEdit_6->text(); /// name
        if (graph_name == "") {
            graph_name += ui->comboBox_17->currentText() + "; "; /// scan
            graph_name += ui->comboBox_3->currentText(); /// pixel count type
        }
        wai->U_automatic_save_distribution(str, graph_name, static_cast<UC_wai::UTE_file_type>(ui->comboBox_8->currentIndex()), ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value());
    }
}

void widepix_analyser_2::on_radioButton_9_clicked(bool checked)
{
    wai->U_set_z_axis_type(checked);
}

void widepix_analyser_2::on_radioButton_10_clicked(bool checked)
{
    wai->U_set_z_axis_type(!checked);
}

void widepix_analyser_2::on_pushButton_72_clicked() ///< Spectra 2D. Show spectra 2D
{
    U_busy();
    UC_wai::UStr_spectra_2d_settings settings;
    settings.bins_x = ui->spinBox_15->value();
    settings.bins_y = ui->spinBox_16->value();
    settings.x_min = ui->comboBox_21->currentText().right(3).toInt();
    settings.x_max = ui->comboBox_28->currentText().right(3).toInt();
    settings.y_max = ui->doubleSpinBox_37->value();
    settings.y_min = ui->doubleSpinBox_36->value();
    wai->U_generate_spectra_2d(settings);
}

void widepix_analyser_2::on_pushButton_74_clicked() ///< Spectra 2D. Show spectra 2D by energy
{
    U_busy();
    UC_wai::UStr_spectra_2d_settings settings;
    settings.bins_x = ui->spinBox_15->value();
    settings.bins_y = ui->spinBox_16->value();
    settings.x_min = ui->doubleSpinBox_20->value();
    settings.x_max = ui->doubleSpinBox_21->value();
    settings.y_max = ui->doubleSpinBox_37->value();
    settings.y_min = ui->doubleSpinBox_36->value();
    wai->U_generate_spectra_2d_energy(settings);
}

void widepix_analyser_2::on_pushButton_75_clicked() ///< Spectra 2D. Set value range
{
    U_busy();
    int bins;
    bins = ui->comboBox_28->currentIndex();
    bins += 1 - ui->comboBox_21->currentIndex();
    ui->spinBox_15->setValue(bins);
    wai->U_generate_spectra_2d_range(ui->comboBox_21->currentIndex(), ui->comboBox_28->currentIndex());
}

void widepix_analyser_2::on_pushButton_76_clicked() ///< Spectra 2D. Set value range by energy
{
    U_busy();
    wai->U_generate_spectra_2d_range_energy(ui->doubleSpinBox_20->value(), ui->doubleSpinBox_21->value());
}

void widepix_analyser_2::on_pushButton_83_clicked() ///< Spectra 2D. Rescale
{
    wai->U_rescale_spectra_2d();
}

void widepix_analyser_2::on_pushButton_79_clicked() ///< Spectra 2D. Resize
{
    wai->U_resize_spectra_2d();
}

void widepix_analyser_2::on_pushButton_81_clicked() ///< Spectra 2D. Save
{
    QString str = QFileDialog::getSaveFileName(this, "Save spectra 2D", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        wai->U_save_spectra_2d(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_11->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_80_clicked() ///< Spectra 2D. Load
{
    QString str = QFileDialog::getOpenFileName(this, "Load spectra 2D", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        wai->U_load_spectra_2d_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_77_clicked() ///< Width +
{
    wai->U_thick_up();
}

void widepix_analyser_2::on_pushButton_82_clicked() ///< Width -
{
    wai->U_thick_down();
}

void widepix_analyser_2::on_pushButton_78_clicked() ///< Spectra 2d. Automatic save by ROI
{
    UC_wai::UStr_spectra_2d_settings settings;
    settings.bins_x = ui->spinBox_15->value();
    settings.bins_y = ui->spinBox_16->value();
    settings.x_min = ui->comboBox_21->currentText().right(3).toInt();
    settings.x_max = ui->comboBox_28->currentText().right(3).toInt();
    settings.y_max = ui->doubleSpinBox_37->value();
    settings.y_min = ui->doubleSpinBox_36->value();
    QString str = QFileDialog::getSaveFileName(this, "Save spectra 2D", "", "Images (*.png *.bpm *.jpg);;Text files (*.txt);;PDF files (*.pdf);;ROOT files (*.root)");
    if (str != "") {
        wai->U_automatic_save_spectra_2d(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_11->currentIndex()), settings);
    }
}

void widepix_analyser_2::on_pushButton_84_clicked() ///< Mask. Mask Gauss
{
    U_busy();
    if (ui->checkBox_4->checkState() == Qt::Checked) {
        emit US_set_mask_gauss((ui->checkBox_5->checkState() == Qt::Checked), ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked));
    } else {
        emit US_set_mask_gauss((ui->checkBox_5->checkState() == Qt::Checked), ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
    }
}

void widepix_analyser_2::on_pushButton_43_clicked() ///< Table. Load
{
    QString str = QFileDialog::getOpenFileName(this, "Load table", "", "Table files (*.csv *.tsv)");
    if (str != "") {
        wai->U_load_table(str);
    }
}

void widepix_analyser_2::on_pushButton_24_clicked()
{
    ui->comboBox_21->setCurrentIndex(0);
    ui->comboBox_28->setCurrentIndex(ui->comboBox_28->count() - 1);
    ui->doubleSpinBox_20->setValue(ui->doubleSpinBox_20->minimum());
    ui->doubleSpinBox_21->setValue(ui->doubleSpinBox_21->maximum());
}

void widepix_analyser_2::on_comboBox_21_currentIndexChanged(int index)
{
    emit US_set_threshold_range(index, ui->comboBox_28->currentIndex());
    if (ui->comboBox_28->currentIndex() < index) {
        ui->comboBox_28->setCurrentIndex(index);
    }
    if (ui->comboBox->currentIndex() < index) {
        ui->comboBox->setCurrentIndex(index);
    }
}

void widepix_analyser_2::on_comboBox_28_currentIndexChanged(int index)
{
    emit US_set_threshold_range(ui->comboBox_21->currentIndex(), index);
    if (ui->comboBox_21->currentIndex() > index) {
        ui->comboBox_21->setCurrentIndex(index);
    }
    if (ui->comboBox->currentIndex() > index) {
        ui->comboBox->setCurrentIndex(index);
    }
}

void widepix_analyser_2::on_doubleSpinBox_20_valueChanged(double arg1)
{
    emit US_set_energy_range(arg1, ui->doubleSpinBox_21->value());
    if (ui->doubleSpinBox_22->value() < arg1) {
        ui->doubleSpinBox_22->setValue(arg1);
    }
    if (ui->doubleSpinBox_21->value() < arg1) {
        ui->doubleSpinBox_21->setValue(arg1);
    }
}

void widepix_analyser_2::on_doubleSpinBox_21_valueChanged(double arg1)
{
    emit US_set_energy_range(ui->doubleSpinBox_20->value(), arg1);
    if (ui->doubleSpinBox_22->value() > arg1) {
        ui->doubleSpinBox_22->setValue(arg1);
    }
    if (ui->doubleSpinBox_20->value() > arg1) {
        ui->doubleSpinBox_20->setValue(arg1);
    }
}

void widepix_analyser_2::on_pushButton_85_clicked()
{
    wai->U_spectra_2d_normalize();
}
