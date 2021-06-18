#ifndef UC_DATA_CONTAINER_H
#define UC_DATA_CONTAINER_H

#include <QObject>
#include <QHash>
#include <QVector>

class UC_data_container
{
public :
    typedef struct {
        QString path;
        QString name;
        int n_thl;
        int thl_max;
        int thl_min;
        int count;
        double time;
        bool both_counters;
        bool calibration;
        double energy;
        bool sample_of_element;
        int element;
        int thl_sample;
        int ff_int;
        int df_int;
    } UTStr_data_container_settings;



private :
    typedef struct {
        int pixel[15 * 256][256];
    } UTStr_frame;

    typedef struct {
        double pixel_double[15 * 256][256];
    } UTStr_frame_double;

    UTStr_data_container_settings settings;

    bool data_additional_using;

    double data_cnt0_mean[15 * 256][256];      ///< Массив средних значений нулевого счетчика для сканирования с образцом.

    QHash<int, UTStr_frame> * data_counter_0;
    QHash<int, UTStr_frame> * data_counter_1;
    QHash<int, UTStr_frame_double> * data_additional;

    QVector<double> id_data[256 * 15][256];

public:
    UC_data_container(UTStr_data_container_settings settings);
    ~UC_data_container();
    /////
    void U_set_data(int thl, int x, int y, int data, int counter);
    int U_get_data(int thl, int x, int y, int counter = 1);
    double U_get_data_scaled(int thl, int x, int y, int counter, double scale = 1);
    /////
    void U_calculate_mean();
    double U_get_mean_cnt0(int x, int y);
    double U_get_corrected_cnt1(int thl, int x, int y);
    double U_get_corrected_cnt1_scaled(int thl, int x, int y, double scale = 1);
    //
    UTStr_data_container_settings U_get_settings();
    void U_set_settings(UTStr_data_container_settings settings);
    QString U_get_name();
    int U_get_n();
    int U_get_thl_max();
    int U_get_thl_min();
    int U_get_count();
    bool U_get_calibration();
    double U_get_energy();
    int U_get_element();
    bool U_get_sample_of_element();
    int U_get_thl_sample();
    //
    void U_create_data_additional();
    void U_set_data_additional(int thl, int x, int y, double data);
    double U_get_data_additional(int thl, int x, int y);
    //
    void U_clean_id_data(int x_min, int x_max, int y_min, int y_max);
    void U_set_id_data(int x, int y, double value);
    double U_get_id_data(int x, int y, int element_index);
};

#endif // UC_DATA_CONTAINER_H
