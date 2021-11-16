#include "uc_data_container.h"

UC_data_container::UC_data_container(UTStr_data_container_settings settings)
{

    if (settings.both_counters) {
        data_counter_0 = new QHash<int, UTStr_frame>();
        data_counter_1 = new QHash<int, UTStr_frame>();
    } else {
        data_counter_1 = new QHash<int, UTStr_frame>();
    }

    for (int x = 0; x < (15 * 256); x++) {
        for (int y = 0; y < 256; y++) {
            data_cnt0_mean[x][y] = 0;
        }
    }

    this->settings = settings;
}

UC_data_container::~UC_data_container()
{
    if (settings.both_counters) {
        data_counter_0->~QHash();
        data_counter_1->~QHash();
    } else {
        data_counter_1->~QHash();
    }
}
/////

void UC_data_container::U_set_data(int thl, int x, int y, int data, int counter) {
    if (settings.both_counters) {
        if (!(data_counter_1->contains(thl))) {
            UTStr_frame frame;
            for (int y = 0; y < 256; y++) {
                for (int x = 0; x < (15 * 256); x++) {
                    frame.pixel[x][y] = 0;
                }
            }
            (*data_counter_0).insert(thl, frame);
            (*data_counter_1).insert(thl, frame);
        }
        if (counter == 0) {
            (*data_counter_0)[thl].pixel[x][y] = data;
            data_cnt0_mean[x][y] += data;
        } else {
            (*data_counter_1)[thl].pixel[x][y] = data;
        }
    } else {
        if (counter == 0) return;
        if (!(data_counter_1->contains(thl))) {
            UTStr_frame frame;
            for (int y = 0; y < 256; y++) {
                for (int x = 0; x < (15 * 256); x++) {
                    frame.pixel[x][y] = 0;
                }
            }
            (*data_counter_1).insert(thl, frame);
        }
        (*data_counter_1)[thl].pixel[x][y] = data;
    }
}

void UC_data_container::U_set_data(int thl, UTStr_frame &data, int counter) {
    if (settings.both_counters) {
        if (counter == 0) {
            (*data_counter_0).insert(thl, data);
            for (int x = 0; x < (15 * 256); x++) {
                for (int y = 0; y < 256; y++) {
                    data_cnt0_mean[x][y] += data.pixel[x][y];
                }
            }
        } else {
            (*data_counter_1).insert(thl, data);
        }
    } else {
        if (counter == 0) return;
        (*data_counter_1).insert(thl, data);
    }
}

int UC_data_container::U_get_data(int thl, int x, int y, int counter) {
    if (!(data_counter_1->contains(thl))) return 0;
    if (counter == 0) {
        if (!settings.both_counters) return 0;
        return (*data_counter_0)[thl].pixel[x][y];
    } else {
        return (*data_counter_1)[thl].pixel[x][y];
    }
}

double UC_data_container::U_get_data_scaled(int thl, int x, int y, int counter, double scale) {
    if (!(data_counter_1->contains(thl))) return 0;
    double data;
    if (counter == 0) {
        if (!settings.both_counters) return 0;
        data = (*data_counter_0)[thl].pixel[x][y];
    } else {
        data = (*data_counter_1)[thl].pixel[x][y];
    }
    data *= scale / (settings.count * settings.time);
    return data;
}
//////
void UC_data_container::U_calculate_mean() {
    for (int x = 0; x < (15 * 256); x++) {
        for (int y = 0; y < 256; y++) {
            data_cnt0_mean[x][y] /= (settings.n_thl);
        }
    }
}

double UC_data_container::U_get_mean_cnt0(int x, int y) {
    return data_cnt0_mean[x][y];
}

double UC_data_container::U_get_corrected_cnt1(int thl, int x, int y) {
    if (!(data_counter_1->contains(thl))) return 0;
    if (!(settings.both_counters)) return 0;
    int denom = (*data_counter_0)[thl].pixel[x][y];
    if (denom == 0) return 0;
    return data_cnt0_mean[x][y] * (*data_counter_1)[thl].pixel[x][y] / denom;
}

double UC_data_container::U_get_corrected_cnt1_scaled(int thl, int x, int y, double scale) {
    if (!(data_counter_1->contains(thl))) return 0;
    if (!(settings.both_counters)) return 0;
    int denom = (*data_counter_0)[thl].pixel[x][y];
    if (denom == 0) return 0;
    double data = data_cnt0_mean[x][y] * (*data_counter_1)[thl].pixel[x][y] / denom;
    data *= scale / (settings.count * settings.time);
    return data;
}
/////

void UC_data_container::U_set_thl_vector(QVector<int> thl_vector) {
    this->thl_vector << thl_vector;
}

QVector<int> * UC_data_container::U_get_thl_vector() {
    return &thl_vector;
}

//
int UC_data_container::U_get_thl_max() {
    return settings.thl_max;
}

int UC_data_container::U_get_thl_min() {
    return settings.thl_min;
}
//
UC_data_container::UTStr_data_container_settings UC_data_container::U_get_settings() {
    return settings;
}

void UC_data_container::U_set_settings(UTStr_data_container_settings settings) {
    this->settings.name                 = settings.name;
    this->settings.count                = settings.count;
    this->settings.time                 = settings.time;
    this->settings.calibration          = settings.calibration;
    this->settings.energy               = settings.energy;
    this->settings.ff_int               = settings.ff_int;
    this->settings.df_int               = settings.df_int;
}

QString UC_data_container::U_get_name() {
    return settings.name;
}

int UC_data_container::U_get_count() {
    return settings.count;
}

bool UC_data_container::U_get_calibration() {
    return settings.calibration;
}

double UC_data_container::U_get_energy() {
    return settings.energy;
}

int UC_data_container::U_get_n() {
    return settings.n_thl;
}
