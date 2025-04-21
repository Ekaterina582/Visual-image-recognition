#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Загружаем изображение
    string imagePath = "C:\\Users\\admin\\Downloads\\челики.jpg";
    Mat image = imread(imagePath);

    // Проверяем, загрузилось ли изображение
    if (image.empty()) {
        cout << "Не удалось загрузить изображение по пути: " << imagePath << endl;
        return -1;
    }

    // Рисуем линии
    line(image, Point(0, 0), Point(image.cols - 1, image.rows - 1), Scalar(255, 0, 0), 5);
    line(image, Point(image.cols - 1, 0), Point(0, image.rows - 1), Scalar(0, 255, 0), 3);

    // Рисуем прямоугольники (размеры адаптируются под размер изображения)
    int rectSize = min(image.cols, image.rows) / 4;
    rectangle(image, Point(50, 50), Point(50 + rectSize, 50 + rectSize), Scalar(0, 0, 255), 3);
    rectangle(image, Point(image.cols - 50 - rectSize, image.rows - 50 - rectSize),
        Point(image.cols - 50, image.rows - 50), Scalar(0, 255, 255), -1);

    // Рисуем окружности
    circle(image, Point(image.cols / 2, image.rows / 2), min(image.cols, image.rows) / 8, Scalar(255, 255, 0), 2);
    circle(image, Point(image.cols - 100, 100), 30, Scalar(255, 0, 255), -1);

    // Рисуем эллипс
    ellipse(image, Point(image.cols / 2, image.rows - 100),
        Size(image.cols / 8, image.rows / 16), 0, 0, 180, Scalar(0, 255, 255), -1);

    // Рисуем многоугольник (шестиугольник)
    vector<Point> hexagon;
    int hexSize = min(image.cols, image.rows) / 6;
    hexagon.push_back(Point(100, image.rows - 100));
    hexagon.push_back(Point(100 + hexSize / 2, image.rows - 100 - hexSize));
    hexagon.push_back(Point(100 + hexSize, image.rows - 100));
    hexagon.push_back(Point(100 + hexSize, image.rows - 100 + hexSize / 2));
    hexagon.push_back(Point(100 + hexSize / 2, image.rows - 100 + hexSize));
    hexagon.push_back(Point(100, image.rows - 100 + hexSize / 2));

    const Point* pts = (const Point*)Mat(hexagon).data;
    int npts = Mat(hexagon).rows;
    polylines(image, &pts, &npts, 1, true, Scalar(255, 255, 255), 2);

    // Добавляем текст
    putText(image, "OpenCV Figures", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

    // Отображаем изображение
    imshow("Geometric Figures on Image", image);

    // Сохраняем результат
    string outputPath = "C:\\Users\\admin\\Downloads\\челики_with_figures.jpg";
    imwrite(outputPath, image);
    cout << "Результат сохранен в: " << outputPath << endl;

    // Ждем нажатия клавиши
    waitKey(0);

    return 0;
}