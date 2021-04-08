#include "widepix_analyser_2.h"
#include "ui_widepix_analyser_2.h"

widepix_analyser_2::widepix_analyser_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::widepix_analyser_2)
{
    ui->setupUi(this);

    wai = new UC_wai();
    wai->U_set_qcp(ui->widget, ui->widget_2, ui->widget_3);
    wai->U_set_table(ui->tableView);

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
    ////
    connect(wai, SIGNAL(US_new_thl(int)), this, SLOT(U_add_thl(int)));
    ////
    connect(wai->U_get_plot(), SIGNAL(US_file_found(QString)),                this, SLOT(U_file_found(QString)));
    connect(wai->U_get_plot(), SIGNAL(US_n_files(int)),                       this, SLOT(U_n_files(int)));
    connect(wai->U_get_plot(), SIGNAL(US_renew_progress_bar(double, double)), this, SLOT(U_renew_progress_bar(double, double)));
    ////
    connect(wai, SIGNAL(US_set_distribution_range(double, double)), this, SLOT(U_set_distribution_range(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_range_data(double, double)), this, SLOT(U_set_distribution_range(double, double)));
    ///
    connect(wai->U_get_plot(), SIGNAL(US_count_mask(int)), this, SLOT(U_set_count_mask(int)));
    ///
    wai->U_start();

    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

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

    ui->pushButton_29->setEnabled(false);
    ui->pushButton_30->setEnabled(false);
    ui->pushButton_31->setEnabled(false);

    ui->pushButton_34->setEnabled(false);
    ui->pushButton_35->setEnabled(false);
    ui->pushButton_36->setEnabled(false);

    ui->pushButton_40->setEnabled(false);
    ui->pushButton_33->setEnabled(false);
    ui->pushButton_32->setEnabled(false);

    ui->comboBox->setEnabled(false);

    //////////////////////////////////////////////////////
    roi = "all chips";

}

widepix_analyser_2::~widepix_analyser_2()
{
    wai->~UC_wai();
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void widepix_analyser_2::U_add_thl(int thl)
{
    ui->comboBox->addItem(QString("thl = %1").arg(thl));
    ui->comboBox_5->addItem(QString("thl = %1").arg(thl));
    ui->comboBox_11->addItem(QString("thl = %1").arg(thl));
    ui->comboBox_12->addItem(QString("thl = %1").arg(thl));
    ui->comboBox_13->addItem(QString("thl = %1").arg(thl));
    ui->comboBox_14->addItem(QString("thl = %1").arg(thl));
    ui->comboBox_15->addItem(QString("thl = %1").arg(thl));
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
//////////////////////////////////////////////////////////////////////////////////////////////
void widepix_analyser_2::on_pushButton_clicked()
{
    QString graph_name;
    graph_name = ui->comboBox_3->currentText() + "; " + ui->comboBox_2->currentText() + "; " + roi;
    wai->U_generate_plot(graph_name);
}

void widepix_analyser_2::on_pushButton_2_clicked()
{
    QString str = QFileDialog::getExistingDirectory(nullptr, "Open frames direction");
    if (str != "") {
        ui->lineEdit->setText(str);
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");
        ui->pushButton_29->setEnabled(true);
    }
}

void widepix_analyser_2::on_pushButton_3_clicked()
{
    QString str = QFileDialog::getExistingDirectory(nullptr, "Open dark field direction");
    if (str != "") {
        ui->lineEdit_2->setText(str);
   }
}

void widepix_analyser_2::on_pushButton_4_clicked()
{
    QString str = QFileDialog::getExistingDirectory(nullptr, "Open flat field direction");
    if (str != "") {
        ui->lineEdit_3->setText(str);
    }
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
    wai->U_set_threshold_range(index, ui->comboBox_11->currentIndex());
}

void widepix_analyser_2::on_pushButton_6_clicked()
{
    wai->U_generate_frame(ui->comboBox->currentIndex());
}

void widepix_analyser_2::on_pushButton_7_clicked()
{
    wai->U_generate_table(ui->comboBox->currentIndex());
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
    wai->U_set_mask(true, ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
}

void widepix_analyser_2::on_pushButton_14_clicked()
{
    wai->U_set_mask(true, true, ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
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
    wai->U_generate_frame_distribution(ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value(), ui->comboBox->currentIndex(), graph_name);
}

void widepix_analyser_2::on_pushButton_21_clicked()
{
    //wai->U_generate_calibration();
}

void widepix_analyser_2::on_pushButton_5_clicked()
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->comboBox->clear();
    ui->comboBox_5->clear();
    ui->comboBox_11->clear();
    ui->comboBox_12->clear();
    ui->comboBox_13->clear();
    ui->comboBox_14->clear();
    ui->comboBox_15->clear();
    wai->U_reset_data();
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

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

    ui->comboBox->setEnabled(false);
}

void widepix_analyser_2::on_pushButton_22_clicked()
{
    wai->U_reset_plot();
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
    QString str = ui->lineEdit->text();
    if (str != "") {
        wai->U_set_data(str, ui->spinBox_9->value(), ui->doubleSpinBox_20->value(), (ui->checkBox_2->checkState() == Qt::Checked));
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);

        ui->pushButton_13->setEnabled(true);
        //ui->pushButton_14->setEnabled(true);
        ui->pushButton_15->setEnabled(true);
        ui->pushButton_23->setEnabled(true);
        ui->pushButton_24->setEnabled(true);

        ui->pushButton_16->setEnabled(true);

        ui->pushButton_6->setEnabled(true);
        ui->pushButton_12->setEnabled(true);

        ui->pushButton_7->setEnabled(true);
        ui->pushButton_25->setEnabled(false);

        ui->pushButton_17->setEnabled(true);
        ui->pushButton_19->setEnabled(true);
        ui->pushButton_20->setEnabled(true);

        ui->pushButton_30->setEnabled(true);
        ui->pushButton_31->setEnabled(true);

        ui->pushButton_34->setEnabled(true);
        ui->pushButton_35->setEnabled(true);
        ui->pushButton_36->setEnabled(true);

        ui->pushButton_40->setEnabled(true);
        ui->pushButton_33->setEnabled(true);
        ui->pushButton_32->setEnabled(true);

        ui->comboBox->setEnabled(true);
    }
}

void widepix_analyser_2::on_pushButton_30_clicked()
{
    QString str = ui->lineEdit_2->text();
    if (str != "") {
        wai->U_set_data_df(str, ui->spinBox_10->value(), ui->doubleSpinBox_19->value(), (ui->checkBox_2->checkState() == Qt::Checked));
    }
}

void widepix_analyser_2::on_pushButton_31_clicked()
{
    QString str = ui->lineEdit_3->text();
    if (str != "") {
        wai->U_set_data_ff(str, ui->spinBox_11->value(), ui->doubleSpinBox_18->value(), (ui->checkBox_2->checkState() == Qt::Checked));
    }
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

    wai->U_count_mask(ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
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
    wai->U_generate_range(ui->comboBox->currentIndex());
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
    wai->U_generate_table();
}

void widepix_analyser_2::on_comboBox_11_currentIndexChanged(int index)
{
    wai->U_set_threshold_range(ui->comboBox_5->currentIndex(), index);
}

void widepix_analyser_2::on_comboBox_15_currentIndexChanged(int index)
{
    wai->U_set_thresholds(index, ui->comboBox_14->currentIndex(), ui->comboBox_13->currentIndex(), ui->comboBox_12->currentIndex());
}

void widepix_analyser_2::on_comboBox_14_currentIndexChanged(int index)
{
    wai->U_set_thresholds(ui->comboBox_15->currentIndex(), index, ui->comboBox_13->currentIndex(), ui->comboBox_12->currentIndex());
}

void widepix_analyser_2::on_comboBox_13_currentIndexChanged(int index)
{
    wai->U_set_thresholds(ui->comboBox_15->currentIndex(), ui->comboBox_14->currentIndex(), index, ui->comboBox_12->currentIndex());
}

void widepix_analyser_2::on_comboBox_12_currentIndexChanged(int index)
{
    wai->U_set_thresholds(ui->comboBox_15->currentIndex(), ui->comboBox_14->currentIndex(), ui->comboBox_13->currentIndex(), index);
}

void widepix_analyser_2::on_pushButton_41_clicked()
{
    wai->U_normalize_spectra();
}
