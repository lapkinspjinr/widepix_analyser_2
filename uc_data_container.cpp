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
    data_additional_using = false;
}

UC_data_container::~UC_data_container()
{
    if (settings.both_counters) {
        data_counter_0->~QHash();
        data_counter_1->~QHash();
    } else {
        data_counter_1->~QHash();
    }
    if (data_additional_using) {
        data_additional->~QHash();
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
    this->settings.sample_of_element    = settings.sample_of_element;
    this->settings.element              = settings.element;
    this->settings.thl_sample           = settings.thl_sample;
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

int UC_data_container::U_get_element() {
    return settings.element;
}

bool UC_data_container::U_get_sample_of_element() {
    return settings.sample_of_element;
}

int UC_data_container::U_get_thl_sample() {
    return settings.thl_sample;
}
//
void UC_data_container::U_create_data_additional() {
    if (data_additional_using) return;
    data_additional = new QHash<int, UTStr_frame_double>();
    for (int thl = settings.thl_min; thl <= settings.thl_max; thl++) {
        UTStr_frame_double * frame = new UTStr_frame_double;
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < (15 * 256); x++) {
                frame->pixel_double[x][y] = 0;
            }
        }
        (*data_additional).insert(thl, *frame);
    }
    data_additional_using = true;
}

void UC_data_container::U_set_data_additional(int thl, int x, int y, double data) {
    (*data_additional)[thl].pixel_double[x][y] = data;
}

double UC_data_container::U_get_data_additional(int thl, int x, int y) {
    if (!(data_additional->contains(thl))) return 0;
    return (*data_additional)[thl].pixel_double[x][y];
}

void UC_data_container::U_clean_id_data(int x_min, int x_max, int y_min, int y_max) {
    for (int x = x_min; x < x_max; x++) {
        for (int y = y_min; y < y_max; y++) {
            id_data[x][y].clear();
        }
    }
}

void UC_data_container::U_set_id_data(int x, int y, double value) {
    id_data[x][y] << value;
}

double UC_data_container::U_get_id_data(int x, int y, int element_index) {
    if (id_data[x][y].isEmpty()) return 0;
    return id_data[x][y][element_index];
}
