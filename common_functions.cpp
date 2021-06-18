#include "common_functions.h"

int U_get_coord_x_chip(int chip, int x) {
    return chip * 256 + x;
}

int U_get_chip_coord_x(int x) {
    return x / 256;
}

int U_get_coord_x_in_chip(int x) {
    return x % 256;
}

QVector<double> U_smooth(QVector<double> x, QVector<double> y, int n) {
    if (n == 0) return y;
    QVector<double> y_out;
    TH1D * h = new TH1D("h", "h", x.size(), x[0], x[x.size() - 1]);
//    TGraph gr(x_ext.size(), x_ext.data(), y_ext.data());
    for (int i = 0; i < x.size(); i++) {
        h->Fill(x[i], y[i]);
    }
    TH1D * hout = new TH1D(*h);
    hout->Smooth(n);
    for (int i = 1; i <= x.size(); i++) {
        y_out << hout->GetBinContent(i);
    }
    delete h;
    delete hout;
    return y_out;
}

QVector<double> U_diff(QVector<double> x, QVector<double> y) {
    QVector<double> y_out;
    y_out << (y[1] - y[0]);
    for (int i = 1; i < x.size() - 1; i++) {
        y_out << (y[i + 1] - y[i - 1]) / 2;
    }
    y_out << (y[x.size() - 1] - y[x.size() - 2]);
    return y_out;
}

QVector<double> U_vector_minus(QVector<double> y) {
    QVector<double> y_out;
    for (int i = 0; i < y.size(); i++) {
        y_out << - y[i];
    }
    return y_out;
}

QVector<double> U_vector_divide(QVector<double> x, QVector<double> y) {
    QVector<double> y_out;
    int size = y.size();
    if (x.size() < size) size = x.size();
    for (int i = 0; i < size; i++) {
        if (y[i] < 1e-10) y_out << 0;
            else y_out << x[i] / y[i];
    }
    return y_out;
}

QVector<double> U_vector_log(QVector<double> x) {
    QVector<double> y_out;
    int size = x.size();
    for (int i = 0; i < size; i++) {
        if (x[i] < 1e-10) y_out << 0;
            else y_out << qLn(x[i]);
    }
    return y_out;
}
