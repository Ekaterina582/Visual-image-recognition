# Лабораторная работа: Создание панорамы из изображений

## Постановка задачи

Целью данной работы является разработка программы для склейки двух изображений в панораму с использованием следующих методов и алгоритмов:

- Детектора **ORB** для поиска ключевых точек
- Метода **Brute-Force Matching** для сопоставления точек
- Алгоритма **RANSAC** для вычисления гомографии
- **Перспективного преобразования** для объединения изображений

Программа должна обрабатывать два изображения с перекрывающимися областями и создавать панорамное изображение, сохраняя промежуточные результаты обработки.

## Описание выполнения работы

1. **Загрузка изображений**  
   Программа загружает два изображения (`левая.jpg` и `правая.jpg`) из указанного пути.

2. **Поиск ключевых точек**  
   С помощью детектора **ORB** находятся ключевые точки и их дескрипторы на обоих изображениях. Результат сохраняется в файлы `keypoints1.jpg` и `keypoints2.jpg`.

3. **Сопоставление дескрипторов**  
   Для сопоставления дескрипторов используется метод **Brute-Force Matching**. Затем применяется **Lowe's ratio test** для фильтрации наилучших совпадений. Визуализация совпадений сохраняется в файл `all_matches.jpg`.

4. **Вычисление гомографии**  
   На основе отфильтрованных совпадений с помощью алгоритма **RANSAC** вычисляется матрица гомографии, которая описывает преобразование между изображениями.

5. **Склейка изображений**  
   Второе изображение преобразуется с использованием вычисленной гомографии и объединяется с первым. Результат сохраняется в файл `final_panorama.jpg`.

6. **Обрезка черных областей**  
   Программа автоматически обрезает черные области, оставшиеся после склейки, чтобы получить аккуратную панораму.

## Вывод

В ходе работы была успешно реализована программа для создания панорамы из двух изображений. Использование детектора **ORB** и алгоритма **RANSAC** позволило точно вычислить преобразование между изображениями, а метод **Brute-Force Matching** обеспечил надежное сопоставление ключевых точек. Результатом работы является панорамное изображение высокого качества, готовое к дальнейшему использованию.
