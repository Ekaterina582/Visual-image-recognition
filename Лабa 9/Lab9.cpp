#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>  // для accumulate

namespace fs = std::filesystem;

struct SampleCard {
    std::string label;
    cv::Mat grayImage;
    std::vector<cv::KeyPoint> features;
    cv::Mat featureDescriptors;
};

void selectBestMatches(const std::vector<std::vector<cv::DMatch>>& allMatches,
    float threshold,
    std::vector<cv::DMatch>& filtered) {
    for (const auto& pair : allMatches) {
        if (pair.size() < 2) continue;
        if (pair[0].distance < threshold * pair[1].distance) {
            filtered.push_back(pair[0]);
        }
    }
}

int main() {
    try {
        const std::string folderPath = "C:/Users/admin/Downloads";
        const std::vector<std::string> cardImages = {
            "valet piki.png", "tuz buby.png", "korol kresti.png",
            "dama chervy.png", "9 kresti.png", "9 buby.png",
            "5 kresti.png", "3 piki.png", "2 chervy.png"
        };
        const std::string targetImagePath = folderPath + "/target.png";

        if (!fs::exists(targetImagePath)) {
            throw std::runtime_error("Целевое изображение не найдено: " + targetImagePath);
        }

        cv::Ptr<cv::SIFT> sift = cv::SIFT::create();
        if (sift.empty()) {
            throw std::runtime_error("Не удалось инициализировать SIFT");
        }

        std::vector<SampleCard> database;
        for (const auto& file : cardImages) {
            const std::string path = folderPath + "/" + file;

            if (!fs::exists(path)) {
                std::cerr << "Файл не найден: " << path << ", пропуск..." << std::endl;
                continue;
            }

            cv::Mat gray = cv::imread(path, cv::IMREAD_GRAYSCALE);
            if (gray.empty()) {
                std::cerr << "Ошибка загрузки изображения: " << path << std::endl;
                continue;
            }

            SampleCard card;
            card.label = file.substr(0, file.find_last_of('.'));
            card.grayImage = gray.clone();
            sift->detectAndCompute(gray, cv::noArray(), card.features, card.featureDescriptors);
            database.push_back(std::move(card));
        }

        // Загружаем оригинальное и grayscale изображение
        cv::Mat targetColor = cv::imread(targetImagePath, cv::IMREAD_COLOR);
        cv::Mat targetGray = cv::imread(targetImagePath, cv::IMREAD_GRAYSCALE);
        if (targetColor.empty() || targetGray.empty()) {
            throw std::runtime_error("Ошибка загрузки целевого изображения");
        }

        std::vector<cv::KeyPoint> targetFeatures;
        cv::Mat targetDescriptors;
        sift->detectAndCompute(targetGray, cv::noArray(), targetFeatures, targetDescriptors);

        cv::Mat annotatedImage = targetColor.clone();
        cv::BFMatcher descriptorMatcher(cv::NORM_L2);

        for (const auto& card : database) {
            if (card.featureDescriptors.empty()) continue;

            std::vector<std::vector<cv::DMatch>> knnMatches;
            descriptorMatcher.knnMatch(card.featureDescriptors, targetDescriptors, knnMatches, 2);

            std::vector<cv::DMatch> validMatches;
            selectBestMatches(knnMatches, 0.75f, validMatches);

            if (validMatches.size() >= 4) {
                std::vector<cv::Point2f> sourcePoints, targetPoints;
                for (const auto& match : validMatches) {
                    sourcePoints.push_back(card.features[match.queryIdx].pt);
                    targetPoints.push_back(targetFeatures[match.trainIdx].pt);
                }

                cv::Mat homography = cv::findHomography(sourcePoints, targetPoints, cv::RANSAC);
                if (homography.empty()) continue;
                std::vector<cv::Point2f> cardCorners = {
                    cv::Point2f(0, 0),
                    cv::Point2f((float)card.grayImage.cols, 0),
                    cv::Point2f((float)card.grayImage.cols, (float)card.grayImage.rows),
                    cv::Point2f(0, (float)card.grayImage.rows)
                };

                std::vector<cv::Point2f> projectedCorners;
                cv::perspectiveTransform(cardCorners, projectedCorners, homography);


                for (size_t i = 0; i < 4; ++i) {
                    cv::line(annotatedImage, projectedCorners[i],
                        projectedCorners[(i + 1) % 4],
                        cv::Scalar(255, 0, 0), 2);
                }


                cv::Point2f sum = std::accumulate(
                    projectedCorners.begin(), projectedCorners.end(),
                    cv::Point2f(0.0f, 0.0f),
                    [](const cv::Point2f& a, const cv::Point2f& b) { return a + b; }
                );
                cv::Point2f textPos = sum * (1.0f / projectedCorners.size());

                cv::putText(annotatedImage, card.label, textPos,
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 2);
            }
        }

        cv::imshow("Обнаруженные карты", annotatedImage);
        cv::waitKey(0);

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}