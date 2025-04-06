#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;

void processImage(const string& imagePath) {
    try {
        // 1. Загрузка изображения
        Mat image = imread(imagePath);
        if (image.empty()) {
            throw runtime_error("Не удалось загрузить изображение по пути: " + imagePath);
        }

        // 2. Преобразование цветового пространства в HSV
        Mat hsvImage;
        cvtColor(image, hsvImage, COLOR_BGR2HSV);

        // 3. Создание масок для красного и зелёного цветов
        // Маски для красного цвета (два диапазона)
        Mat maskRed1, maskRed2, maskRed;
        inRange(hsvImage, Scalar(0, 100, 100), Scalar(10, 255, 255), maskRed1);
        inRange(hsvImage, Scalar(160, 100, 100), Scalar(180, 255, 255), maskRed2);
        bitwise_or(maskRed1, maskRed2, maskRed);

        // Маска для зелёного цвета
        Mat maskGreen;
        inRange(hsvImage, Scalar(35, 100, 100), Scalar(85, 255, 255), maskGreen);

        // 4. Применение масок к изображению
        Mat redResult, greenResult;
        bitwise_and(image, image, redResult, maskRed);
        bitwise_and(image, image, greenResult, maskGreen);

        // 5. Отображение результатов
        namedWindow("Original Image", WINDOW_NORMAL);
        namedWindow("Red Mask", WINDOW_NORMAL);
        namedWindow("Green Mask", WINDOW_NORMAL);
        namedWindow("Red Areas", WINDOW_NORMAL);
        namedWindow("Green Areas", WINDOW_NORMAL);

        imshow("Original Image", image);
        imshow("Red Mask", maskRed);
        imshow("Green Mask", maskGreen);
        imshow("Red Areas", redResult);
        imshow("Green Areas", greenResult);

        // Сохранение результатов
        imwrite("red_mask.jpg", maskRed);
        imwrite("green_mask.jpg", maskGreen);
        imwrite("red_areas.jpg", redResult);
        imwrite("green_areas.jpg", greenResult);

        waitKey(0);
        destroyAllWindows();

    }
    catch (const exception& e) {
        cerr << "Произошла ошибка: " << e.what() << endl;
    }
}

int main() {
    // Укажите путь к вашему изображению
    string imagePath = "C:\\Users\\admin\\Downloads\\челики.jpg";
    processImage(imagePath);
    return 0;
}