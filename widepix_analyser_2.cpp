#include "widepix_analyser_2.h"
#include "ui_widepix_analyser_2.h"

widepix_analyser_2::widepix_analyser_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::widepix_analyser_2)
{
    ui->setupUi(this);

    wai = new UC_wai();
    wai->U_set_qcp(ui->widget, ui->widget_2, ui->widget_3);
    wai->U_set_colibration_qcp(ui->widget_4, ui->widget_5);
    wai->U_set_identification_qcp(ui->widget_7);
    wai->U_set_table(ui->tableView);
    wai->U_set_list(ui->listView);
    wai->U_set_id_table(ui->tableView_2);

    connect(wai, SIGNAL(US_set_spectra_max_x(double)), ui->doubleSpinBox_4, SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_spectra_min_x(double)), ui->doubleSpinBox_3, SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_spectra_max_y(double)), ui->doubleSpinBox,   SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_spectra_min_y(double)), ui->doubleSpinBox_2, SLOT(setValue(double)));

    connect(ui->doubleSpinBox_4, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_max_x(double)));
    connect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_min_x(double)));
    connect(ui->doubleSpinBox,   SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_max_y(double)));
    connect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), wai, SLOT(U_set_spectra_min_y(double)));
    ////
    connect(wai, SIGNAL(US_set_frame_max_x(double)), ui->doubleSpinBox_8,  SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_frame_min_x(double)), ui->doubleSpinBox_7,  SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_frame_max_y(double)), ui->doubleSpinBox_5,  SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_frame_min_y(double)), ui->doubleSpinBox_6,  SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_frame_max_z(double)), ui->doubleSpinBox_9,  SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_frame_min_z(double)), ui->doubleSpinBox_10, SLOT(setValue(double)));

    connect(ui->doubleSpinBox_8,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_max_x(double)));
    connect(ui->doubleSpinBox_7,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_min_x(double)));
    connect(ui->doubleSpinBox_5,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_max_y(double)));
    connect(ui->doubleSpinBox_6,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_min_y(double)));
    connect(ui->doubleSpinBox_9,  SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_max_z(double)));
    connect(ui->doubleSpinBox_10, SIGNAL(valueChanged(double)), wai, SLOT(U_set_frame_min_z(double)));
    ////
    connect(wai, SIGNAL(US_set_distribution_max_x(double)), ui->doubleSpinBox_14, SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_distribution_min_x(double)), ui->doubleSpinBox_13, SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_distribution_max_y(double)), ui->doubleSpinBox_12, SLOT(setValue(double)));
    connect(wai, SIGNAL(US_set_distribution_min_y(double)), ui->doubleSpinBox_15, SLOT(setValue(double)));

    connect(ui->doubleSpinBox_14, SIGNAL(valueChanged(double)), wai, SLOT(U_set_distribution_max_x(double)));
    connect(ui->doubleSpinBox_13, SIGNAL(valueChanged(double)), wai, SLOT(U_set_distribution_min_x(double)));
    connect(ui->doubleSpinBox_12, SIGNAL(valueChanged(double)), wai, SLOT(U_set_distribution_max_y(double)));
    connect(ui->doubleSpinBox_15, SIGNAL(valueChanged(double)), wai, SLOT(U_set_distribution_min_y(double)));
    ////
    connect(wai, SIGNAL(US_mouse_data(QString)), ui->statusBar, SLOT(showMessage(QString)));
    ////////
    connect(wai->U_get_plot(), SIGNAL(US_thl_range(int, int)),                 this, SLOT(U_add_thl(int, int)));
    connect(wai->U_get_plot(), SIGNAL(US_file_found(QString)),                this, SLOT(U_file_found(QString)));
    connect(wai->U_get_plot(), SIGNAL(US_n_files(int)),                       this, SLOT(U_n_files(int)));
    connect(wai->U_get_plot(), SIGNAL(US_renew_progress_bar(double, double)), this, SLOT(U_renew_progress_bar(double, double)));
    ////
    connect(wai, SIGNAL(US_set_distribution_range(double, double)), this, SLOT(U_set_distribution_range(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_range_data(double, double)), this, SLOT(U_set_distribution_range(double, double)));
    ///
    connect(wai->U_get_plot(), SIGNAL(US_count_mask(int)), this, SLOT(U_set_count_mask(int)));
    connect(wai, SIGNAL(US_set_scan(int)), this, SLOT(U_set_scan(int)));
    ///
    connect(wai, SIGNAL(US_set_scan(int)), this, SLOT(U_set_scan(int)));
    ///
    connect(wai, SIGNAL(US_set_roi_range(int, int, int, int)),  this, SLOT(U_set_roi_range(int, int, int, int)));
    connect(wai, SIGNAL(US_set_thl_range(double, double)),            this, SLOT(U_set_thl_range(double, double)));
    //
    connect(this, SIGNAL(US_set_threshold_level(int)),            wai->U_get_plot(), SLOT(U_set_threshold_level(int)));
    //
    connect(this, SIGNAL(US_save_calibration(QString)),            wai->U_get_plot(), SLOT(U_save_calibration(QString)));
    connect(this, SIGNAL(US_load_calibration(QString)),            wai->U_get_plot(), SLOT(U_load_calibration(QString)));

    connect(wai->U_get_plot(), SIGNAL(US_identification_data(double, double)),            this, SLOT(U_set_id(double, double)));

    connect(wai, SIGNAL(US_renew_scan_settings(UC_data_container::UTStr_data_container_settings)),            this, SLOT(U_change_scan_settings(UC_data_container::UTStr_data_container_settings)));

    connect(wai->U_get_plot(), SIGNAL(US_identification_scan_list(QList<QString>)),            this, SLOT(U_renew_identification_elements(QList<QString>)));

    wai->U_start();

    //ui->pushButton_3->setEnabled(false);
    //ui->pushButton_2->setEnabled(false);
    ui->pushButton_29->setEnabled(false);
    ui->pushButton_21->setEnabled(false);

    ui->pushButton_13->setEnabled(false);
    //ui->pushButton_14->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_23->setEnabled(false);
    ui->pushButton_24->setEnabled(false);

    ui->pushButton_16->setEnabled(false);

    ui->pushButton_6->setEnabled(false);
    ui->pushButton_12->setEnabled(false);

    ui->pushButton_7->setEnabled(false);
    ui->pushButton_25->setEnabled(false);

    ui->pushButton_17->setEnabled(false);
    ui->pushButton_19->setEnabled(false);
    ui->pushButton_20->setEnabled(false);

    ui->pushButton_31->setEnabled(false);

    ui->pushButton_34->setEnabled(false);
    ui->pushButton_35->setEnabled(false);
    ui->pushButton_36->setEnabled(false);

    ui->pushButton_40->setEnabled(false);
    ui->pushButton_33->setEnabled(false);
    ui->pushButton_32->setEnabled(false);

    //ui->comboBox->setEnabled(false);

    //////////////////////////////////////////////////////
    roi = "all chips";

}

widepix_analyser_2::~widepix_analyser_2()
{
    wai->~UC_wai();
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void widepix_analyser_2::U_add_thl(int thl_min, int thl_max)
{
    QString str = QString("thl = %1").arg(thl_min);
    int i = ui->comboBox->findText(str);

    ui->comboBox->clear();
    ui->comboBox_5->clear();
    ui->comboBox_11->clear();
    ui->comboBox_12->clear();
    ui->comboBox_13->clear();
    ui->comboBox_14->clear();
    ui->comboBox_15->clear();
    for (int thl = thl_min; thl <= thl_max; thl++) {
        ui->comboBox->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_5->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_11->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_12->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_13->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_14->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_15->addItem(QString("thl = %1").arg(thl));
    }
    ui->comboBox_11->setCurrentIndex(ui->comboBox_11->count()-1);
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

void widepix_analyser_2::U_set_thl_range(double thl_min, double thl_max)
{
    int thl_min_int = static_cast<int>(std::round(thl_min));
    int thl_max_int = static_cast<int>(std::round(thl_max));
    ui->comboBox_5->setCurrentText(QString("thl = %1").arg(thl_min_int));
    ui->comboBox_11->setCurrentText(QString("thl = %1").arg(thl_max_int));
}

void widepix_analyser_2::U_set_scan(int index)
{
    ui->comboBox_17->setCurrentIndex(index);
    ui->label->setText("Current scan: " + ui->listView->currentIndex().data().toString());

}


void widepix_analyser_2::U_set_id(double x, double y)
{
    ui->label_12->setNum(x);
    ui->label_13->setNum(y);
}

void widepix_analyser_2::U_change_scan_settings(UC_data_container::UTStr_data_container_settings settings)
{
    ui->lineEdit_3->setText(settings.path);
    ui->lineEdit_4->setText(settings.name);
    if (settings.sample_of_element) {
        ui->checkBox_14->setCheckState(Qt::Checked);
    } else {
        ui->checkBox_14->setCheckState(Qt::Unchecked);
    }
    ui->comboBox_19->setCurrentIndex(settings.element);
    ui->spinBox_9->setValue(settings.thl_sample);
    if (settings.calibration) {
        ui->checkBox_3->setCheckState(Qt::Checked);
    } else {
        ui->checkBox_3->setCheckState(Qt::Unchecked);
    }
    ui->doubleSpinBox_19->setValue(settings.energy);
    if (settings.both_counters) {
        ui->checkBox_2->setCheckState(Qt::Checked);
    } else {
        ui->checkBox_2->setCheckState(Qt::Unchecked);
    }
    ui->spinBox_11->setValue(settings.count);
    ui->doubleSpinBox_18->setValue(settings.time);
    if (settings.ff_int == -1) {
        ui->checkBox_15->setCheckState(Qt::Unchecked);
    } else {
        ui->checkBox_15->setCheckState(Qt::Checked);
        ui->comboBox_22->setCurrentIndex(settings.ff_int);
    }
    if (settings.df_int == -1) {
        ui->checkBox_16->setCheckState(Qt::Unchecked);
    } else {
        ui->checkBox_16->setCheckState(Qt::Checked);
        ui->comboBox_23->setCurrentIndex(settings.df_int);
    }
}

void widepix_analyser_2::U_renew_identification_elements(QList<QString> list)
{
    ui->comboBox_24->clear();
    int n = list.size();
    for (int i = 0; i < n; i++) {
        ui->comboBox_24->addItem(list[i]);
    }

}
//
void widepix_analyser_2::U_new_spectra(QString name)
{
    ui->comboBox_25->addItem(name);
    ui->comboBox_26->addItem(name);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void widepix_analyser_2::on_pushButton_clicked()
{
    QString graph_name = ui->lineEdit_5->text();
    if (graph_name == "") {
        graph_name += ui->comboBox_17->currentText() + "; ";
        graph_name += ui->comboBox_2->currentText() + "; ";
        graph_name += ui->comboBox_3->currentText() + "; ";
        QString roi = ui->comboBox_6->currentText();
        if (roi == "other") {
            roi = ui->lineEdit->text();
            if (roi == "") {
                roi = "xmin=%1, xmax=%2, ymin=%3, ymax=%4";
                roi = roi.arg(ui->spinBox_5->value()).arg(ui->spinBox_7->value()).arg(ui->spinBox_4->value()).arg(ui->spinBox_6->value());
            }
        }
        graph_name += roi;
    }
    wai->U_generate_spectra(graph_name);
    ui->comboBox_25->addItem(graph_name);
    ui->comboBox_26->addItem(graph_name);
}

void widepix_analyser_2::on_pushButton_2_clicked()
{
//    wai->U_set_ff(ui->listView->currentIndex().row());
//    ui->label_2->setText("Flat field scan: " + ui->listView->currentIndex().data().toString());
}

void widepix_analyser_2::on_pushButton_3_clicked()
{
    wai->U_calculating_spectra(static_cast<UC_wai::UTE_calculating_spectras>(ui->comboBox_27->currentIndex()), ui->comboBox_25->currentIndex(), ui->comboBox_26->currentIndex());
}

void widepix_analyser_2::on_pushButton_4_clicked()
{
    QString str = QFileDialog::getExistingDirectory(nullptr, "Open scan direction");
    if (str != "") {
        ui->lineEdit_3->setText(str);
    }
    ui->pushButton_31->setEnabled(true);
}

void widepix_analyser_2::on_comboBox_currentIndexChanged(int index)
{

}

void widepix_analyser_2::on_checkBox_3_toggled(bool checked)
{

}

void widepix_analyser_2::on_comboBox_2_currentIndexChanged(int index)
{
    wai->U_set_frame_type(static_cast<UC_plot::UTE_frame_type>(index));
}

void widepix_analyser_2::on_comboBox_3_currentIndexChanged(int index)
{
    wai->U_set_pixel_type(static_cast<UC_plot::UTE_pixel_type>(index));
}

void widepix_analyser_2::on_comboBox_4_currentIndexChanged(int index)
{
    wai->U_set_identification_type(static_cast<UC_plot::UTE_identification_type>(index));
}

void widepix_analyser_2::on_comboBox_5_currentIndexChanged(int index)
{
    //wai->U_set_threshold_range(index, ui->comboBox_11->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_pushButton_6_clicked()
{
    wai->U_generate_frame(ui->comboBox->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_pushButton_7_clicked()
{
    UC_pixels_info::UTStr_data_enable data_enable;
    data_enable.max_enable = (ui->checkBox_10->checkState() == Qt::Checked);
    data_enable.min_enable = (ui->checkBox_8->checkState() == Qt::Checked);
    data_enable.sum_enable = (ui->checkBox_5->checkState() == Qt::Checked);
    data_enable.mean_enable = (ui->checkBox_9->checkState() == Qt::Checked);
    data_enable.median_enable = (ui->checkBox_4->checkState() == Qt::Checked);
    data_enable.zeros_enable = (ui->checkBox_11->checkState() == Qt::Checked);
    data_enable.std_dev_enable = (ui->checkBox_7->checkState() == Qt::Checked);
    data_enable.snr_enable = (ui->checkBox_6->checkState() == Qt::Checked);
    data_enable.id1_enable = (ui->checkBox_12->checkState() == Qt::Checked);
    wai->U_set_data_enable(data_enable);

    wai->U_generate_table(ui->comboBox->currentText().remove(0, 6).toInt());
    ui->pushButton_25->setEnabled(true);
}

void widepix_analyser_2::on_comboBox_6_currentIndexChanged(int index)
{
    if (index < 15) {
        ui->spinBox_6->setValue(256);
        ui->spinBox_4->setValue(0);
        ui->spinBox_5->setValue(index * 256);
        ui->spinBox_7->setValue(index * 256 + 256);
    }
    if (index == 15) {
        ui->spinBox_6->setValue(256);
        ui->spinBox_4->setValue(0);
        ui->spinBox_5->setValue(0);
        ui->spinBox_7->setValue(3840);
    }
}

void widepix_analyser_2::on_pushButton_8_clicked()
{
    wai->U_set_roi(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
    int index = ui->comboBox_6->currentIndex();
    if (index < 16) {
        roi = ui->comboBox_6->currentText();
    } else {
        roi = "other";
    }
    ui->label_3->setNum(ui->spinBox_5->value());
    ui->label_20->setNum(ui->spinBox_4->value());
    ui->label_21->setNum(ui->spinBox_6->value());
    ui->label_22->setNum(ui->spinBox_7->value());
}

void widepix_analyser_2::on_pushButton_9_clicked()
{
    wai->U_resize_spectra();
}

void widepix_analyser_2::on_pushButton_11_clicked()
{
    wai->U_resize_frame();
}

void widepix_analyser_2::on_pushButton_10_clicked()
{
    wai->U_rescale_frame();
}

void widepix_analyser_2::on_pushButton_12_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save frame");
    if (str != "") {
        wai->U_save_frame(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_7->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_13_clicked()
{
    wai->U_set_mask(true, ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_pushButton_14_clicked()
{
    wai->U_diff_spectra();
}

void widepix_analyser_2::on_pushButton_15_clicked()
{
    wai->U_reset_mask();
}

void widepix_analyser_2::on_pushButton_16_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save spectra");
    if (str != "") {
        wai->U_save_spectra(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_8->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_20_clicked()
{
    QString graph_name;
    graph_name = ui->comboBox_3->currentText() + "; " + ui->comboBox_2->currentText() + "; " + roi;
    wai->U_generate_distribution(ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value(), graph_name);
}

void widepix_analyser_2::on_pushButton_17_clicked()
{
    QString graph_name;
    graph_name = ui->comboBox_3->currentText() + "; " + roi;
    graph_name += "; " + ui->comboBox->currentText();
    wai->U_generate_frame_distribution(ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value(), ui->comboBox->currentText().remove(0, 6).toInt(), graph_name);
}

void widepix_analyser_2::on_pushButton_21_clicked()
{
    int index = ui->listView->currentIndex().row();
    wai->U_delete_scan(index);
    if (ui->comboBox_17->currentIndex() == index) {
        ui->comboBox_17->removeItem(index);
        if (ui->comboBox_17->count() != 0) ui->comboBox_17->setCurrentIndex(0);
    } else {
        ui->comboBox_17->removeItem(index);
    }
    ui->label->setText("Current scan: " + ui->comboBox_17->currentText());
    ui->comboBox_22->removeItem(index);
    ui->comboBox_23->removeItem(index);
}

void widepix_analyser_2::on_pushButton_5_clicked()
{
    wai->U_reset_data();
    ui->lineEdit_3->setText("");
    ui->comboBox->clear();
    ui->comboBox_5->clear();
    ui->comboBox_11->clear();
    ui->comboBox_12->clear();
    ui->comboBox_13->clear();
    ui->comboBox_14->clear();
    ui->comboBox_15->clear();
    ui->comboBox_17->clear();

    ui->comboBox_22->clear();
    ui->comboBox_23->clear();

    //ui->pushButton_3->setEnabled(false);
    //ui->pushButton_2->setEnabled(false);
    ui->pushButton_29->setEnabled(false);
    ui->pushButton_21->setEnabled(false);

    ui->pushButton_13->setEnabled(false);
    //ui->pushButton_14->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_23->setEnabled(false);
    ui->pushButton_24->setEnabled(false);

    ui->pushButton_16->setEnabled(false);

    ui->pushButton_6->setEnabled(false);
    ui->pushButton_12->setEnabled(false);

    ui->pushButton_7->setEnabled(false);
    ui->pushButton_25->setEnabled(false);

    ui->pushButton_17->setEnabled(false);
    ui->pushButton_19->setEnabled(false);
    ui->pushButton_20->setEnabled(false);

    ui->pushButton_34->setEnabled(false);
    ui->pushButton_35->setEnabled(false);
    ui->pushButton_36->setEnabled(false);

    ui->pushButton_40->setEnabled(false);
    ui->pushButton_33->setEnabled(false);
    ui->pushButton_32->setEnabled(false);

    //ui->comboBox->setEnabled(false);
}

void widepix_analyser_2::on_pushButton_22_clicked()
{
    wai->U_reset_spectra();
    ui->comboBox_25->clear();
    ui->comboBox_26->clear();
}

void widepix_analyser_2::on_pushButton_18_clicked()
{
    wai->U_resize_distribution();
}



void widepix_analyser_2::on_pushButton_23_clicked()
{
    wai->U_set_mask_overflowed((ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
}

void widepix_analyser_2::on_pushButton_24_clicked()
{
    wai->U_set_mask_overflowed((ui->checkBox->checkState() == Qt::Checked));
}

void widepix_analyser_2::on_pushButton_19_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save distribution");
    if (str != "") {
        wai->U_save_distribution(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_9->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_25_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save table");
    if (str != "") {
        wai->U_save_table(str, static_cast<UC_wai::UTE_table_file_type>(ui->comboBox_10->currentIndex()));
    }
}

void widepix_analyser_2::on_spinBox_6_valueChanged(int arg1)
{
    if (arg1 != 256) ui->comboBox_6->setCurrentIndex(16);
}

void widepix_analyser_2::on_spinBox_7_valueChanged(int arg1)
{
    if ((arg1 % 256) != 0) ui->comboBox_6->setCurrentIndex(16);
}

void widepix_analyser_2::on_spinBox_4_valueChanged(int arg1)
{
    if (arg1 != 0) ui->comboBox_6->setCurrentIndex(16);
}

void widepix_analyser_2::on_spinBox_5_valueChanged(int arg1)
{
   if ((arg1 % 256) != 0) ui->comboBox_6->setCurrentIndex(16);
}

void widepix_analyser_2::on_pushButton_26_clicked()
{
    wai->U_delete_last_graph();
    ui->comboBox_25->removeItem(ui->comboBox_25->count());
    ui->comboBox_26->removeItem(ui->comboBox_25->count());
}

void widepix_analyser_2::on_pushButton_27_clicked()
{
    wai->U_delete_last_distribution();
}

void widepix_analyser_2::on_pushButton_28_clicked()
{
    wai->U_reset_distribution();
}

void widepix_analyser_2::on_pushButton_29_clicked()
{
    wai->U_set_scan(ui->listView->currentIndex().row());
    ui->label->setText("Current scan: " + ui->listView->currentIndex().data().toString());
}

void widepix_analyser_2::on_pushButton_30_clicked()
{
    wai->U_smooth_spectra();
}

void widepix_analyser_2::on_pushButton_31_clicked()
{
    UC_data_container::UTStr_data_container_settings settings;
    settings.path = ui->lineEdit_3->text();
    settings.count = ui->spinBox_11->value();
    settings.time = ui->doubleSpinBox_18->value();
    settings.both_counters = (ui->checkBox_2->checkState() == Qt::Checked);
    settings.calibration = (ui->checkBox_3->checkState() == Qt::Checked);
    settings.energy = ui->doubleSpinBox_19->value();
    settings.sample_of_element = (ui->checkBox_14->checkState() == Qt::Checked);
    settings.element = ui->comboBox_19->currentIndex();
    settings.thl_sample = ui->spinBox_9->value();
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
        if (settings.sample_of_element) {
            str += ui->comboBox_19->currentText() + "; ";
        }
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
    wai->U_set_data(settings, str);
    ui->comboBox_17->addItem(str);
    ui->comboBox_22->addItem(str);
    ui->comboBox_23->addItem(str);

    //ui->pushButton_3->setEnabled(true);
    //ui->pushButton_2->setEnabled(true);
    ui->pushButton_29->setEnabled(true);
    ui->pushButton_21->setEnabled(true);
    ui->pushButton_15->setEnabled(true);
    ui->pushButton_13->setEnabled(true);
    ui->pushButton_34->setEnabled(true);
    ui->pushButton_35->setEnabled(true);
    ui->pushButton_23->setEnabled(true);
    ui->pushButton_24->setEnabled(true);
    ui->pushButton_36->setEnabled(true);
    ui->pushButton_16->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_40->setEnabled(true);
    ui->pushButton_25->setEnabled(true);
    ui->pushButton_32->setEnabled(true);
    ui->pushButton_33->setEnabled(true);
    ui->pushButton_17->setEnabled(true);
    ui->pushButton_20->setEnabled(true);
    ui->pushButton_19->setEnabled(true);
    ui->comboBox->setEnabled(true);
}

void widepix_analyser_2::on_pushButton_34_clicked()
{
    wai->U_set_mask_plots(ui->doubleSpinBox_11->value());
}

void widepix_analyser_2::on_pushButton_35_clicked()
{
    wai->U_delete_mask_plots();
}

void widepix_analyser_2::on_pushButton_36_clicked()
{

   wai->U_count_mask(ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_radioButton_toggled(bool checked)
{
    if (checked) ui->radioButton_2->setDown(false);
}

void widepix_analyser_2::on_radioButton_2_toggled(bool checked)
{
    if (checked) ui->radioButton->setDown(false);
}

void widepix_analyser_2::on_pushButton_33_clicked()
{
    wai->U_generate_range(ui->comboBox->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_pushButton_32_clicked()
{
    wai->U_generate_range();
}

void widepix_analyser_2::on_pushButton_37_clicked()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Open spectra");
    if (str != "") {
        wai->U_load_spectra_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_38_clicked()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Open frame");
    if (str != "") {
        wai->U_load_frame_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_39_clicked()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Open distribution");
    if (str != "") {
        wai->U_load_distribution_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_40_clicked()
{
    UC_pixels_info::UTStr_data_enable data_enable;
    data_enable.max_enable = (ui->checkBox_10->checkState() == Qt::Checked);
    data_enable.min_enable = (ui->checkBox_8->checkState() == Qt::Checked);
    data_enable.sum_enable = (ui->checkBox_5->checkState() == Qt::Checked);
    data_enable.mean_enable = (ui->checkBox_9->checkState() == Qt::Checked);
    data_enable.median_enable = (ui->checkBox_4->checkState() == Qt::Checked);
    data_enable.zeros_enable = (ui->checkBox_11->checkState() == Qt::Checked);
    data_enable.std_dev_enable = (ui->checkBox_7->checkState() == Qt::Checked);
    data_enable.snr_enable = (ui->checkBox_6->checkState() == Qt::Checked);
    data_enable.id1_enable = (ui->checkBox_12->checkState() == Qt::Checked);
    wai->U_set_data_enable(data_enable);
    wai->U_generate_table();
}

void widepix_analyser_2::on_comboBox_11_currentIndexChanged(int index)
{
    //wai->U_set_threshold_range(ui->comboBox_5->currentIndex(), index);
}

void widepix_analyser_2::on_comboBox_15_currentIndexChanged(int index)
{
    //wai->U_set_thresholds(index, ui->comboBox_14->currentIndex(), ui->comboBox_13->currentIndex(), ui->comboBox_12->currentIndex());
}

void widepix_analyser_2::on_comboBox_14_currentIndexChanged(int index)
{
    //wai->U_set_thresholds(ui->comboBox_15->currentIndex(), index, ui->comboBox_13->currentIndex(), ui->comboBox_12->currentIndex());
}

void widepix_analyser_2::on_comboBox_13_currentIndexChanged(int index)
{
    //wai->U_set_thresholds(ui->comboBox_15->currentIndex(), ui->comboBox_14->currentIndex(), index, ui->comboBox_12->currentIndex());
}

void widepix_analyser_2::on_comboBox_12_currentIndexChanged(int index)
{
   // wai->U_set_thresholds(ui->comboBox_15->currentIndex(), ui->comboBox_14->currentIndex(), ui->comboBox_13->currentIndex(), index);
}

void widepix_analyser_2::on_pushButton_41_clicked()
{
    wai->U_normalize_spectra();
}

void widepix_analyser_2::on_pushButton_2_clicked(bool checked)
{

}

void widepix_analyser_2::on_comboBox_17_currentIndexChanged(int index)
{
    wai->U_set_scan(index);
    ui->label->setText("Current scan: " + ui->comboBox_17->currentText());
}

void widepix_analyser_2::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
    QString str = arg1;
    wai->U_set_threshold_range(str.remove(0, 6).toInt(), ui->comboBox_11->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_comboBox_11_currentIndexChanged(const QString &arg1)
{
    QString str = arg1;
    wai->U_set_threshold_range(ui->comboBox_5->currentText().remove(0, 6).toInt(), str.remove(0, 6).toInt());
}

void widepix_analyser_2::on_comboBox_15_currentIndexChanged(const QString &arg1)
{
    QString str = arg1;
    wai->U_set_thresholds(str.remove(0, 6).toInt(), ui->comboBox_14->currentText().remove(0, 6).toInt(), ui->comboBox_13->currentText().remove(0, 6).toInt(), ui->comboBox_12->currentText().remove(0, 6).toInt());

}

void widepix_analyser_2::on_comboBox_14_currentIndexChanged(const QString &arg1)
{
    QString str = arg1;
    wai->U_set_thresholds(ui->comboBox_15->currentText().remove(0, 6).toInt(), str.remove(0, 6).toInt(), ui->comboBox_13->currentText().remove(0, 6).toInt(), ui->comboBox_12->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_comboBox_13_currentIndexChanged(const QString &arg1)
{
    QString str = arg1;
    wai->U_set_thresholds(ui->comboBox_15->currentText().remove(0, 6).toInt(), ui->comboBox_14->currentText().remove(0, 6).toInt(), str.remove(0, 6).toInt(), ui->comboBox_12->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_comboBox_12_currentIndexChanged(const QString &arg1)
{
    QString str = arg1;
    wai->U_set_thresholds(ui->comboBox_15->currentText().remove(0, 6).toInt(), ui->comboBox_14->currentText().remove(0, 6).toInt(), ui->comboBox_13->currentText().remove(0, 6).toInt(), str.remove(0, 6).toInt());
}

void widepix_analyser_2::on_radioButton_3_clicked(bool checked)
{
    //if (checked) ui->radioButton_4->setChecked(false);
    wai->U_set_interaction_mode(checked);
}

void widepix_analyser_2::on_radioButton_4_clicked(bool checked)
{
    //if (checked) ui->radioButton_3->setChecked(false);
    wai->U_set_interaction_mode(!checked);
}

void widepix_analyser_2::on_pushButton_43_clicked()
{
    switch (ui->tabWidget->currentIndex()) {
        case 0 : {
            wai->U_mask_selected_value(ui->checkBox->checkState() == Qt::Checked/*, ui->comboBox->currentText().remove(0, 6).toInt()*/);
            break;
        }
        case 2 : {
            wai->U_mask_selected(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
            break;
        }
        case 4 : {
            wai->U_mask_selected_value(ui->checkBox->checkState() == Qt::Checked/*, ui->comboBox->currentText().remove(0, 6).toInt()*/);
            break;
        }
    }
}

void widepix_analyser_2::on_pushButton_44_clicked()
{
    switch (ui->tabWidget->currentIndex()) {
        case 0 : {
            wai->U_mask_selected_value_thl(ui->checkBox->checkState() == Qt::Checked, ui->comboBox->currentText().remove(0, 6).toInt());
            break;
        }
        case 2 : {
            wai->U_mask_selected(ui->spinBox_5->value(), ui->spinBox_7->value(), ui->spinBox_4->value(), ui->spinBox_6->value());
            break;
        }
        case 4 : {
            wai->U_mask_selected_value_thl(ui->checkBox->checkState() == Qt::Checked, ui->comboBox->currentText().remove(0, 6).toInt());
            break;
        }
    }
}

void widepix_analyser_2::on_radioButton_5_clicked(bool checked)
{
    wai->U_set_x_axis_type(checked);
}

void widepix_analyser_2::on_radioButton_6_clicked(bool checked)
{
    wai->U_set_x_axis_type(!checked);
}

void widepix_analyser_2::on_radioButton_7_clicked()
{

}

void widepix_analyser_2::on_radioButton_7_clicked(bool checked)
{
    wai->U_set_y_axis_type(checked);
}

void widepix_analyser_2::on_radioButton_8_clicked(bool checked)
{
    wai->U_set_y_axis_type(!checked);
}

void widepix_analyser_2::on_pushButton_42_clicked()
{
    wai->U_generate_calibration_chip(ui->comboBox_18->currentIndex());
}

void widepix_analyser_2::on_pushButton_45_clicked()
{
    wai->U_generate_calibration();
}

void widepix_analyser_2::on_pushButton_46_clicked()
{
    QString graph_name;
    graph_name = ui->comboBox_3->currentText() + "; " + ui->comboBox_2->currentText() + "; " + roi;
    wai->U_generate_spectra(graph_name, ui->doubleSpinBox_20->value(), ui->doubleSpinBox_21->value(), ui->spinBox_2->value());
}

void widepix_analyser_2::on_pushButton_47_clicked()
{
    wai->U_generate_frame(ui->doubleSpinBox_22->value());
}

void widepix_analyser_2::on_pushButton_48_clicked()
{
    wai->U_generate_table(ui->doubleSpinBox_22->value());
}

void widepix_analyser_2::on_pushButton_49_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save chip fit");
    if (str != "") {
        wai->U_save_calibration_chip(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_16->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_52_clicked()
{
    wai->U_reset_calibration_chip();
}

void widepix_analyser_2::on_pushButton_54_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save spectra");
    if (str != "") {
        wai->U_automatic_save_spectra(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_8->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_57_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save calibration");
    if (str != "") {
        wai->U_save_calibration(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_20->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_55_clicked()
{
    QString str;
    str = ui->lineEdit_2->text();
    if (str == "") str = "El";
    wai->U_generate_identification_roi(str, static_cast<UC_plot::UTE_identification_type>(ui->comboBox_4->currentIndex()));
}

void widepix_analyser_2::on_pushButton_56_clicked()
{
    wai->U_reset_identification_roi(static_cast<UC_plot::UTE_identification_type>(ui->comboBox_4->currentIndex()));
}

void widepix_analyser_2::on_pushButton_60_clicked()
{
    wai->U_reset_calibration();
}

void widepix_analyser_2::on_checkBox_13_toggled(bool checked)
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

    }
}

void widepix_analyser_2::on_doubleSpinBox_3_valueChanged(double arg1)
{

}

void widepix_analyser_2::on_pushButton_50_clicked()
{
    wai->U_resize_calibration_chip();
}

void widepix_analyser_2::on_pushButton_51_clicked()
{
    wai->U_generate_additional_data();
}

void widepix_analyser_2::on_pushButton_53_clicked()
{
    emit US_set_threshold_level(ui->doubleSpinBox_31->value());
}

void widepix_analyser_2::on_pushButton_59_clicked()
{
    wai->U_toggle_calibration();
}

void widepix_analyser_2::on_pushButton_62_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save calibration");
    if (str != "") {
        emit US_save_calibration(str);
    }
}

void widepix_analyser_2::on_pushButton_61_clicked()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Load calibration");
    if (str != "") {
        emit US_load_calibration(str);
    }
}

void widepix_analyser_2::on_pushButton_63_clicked()
{
    wai->U_generate_identification_frame(ui->comboBox_24->currentIndex());
}

void widepix_analyser_2::on_pushButton_64_clicked()
{
    //wai->U_generate_identification();
}

void widepix_analyser_2::on_pushButton_65_clicked()
{
    wai->U_add_roi(ui->spinBox_5->value(), ui->spinBox_4->value(), ui->spinBox_6->value(), ui->spinBox_7->value());
    QString str = ui->lineEdit->text();
    if (str == "") {
        str = "xmin = %1; ymin = %2; ymax = %3; xmax=%4";
        str = str.arg(ui->spinBox_5->value());
        str = str.arg(ui->spinBox_4->value());
        str = str.arg(ui->spinBox_6->value());
        str = str.arg(ui->spinBox_7->value());
    }
    ui->comboBox_21->addItem(str);
}

void widepix_analyser_2::on_pushButton_66_clicked()
{
    int index = ui->comboBox_21->currentIndex();
    wai->U_delete_roi(index);
    ui->comboBox_21->removeItem(index);
}

void widepix_analyser_2::on_comboBox_21_currentIndexChanged(int index)
{
    if (ui->comboBox_21->count() == 0) return;
    QRect roi = wai->U_get_roi(index);
    ui->spinBox_5->setValue(roi.left());
    ui->spinBox_4->setValue(roi.right() + 1);
    ui->spinBox_6->setValue(roi.top());
    ui->spinBox_7->setValue(roi.bottom() + 1);
}

void widepix_analyser_2::on_pushButton_67_clicked()
{
    UC_data_container::UTStr_data_container_settings settings;
    settings.count = ui->spinBox_11->value();
    settings.time = ui->doubleSpinBox_18->value();
    settings.calibration = (ui->checkBox_3->checkState() == Qt::Checked);
    settings.energy = ui->doubleSpinBox_19->value();
    settings.sample_of_element = (ui->checkBox_14->checkState() == Qt::Checked);
    settings.element = ui->comboBox_19->currentIndex();
    settings.thl_sample = ui->spinBox_9->value();
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
        if (settings.sample_of_element) {
            str += ui->comboBox_19->currentText() + "; ";
        }
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
    wai->U_change_scan_setting(ui->listView->currentIndex().row(), settings);
}

void widepix_analyser_2::on_listView_clicked(const QModelIndex &index)
{
    wai->U_change_scan(ui->listView->currentIndex().row());

}

void widepix_analyser_2::on_spinBox_3_valueChanged(int arg1)
{
    wai->U_set_rebin(arg1, ui->spinBox_8->value(), ui->spinBox_10->value());
}

void widepix_analyser_2::on_spinBox_8_valueChanged(int arg1)
{
    wai->U_set_rebin(ui->spinBox_3->value(), arg1, ui->spinBox_10->value());
}

void widepix_analyser_2::on_spinBox_10_valueChanged(int arg1)
{
    wai->U_set_rebin(ui->spinBox_3->value(), ui->spinBox_8->value(), arg1);
}

void widepix_analyser_2::on_pushButton_68_clicked()
{
    wai->U_generate_identification_data();
}

void widepix_analyser_2::on_spinBox_12_valueChanged(int arg1)
{
    wai->U_set_smoothing(arg1);
}
