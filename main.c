#include <stdio.h> // Библиотека вывода и ввода

#define INVENTORY_SIZE 10 // Размер рюкзака
#define START_DAY 1 // Начальный день
#define START_HOUR 8 // Начальный час
#define HOURS_PER_DAY 24 // Часов в дне

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

const char* get_item_name(int item_id) { // Функция ID предмета в название
    switch (item_id) { // Проверяет чему равен переданный номер
        case ITEM_WOOD: return "Дерево"; // Если номер совпал то возвращается его название
        case ITEM_STONE: return "Камень";
        case ITEM_SEEDS: return "Семена";
        case ITEM_IRON: return "Железо";
        case ITEM_COAL: return "Уголь";
        case ITEM_GOLD: return "Золото";
        case ITEM_WATER: return "Вода";
        case ITEM_HERB: return "Лечебная трава";
        case ITEM_SWORD: return "Меч";
        default: return "Пусто"; // Если передан ID которого нет в списке
    }
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

    while (1) { // Бесконечный цикл для циклической работы программы
        printf("\n===== Весёлый фермер =====\n"); // Вывод заголовка, переход на новую строку
        printf("[0] Выход\n"); // Вывод пункта
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Очистка от мусора\n");
        printf("Выберите пункт меню: "); //Выбор пункта в меню

        if (scanf("%d", &choice) != 1) { // Считывает число, записывает в choice, возвращает количество успешно считанных переменных
            printf("Ошибка: нужно ввести число.\n"); // Если пользователь ввёл букву
            int ch; // Переменная для символов с клавиатуры
            while ((ch=getchar())!='\n' && ch!=EOF); // Читает все символы, пока не будет enter, или файл ввода оборвётся
            continue; // Прекращает цикл, возвращается в while
        }

        switch (choice) { // Сравнивает переменную с вариантами
            case 0: // Пункт 0
                printf("Выход из игры. До свидания!\n");
                return 0; // Завершение программы    
            case 1: // Пункт 1
                printf("\nТекущее время: День %d, %02d:00\n", current_day, current_hour); // Вывод времени
                break; // Прерывает выполнение switch
            case 2: // Пункт 2
                printf("\nСколько часов вы хотите поработать?");
                if (scanf("%d", &hours_worked)!=1) { // Проверка на дурака
                    printf("Ошибка: нужно ввести целое число часов!\n");
                    int ch;
                    while ((ch=getchar())!='\n' && ch!=EOF);
                    break;
                }
                if (hours_worked<=0) {
                    printf("Количество часов должно быть положительным числом!\n");
                    break;
                }
                {
                    int total_hours=current_hour+hours_worked;
                    current_day+=total_hours / HOURS_PER_DAY;
                    current_hour=total_hours % HOURS_PER_DAY;
                }
                printf("Вы усердно поработали %d ч. Время пролетело!\n", hours_worked);
                printf("Новое время: День %d, %02d:00\n", current_day, current_hour);
                break;
            case 3: // Пункт 3
                printf("\n--- СОСТОЯНИЕ ИНВЕНТАРЯ ---\n");
                for (int i = 0; i < INVENTORY_SIZE; i++) { // Создаём переменную, условие продолжения цикла, увеличиваем на 1
                    if (inventory[i] == ITEM_EMPTY) { // Если пустой слот 
                        printf("Слот %d: [0]\n", i); // Вместо i номер слота 
                    } else { // Если есть предмет
                        printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i])); // Вывод ячейки инвентаря
                    }
                }
                printf("---------------------------\n"); // Завершающая линия
                break;
            case 4: { // 4 пункт 
                int slot_index = -1; // Место для индекса ячейки 
                int item_id = -1; // Место для идентификатора предмета

                printf("\nВведите номер слота (от 0 до %d): ", INVENTORY_SIZE - 1); // Вывод номеров слотов
                if (scanf("%d", &slot_index) != 1) {
                    printf("Ошибка: нужно ввести число!\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }
                if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
                    printf("Ошибка: слот с номером %d не существует! Допустимы только 0..%d.\n", 
                           slot_index, INVENTORY_SIZE - 1);
                    break;
                }
                printf("Введите ID предмета (от 1 до 9): ");
                if (scanf("%d", &item_id) != 1) {
                    printf("Ошибка: нужно ввести число!\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }
                if (item_id <= ITEM_EMPTY || item_id > ITEM_SWORD) {
                    printf("Ошибка: ID предмета должен быть от 1 до 9!\n");
                    break;
                }
                inventory[slot_index] = item_id;
                printf("Успешно! В слот %d помещен предмет [%d] (%s).\n", 
                       slot_index, item_id, get_item_name(item_id));
                break;
            }
            case 5: {
                int slot_index = -1;
                printf("\nВведите номер слота для очистки (от 0 до %d): ", INVENTORY_SIZE - 1);
                if (scanf("%d", &slot_index) != 1) {
                    printf("Ошибка: нужно ввести число!\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }
                if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
                    printf("Ошибка: слот с номером %d не существует! Допустимы только 0..%d.\n", 
                           slot_index, INVENTORY_SIZE - 1);
                    break;
                }
                if (inventory[slot_index] == ITEM_EMPTY) {
                    printf("Слот %d и так уже пуст!\n", slot_index);
                } else {
                    int removed_item = inventory[slot_index]; 
                    inventory[slot_index] = ITEM_EMPTY;       
                    
                    printf("Вы выбросили предмет [%d] (%s) из слота %d.\n", 
                           removed_item, get_item_name(removed_item), slot_index);
                }

                break;
            }
            case 6: {
                int garbage_id = -1;
                int cleaned_count = 0;

                printf("\n--- ВАРИАНТ 5: ОЧИСТКА ОТ МУСОРА ---\n");
                printf("Введите ID предмета, который нужно удалить из всего инвентаря (1-9): ");
                
                if (scanf("%d", &garbage_id) != 1) {
                    printf("Ошибка: нужно ввести число!\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }

                if (garbage_id <= ITEM_EMPTY || garbage_id > ITEM_SWORD) {
                    printf("Ошибка: ID предмета должен быть от 1 до 9!\n");
                    break;
                }

                
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i] == garbage_id) {
                        inventory[i] = ITEM_EMPTY; 
                        cleaned_count++;           
                    }
                }

                if (cleaned_count == 0) {
                    printf("Предмет с ID [%d] (%s) не найден в инвентаре.\n", 
                           garbage_id, get_item_name(garbage_id));
                } else {
                    printf("Успешно удалено предметов [%d] (%s): %d шт.\n", 
                           garbage_id, get_item_name(garbage_id), cleaned_count);
                }

                printf("\nИтоговое состояние инвентаря:\n");
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i] == ITEM_EMPTY) {
                        printf("Слот %d: [0]\n", i);
                    } else {
                        printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i]));
                    }
                }
                printf("------------------------------------\n");

                break;
            }
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