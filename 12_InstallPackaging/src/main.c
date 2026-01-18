/**
 * @file main.c
 * @brief Основная программа игры Mastermind
 * 
 * Точка входа в программу. Управляет игровым циклом,
 * обработкой ввода пользователя и выводом результатов.
 * 
 * @author VeryLittleAnna
 * @date 2026
 */

#include "lib/mastermind.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 20
#endif

#define _(STRING) gettext(STRING)
#define N_(STRING) (STRING)

/**
 * Выводит справочную информацию о программе
 * Отображает доступные опции командной строки и описание программы.
 * Используется при вызове с флагами -h или --help.
 */
void print_help() {
    printf(_("Использование: mastermind [ОПЦИЯ]\n"));
    printf(_("Опции:\n"));
    printf(_("  -h, --help     Показать эту справку\n"));
}

/**
 * Точка входа в программу Mastermind
 * 
 * Основная функция, управляющая игровым процессом:
 * 1. Инициализирует локализацию
 * 2. Обрабатывает аргументы командной строки
 * 3. Создает игровую сессию
 * 4. Управляет игровым циклом
 * 5. Освобождает ресурсы
 * 
 * @param argc количество аргументов командной строки
 * @param argv массив строк аргументов командной строки
 * @return 0 при успешном завершении, 1 при ошибке
 * 
 * @note Поддерживаемые команды в игре:
 *       - "подсказка" - получить подсказку
 *       - "история" - показать историю попыток
 *       - "выход" - завершить игру
 */
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain("mastermind", LOCALEDIR);
    textdomain("mastermind");

    if (argc >= 2) {
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			print_help();
			return 0;
		}
        // возможно, захочу добавить опций
        else {
			fprintf(stderr, _("Ошибка: неизвестная опция '%s'\n"), argv[1]);
			fprintf(stderr, _("Посмотрите '%s --help' для справки.\n"), argv[0]);
			return 1;
		}
    }
    Game* game = game_create();

    char buffer[BUFFER_SIZE];

    printf(_("Загадана строка (%d цифр из %s)\n"), CODE_LENGTH, ALPHABET);
    printf(_("Команды для игры: подсказка, выход, история\n"));
    printf(_("Ответ на догадку в формате (n, m), где n - количество верных символов с учетом позиции, m - количество верных символов на неверных позициях\n"));
    while (!game_is_over(game)) {
        printf(_("Введите вашу догадку (%d цифр из %s): "), CODE_LENGTH, ALPHABET);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (!strcmp(buffer, _("подсказка"))) {
            get_hint(game);
            continue;
        }
        if (!strcmp(buffer, _("история"))) {
            print_history(game);
            continue;
        }
        if (!strcmp(buffer, _("выход"))) {
            break;
        }
        if (strlen(buffer) != CODE_LENGTH) {
            printf(_("Догадка должна содержать ровно %d символов!\n"), CODE_LENGTH);
            continue;
        }
        if (!_is_valid_guess(buffer)) {
            printf(_("Догадка должна содержать только символы из алфавита: %s\n"), ALPHABET);
            continue;
        }
        Feedback result = game_check_guess(game, buffer);
        print_feedback(&result);
    }
    const char* verdict = get_verdict(game);
    if (verdict != NULL) {
        printf("%s\n", verdict);
    }

    game_clean(game);
    return 0;
}