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

public:
    explicit widepix_analyser_2(QWidget *parent = nullptr);
    ~widepix_analyser_2();

private slots:
    ///////////////////////////////////
    void U_add_thl(int thl);
    void U_file_found(QString file_name);
    void U_n_files(int n);
    void U_renew_progress_bar(double current, double total);
    void U_set_distribution_range(double lower, double upper);
    void U_set_count_mask(int n);
    //////////////////////////////////////

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_checkBox_3_toggled(bool checked);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_comboBox_5_currentIndexChanged(int index);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_comboBox_6_currentIndexChanged(int index);

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_25_clicked();

    void on_spinBox_6_valueChanged(int arg1);

    void on_spinBox_7_valueChanged(int arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_spinBox_5_valueChanged(int arg1);

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

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

    void on_comboBox_11_currentIndexChanged(int index);

    void on_comboBox_15_currentIndexChanged(int index);

    void on_comboBox_14_currentIndexChanged(int index);

    void on_comboBox_13_currentIndexChanged(int index);

    void on_comboBox_12_currentIndexChanged(int index);

    void on_pushButton_41_clicked();

private:
    Ui::widepix_analyser_2 *ui;
};

#endif // WIDEPIX_ANALYSER_2_H
