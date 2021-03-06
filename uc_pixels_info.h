#ifndef UC_PIXELS_INFO_H
#define UC_PIXELS_INFO_H

#include <QtMath>
#include <QVector>
#include <QRect>

#include <TH1D.h>

#include <vector>
#include <algorithm>

class UC_roi {
private :
    QString name;
    int x_min;
    int x_max;
    int y_min;
    int y_max;
public :
    UC_roi(QString name, int x_min, int y_min, int x_max, int y_max);
    UC_roi();
    bool U_contains(int x, int y);
    void U_set_roi(QString name, int x_min, int y_min, int x_max, int y_max);
    QString U_get_name();
    int U_get_x_min();
    int U_get_y_min();
    int U_get_x_max();
    int U_get_y_max();
};


/*!
 * \brief Класс-контейнер для статистических данных
 *
 * Данный класс хранит и рассчитывает статистические данные определенных областей детектора.
 * Он используется в классе UC_plot и передается им в сигнале как контейнер внешнему классу для вывода.
 */
class UC_pixels_info : public UC_roi{
private :
    int n;                          ///< Число пикселей
    double min;                     ///< Минимальное значение
    double max;                     ///< Максимальное значение
    double sum;                     ///< Сумма значений
    double mean;                    ///< Среднее значение
    double median;                  ///< Медианное значение
    int zeros;                      ///< Число нулевых значений в данной области
    int overflows;                  ///< Число переполнений в данной области
    int masked;                     ///< Число маскированных пикселей в данной области
    double std_dev;                 ///< Стандартное отклонение
    double snr;                     ///< Отношение сигнал - шум
    double max_density;
    double variance;
    QVector<double> data;

public :
    /*!
     * Конструктор. Создает экземпляр класа и заполняет все поля нулями.
     * \brief UC_pixels_info Конструктор. Создает экземпляр класа и заполняет все поля нулями.
     * \param[in] pixels_area Область пикселей, о которых будут храниться данные.
     */
    UC_pixels_info(QString name, int x_min, int y_min, int x_max, int y_max);

    UC_pixels_info(UC_roi roi);

    /*!
     * Cброс данных.
     * \brief U_reset Cброс данных.
     */
    void U_reset();

    /*!
     * Функции для первого цикла добавления данных.
     */

    /*!
     * Добавляет значение пикселя на первом цикле. Задавать значения нудно два раза в виде двух циклов.
     * На первом цикле вычисляются среднее значение. На втором цикле вычисляются стандартное отклонение и отношение сигнал - шум,
     * так как они зависят от среднего значения и не могут вычисляться одновременно с ним.
     * \brief U_add_pixel_1 Добавить данные на первом цикле.
     * \param[in] z значение пикселя
     */
    void U_add_pixel(double z, int x, int y, bool mask, bool overflow);
    /*!
     * Добавляет маскированный пиксель.
     * \brief U_add_masked Добавить маскированный пиксель.
     */
    void U_add_masked();
    /*!
     * Добавляет переполненный пиксель.
     * \brief U_add_overflow Добавить переполненный пиксель.
     */
    void U_add_overflow();
    /*!
     * Окончание первого цикла добавления данных.
     * Расчет параметров после первого цикла.
     * Требуется для расчета среднего, медианы.
     * \brief U_finalize_1 Окончание первого цикла добавления данных.
     */
    void U_finalize();

    /*!
     * Функции для доступа к данным
     */

    /*!
     * Возвращает число пикселей в данной области.
     * \brief U_get_n Получить число пикселей в данной области.
     * \return Число пикселей в данной области.
     */
    int U_get_n();
    /*!
     * Возвращает минимальное значение.
     * \brief U_get_min Получить минимальное значение.
     * \return Минимальное значение.
     */
    double U_get_min();
    /*!
     * Возвращает максимальное значение.
     * \brief U_get_max Получить максимальное значение.
     * \return Максимальное значение.
     */
    double U_get_max();
    /*!
     * Возвращает сумму значений пикселей из области.
     * \brief U_get_sum Получить сумму значений пикселей из области.
     * \return Сумма значений.
     */
    double U_get_sum();
    /*!
     * Возвращает среднее значение пикселей в области.
     * \brief U_get_mean Получить среднее значение пикселей в области.
     * \return Среднее значение.
     */
    double U_get_mean();
    /*!
     * Возвращает медианное значение пикселей в области.
     * \brief U_get_median Получить медианное значение пикселей в области.
     * \return Медианное значние.
     */
    double U_get_median();
    /*!
     * Возвращает число нулевых значений пикселей в области.
     * \brief U_get_zeros Получить число нулевых значений пикселей в области.
     * \return Число нулей.
     */
    int U_get_zeros();
    /*!
     * Возвращает число переполненных пикселей в области.
     * \brief U_get_overflows Получить число переполненных пикселей в области.
     * \return Число переполненных пикселей.
     */
    int U_get_overflows();
    /*!
     * Возвращает число замаскированных пикселей в области.
     * \brief U_get_masked Получить число замаскированных пикселей в области.
     * \return Число замаскированных пикселей.
     */
    int U_get_masked();
    /*!
     * Возвращает стандартное отклонение пикселей в области.
     * \brief U_get_std_dev Получить стандартное отклонение пикселей в области.
     * \return Стандартное отклонение.
     */
    double U_get_std_dev();
    /*!
     * Возвращает отношение сигнал - шум в области.
     * \brief U_get_snr Получить отношение сигнал - шум в области.
     * \return Отношение сигнал - шум.
     */
    double U_get_snr();
    double U_get_max_density();
    double U_get_variance();

    //

    QRect U_get_rect();

};


#endif // UC_PIXELS_INFO_H
