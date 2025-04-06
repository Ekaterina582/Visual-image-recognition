#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

// Добавляет текстовую метку на изображение
void addLabel(cv::Mat& image, const std::string& label) {
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.7;
    int thickness = 2;
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(label, fontFace, fontScale, thickness, &baseline);
    cv::Point textOrg((image.cols - textSize.width) / 2, 30);
    cv::putText(image, label, textOrg, fontFace, fontScale, cv::Scalar(255, 255, 255), thickness);
}

// Приводит изображение к единому формату (3-канальному BGR) и размеру
cv::Mat ensureSameFormat(cv::Mat img, const cv::Size& size) {
    cv::Mat result;
    if (img.channels() == 1) {
        cv::cvtColor(img, result, cv::COLOR_GRAY2BGR);
    }
    else if (img.channels() == 4) {
        cv::cvtColor(img, result, cv::COLOR_BGRA2BGR);
    }
    else {
        result = img.clone();
    }
    cv::resize(result, result, size);
    return result;
}

int main() {
    // 1. Загрузка изображения
    std::string imagePath = "C:\\Users\\admin\\Downloads\\челики.jpg";
    cv::Mat originalImage = cv::imread(imagePath, cv::IMREAD_COLOR);

    if (originalImage.empty()) {
        std::cerr << "Ошибка: изображение не найдено по пути " << imagePath << std::endl;
        return -1;
    }

    // 2. Преобразование в оттенки серого
    cv::Mat grayImage;
    cv::cvtColor(originalImage, grayImage, cv::COLOR_BGR2GRAY);

    // 3. Размытие Гаусса
    cv::Mat blurredImage;
    cv::GaussianBlur(grayImage, blurredImage, cv::Size(9, 9), 2, 2);

    // 4. Обнаружение кругов (метод Хафа)
    cv::Mat circlesImage = originalImage.clone();
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(blurredImage, circles, cv::HOUGH_GRADIENT, 1,
        grayImage.rows / 8, 100, 30, 0, 0);

    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(circlesImage, center, radius, cv::Scalar(0, 0, 255), 3);
    }

    // 5. Обнаружение линий (метод Хафа)
    cv::Mat edges;
    cv::Canny(blurredImage, edges, 50, 150, 3);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10);

    cv::Mat linesImage = originalImage.clone();
    for (size_t i = 0; i < lines.size(); i++) {
        cv::line(linesImage,
            cv::Point(lines[i][0], lines[i][1]),
            cv::Point(lines[i][2], lines[i][3]),
            cv::Scalar(0, 255, 0), 2);
    }

    // 6. Преобразование в различные цветовые пространства
    cv::Mat hsvImage, labImage, yuvImage, xyzImage;
    cv::cvtColor(originalImage, hsvImage, cv::COLOR_BGR2HSV);
    cv::cvtColor(originalImage, labImage, cv::COLOR_BGR2Lab);
    cv::cvtColor(originalImage, yuvImage, cv::COLOR_BGR2YUV);
    cv::cvtColor(originalImage, xyzImage, cv::COLOR_BGR2XYZ);

    // Подготовка изображений для отображения (одинаковый размер и формат)
    cv::Size displaySize(300, 300);
    cv::Mat originalDisplay = ensureSameFormat(originalImage, displaySize);
    cv::Mat grayDisplay = ensureSameFormat(grayImage, displaySize);
    cv::Mat circlesDisplay = ensureSameFormat(circlesImage, displaySize);
    cv::Mat linesDisplay = ensureSameFormat(linesImage, displaySize);
    cv::Mat hsvDisplay = ensureSameFormat(hsvImage, displaySize);
    cv::Mat labDisplay = ensureSameFormat(labImage, displaySize);
    cv::Mat yuvDisplay = ensureSameFormat(yuvImage, displaySize);
    cv::Mat xyzDisplay = ensureSameFormat(xyzImage, displaySize);

    // Добавление меток
    addLabel(originalDisplay, "Original");
    addLabel(grayDisplay, "Grayscale");
    addLabel(circlesDisplay, "Circles");
    addLabel(linesDisplay, "Lines");
    addLabel(hsvDisplay, "HSV");
    addLabel(labDisplay, "Lab");
    addLabel(yuvDisplay, "YUV");
    addLabel(xyzDisplay, "XYZ");

    // 7. Создание сетки изображений
    cv::Mat topRow, middleRow, bottomRow, result;
    cv::hconcat(std::vector<cv::Mat>{originalDisplay, grayDisplay, circlesDisplay, linesDisplay}, topRow);
    cv::hconcat(std::vector<cv::Mat>{hsvDisplay, labDisplay, yuvDisplay, xyzDisplay}, middleRow);
    cv::vconcat(std::vector<cv::Mat>{topRow, middleRow}, result);

    // 8. Отображение результатов
    cv::namedWindow("Image Processing Results", cv::WINDOW_NORMAL);
    cv::imshow("Image Processing Results", result);
    cv::waitKey(0);

    return 0;
}