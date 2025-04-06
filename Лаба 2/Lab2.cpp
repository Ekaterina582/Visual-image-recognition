#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Указание пути к изображению
    string imagePath = "C:\\Users\\admin\\Downloads\\челики.jpg";

    // Загрузка изображения
    Mat originalImage = imread(imagePath);

    if (originalImage.empty()) {
        cerr << "Ошибка: не удалось загрузить изображение по пути: " << imagePath << endl;
        return -1;
    }

    // Создаем копию для обработки
    Mat processedImage = originalImage.clone();

    // Получаем размеры изображения
    int width = processedImage.cols;
    int height = processedImage.rows;

    // Разделяем изображение на 4 равные части
    Rect segments[4] = {
        Rect(0, 0, width / 2, height / 2),               // Верхний левый (A)
        Rect(width / 2, 0, width / 2, height / 2),         // Верхний правый (B)
        Rect(0, height / 2, width / 2, height / 2),        // Нижний левый (C)
        Rect(width / 2, height / 2, width / 2, height / 2)   // Нижний правый (D)
    };

    // Применяем разные эффекты к каждому сегменту
    // Сегмент A (инверсия цветов)
    Mat segmentA = processedImage(segments[0]);
    bitwise_not(segmentA, segmentA);

    // Сегмент B (оттенки серого)
    Mat segmentB = processedImage(segments[1]);
    cvtColor(segmentB, segmentB, COLOR_BGR2GRAY);
    cvtColor(segmentB, segmentB, COLOR_GRAY2BGR);

    // Сегмент C (зеленая заливка)
    Mat segmentC = processedImage(segments[2]);
    segmentC.setTo(Scalar(0, 255, 0));

    // Сегмент D (размытие)
    Mat segmentD = processedImage(segments[3]);
    GaussianBlur(segmentD, segmentD, Size(15, 15), 0);

    // Добавляем подписи к сегментам
    putText(processedImage, "A", Point(20, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
    putText(processedImage, "B", Point(width / 2 + 20, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
    putText(processedImage, "C", Point(20, height / 2 + 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
    putText(processedImage, "D", Point(width / 2 + 20, height / 2 + 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

    // Отображаем оригинальное и обработанное изображения
    namedWindow("Original Image", WINDOW_AUTOSIZE);
    namedWindow("Processed Image", WINDOW_AUTOSIZE);

    imshow("Original Image", originalImage);
    imshow("Processed Image", processedImage);

    // Сохраняем результат
    string outputPath = "C:\\Users\\admin\\Downloads\\челики_processed.jpg";
    imwrite(outputPath, processedImage);
    cout << "Обработанное изображение сохранено как: " << outputPath << endl;

    waitKey(0);
    destroyAllWindows();

    return 0;
}