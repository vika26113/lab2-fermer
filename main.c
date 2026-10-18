#include <stdio.h> // Библиотека вывода и ввода
#include <string.h>  // Библиотека для строк
#include <windows.h> // Библиотека для windows
#define INVENTORY_SIZE 10 // Размер рюкзака
#define START_DAY 1 // Начальный день
#define START_HOUR 8 // Начальный час
#define HOURS_PER_DAY 24 // Часов в дне
#define MAX_NAME_LEN 32  // Максимальная длина имени, названий (31 символ и \0)
#define LINE_BUFFER_LEN 128 // 
#define MAX_PRICES 30  // 
#define MAX_ITEMS 10 // Всего предметов от 0 до 9
#define ITEM_EMPTY 0 // Пусто
#define ITEM_WOOD 1 // Дерево
#define ITEM_STONE 2 // Камень
#define ITEM_SEEDS 3 // Семена
#define ITEM_IRON 4 // Железо
#define ITEM_COAL 5 // Уголь
#define ITEM_GOLD 6 // Золото
#define ITEM_WATER 7 // Вода
#define ITEM_HERB 8 // Лечебная трава
#define ITEM_SWORD 9 // Меч
void input_farmer_name(char farmer_name[MAX_NAME_LEN]) { // Функция ввода имени, массив символов для имени
    printf("Введите имя вашего фермера (до 31 символа): ");
    if (fgets(farmer_name, MAX_NAME_LEN, stdin) != NULL) { // Проверка что считывание без ошибок
        farmer_name[strcspn(farmer_name, "\n")] = '\0'; // Убирает символ /n, для красивого вывода
    }
    if (strlen(farmer_name) == 0) { // Если не ввели имя
        strcpy(farmer_name, "Фермер"); // Копируем имя фермер
    }
}
void set_default_items(char item_names[MAX_ITEMS][MAX_NAME_LEN]) { // Функция заполнения предмеов, еслифайл не найден
    strcpy(item_names[0], "Пусто"); // Записывает предмет в массив
    strcpy(item_names[1], "Дерево");
    strcpy(item_names[2], "Камень");
    strcpy(item_names[3], "Семена");
    strcpy(item_names[4], "Железо");
    strcpy(item_names[5], "Уголь");
    strcpy(item_names[6], "Золото");
    strcpy(item_names[7], "Вода");
    strcpy(item_names[8], "Трава");
    strcpy(item_names[9], "Меч");
}
void load_items_from_file(char item_names[MAX_ITEMS][MAX_NAME_LEN]) { // Чтение каталога из items 
    FILE *file = fopen("items.txt", "r"); // Открывает и считывает файл
    if (file == NULL) { // Если нет файла
        printf("Предупреждение: файл items.txt не найден! Загружены стандартные предметы.\n");
        set_default_items(item_names); // Вызывает функцию заполнения предметами
        return; 
    }
    for (int i = 0; i < MAX_ITEMS; i++) { // Считываем 10 строчек
        if (fgets(item_names[i], MAX_NAME_LEN, file) != NULL) { // Считываем по одной строчке из файла
            item_names[i][strcspn(item_names[i], "\n")] = '\0'; // Убирает /n
        } 
        else {
            strcpy(item_names[i], "Неизвестно"); // Если ничего не ввели
        }
    }
    fclose(file); // Закрывает файл
    printf("Каталог предметов успешно загружен из файла items.txt!\n");
}
void display_time(int day, int hour) {
    printf("\n--- ТЕКУЩЕЕ ВРЕМЯ ---\n");
    printf("День: %d\n", day);
    printf("Время: %02d:00\n", hour);
    printf("---------------------\n");
}
void advance_time(int *day, int *hour) {
    int hours_worked = 0;
    printf("\nСколько часов вы хотите поработать? ");
    if (scanf("%d", &hours_worked) != 1) {
        printf("Ошибка: нужно ввести целое число часов!\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return;
    }
    if (hours_worked <= 0) {
        printf("Количество часов должно быть положительным числом (> 0)!\n");
        return;
    }
    int total_hours = *hour + hours_worked;
    *day += total_hours / HOURS_PER_DAY;
    *hour = total_hours % HOURS_PER_DAY;
    printf("Вы усердно поработали %d ч. Время пролетело!\n", hours_worked);
    printf("Новое время: День %d, %02d:00\n", *day, *hour);
}
void display_inventory(const int inventory[INVENTORY_SIZE], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    printf("\n--- СОСТОЯНИЕ ИНВЕНТАРЯ ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int item_id = inventory[i]; 
        if (item_id == ITEM_EMPTY) {
            printf("Слот %d: [0] (%s)\n", i, item_names[0]);
        } else {
            printf("Слот %d: [%d] (%s)\n", i, item_id, item_names[item_id]);
        }
    }
    printf("---------------------------\n");
}
void put_item(int inventory[INVENTORY_SIZE], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    int slot_index = -1;
    int item_id = -1;
    printf("\nВведите номер слота (от 0 до %d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &slot_index) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return;
    }
    if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
        printf("Ошибка: слот с номером %d не существует! Допустимы только 0..%d.\n", 
               slot_index, INVENTORY_SIZE - 1);
        return;
    }
    printf("Введите ID предмета (от 1 до %d): ", MAX_ITEMS - 1);
    if (scanf("%d", &item_id) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return;
    }
    if (item_id <= ITEM_EMPTY || item_id >= MAX_ITEMS) {
        printf("Ошибка: ID предмета должен быть от 1 до %d!\n", MAX_ITEMS - 1);
        return;
    }
    inventory[slot_index] = item_id;
    printf("Успешно! В слот %d помещен предмет [%d] (%s).\n", 
           slot_index, item_id, item_names[item_id]);
}
void drop_item(int inventory[INVENTORY_SIZE], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    int slot_index = -1;
    printf("\nВведите номер слота для очистки (от 0 до %d): ", INVENTORY_SIZE - 1);
    if (scanf("%d", &slot_index) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return;
    }
    if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
        printf("Ошибка: слот с номером %d не существует! Допустимы только 0..%d.\n", 
               slot_index, INVENTORY_SIZE - 1);
        return;
    }
    if (inventory[slot_index] == ITEM_EMPTY) {
        printf("Слот %d и так уже пуст!\n", slot_index);
    } else {
        int removed_item = inventory[slot_index];
        inventory[slot_index] = ITEM_EMPTY; 
        printf("Вы выбросили предмет [%d] (%s) из слота %d.\n", 
               removed_item, item_names[removed_item], slot_index);
    }
}
void clean_garbage(int inventory[INVENTORY_SIZE], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    int garbage_id = -1;
    int cleaned_count = 0;
    printf("\n--- ОЧИСТКА ОТ МУСОРА ---\n");
    printf("Введите ID предмета, который нужно удалить из всего инвентаря (1-%d): ", MAX_ITEMS - 1);
    if (scanf("%d", &garbage_id) != 1) {
        printf("Ошибка: нужно ввести число!\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return; 
    }
    if (garbage_id <= ITEM_EMPTY || garbage_id >= MAX_ITEMS) {
        printf("Ошибка: ID предмета должен быть от 1 до %d!\n", MAX_ITEMS - 1);
        return;
    }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == garbage_id) {
            inventory[i] = ITEM_EMPTY; 
            cleaned_count++;           
        }
    }
    if (cleaned_count == 0) {
        printf("Предмет [%d] (%s) не найден в инвентаре.\n", 
               garbage_id, item_names[garbage_id]);
    } else {
        printf("Успешно удалено предметов [%d] (%s): %d шт.\n", 
               garbage_id, item_names[garbage_id], cleaned_count);
    }
}
void search_item_by_name(const int inventory[INVENTORY_SIZE], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    char search_query[MAX_NAME_LEN];
    printf("\nВведите название предмета для поиска: ");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    if (fgets(search_query, MAX_NAME_LEN, stdin) != NULL) {
        search_query[strcspn(search_query, "\n")] = '\0';
    }
    int found_count = 0; 
    printf("\n--- РЕЗУЛЬТАТЫ ПОИСКА ДЛЯ \"%s\" ---\n", search_query);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int item_id = inventory[i]; 
        if (item_id >= 0 && item_id < MAX_ITEMS) {
            if (strcmp(item_names[item_id], search_query) == 0) {
                printf("Найдено! Слот %d: [%d] (%s)\n", i, item_id, item_names[item_id]);
                found_count++;
            }
        }
    }
    if (found_count == 0) {
        printf("Предмет \"%s\" не найден в вашем рюкзаке.\n", search_query);
    }
    printf("---------------------------------------\n");
}
void write_to_diary(const char *farmer_name, int day, int hour) {
    char diary_entry[LINE_BUFFER_LEN];
    printf("\nВведите запись для дневника фермера: ");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    if (fgets(diary_entry, sizeof(diary_entry), stdin) != NULL) {
        diary_entry[strcspn(diary_entry, "\n")] = '\0'; // Убираем перенос строки
    }
    FILE *file = fopen("diary.txt", "a");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл diary.txt для записи!\n");
        return;
    }
    fprintf(file, "[День %d, Время %02d:00] Фермер %s: %s\n", day, hour, farmer_name, diary_entry);
    fclose(file); 
    printf("Запись успешно добавлена в дневник (diary.txt)!\n");
}
void evaluate_inventory_at_merchant(const int inventory[INVENTORY_SIZE], char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    char price_names[MAX_PRICES][MAX_NAME_LEN];
    int price_values[MAX_PRICES];
    int price_count = 0;
    FILE *in_file = fopen("input.txt", "r");
    if (in_file == NULL) {
        printf("\nОшибка: файл с прейскурантом input.txt не найден!\n");
        return;
    }
    while (price_count < MAX_PRICES && fscanf(in_file, "%31s %d", price_names[price_count], &price_values[price_count]) == 2) {
        price_count++;
    }
    fclose(in_file); 
    FILE *out_file = fopen("output.txt", "w");
    if (out_file == NULL) {
        printf("\nОшибка: не удалось создать файл output.txt!\n");
        return;
    }
    int total_sum = 0;
    int max_item_price = -1;
    char max_item_name[MAX_NAME_LEN] = "Нет предметов";
    printf("\n=== ОЦЕНОЧНАЯ ВЕДОМОСТЬ СКУПЩИКА ===\n");
    fprintf(out_file, "=== ОЦЕНОЧНАЯ ВЕДОМОСТЬ СКУПЩИКА ===\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int item_id = inventory[i];
        if (item_id == ITEM_EMPTY) {
            continue;
        }
        const char *current_name = item_names[item_id];
        int current_price = 0; 
        for (int p = 0; p < price_count; p++) {
            if (strcmp(current_name, price_names[p]) == 0) {
                current_price = price_values[p];
                break; 
            }
        }
        total_sum += current_price;
        if (current_price > max_item_price) {
            max_item_price = current_price;
            strcpy(max_item_name, current_name);
        }
        printf("Слот %d: [%d] %s — Цена: %d монет\n", i, item_id, current_name, current_price);
        fprintf(out_file, "Слот %d: [%d] %s — Цена: %d монет\n", i, item_id, current_name, current_price);
    }
    printf("------------------------------------\n");
    fprintf(out_file, "------------------------------------\n");
    if (max_item_price > 0) {
        printf("Самый дорогой предмет в рюкзаке: %s (%d монет)\n", max_item_name, max_item_price);
        fprintf(out_file, "Самый дорогой предмет в рюкзаке: %s (%d монет)\n", max_item_name, max_item_price);
    } else {
        printf("В рюкзаке нет ценных предметов.\n");
        fprintf(out_file, "В рюкзаке нет ценных предметов.\n");
    }
    printf("ИТОГОВАЯ ВЫРУЧКА ЗА ВСЁ: %d монет\n", total_sum);
    fprintf(out_file, "ИТОГОВАЯ ВЫРУЧКА ЗА ВСЁ: %d монет\n", total_sum);
    printf("====================================\n");
    fprintf(out_file, "====================================\n");
    fclose(out_file); 
    printf("Ведомость успешно сохранена в файл output.txt!\n");
}
int main(void) { // Точка входа любой программы, не принимает аргументов
    int current_day = START_DAY; // Здесь будет стартовое значение (1)
    int current_hour = START_HOUR; // Здесь будет стартовое значение (8)
    int inventory[INVENTORY_SIZE] = { // Объявление массива
        ITEM_WOOD, ITEM_STONE, ITEM_EMPTY, ITEM_SEEDS, ITEM_IRON, // Раскладываем инвентарь
        ITEM_COAL, ITEM_EMPTY, ITEM_GOLD, ITEM_HERB, ITEM_EMPTY
    };
    int choice=-1; // Переменная выбора, запишет число выбранное пользователем
    int hours_worked=0; // Переменная количества часов отработанных пользователем
    SetConsoleOutputCP(CP_UTF8); // Для windows
    SetConsoleCP(CP_UTF8); // Для windows
    char farmer_name[MAX_NAME_LEN] = ""; // Массив для имени
    input_farmer_name(farmer_name); // Вызов функции для ввода имени
    printf("\nЗдравствуйте, %s!\n", farmer_name);
    char item_names[MAX_ITEMS][MAX_NAME_LEN]; // Двумерный массив для названий предметов
    load_items_from_file(item_names); // Загрузка предметов из файла
    while (1) { // Бесконечный цикл для циклической работы программы
        printf("\n=== Весёлый фермер (Фермер: %s) ===\n", farmer_name);// Вывод заголовка, переход на новую строку
        printf("[0] Выход\n"); // Вывод пункта
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Очистка от мусора\n");
        printf("[7] Поиск предмета в рюкзаке по названию\n");
        printf("[8] Записать состояние в дневник фермера (diary.txt)\n");
        printf("[9] Оценка рюкзака у скупщика\n");
        printf("Выберите пункт меню: "); //Выбор пункта в меню

        if (scanf("%d", &choice) != 1) { // Считывает число, записывает в choice, возвращает количество успешно считанных переменных
            printf("Ошибка: нужно ввести число.\n"); // Если пользователь ввёл букву
            int ch; // Переменная для символов с клавиатуры
            while ((ch=getchar())!='\n' && ch!=EOF); // Читает все символы, пока не будет enter, или файл ввода оборвётся
            continue; // Прекращает цикл, возвращается в while
        }
        switch (choice) { // Сравнивает переменную с вариантами
            case 0: // Пункт 0
                printf("Выход из игры. До свидания, %s!\n", farmer_name);
                return 0; // Завершение программы    
            case 1: // Пункт 1
                display_time(current_day, current_hour); // Вызываем функцию для вывода текущего времени
                break;
            case 2: // Пункт 2
                advance_time(&current_day, &current_hour); // Вызываем функцию для работы фермера
                break;
            case 3: // Пункт 3
                display_inventory(inventory, item_names); // Вызов функции для списка предметов
                break;
            case 4: // 4 пункт 
                put_item(inventory, item_names); // Вызов функции для добавления предмета в слот
                break;
            case 5: // Пункт 5
                drop_item(inventory, item_names); // Вызов функции для удаления предмета
                break;
            case 6: // Пункт 6
                clean_garbage(inventory, item_names); // Вызов функции для очистки инвентаря от выбранного мусора
                break;
            case 7:
                search_item_by_name(inventory, item_names); // Вызов функции для поиска предмета по названию
                break;
            case 8:
                write_to_diary(farmer_name, current_day, current_hour); // Вызов функции для записи в дневник 
                break;
            case 9:
                evaluate_inventory_at_merchant(inventory, item_names); // Вызов функции для оценки рюкзака у скупщика
                break;
            default:
                printf("Нет такого пункта меню.\n");
                break;
        }
        
        if (choice == 0) {
            break;
        }
    }

    return 0; // Завершение прораммы
} // Конец функции main