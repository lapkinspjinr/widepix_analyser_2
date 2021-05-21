#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <QVector>

#include <TGraph.h>
#include <TH1D.h>
#include <TF1.h>

QVector<double> U_smooth(QVector<double> x, QVector<double> y);
QVector<double> U_diff(QVector<double> x, QVector<double> y);
QVector<double> U_vector_minus(QVector<double> x, QVector<double> y);


#endif // COMMON_FUNCTIONS_H
