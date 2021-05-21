#include "common_functions.h"

QVector<double> U_smooth(QVector<double> x, QVector<double> y) {
    QVector<double> y_out;
    QVector<double> x_ext, y_ext;
    for (int i = 0; i < 5; i++) {
        x_ext << x[0] - 5 + i;
        y_ext << y[0];
    }
    x_ext << x;
    y_ext << y;
    for (int i = 1; i < 6; i++) {
        x_ext << x[x.size() - 1] + i;
        y_ext << y[x.size() - 1];
    }
    //TH1D * h = new TH1D("h", "h", x.size(), x[0], x[x.size() - 1]);
    TGraph gr(x_ext.size(), x_ext.data(), y_ext.data());
//    for (int i = 0; i < x_ext.size(); i++) {
//        h->Fill(x_ext[i], y[i]);
//    }
    TF1 fit("fit", "gaus");
    for (int i = 0; i < x.size(); i++) {
        //h->Fit(&fit, "QLN", "", x_ext[i - 5], x_ext[i + 5]);
        gr.Fit(&fit, "MQ", "", x_ext[i], x_ext[i + 10]);
        y_out << fit.Eval(x_ext[i + 5]);
    }
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

QVector<double> U_vector_minus(QVector<double> x, QVector<double> y) {
    QVector<double> y_out;
    for (int i = 0; i < x.size(); i++) {
        y_out << - y[i];
    }
    return y_out;
}
