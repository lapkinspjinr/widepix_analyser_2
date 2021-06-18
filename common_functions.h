#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <QVector>
#include <QtMath>

#include <TGraph.h>
#include <TH1D.h>
#include <TF1.h>
#include <TGraphSmooth.h>

int U_get_coord_x_chip(int chip, int x);
int U_get_chip_coord_x(int x);
int U_get_coord_x_in_chip(int x);

QVector<double> U_smooth(QVector<double> x, QVector<double> y, int n);
QVector<double> U_diff(QVector<double> x, QVector<double> y);
QVector<double> U_vector_minus(QVector<double> y);
QVector<double> U_vector_divide(QVector<double> x, QVector<double> y);
QVector<double> U_vector_log(QVector<double> x);


#endif // COMMON_FUNCTIONS_H