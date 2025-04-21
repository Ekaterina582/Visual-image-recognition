#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main() {
    // Путь к видеофайлу
    string videoPath = "C:\\Users\\admin\\Downloads\\video.mp4";

    // Создаем объект для захвата видео
    VideoCapture cap(videoPath);

    // Проверяем, открылось ли видео
    if (!cap.isOpened()) {
        cerr << "Ошибка: не удалось открыть видеофайл " << videoPath << endl;
        return -1;
    }

    // Получаем параметры видео (ширина, высота, FPS)
    int frameWidth = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);

    // Создаем объект для записи результата
    VideoWriter writer("output.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), fps, Size(frameWidth, frameHeight));

    // Основной цикл обработки кадров
    Mat frame, gray, blurred, edges, binary;
    while (cap.read(frame)) {
        // 1. Конвертация в grayscale
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // 2. Размытие для шумоподавления
        GaussianBlur(gray, blurred, Size(5, 5), 1.5);

        // 3. Детекция границ (Canny)
        Canny(blurred, edges, 50, 150);

        // 4. Бинаризация с методом Otsu
        threshold(blurred, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

        // 5. Поиск контуров
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

        // 6. Обработка найденных контуров
        for (size_t i = 0; i < contours.size(); i++) {
            // Пропускаем маленькие контуры
            if (contourArea(contours[i]) < 100)
                continue;

            // Аппроксимация формы
            vector<Point> approx;
            approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);

            // Вычисление моментов для центра масс
            Moments m = moments(contours[i]);
            Point center(m.m10 / m.m00, m.m01 / m.m00);

            // Определение типа фигуры по количеству вершин
            string shape;
            if (approx.size() == 3) {
                shape = "TRI";
            }
            else if (approx.size() == 4) {
                // Проверяем, квадрат это или прямоугольник
                Rect rect = boundingRect(approx);
                float aspect = static_cast<float>(rect.width) / rect.height;
                shape = (aspect >= 0.95 && aspect <= 1.05) ? "SQUARE" : "RECT";
            }
            else if (approx.size() > 8) {
                shape = "CIRCLE";
            }
            else {
                shape = "UNKNOWN";
            }

            // Рисуем контур и подпись
            drawContours(frame, contours, static_cast<int>(i), Scalar(0, 255, 0), 2);
            putText(frame, shape, center, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2);
            circle(frame, center, 4, Scalar(0, 0, 255), -1);
        }

        // Отображаем результат
        imshow("Contour Tracking", frame);

        // Записываем кадр в выходное видео
        writer.write(frame);

        // Выход по нажатию ESC
        if (waitKey(30) == 27)
            break;
    }

    // Освобождаем ресурсы
    cap.release();
    writer.release();
    destroyAllWindows();

    return 0;
}