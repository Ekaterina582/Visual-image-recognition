#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Загрузка изображения
    string image_path = "C:\\Users\\admin\\Downloads\\челики.jpg";
    Mat image = imread(image_path, IMREAD_COLOR);

    if (image.empty()) {
        cerr << "Ошибка: Не удалось загрузить изображение!" << endl;
        return -1;
    }

    // Преобразование в HSV
    Mat hsv_image;
    cvtColor(image, hsv_image, COLOR_BGR2HSV);

    // Определение диапазона красного цвета в HSV
    Mat mask1, mask2;
    inRange(hsv_image, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
    inRange(hsv_image, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);
    Mat red_mask = mask1 | mask2;

    // Применение маски к оригинальному изображению
    Mat masked_image;
    bitwise_and(image, image, masked_image, red_mask);

    // Преобразование в оттенки серого и размытие
    Mat gray, blurred;
    cvtColor(masked_image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blurred, Size(9, 9), 2, 2);

    // Обнаружение кругов с помощью преобразования Хафа
    vector<Vec3f> circles;
    HoughCircles(blurred, circles, HOUGH_GRADIENT, 1,
        gray.rows / 8,  // Минимальное расстояние между центрами
        100, 30, 1, 30); // Параметры

    // Отрисовка обнаруженных кругов
    for (size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // Отрисовка центра круга
        circle(image, center, 3, Scalar(0, 255, 0), -1);
        // Отрисовка самого круга
        circle(image, center, radius, Scalar(0, 0, 255), 3);
    }

    // Отображение результатов
    namedWindow("Detected Circles", WINDOW_NORMAL);
    imshow("Detected Circles", image);
    waitKey(0);

    return 0;
}