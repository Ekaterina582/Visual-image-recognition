# Лабораторная работа: Детектирование и классификация геометрических фигур на изображении  

## Постановка задачи  
Цель работы — разработать программу на C++ с использованием библиотеки OpenCV, которая:  
- Автоматически обнаруживает геометрические фигуры на изображении.  
- Классифицирует их (треугольник, квадрат, прямоугольник, круг, многоугольник).  
- Визуализирует результат, выделяя контуры и подписывая типы фигур.  

Исходные данные: изображение `фигуры.jpg`, содержащее геометрические фигуры разной формы и размера.  

## Описание выполнения работы  

### 1. Предварительная обработка изображения  
- **Загрузка изображения**: Программа считывает файл `C:\Users\admin\Downloads\фигуры.jpg`.  
- **Увеличение насыщенности**: Изображение преобразуется в цветовое пространство HSV, после чего канал насыщенности (S) усиливается для улучшения видимости фигур.  
- **Перевод в градации серого**: Используется канал яркости (V) для последующей обработки.  

### 2. Детектирование границ и контуров  
- **Фильтрация**: Применяется размытие по Гауссу для уменьшения шума.  
- **Алгоритм Canny**: Выделяются границы фигур.  
- **Поиск контуров**: Найденные границы преобразуются в замкнутые контуры с помощью функции `findContours`.  

### 3. Классификация фигур  
Для каждого контура выполняется:  
- **Аппроксимация**: Определяется количество вершин с помощью `approxPolyDP`.  
- **Анализ формы**:  
  - 3 вершины → треугольник,  
  - 4 вершины → квадрат (если соотношение сторон ≈1) или прямоугольник,  
  - 5 вершин → пятиугольник,  
  - Высокая округлость → круг, иначе — многоугольник.  

### 4. Визуализация результата  
- Контуры фигур выделяются зеленым цветом.  
- Рядом с центром масс каждой фигуры подписывается её тип.  
- Результат сохраняется в файл `фигуры_результат.jpg`.  

## Вывод  
Программа успешно решает задачу детектирования и классификации геометрических фигур. Основные этапы работы:  
1. Улучшение изображения (HSV-преобразование, увеличение насыщенности).  
2. Надежное обнаружение границ (Canny + GaussianBlur).  
3. Логика классификации на основе аппроксимации контуров.  

Результат демонстрирует точность определения форм, что подтверждает эффективность использованных методов OpenCV. Для дальнейшего развития проекта можно добавить обработку перекрывающихся фигур или расширить классификатор для более сложных форм.  
