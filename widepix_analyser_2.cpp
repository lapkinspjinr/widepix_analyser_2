#include "widepix_analyser_2.h"
#include "ui_widepix_analyser_2.h"

widepix_analyser_2::widepix_analyser_2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::widepix_analyser_2)
{
    ui->setupUi(this);

    wai = new UC_wai();

    wai->U_set_spectra_qcp(ui->widget);
    wai->U_set_frame_qcp(ui->widget_2);
    wai->U_set_distribution_qcp(ui->widget_3);
    wai->U_set_chip_fit_qcp(ui->widget_4);
    wai->U_set_calibration_qcp(ui->widget_5);
    wai->U_set_id_frame_qcp(ui->widget_7);
    wai->U_set_list(ui->listView);
    wai->U_set_table(ui->tableView);
    wai->U_set_id_table(ui->tableView_2);

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

    on_checkBox_13_toggled(true);
    //
    connect(wai, SIGNAL(US_mouse_data(QString)),                                                    ui->statusBar,  SLOT(showMessage(QString)));
    connect(wai, SIGNAL(US_set_distribution_range(double, double)),                                 this,           SLOT(U_set_distribution_range(double, double)));
    connect(wai, SIGNAL(US_set_scan(int)),                                                          this,           SLOT(U_set_scan(int)));
    connect(wai, SIGNAL(US_set_scan(int)),                                                          this,           SLOT(U_set_scan(int)));
    connect(wai, SIGNAL(US_set_thl_range(int, int)),                                          this,           SLOT(U_set_thl_range(int, int)));
    connect(wai, SIGNAL(US_renew_scan_settings(UC_data_container::UTStr_data_container_settings)),  this,           SLOT(U_change_scan_settings(UC_data_container::UTStr_data_container_settings)));
    connect(wai, SIGNAL(US_add_roi(QString)),  this,           SLOT(U_add_roi(QString)));
    connect(wai, SIGNAL(US_set_roi_range(int, int, int, int)),    this, SLOT(U_set_roi_range(int, int, int, int)));

    connect(this, SIGNAL(US_set_threshold_level(double)),  wai->U_get_plot(), SLOT(U_set_threshold_level(double)));
    connect(this, SIGNAL(US_save_calibration(QString)), wai->U_get_plot(), SLOT(U_save_calibration(QString)));
    connect(this, SIGNAL(US_load_calibration(QString)), wai->U_get_plot(), SLOT(U_load_calibration(QString)));

    connect(wai->U_get_plot(), SIGNAL(US_thl_range(int, int)),                  this, SLOT(U_add_thl(int, int)));
    connect(wai->U_get_plot(), SIGNAL(US_file_found(QString)),                  this, SLOT(U_file_found(QString)));
    connect(wai->U_get_plot(), SIGNAL(US_n_files(int)),                         this, SLOT(U_n_files(int)));
    connect(wai->U_get_plot(), SIGNAL(US_renew_progress_bar(double, double)),   this, SLOT(U_renew_progress_bar(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_range_data(double, double)),           this, SLOT(U_set_distribution_range(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_count_mask(int)),                      this, SLOT(U_set_count_mask(int)));
    //connect(wai->U_get_plot(), SIGNAL(US_id_data(double, double)),              this, SLOT(U_set_id(double, double)));
    connect(wai->U_get_plot(), SIGNAL(US_id_scan_list(QList<QString>)),         this, SLOT(U_renew_identification_elements(QList<QString>)));


    wai->U_start();

    list_enabling << ui->pushButton_29;
    list_enabling << ui->pushButton_21;
    list_enabling << ui->pushButton_13;
    list_enabling << ui->pushButton_15;
    list_enabling << ui->pushButton_23;
    list_enabling << ui->pushButton_24;
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

    ui->comboBox->clear();
    ui->comboBox_12->clear();
    ui->comboBox_13->clear();
    ui->comboBox_14->clear();
    ui->comboBox_15->clear();
    ui->spinBox_13->setRange(thl_min, thl_max);
    ui->spinBox_14->setRange(thl_min, thl_max);
    ui->spinBox_13->setValue(thl_min);
    ui->spinBox_14->setValue(thl_max);
    for (int thl = thl_min; thl <= thl_max; thl++) {
        ui->comboBox->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_12->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_13->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_14->addItem(QString("thl = %1").arg(thl));
        ui->comboBox_15->addItem(QString("thl = %1").arg(thl));
    }
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

void widepix_analyser_2::U_set_thl_range(int thl_min, int thl_max)
{

    ui->spinBox_13->setValue(thl_min);
    ui->spinBox_14->setValue(thl_max);
}

void widepix_analyser_2::U_set_scan(int index)
{
    ui->comboBox_17->setCurrentIndex(index);
    ui->label->setText("Current scan: " + ui->listView->currentIndex().data().toString());
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

void widepix_analyser_2::U_new_spectra(QString name)
{
    ui->comboBox_25->addItem(name);
    ui->comboBox_26->addItem(name);
}

void widepix_analyser_2::U_add_roi(QString name)
{
    ui->comboBox_6->addItem(name);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void widepix_analyser_2::on_pushButton_clicked() ///< Spectra. Add graph
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

void widepix_analyser_2::on_pushButton_3_clicked() ///< Spectra. Create spectra
{
    wai->U_calculating_spectra(static_cast<UC_wai::UTE_calculating_spectras>(ui->comboBox_27->currentIndex()), ui->comboBox_25->currentIndex(), ui->comboBox_26->currentIndex());
}

void widepix_analyser_2::on_pushButton_4_clicked() ///< Source. Browse
{
    QString str = QFileDialog::getExistingDirectory(this, "Open scan direction");
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

void widepix_analyser_2::on_comboBox_4_currentIndexChanged(int index) ///< Identification type
{
    wai->U_set_id_type(static_cast<UC_plot::UTE_id_type>(index));
}

void widepix_analyser_2::on_pushButton_6_clicked() ///< Frame. Show frame
{
    wai->U_generate_frame(ui->comboBox->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_pushButton_7_clicked() ///< Table. Reload table for 1 frame
{
    wai->U_generate_table(ui->comboBox->currentText().remove(0, 6).toInt());
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
    QString str = QFileDialog::getSaveFileName(this, "Save frame");
    if (str != "") {
        wai->U_save_frame(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_7->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_13_clicked() ///< Set mask
{
    wai->U_set_mask(true, ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_pushButton_15_clicked() ///< Reset mask
{
    wai->U_reset_mask();
}

void widepix_analyser_2::on_pushButton_16_clicked() ///< Spectra. Save
{
    QString str = QFileDialog::getSaveFileName(this, "Save spectra");
    if (str != "") {
        wai->U_save_spectra(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_8->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_20_clicked() ///< Distribution. Distribution all frames
{
    QString graph_name = ui->lineEdit_6->text();
    if (graph_name == "") {
        graph_name += ui->comboBox_17->currentText() + "; ";
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
    wai->U_generate_distribution(ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value(), graph_name);
}

void widepix_analyser_2::on_pushButton_17_clicked() ///< Distribution. Distribution 1 frame
{
    QString graph_name = ui->lineEdit_6->text();
    if (graph_name == "") {
        graph_name += ui->comboBox_17->currentText() + "; ";
        graph_name += ui->comboBox_3->currentText() + "; ";
        graph_name += ui->comboBox->currentText() + "; ";
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
    wai->U_generate_distribution(ui->spinBox->value(), ui->doubleSpinBox_17->value(), ui->doubleSpinBox_16->value(), ui->comboBox->currentText().remove(0, 6).toInt(), graph_name);
}

void widepix_analyser_2::on_pushButton_21_clicked() ///< Source. Delete scan
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

void widepix_analyser_2::on_pushButton_5_clicked() ///< Source. Reset
{
    wai->U_reset_data();
    ui->lineEdit_3->setText("");
    ui->comboBox->clear();
    ui->comboBox_12->clear();
    ui->comboBox_13->clear();
    ui->comboBox_14->clear();
    ui->comboBox_15->clear();
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

void widepix_analyser_2::on_pushButton_23_clicked() ///< Mask overflow 1 frame
{
    wai->U_set_mask_overflowed((ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentIndex());
}

void widepix_analyser_2::on_pushButton_24_clicked() ///< Mask overflow all frames
{
    wai->U_set_mask_overflowed((ui->checkBox->checkState() == Qt::Checked));
}

void widepix_analyser_2::on_pushButton_19_clicked() ///< Distribution. Save
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save distribution");
    if (str != "") {
        wai->U_save_distribution(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_9->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_25_clicked() ///< Table. Save
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save table");
    if (str != "") {
        wai->U_save_table(str, static_cast<UC_wai::UTE_table_file_type>(ui->comboBox_10->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_26_clicked() ///< Spectra. Delete last graph
{
    wai->U_delete_last_spectra();
    ui->comboBox_25->removeItem(ui->comboBox_25->count() - 1);
    ui->comboBox_26->removeItem(ui->comboBox_25->count() - 1);
}

void widepix_analyser_2::on_pushButton_27_clicked() ///< Distribution. Delete last graph
{
    wai->U_delete_last_distribution();
}

void widepix_analyser_2::on_pushButton_28_clicked() ///< Distribution. Reset
{
    wai->U_reset_distribution();
}

void widepix_analyser_2::on_pushButton_29_clicked() ///< Source. Set scan
{
    wai->U_set_scan(ui->listView->currentIndex().row());
    ui->label->setText("Current scan: " + ui->listView->currentIndex().data().toString());
}

void widepix_analyser_2::on_pushButton_31_clicked() ///< Source. Add scan
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

    for (int i = 0; i < list_enabling.size(); i++) {
        list_enabling[i]->setEnabled(true);
    }
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
   wai->U_count_mask(ui->radioButton->isChecked(), ui->doubleSpinBox_11->value(), (ui->checkBox->checkState() == Qt::Checked), ui->comboBox->currentText().remove(0, 6).toInt());
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
    wai->U_generate_range(ui->comboBox->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_pushButton_32_clicked() ///< Distribution. Set range all frames
{
    wai->U_generate_range();
}

void widepix_analyser_2::on_pushButton_37_clicked() ///< Spectra. Load txt
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Open spectra");
    if (str != "") {
        wai->U_load_spectra_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_38_clicked() ///< Frame. Load txt
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Open frame");
    if (str != "") {
        wai->U_load_frame_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_39_clicked() ///< Distribution. Load txt
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Open distribution");
    if (str != "") {
        wai->U_load_distribution_txt(str);
    }
}

void widepix_analyser_2::on_pushButton_40_clicked() ///< Table. Reload table all frames
{
    wai->U_generate_table();
}

void widepix_analyser_2::on_pushButton_41_clicked() ///< Spectra. Normalize
{
    wai->U_normalize_spectra();
}

void widepix_analyser_2::on_comboBox_17_currentIndexChanged(int index) ///< Scan
{
    wai->U_set_scan(index);
    ui->label->setText("Current scan: " + ui->comboBox_17->currentText());
}

void widepix_analyser_2::on_comboBox_5_currentIndexChanged(const QString &arg1) ///< THL range. min
{
    QString str = arg1;
    wai->U_set_threshold_range(str.remove(0, 6).toInt(), ui->spinBox_14->value());
}

void widepix_analyser_2::on_comboBox_11_currentIndexChanged(const QString &arg1) ///< THL range. max
{
    QString str = arg1;
    wai->U_set_threshold_range(ui->spinBox_13->value(), str.remove(0, 6).toInt());
}

void widepix_analyser_2::on_comboBox_15_currentIndexChanged(const QString &arg1) ///< Identification. Windows THL. first left
{
    QString str = arg1;
    wai->U_set_thresholds(str.remove(0, 6).toInt(), ui->comboBox_14->currentText().remove(0, 6).toInt(), ui->comboBox_13->currentText().remove(0, 6).toInt(), ui->comboBox_12->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_comboBox_14_currentIndexChanged(const QString &arg1) ///< Identification. Windows THL. first right
{
    QString str = arg1;
    wai->U_set_thresholds(ui->comboBox_15->currentText().remove(0, 6).toInt(), str.remove(0, 6).toInt(), ui->comboBox_13->currentText().remove(0, 6).toInt(), ui->comboBox_12->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_comboBox_13_currentIndexChanged(const QString &arg1) ///< Identification. Windows THL. Second left
{
    QString str = arg1;
    wai->U_set_thresholds(ui->comboBox_15->currentText().remove(0, 6).toInt(), ui->comboBox_14->currentText().remove(0, 6).toInt(), str.remove(0, 6).toInt(), ui->comboBox_12->currentText().remove(0, 6).toInt());
}

void widepix_analyser_2::on_comboBox_12_currentIndexChanged(const QString &arg1) ///< Identification. Windows THL. Second right
{
    QString str = arg1;
    wai->U_set_thresholds(ui->comboBox_15->currentText().remove(0, 6).toInt(), ui->comboBox_14->currentText().remove(0, 6).toInt(), ui->comboBox_13->currentText().remove(0, 6).toInt(), str.remove(0, 6).toInt());
}

void widepix_analyser_2::on_radioButton_3_clicked(bool checked) ///< Graphs interaction mode. Select
{
    wai->U_set_interaction_mode(checked);
}

void widepix_analyser_2::on_radioButton_4_clicked(bool checked) ///< Graphs interaction mode. Drag
{
    wai->U_set_interaction_mode(!checked);
}

void widepix_analyser_2::on_pushButton_43_clicked() ///< Mask selected
{
    switch (ui->tabWidget->currentIndex()) {
        case 0 : {
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
    }
}

void widepix_analyser_2::on_pushButton_44_clicked() ///< Mask selected 1 frame
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
    wai->U_generate_chip_fit(ui->comboBox_18->currentIndex());
}

void widepix_analyser_2::on_pushButton_45_clicked() ///< Calibration. Generate calibration
{
    wai->U_generate_calibration();
}

void widepix_analyser_2::on_pushButton_46_clicked() ///< Spectra. Add graph by energy
{
    QString graph_name;
    graph_name = ui->comboBox_3->currentText() + "; " + ui->comboBox_2->currentText() + "; " + roi;
    wai->U_generate_spectra(graph_name, ui->spinBox_2->value());
}

void widepix_analyser_2::on_pushButton_47_clicked() ///< Frame. Frame by energy
{
    wai->U_generate_frame(ui->doubleSpinBox_22->value());
}

void widepix_analyser_2::on_pushButton_48_clicked() ///< Table. Reload table for energy
{
    wai->U_generate_table(ui->doubleSpinBox_22->value());
}

void widepix_analyser_2::on_pushButton_49_clicked() ///< Calibration. Save // chip fit
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save chip fit");
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
    QString str = QFileDialog::getSaveFileName(this, "Save spectra");
    if (str != "") {
        wai->U_automatic_save_spectra(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_8->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_57_clicked() ///< Calibration. Save
{
    QString str = QFileDialog::getSaveFileName(this, "Save calibration");
    if (str != "") {
        wai->U_save_calibration(str, static_cast<UC_wai::UTE_file_type>(ui->comboBox_20->currentIndex()));
    }
}

void widepix_analyser_2::on_pushButton_55_clicked() ///< Identification. generate id for Roi
{
    QString str;
    str = ui->lineEdit_2->text();
    if (str == "") str = "El";
    wai->U_generate_id_roi(str, static_cast<UC_plot::UTE_id_type>(ui->comboBox_4->currentIndex()));
}

void widepix_analyser_2::on_pushButton_56_clicked() ///< Identification. reset
{
    wai->U_reset_id_roi(static_cast<UC_plot::UTE_id_type>(ui->comboBox_4->currentIndex()));
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

        connect(wai, SIGNAL(US_set_calibration_max_x(double)), ui->doubleSpinBox_24, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_calibration_min_x(double)), ui->doubleSpinBox_25, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_calibration_max_y(double)), ui->doubleSpinBox_23, SLOT(setValue(double)));
        connect(wai, SIGNAL(US_set_calibration_min_y(double)), ui->doubleSpinBox_26, SLOT(setValue(double)));
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

        disconnect(wai, SIGNAL(US_set_calibration_max_x(double)), ui->doubleSpinBox_24, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_calibration_min_x(double)), ui->doubleSpinBox_25, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_calibration_max_y(double)), ui->doubleSpinBox_23, SLOT(setValue(double)));
        disconnect(wai, SIGNAL(US_set_calibration_min_y(double)), ui->doubleSpinBox_26, SLOT(setValue(double)));
    }
}

void widepix_analyser_2::on_pushButton_50_clicked() ///< Calibration. resize // chip fit
{
    wai->U_resize_chip_fit();
}

void widepix_analyser_2::on_pushButton_51_clicked() ///< Identification. generate
{
    wai->U_generate_additional_data();
}

void widepix_analyser_2::on_pushButton_53_clicked() ///< Calibration. Set threshold level
{
    emit US_set_threshold_level(ui->doubleSpinBox_31->value());
}

void widepix_analyser_2::on_pushButton_62_clicked() ///< Calibration. Save calibration
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save calibration");
    if (str != "") {
        emit US_save_calibration(str);
    }
}

void widepix_analyser_2::on_pushButton_61_clicked() ///< Calibration. Load calibration
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Load calibration");
    if (str != "") {
        emit US_load_calibration(str);
    }
}

void widepix_analyser_2::on_pushButton_63_clicked() ///< Identification. show frame
{
    wai->U_generate_id_frame(ui->comboBox_24->currentIndex());
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

void widepix_analyser_2::on_pushButton_67_clicked() ///< Source. Change scan settings
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

void widepix_analyser_2::on_listView_clicked(const QModelIndex &index) ///< Source. List widget
{
    wai->U_change_scan(ui->listView->currentIndex().row());
}

void widepix_analyser_2::on_spinBox_3_valueChanged(int arg1) ///< Identification. x rebin
{
    wai->U_set_rebin(arg1, ui->spinBox_8->value(), ui->spinBox_10->value());
}

void widepix_analyser_2::on_spinBox_8_valueChanged(int arg1) ///< Identification. y rebin
{
    wai->U_set_rebin(ui->spinBox_3->value(), arg1, ui->spinBox_10->value());
}

void widepix_analyser_2::on_spinBox_10_valueChanged(int arg1) ///< Identification. thl rebin
{
    wai->U_set_rebin(ui->spinBox_3->value(), ui->spinBox_8->value(), arg1);
}

void widepix_analyser_2::on_pushButton_68_clicked() ///< Identification. generate id
{
    wai->U_generate_id_data();
}

void widepix_analyser_2::on_spinBox_12_valueChanged(int arg1) ///< Smoothing
{
    wai->U_set_smoothing(arg1);
}

void widepix_analyser_2::on_checkBox_17_toggled(bool checked) ///< renew roi ranges
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

void widepix_analyser_2::on_pushButton_2_clicked()
{
    QString str = QFileDialog::getSaveFileName(this, "Save RoIs");
    if (str != "") {
        wai->U_save_roi(str);
    }
}

void widepix_analyser_2::on_pushButton_14_clicked()
{
    QString str = QFileDialog::getOpenFileName(this, "Load RoIs");
    if (str != "") {
        wai->U_load_roi(str);
    }
}

void widepix_analyser_2::on_pushButton_30_clicked()
{
    QString str = QFileDialog::getSaveFileName(this, "Save mask");
    if (str != "") {
        wai->U_save_mask(str);
    }
}

void widepix_analyser_2::on_pushButton_69_clicked()
{
    QString str = QFileDialog::getOpenFileName(this, "Load mask");
    if (str != "") {
        wai->U_load_mask(str);
    }
}
