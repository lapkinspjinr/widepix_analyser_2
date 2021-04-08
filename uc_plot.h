/*!
    \file
    \brief Заголовочный файл с классами, обеспечивающими обработку данных в отдельном потоке

    Данный файл содержаит описание двух классов:
    Класс UC_pixels_info - класс - контейнер для расчета статистических данных разных областей детектора.
    Класс UC_plot - используется для непосредственного хранения и обработки данных.
*/

#ifndef UC_PLOT_H
#define UC_PLOT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QFile>
#include <QDir>
#include <QVector>
#include <QTextStream>
#include <QtMath>

#include <vector>
#include <algorithm>


/*!
 * \brief Класс-контейнер для статистических данных
 *
 * Данный класс хранит и рассчитывает статистические данные определенных областей детектора.
 * Он используется в классе UC_plot и передается им в сигнале как контейнер внешнему классу для вывода.
 */
class UC_pixels_info {

public :

    /// Типы областей детектора
    typedef enum {
        UTE_PA_chip1,                   ///< Первый чип
        UTE_PA_chip2,                   ///< Второй чип
        UTE_PA_chip3,                   ///< Третий чип
        UTE_PA_chip4,                   ///< Четвертый чип
        UTE_PA_chip5,                   ///< Пятый чип
        UTE_PA_chip6,                   ///< Шестой чип
        UTE_PA_chip7,                   ///< Седьмой чип
        UTE_PA_chip8,                   ///< Восьмой чип
        UTE_PA_chip9,                   ///< Девятый чип
        UTE_PA_chip10,                  ///< Десятый чип
        UTE_PA_chip11,                  ///< Одиннадцатый чип
        UTE_PA_chip12,                  ///< Двенадцатый чип
        UTE_PA_chip13,                  ///< Тринадцатый чип
        UTE_PA_chip14,                  ///< Четырнадцатый чип
        UTE_PA_chip15,                  ///< Пятнадцатый чип
        UTE_PA_widepix,                 ///< Детектор целиком
        UTE_PA_roi,                     ///< Область интереса
        UTE_PA_masked,                  ///< Маскированные пиксели
        UTE_PA_unmasked,                ///< Не маскированные пиксели
        UTE_PA_not_overflow,            ///< Не переполненные пиксели
        UTE_PA_roi_unmasked_not_ovf,    ///< Не переполненные и немаскированные пиксели в области интереса
    } UTE_pixels_area;
private :
    UTE_pixels_area pixels_area;    ///< Положение пикселей, о которых храняться данные
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
    std::vector<double> data;       ///< Вспомогательный вектор для расчета медианы
public :
    /*!
     * Конструктор. Создает экземпляр класа и заполняет все поля нулями.
     * \brief UC_pixels_info Конструктор. Создает экземпляр класа и заполняет все поля нулями.
     * \param[in] pixels_area Область пикселей, о которых будут храниться данные.
     */
    UC_pixels_info(UTE_pixels_area pixels_area);
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
    void U_add_pixel_1(double z);
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
    void U_finalize_1();

    /*!
     * Функции для второго цикла добавления данных.
     */

    /*!
     * Добавляет значение пикселя на втором цикле. Задавать значения нудно два раза в виде двух циклов.
     * На первом цикле вычисляются среднее значение. На втором цикле вычисляются стандартное отклонение и отношение сигнал - шум,
     * так как они зависят от среднего значения и не могут вычисляться одновременно с ним.
     * \brief U_add_pixel_2 Добавить данные на втором цикле.
     * \param[in] z значение пикселя
     */
    void U_add_pixel_2(double z);
    /*!
     * Окончание второго цикла добавления данных.
     * Расчет параметров после второго цикла.
     * Требуется для расчета стандартного отклонения, отношения сигнал - шум.
     * \brief U_finalize_1 Окончание второго цикла добавления данных.
     */
    void U_finalize_2();

    /*!
     * Функции для доступа к данным
     */

    /*!
     * Возвращает область пикселей.
     * \brief U_get_pixel_area Получить область пикселей.
     * \return Область пикселей, о которых хранятся данные.
     */
    UTE_pixels_area U_get_pixel_area();
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
};

/*!
 * \brief Класс, который хранит данные и выполняет расчеты.
 *
 * Основной клас который загружает и хранит данные и проводит с ними все манипуляции.
 * Должен работать в отдельтном потоке.
 */
class UC_plot : public QObject
{
    bool both_counters;         ///< Если в загружаемых данных 2 счетчика, то истина, иначе ложь.
    int count;                  ///< Число кадров при одном пороге в сканировании с образцом.
    int count_df;               ///< Число кадров при одном пороге в сканировании с выключенным источником.
    int count_ff;               ///< Число кадров при одном пороге в сканировании без образца.
    int n_thl;                  ///< Число различных порогов в сканировании.
    int x_min;                  ///< Левая граница области интереса.
    int x_max;                  ///< Правая граница области интереса.
    int y_min;                  ///< Нижняя граница области интереса.
    int y_max;                  ///< Верхняя граница области интереса.
    double time;                ///< Время экспозиции одного кадра в сканировании с образцом.
    double time_df;             ///< Время экспозиции одного кадра в сканировании с выключенным источником.
    double time_ff;             ///< Время экспозиции одного кадра в сканировании без образца.
    double scale_df;            ///< Масштабный множитель для сканирования с выключенным источником.
    double scale_ff;            ///< Масштабный множитель для сканирования без образца.

    int thl_id_1, thl_id_2, thl_id_3, thl_id_4;
    int thl_index_min, thl_index_max;

    /// Массивы вспомогательных данных.
    bool mask[256 * 256 * 15];                  ///< Массив масок. По одно значению на каждый пиксель
    double data_cnt0_mean[256 * 256 * 15];      ///< Массив средних значений нулевого счетчика для сканирования с образцом.
    double data_df_cnt0_mean[256 * 256 * 15];   ///< Массив средних значений нулевого счетчика для сканирования с выключенным источником.
    double data_ff_cnt0_mean[256 * 256 * 15];   ///< Массив средних значений нулевого счетчика для сканирования без образца.
    double * ff_minus_df_mean;                  ///< Динамический массив средних значений обоих счетчиков для сканирования без образца.

    /// Динамические массивы основных данных
    int * data;                 ///< Динамический массив значений обоих счетчиков для сканирования c образцом.
    int * data_ff;              ///< Динамический массив значений обоих счетчиков для сканирования без образца.
    int * data_df;              ///< Динамический массив значений обоих счетчиков для сканирования с выключенным источником.
    int * data_x;               ///< Динамический массив значений порогов.

public :
    ///Методы расчета кадра.
    typedef enum {
        UTE_FT_average,                     ///< Среднее значение кадра.
        UTE_FT_sum,                         ///< Сумма значений в кадре.
        UTE_FT_median,                      ///< Медианное значение в кадре.
        UTE_FT_max,                         ///< Максимальное значение в кадре.
        UTE_FT_min,                         ///< Минимальное значние в кадре.
        UTE_FT_zeros,                       ///< Число нулевых значений в кадре.
        UTE_FT_overflowed,                  ///< Число переполнений в кадре.
        UTE_FT_standart_deviation,          ///< Стандартное отклонение значений в кадре.
        UTE_FT_signal_to_noise_resolution,  ///< Отношение сигнал - шум для значений в кадре.
    } UTE_frame_type;

    typedef enum {
        UTE_IT_no_identification,
        UTE_IT_1,
    } UTE_identification_type;

    ///Методы расчета пикселя.
    typedef enum {
        UTE_PT_cnt0,                        ///< Значение нулевого счетчика при сканировании с образцом.
        UTE_PT_cnt1,                        ///< Значение первого счетчика при сканировании с образцом.
        UTE_PT_cnt0ff,                      ///< Значение нулевого счетчика при сканировании без образца.
        UTE_PT_cnt1ff,                      ///< Значение первого счетчика при сканировании без образца.
        UTE_PT_cnt0df,                      ///< Значение нулевого счетчика при сканировании с выключенным источником.
        UTE_PT_cnt1df,                      ///< Значение первого счетчика при сканировании с выключенным источником.
        UTE_PT_cnt1_divide_on_cnt0,         ///< Отношение значений первого счетчика ко значению нулевого счетчика, взятых при сканировании с образцом.
        UTE_PT_cnt0_subtr_cnt1,             ///< Разность значений нулевого и первого счетчиков, взятых при сканировании с образцом.
        UTE_PT_diff_cnt1,                   ///< Приращение значения первого счетчика при понижении порога в сканировании с образцом.
        UTE_PT_diff_cnt1ff,                 ///< Приращение значения первого счетчика при понижении порога в сканировании без образца.
        UTE_PT_diff_cnt1df,                 ///< Приращение значения первого счетчика при понижении порога в сканировании с выключенным источником.
        UTE_PT_ffc,                         ///< Коррекция плоского поля. Значения берутся из сырых данных.
        UTE_PT_mu,                          ///< Коэффициент поглощения. Значения берутся из сырых данных.
        UTE_PT_diff_ffc,                    ///< Коррекция плоского поля. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_diff_mu,                     ///< Коэффициент поглощения. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_ffc_diff,                    ///< Коррекция плоского поля. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_mu_diff,                     ///< Коэффициент поглощения. Значения берутся равными соответствующим приращениям при понижении порога.

        UTE_PT_cnt1_corr_cnt0,              ///< Значение первого счетчика, скореектированного по среднему значению нулевого счетчика, при сканировании с образцом.
        UTE_PT_cnt1ff_corr_cnt0,            ///< Значение первого счетчика, скореектированного по среднему значению нулевого счетчика, при сканировании без образца.
        UTE_PT_cnt1df_corr_cnt0,            ///< Значение первого счетчика, скореектированного по среднему значению нулевого счетчика, при сканировании с выключенным источником.
        UTE_PT_diff_cnt1_corr_cnt0,         ///< Приращение значения первого счетчика, скореектированного по среднему значению нулевого счетчика, при понижении порога в сканировании с образцом.
        UTE_PT_diff_cnt1ff_corr_cnt0,       ///< Приращение значения первого счетчика, скореектированного по среднему значению нулевого счетчика, при понижении порога в сканировании без образца.
        UTE_PT_diff_cnt1df_corr_cnt0,       ///< Приращение значения первого счетчика, скореектированного по среднему значению нулевого счетчика, при понижении порога в сканировании с выключенным источником.
        UTE_PT_ffc_corr_cnt0,               ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся из сырых данных.
        UTE_PT_mu_corr_cnt0,                ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся из сырых данных.
        UTE_PT_diff_ffc_corr_cnt0,          ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_diff_mu_corr_cnt0,           ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_ffc_diff_corr_cnt0,          ///< Коррекция плоского поля. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_mu_diff_corr_cnt0,           ///< Коэффициент поглощения. Значения, скореектированные по среднему значению нулевого счетчика, берутся равными соответствующим приращениям при понижении порога.

        UTE_PT_mu_max,                      ///< Детектирование максимума коэффициента поглощения. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_ffc_min,                     ///< Детектирование минимума коррекции плоского поля. Значения берутся равными соответствующим приращениям при понижении порога.
        UTE_PT_noise_edge,                  ///< Детектирование края шума.
        UTE_PT_ffc_non_xray,                ///< Коррекция плоского поля для нерентгеновских изображений.
        UTE_PT_cnt0_deviation,              ///< Коррекция плоского поля для нерентгеновских изображений.
        UTE_PT_cnt0ff_deviation,
        UTE_PT_cnt0df_deviation,
    } UTE_pixel_type;


private :

    UTE_frame_type frame_type;  ///< Текущий метод расчета кадра.
    UTE_pixel_type pixel_type;  ///< Текущий метод расчета пикселей.
    UTE_identification_type identification_type;


    Q_OBJECT
public:

    /*!
     * Конструктор. Создает экземпляр класса и инициализирует его переменные. Подготаливает экземляр класса к использованию.
     * \brief UC_plot Конструктор. Создает экземпляр класса и инициализирует его переменные.
     * \param[in] parent Родительский объект.
     */
    explicit UC_plot(QObject *parent = nullptr);
    /*!
     * Деструктор. Уничтожает экземпляр класса. Освобождает память динамических массивов.
     * \brief ~UC_plot Деструктор. Уничтожает экземпляр класса.
     */
    ~UC_plot();

    /// Функции преобразования координат.

    /*!
     * Преобразование координаты x в определенном чипе и номера чипа в координату детектора.
     * \brief U_get_coord_x_chip Преобразование координаты x в определенном чипе и номера чипа в координату детектора.
     * \param[in] chip Номер чипа.
     * \param[in] x Координта х в чипе.
     * \return Координата х в детекторе.
     */
    int U_get_coord_x_chip(int chip, int x);
    /*!
     * Преобразование координаты x в номер соответствующего чипа.
     * \brief U_get_chip_coord_x Преобразование координаты x в номер соответствующего чипа.
     * \param[in] x Координата х в детекторе.
     * \return Номер чипа.
     */
    int U_get_chip_coord_x(int x);
    /*!
     * Преобразование координаты x в детекторе в координату х в чипе.
     * \brief U_get_coord_x_in_chip Преобразование координаты x в детекторе в координату х в чипе.
     * \param[in] x Координата х в детекторе.
     * \return Координта х в чипе.
     */
    int U_get_coord_x_in_chip(int x);

    /// Функции загрузки и получения данных в динамические массивы.

    /*!
     * Функция возвращает сырые данные сканирования с образцом из массива.
     * \brief U_get_data Функция возвращает сырые данные сканирования с образцом из массива.
     * \param[in] thl_index Номер порога.
     * \param[in] x Координата х детектора.
     * \param[in] y Координата y детектора.
     * \param[in] th Номер счетчика. Может принимать значение 0 или 1.
     * \return Значение счетчика th пикселя с координатами (x, y) при пороге с номером thl_index.
     */
    int U_get_data(int thl_index, int x, int y, int th);
    /*!
     * Функция записывает данные сканирования с образцом в динамический массв для хранения.
     * \brief U_set_data Записать данные сканирования с образцом в динамический массв для хранения.
     * \param[in] thl_index Номер порога.
     * \param[in] x Координата х детектора.
     * \param[in] y Координата y детектора.
     * \param[in] th Номер счетчика. Может принимать значение 0 или 1.
     * \param[in] cnt_data Значение счетчика.
     */
    void U_set_data(int thl_index, int x, int y, int th, int cnt_data);
    /*!
     * Функция возвращает значение порога в единицах ЦАПа детектора по номеру порога.
     * \brief U_get_data_x Взвращает значение порога по его номеру.
     * \param[in] thl_index Номер пороа.
     * \return Значение порога в единицах ЦАПа детектора.
     */
    int U_get_data_x(int thl_index);
    /*!
     * Функция записывает значение порога в массив.
     * \brief U_set_data_x Запись значения порога в массив.
     * \param[in] thl_index Номер порога.
     * \param[in] thl Значение порога в единицах ЦАПа детектора.
     */
    void U_set_data_x(int thl_index, int thl);
    /*!
     * Функция возвращает сырые данные сканирования без образца из массива.
     * \brief U_get_data Функция возвращает сырые данные сканирования без образца из массива.
     * \param[in] thl_index Номер порога.
     * \param[in] x Координата х детектора.
     * \param[in] y Координата y детектора.
     * \param[in] th Номер счетчика. Может принимать значение 0 или 1.
     * \return Значение счетчика th пикселя с координатами (x, y) при пороге с номером thl_index.
     */
    int U_get_data_ff(int thl_index, int x, int y, int th);
    /*!
     * Функция записывает данные сканирования без образца в динамический массв для хранения.
     * \brief U_set_data Записать данные сканирования без образца в динамический массв для хранения.
     * \param[in] thl_index Номер порога.
     * \param[in] x Координата х детектора.
     * \param[in] y Координата y детектора.
     * \param[in] th Номер счетчика. Может принимать значение 0 или 1.
     * \param[in] cnt_data Значение счетчика.
     */
    void U_set_data_ff(int thl_index, int x, int y, int th, int cnt_data);
    /*!
     * Функция возвращает сырые данные сканирования с выключенным источником из массива.
     * \brief U_get_data Функция возвращает сырые данные сканирования с выключенным источником из массива.
     * \param[in] thl_index Номер порога.
     * \param[in] x Координата х детектора.
     * \param[in] y Координата y детектора.
     * \param[in] th Номер счетчика. Может принимать значение 0 или 1.
     * \return Значение счетчика th пикселя с координатами (x, y) при пороге с номером thl_index.
     */
    int U_get_data_df(int thl_index, int x, int y, int th);
    /*!
     * Функция записывает данные сканирования с выключенным источником в динамический массв для хранения.
     * \brief U_set_data Записать данные сканирования с выключенным источником в динамический массв для хранения.
     * \param[in] thl_index Номер порога.
     * \param[in] x Координата х детектора.
     * \param[in] y Координата y детектора.
     * \param[in] th Номер счетчика. Может принимать значение 0 или 1.
     * \param[in] cnt_data Значение счетчика.
     */
    void U_set_data_df(int thl_index, int x, int y, int th, int cnt_data);

    ///Функции выделения памяти для динамических массивов

    /*!
     * Функция выделяет память для размещения сырых данных сканирования с образцом.
     * \brief U_data_mem_alloc Выделнения памяти для сырых данных сканирования с образцом.
     * \param[in] thl_counts Число порогов.
     */
    void U_data_mem_alloc(int thl_counts);
    /*!
     * Функция выделяет память для размещения занчений порогов.
     * \brief U_data_mem_alloc Выделнения памяти для значений порогов.
     * \param[in] thl_counts Число порогов.
     */
    void U_data_x_mem_alloc(int thl_counts);
    /*!
     * Функция выделяет память для размещения сырых данных сканирования с выключенным источником.
     * \brief U_data_mem_alloc Выделнения памяти для сырых данных сканирования с выключенным источником.
     * \param[in] thl_counts Число порогов.
     */
    void U_data_df_mem_alloc(int thl_counts);
    /*!
     * Функция выделяет память для размещения сырых данных сканирования без образца.
     * \brief U_data_mem_alloc Выделнения памяти для сырых данных сканирования без образца.
     * \param[in] thl_counts Число порогов.
     */
    void U_data_ff_mem_alloc(int thl_counts);

    /// Функции вычисления средних значений нулевых счетчиков.

    /*!
     * Функция добавляет в массив значение
     * \brief U_add_data_cnt0_mean
     * \param[in] x
     * \param[in] y
     * \param[in] cnt_data
     */
    void U_add_data_cnt0_mean(int x, int y, int cnt_data);
    void U_divide_data_cnt0_mean();
    double U_get_data_cnt0_mean(int x, int y);
    ///
    void U_add_data_df_cnt0_mean(int x, int y, int cnt_data);
    void U_divide_data_df_cnt0_mean();
    double U_get_data_df_cnt0_mean(int x, int y);
    ///
    void U_add_data_ff_cnt0_mean(int x, int y, int cnt_data);
    void U_divide_data_ff_cnt0_mean();
    double U_get_data_ff_cnt0_mean(int x, int y);
//////////////////////////////////////////////////////////////////////////////////
    double U_get_data_corrected(int thl_index, int x, int y);
    double U_get_data_ff_corrected(int thl_index, int x, int y);
    double U_get_data_df_corrected(int thl_index, int x, int y);
//////////////////////////////////////////////////////////////////////////
    void U_alloc_ff_minus_df_mean();
    void U_make_ff_minus_df_mean();
    double U_get_ff_minus_df_mean(int thl_index, int x);
/////////////////////////////////////////////////////////////////////////////////
    void U_set_mask(int x, int y, bool value);
    bool U_get_mask(int x, int y);
///////////////////////////////////////////////////////////////////////////////
    double U_get_pixel_data(UTE_pixel_type type, int thl_index, int x, int y);
    int U_get_pixel_data_1(int thl_index, int x, int y); //UTE_PT_cnt0
    int U_get_pixel_data_2(int thl_index, int x, int y); //UTE_PT_cnt1
    int U_get_pixel_data_3(int thl_index, int x, int y); //UTE_PT_cnt0ff
    int U_get_pixel_data_4(int thl_index, int x, int y); //UTE_PT_cnt1ff
    int U_get_pixel_data_5(int thl_index, int x, int y); //UTE_PT_cnt0df
    int U_get_pixel_data_6(int thl_index, int x, int y); //UTE_PT_cnt1df
    double U_get_pixel_data_7(int thl_index, int x, int y); //UTE_PT_cnt1_divide_on_cnt0
    int U_get_pixel_data_8(int thl_index, int x, int y); //UTE_PT_cnt0_subtr_cnt1
    double U_get_pixel_data_9(int thl_index, int x, int y); //UTE_PT_diff_cnt1
    double U_get_pixel_data_10(int thl_index, int x, int y); //UTE_PT_diff_cnt1ff
    double U_get_pixel_data_11(int thl_index, int x, int y); //UTE_PT_diff_cnt1df
    double U_get_pixel_data_12(int thl_index, int x, int y); //UTE_PT_ffc
    double U_get_pixel_data_13(int thl_index, int x, int y); //UTE_PT_mu
    double U_get_pixel_data_14(int thl_index, int x, int y); //UTE_PT_diff_ffc
    double U_get_pixel_data_15(int thl_index, int x, int y); //UTE_PT_diff_mu
    double U_get_pixel_data_16(int thl_index, int x, int y); //UTE_PT_ffc_diff
    double U_get_pixel_data_17(int thl_index, int x, int y); //UTE_PT_mu_diff
    ///
    double U_get_pixel_data_18(int thl_index, int x, int y); //UTE_PT_cnt1_corr_cnt0
    double U_get_pixel_data_19(int thl_index, int x, int y); //UTE_PT_cnt1ff_corr_cnt0
    double U_get_pixel_data_20(int thl_index, int x, int y); //UTE_PT_cnt1df_corr_cnt0
    double U_get_pixel_data_21(int thl_index, int x, int y); //UTE_PT_diff_cnt1_corr_cnt0
    double U_get_pixel_data_22(int thl_index, int x, int y); //UTE_PT_diff_cnt1ff_corr_cnt0
    double U_get_pixel_data_23(int thl_index, int x, int y); //UTE_PT_diff_cnt1df_corr_cnt0
    double U_get_pixel_data_24(int thl_index, int x, int y); //UTE_PT_ffc_corr_cnt0
    double U_get_pixel_data_25(int thl_index, int x, int y); //UTE_PT_mu_corr_cnt0
    double U_get_pixel_data_26(int thl_index, int x, int y); //UTE_PT_diff_ffc_corr_cnt0
    double U_get_pixel_data_27(int thl_index, int x, int y); //UTE_PT_diff_mu_corr_cnt0
    double U_get_pixel_data_28(int thl_index, int x, int y); //UTE_PT_ffc_diff_corr_cnt0
    double U_get_pixel_data_29(int thl_index, int x, int y); //UTE_PT_mu_diff_corr_cnt0
    ///
    int U_get_pixel_data_30(int thl_index, int x, int y); //UTE_PT_mu_max
    int U_get_pixel_data_31(int thl_index, int x, int y); //UTE_PT_ffc_min
    int U_get_pixel_data_32(int thl_index, int x, int y); //UTE_PT_noise_edge
    double U_get_pixel_data_33(int thl_index, int x, int y); //UTE_PT_ffc_non_xray
    double U_get_pixel_data_34(int thl_index, int x, int y); //UTE_PT_cnt0_deviation
    double U_get_pixel_data_35(int thl_index, int x, int y); //UTE_PT_cnt0ff_deviation
    double U_get_pixel_data_36(int thl_index, int x, int y); //UTE_PT_cnt0df_deviation


////////////////////////////////////////////////////////////////////////////////
    double U_get_frame_data(UTE_frame_type type_spectra, UTE_pixel_type type_pixel, int thl_index);
    double U_get_frame_data_1(UTE_pixel_type type, int thl_index); //UTE_FT_average
    double U_get_frame_data_2(UTE_pixel_type type, int thl_index); //UTE_FT_sum
    double U_get_frame_data_3(UTE_pixel_type type, int thl_index); //UTE_FT_median
    double U_get_frame_data_4(UTE_pixel_type type, int thl_index); //UTE_FT_max
    double U_get_frame_data_5(UTE_pixel_type type, int thl_index); //UTE_FT_min
    int U_get_frame_data_6(UTE_pixel_type type, int thl_index); //UTE_FT_zeros
    int U_get_frame_data_7(int thl_index); //UTE_FT_overflowed
    double U_get_frame_data_8(UTE_pixel_type type, int thl_index); //UTE_FT_standart_deviation
    double U_get_frame_data_9(UTE_pixel_type type, int thl_index); //UTE_FT_signal_to_noise_resolution
////////////////////////////////////////////////////////////////////////////////////
    double U_get_identification_data(UTE_identification_type type, int thl_index, int x, int y);
    double U_get_identification_data_1(int x, int y); //UTE_PT_cnt0
    int U_get_identification_data_2(int x, int y); //UTE_PT_cnt1
///////////////////////////////////////////////////////////////////////////////////
    void U_renew_scale();

signals:
    void US_plot_data(double x, double y);
    void US_frame_data(double x, double y, double z);
    void US_chip_data(UC_pixels_info pixels_info);
    void US_distribution_data(double x, double y);
    void US_range_data(double lower, double upper);
    ///////////////////////////////////////////////////////////////////////////////////

    void US_replot_spectra();
    void US_replot_frame();
    void US_rewrite_table();
    void US_replot_distribution();
    /////////////////////////////////////////////////////////////////////////////
    void US_new_thl(int thl);
    void US_count_mask(int n);
    void US_thl_min(int thl);
    void US_thl_max(int thl);
    ////////////////////////////////////////////////////////////////////////
    void US_file_found(QString file_name);
    void US_n_files(int n);
    void US_renew_progress_bar(double current, double total);

public slots:
    void U_set_data(QString path, int count, double time, bool both_counters);
    void U_set_data_ff(QString path, int count, double time, bool both_counters);
    void U_set_data_df(QString path, int count, double time, bool both_counters);
    void U_reset_data();
//////////////////////////////////////////////////////////////////////////////////
    void U_generate_spectra();
    void U_generate_frame(int thl_index);
    void U_generate_table(int thl_index);
    void U_generate_table();
    void U_generate_distribution(int n_bins, double min, double max);
    void U_generate_frame_distribution(int n_bins, double min, double max, int thl_index);
    void U_generate_range(int thl_index);
    void U_generate_range();
/////////////////////////////////////////////////////////////////
    void U_set_frame_type(UC_plot::UTE_frame_type frame_type);
    void U_set_pixel_type(UC_plot::UTE_pixel_type pixel_type);
    void U_set_roi(int x_min, int x_max, int y_min, int y_max);
    void U_set_identification_type(UC_plot::UTE_identification_type identification_type);
    void U_set_thresholds(int thl_id_1, int thl_id_2, int thl_id_3, int thl_id_4);
    void U_set_threshold_range(int thl_min, int thl_max);
//////////////////////////////////////////////////////////////////////
    void U_set_mask(bool mask, bool more, double value, bool in_roi, int thl_index);
    void U_count_mask(bool more, double value, bool in_roi, int thl_index);
    void U_set_mask_overflowed(bool in_roi, int thl_index);
    void U_set_mask_overflowed(bool in_roi);
    void U_reset_mask();
};




#endif // UC_PLOT_H
