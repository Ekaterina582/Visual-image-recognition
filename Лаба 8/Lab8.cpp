#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main() {
    // Загрузка изображений
    Mat img1 = imread("C:\\Users\\admin\\Downloads\\левая.jpg");
    Mat img2 = imread("C:\\Users\\admin\\Downloads\\правая.jpg");

    if (img1.empty() || img2.empty()) {
        cout << "Не удалось загрузить изображения!" << endl;
        return -1;
    }

    // Инициализация ORB детектора
    Ptr<ORB> orb = ORB::create();
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    // Поиск ключевых точек и вычисление дескрипторов
    orb->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(img2, noArray(), keypoints2, descriptors2);

    // Сохранение изображений с ключевыми точками
    Mat img_keypoints1, img_keypoints2;
    drawKeypoints(img1, keypoints1, img_keypoints1);
    drawKeypoints(img2, keypoints2, img_keypoints2);
    imwrite("keypoints1.jpg", img_keypoints1);
    imwrite("keypoints2.jpg", img_keypoints2);

    // Сопоставление дескрипторов с помощью brute-force matcher
    BFMatcher matcher(NORM_HAMMING);
    vector<vector<DMatch>> knn_matches;
    matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);

    // Фильтрация совпадений с использованием теста Lowe's ratio
    vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < 0.75 * knn_matches[i][1].distance) {
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    // Визуализация всех совпадений
    Mat img_matches;
    drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches);
    imwrite("all_matches.jpg", img_matches);

    // Получение координат ключевых точек для хороших совпадений
    vector<Point2f> points1, points2;
    for (size_t i = 0; i < good_matches.size(); i++) {
        points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
        points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }

    // Вычисление матрицы гомографии с использованием RANSAC
    Mat H = findHomography(points2, points1, RANSAC);

    // Применение перспективного преобразования ко второму изображению
    Mat img_panorama;
    warpPerspective(img2, img_panorama, H, Size(img1.cols + img2.cols, img1.rows));

    // Склейка изображений
    Mat half(img_panorama, Rect(0, 0, img1.cols, img1.rows));
    img1.copyTo(half);

    // Обрезка черных областей
    // Находим границы панорамы
    int min_x = 0;
    int min_y = 0;
    int max_x = img_panorama.cols - 1;
    int max_y = img_panorama.rows - 1;

    // Ищем первый и последний столбец с ненулевыми пикселями
    for (int x = 0; x < img_panorama.cols; x++) {
        for (int y = 0; y < img_panorama.rows; y++) {
            if (img_panorama.at<Vec3b>(y, x) != Vec3b(0, 0, 0)) {
                min_x = x;
                x = img_panorama.cols; // выход из внешнего цикла
                break;
            }
        }
    }

    for (int x = img_panorama.cols - 1; x >= 0; x--) {
        for (int y = 0; y < img_panorama.rows; y++) {
            if (img_panorama.at<Vec3b>(y, x) != Vec3b(0, 0, 0)) {
                max_x = x;
                x = 0; // выход из внешнего цикла
                break;
            }
        }
    }

    // Обрезаем изображение
    Mat cropped_panorama = img_panorama(Rect(min_x, min_y, max_x - min_x, max_y - min_y));

    // Сохранение финального результата
    imwrite("final_panorama.jpg", cropped_panorama);

    cout << "Панорама успешно создана!" << endl;

    return 0;
}