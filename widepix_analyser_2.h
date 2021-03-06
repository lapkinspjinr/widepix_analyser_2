#ifndef WIDEPIX_ANALYSER_2_H
#define WIDEPIX_ANALYSER_2_H

#include <QMainWindow>

#include "uc_wai.h"

namespace Ui {
class widepix_analyser_2;
}

class widepix_analyser_2 : public QMainWindow
{
    Q_OBJECT

    UC_wai * wai;
    QString roi;
    QList<QWidget * > list_enabling;
    QList<QWidget * > list_enabling_busy;

public:
    explicit widepix_analyser_2(QWidget *parent = nullptr);
    ~widepix_analyser_2();

private slots:
    ///////////////////////////////////
    void U_add_thl(QVector<int> thl_vector);
    void U_set_thl_range(int thl_min, int thl_max);
    void U_set_energy_range(double energy_min, double energy_max);
    //
    void U_file_found(QString file_name);
    void U_n_files(int n);
    void U_renew_progress_bar(double current, double total);
    void U_set_distribution_range(double lower, double upper);
    void U_set_spectra_2D_range(double lower, double upper);
    void U_set_count_mask(int n);
    void U_set_roi_range(int x_min, int x_max, int y_min, int y_max);
    void U_set_scans(QList<UC_data_container> * scans_list_ptr, int active_index);
    void U_change_scan_settings(UC_data_container::UTStr_data_container_settings *settings);
    void U_renew_identification_elements(QList<QString> list);
    void U_new_spectra(QString name);
    void U_add_roi(QString name);
    void U_ready();
    void U_busy();
    //
    //////////////////////////////////////

signals :
    void US_set_threshold_level(double level);
    void US_save_calibration(QString file_name);
    void US_load_calibration(QString file_name);

    void US_set_data(UC_data_container::UTStr_data_container_settings * setting_ptr);
    void US_set_scan(int index);
    void US_change_scan_settings(int index, UC_data_container::UTStr_data_container_settings * setting_ptr);
    void US_delete_scan(int index);
    void US_reset_data();
    void US_get_scan_settings(int index);
    void US_scan_changed();
    //
    void US_stop();
    //
    void US_set_mask(bool mask, bool more, double value, bool in_roi, int thl);
    void US_set_mask(bool mask, bool more, double value, bool in_roi);
    void US_count_mask(bool more, double value, bool in_roi, int thl);
    void US_count_mask(bool more, double value, bool in_roi);
    void US_set_mask_overflowed(bool in_roi, int thl);
    void US_set_mask_overflowed(bool in_roi);
    void US_set_mask_gauss(bool mask, bool more, double value, bool in_roi, int thl);
    void US_set_mask_gauss(bool mask, bool more, double value, bool in_roi);
    void US_reset_mask();
    void US_save_mask(QString file_name);
    void US_load_mask(QString file_name);
    //
    void US_get_max_range();
    //
    void US_set_threshold_range(int thl_index_min, int thl_index_max);
    void US_set_energy_range(double energy_min, double energy_max);

private slots:

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_36_clicked();

    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_pushButton_33_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_41_clicked();

    void on_comboBox_15_currentIndexChanged(const QString &arg1);

    void on_comboBox_14_currentIndexChanged(const QString &arg1);

    void on_comboBox_13_currentIndexChanged(const QString &arg1);

    void on_comboBox_12_currentIndexChanged(const QString &arg1);

    void on_radioButton_3_clicked(bool checked);

    void on_radioButton_4_clicked(bool checked);

    void on_pushButton_44_clicked();

    void on_radioButton_5_clicked(bool checked);

    void on_radioButton_6_clicked(bool checked);

    void on_radioButton_7_clicked(bool checked);

    void on_radioButton_8_clicked(bool checked);

    void on_pushButton_42_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_56_clicked();

    void on_pushButton_60_clicked();

    void on_checkBox_13_toggled(bool checked);

    void on_pushButton_50_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_62_clicked();

    void on_pushButton_61_clicked();

    void on_pushButton_63_clicked();

    void on_pushButton_65_clicked();

    void on_pushButton_66_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_spinBox_3_valueChanged(int arg1);

    void on_spinBox_8_valueChanged(int arg1);

    void on_spinBox_10_valueChanged(int arg1);

    void on_pushButton_68_clicked();

    void on_spinBox_12_valueChanged(int arg1);

    void on_checkBox_17_toggled(bool checked);

    void on_comboBox_6_activated(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_69_clicked();

    void on_checkBox_18_toggled(bool checked);

    void on_comboBox_5_activated(int index);

    void on_comboBox_17_activated(int index);

    void on_pushButton_70_clicked();

    void on_pushButton_71_clicked();

    void on_pushButton_73_clicked();

    void on_radioButton_9_clicked(bool checked);

    void on_radioButton_10_clicked(bool checked);

    void on_pushButton_72_clicked();

    void on_pushButton_74_clicked();

    void on_pushButton_75_clicked();

    void on_pushButton_76_clicked();

    void on_pushButton_83_clicked();

    void on_pushButton_79_clicked();

    void on_pushButton_81_clicked();

    void on_pushButton_80_clicked();

    void on_pushButton_77_clicked();

    void on_pushButton_82_clicked();

    void on_pushButton_78_clicked();

    void on_pushButton_84_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_24_clicked();

    void on_comboBox_21_currentIndexChanged(int index);

    void on_comboBox_28_currentIndexChanged(int index);

    void on_doubleSpinBox_20_valueChanged(double arg1);

    void on_doubleSpinBox_21_valueChanged(double arg1);

    void on_pushButton_85_clicked();

private:
    Ui::widepix_analyser_2 *ui;
};

#endif // WIDEPIX_ANALYSER_2_H
