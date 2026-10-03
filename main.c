#include <stdio.h>

#define INVENTORY_SIZE 10
#define START_DAY 1
#define START_HOUR 8
#define HOURS_PER_DAY 24

#define ITEM_EMPTY 0
#define ITEM_WOOD 1
#define ITEM_STONE 2
#define ITEM_SEEDS 3
#define ITEM_IRON 4
#define ITEM_COAL 5
#define ITEM_GOLD 6
#define ITEM_WATER 7
#define ITEM_HERB 8
#define ITEM_SWORD 9

const char* get_item_name(int item_id) {
    switch (item_id) {
        case ITEM_WOOD: return "Дерево";
        case ITEM_STONE: return "Камень";
        case ITEM_SEEDS: return "Семена";
        case ITEM_IRON: return "Железо";
        case ITEM_COAL: return "Уголь";
        case ITEM_GOLD: return "Золото";
        case ITEM_WATER: return "Вода";
        case ITEM_HERB: return "Лечебная трава";
        case ITEM_SWORD: return "Меч";
        default: return "Пусто";
    }
}
int main(void) {
    int current_day = START_DAY;
    int current_hour = START_HOUR;
    int inventory[INVENTORY_SIZE] = {
        ITEM_WOOD, ITEM_STONE, ITEM_EMPTY, ITEM_SEEDS, ITEM_IRON, 
        ITEM_COAL, ITEM_EMPTY, ITEM_GOLD, ITEM_HERB, ITEM_EMPTY
    };
    int choice=-1;
    int hours_worked=0;

    while (1) {
        printf("\n===== Весёлый фермер =====\n");
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Выполнить задание по варианту\n");
        printf("Выберите пункт меню: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: нужно ввести число.\n");
            int ch;
            while ((ch=getchar())!='\n' && ch!=EOF);
            continue;
        }

        switch (choice) {
            case 0:
                printf("Выход из игры. До свидания!\n");
                return 0;
                break;
            case 1:
                printf("\nТекущее время: День %d, %02d:00\n", current_day, current_hour);
                break;
            case 2:
                printf("\nСколько часов вы хотите поработать?");
                if (scanf("%d", &hours_worked)!=1) {
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
            case 3:
                printf("\n--- СОСТОЯНИЕ ИНВЕНТАРЯ ---\n");
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i] == ITEM_EMPTY) {
                        printf("Слот %d: [0]\n", i);
                    } else {
                        printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i]));
                    }
                }
                printf("---------------------------\n");
                break;
            case 4: {
                int slot_index = -1;
                int item_id = -1;

                printf("\nВведите номер слота (от 0 до %d): ", INVENTORY_SIZE - 1);
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
            case 5:
                /* выбросить предмет */
                break;
            case 6:
                /* задание по варианту */
                break;
            default:
                printf("Нет такого пункта меню.\n");
                break;
        }

        if (choice == 0) {
            break;
        }
    }

    return 0;
}