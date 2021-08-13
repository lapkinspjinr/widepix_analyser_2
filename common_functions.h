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
double U_diff(double yin2, double yin1, double yi1, double yi2, double xin1, double xi1);
QVector<double> U_vector_minus(QVector<double> y);
void U_vector_divide(QVector<double> &x1, QVector<double> &y1, QVector<double> &x2, QVector<double> &y2);
QVector<double> U_vector_log(QVector<double> x);


#endif // COMMON_FUNCTIONS_H
