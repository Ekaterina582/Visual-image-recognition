#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

// Функция для определения типа фигуры по контуру
string detectShape(const vector<Point>& contour) {
    double epsilon = 0.04 * arcLength(contour, true);
    vector<Point> approx;
    approxPolyDP(contour, approx, epsilon, true);

    string shape;
    if (approx.size() == 3) {
        shape = "Triangle";
    }
    else if (approx.size() == 4) {
        // Проверяем, квадрат это или прямоугольник
        Rect rect = boundingRect(approx);
        float aspectRatio = (float)rect.width / rect.height;
        if (aspectRatio >= 0.95 && aspectRatio <= 1.05) {
            shape = "Square";
        }
        else {
            shape = "Rectangle";
        }
    }
    else if (approx.size() == 5) {
        shape = "Pentagon";
    }
    else {
        // Для круга или многоугольника с большим числом сторон
        double area = contourArea(contour);
        double perimeter = arcLength(contour, true);
        double circularity = 4 * CV_PI * area / (perimeter * perimeter);

        if (circularity > 0.85) {
            shape = "Circle";
        }
        else {
            shape = "Polygon";
        }
    }

    return shape;
}

int main() {
    // Загрузка изображения
    string imagePath = "C:\\Users\\admin\\Downloads\\фигуры.jpg";
    Mat image = imread(imagePath);

    if (image.empty()) {
        cerr << "Could not open or find the image!" << endl;
        return -1;
    }

    // Преобразование в HSV и увеличение насыщенности
    Mat hsvImage;
    cvtColor(image, hsvImage, COLOR_BGR2HSV);

    vector<Mat> hsvChannels;
    split(hsvImage, hsvChannels);
    hsvChannels[1] = hsvChannels[1] * 1.5; // Увеличиваем насыщенность
    merge(hsvChannels, hsvImage);

    // Преобразование в оттенки серого (используем канал Value)
    Mat grayImage;
    cvtColor(hsvImage, grayImage, COLOR_HSV2BGR);
    cvtColor(grayImage, grayImage, COLOR_BGR2GRAY);

    // Размытие по Гауссу
    GaussianBlur(grayImage, grayImage, Size(5, 5), 0);

    // Детектирование границ Canny
    Mat edges;
    Canny(grayImage, edges, 50, 150);

    // Поиск контуров
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edges, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Обработка и классификация каждого контура
    for (size_t i = 0; i < contours.size(); i++) {
        // Пропускаем маленькие контуры (шум)
        if (contourArea(contours[i]) < 500) continue;

        string shape = detectShape(contours[i]);

        // Рисуем контур
        drawContours(image, contours, i, Scalar(0, 255, 0), 2);

        // Вычисляем центр масс для подписи
        Moments M = moments(contours[i]);
        int cx = static_cast<int>(M.m10 / M.m00);
        int cy = static_cast<int>(M.m01 / M.m00);

        // Подписываем фигуру
        putText(image, shape, Point(cx - 50, cy), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
    }

    // Отображение результата
    namedWindow("Detected Shapes", WINDOW_NORMAL);
    imshow("Detected Shapes", image);
    waitKey(0);

    // Сохранение результата
    imwrite("C:\\Users\\admin\\Downloads\\фигуры_результат.jpg", image);

    return 0;
}