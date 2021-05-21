#ifndef UC_DATA_CONTAINER_H
#define UC_DATA_CONTAINER_H

#include <QObject>
#include <QHash>

class UC_data_container
{
public :
    typedef struct {
        QString path;
        int n_thl;
        int count;
        double time;
        bool both_counters;
        bool calibration;
        double energy;
    } UTStr_data_container_settings;

private :
    typedef struct {
        int pixel[15 * 256][256];
    } UTStr_frame;

    UTStr_data_container_settings settings;

    int thl_min;
    int thl_max;

    double data_cnt0_mean[15 * 256][256];      ///< Массив средних значений нулевого счетчика для сканирования с образцом.

    QHash<int, UTStr_frame> * data_counter_0;
    QHash<int, UTStr_frame> * data_counter_1;

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
    /////
    int U_get_thl_max();
    int U_get_thl_min();
    int U_get_count();
    bool U_get_calibration();
    int U_get_energy();
    int U_get_n();
};

#endif // UC_DATA_CONTAINER_H
