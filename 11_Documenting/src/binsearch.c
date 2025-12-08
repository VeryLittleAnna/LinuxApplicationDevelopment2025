/**
 * @file binsearch.c
 * @brief Программа для угадывания чисел с поддержкой римской системы счисления
 * @author Anna Grinenko
 * @version 1.0
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <locale.h>
#include <libgen.h>
#include <libintl.h>
#include <getopt.h>

#define _(STRING) gettext(STRING)
#define N_(STRING) (STRING)

/** @def MAX_NUMBER
 *  @brief Максимальное число для угадывания
 */
#define MAX_NUMBER 3999

/** @def MAX_ROMAN_NUMBER
 *  @brief Максимальное число для угадывания в римской системе счисления
 */
#define MAX_ROMAN_NUMBER "MMMCMXCIX"

/** @def MIN_NUMBER
 *  @brief Минимальное число для угадывания
 */
#define MIN_NUMBER 1

/** @def MAX_ROMAN_LEN
 *  @brief Максимальная длина числа в римском системе счисления
 */
#define MAX_ROMAN_LEN 15

/** @def MAX_MATCH_DICT_LEN
 *  @brief Длина списка соответствий 
 */
#define MAX_MATCH_DICT_LEN 13

/** @def MAX_ANSWER_LEN
 *  @brief Максимальная длина ответа пользователя
 */
#define MAX_ANSWER_LEN 10

/** @var use_roman
 *  @brief Флаг использования римской системы счисления
 *  
 *  Если равен 1, программа работает с римскими числами.
 *  Если равен 0, программа работает с арабскими числами (по умолчанию).
 */
static int use_roman = 0;

/**
 * @brief Массив арабских значений для конвертации в римские числа
 * 
 * Значения расположены в порядке убывания и включают в себя "составные" числа, получающиеся вычитаемыми комбинациями (например 4 - IV).
 */
static const int arabic[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
/**
 * @brief Массив соответствующих римских символов
 * 
 * Каждый элемент соответствует значению из массива arabic[] в том же порядке.
 */
static const char *roman[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

/**
 * @brief Преобразует арабское число в римское
 * 
 * @param num Арабское число для конвертации (1-3999)
 * @param result Указатель на буфер для результата (должен быть не менее MAX_ROMAN_LEN+1)
 */
void arabic_to_roman(int num, char *result) {
    result[0] = '\0';
    for (int i = 0; i < MAX_MATCH_DICT_LEN; i++) {
        while (num >= arabic[i]) {
            strcat(result, roman[i]);
            num -= arabic[i];
        }
    }
}


/**
 * @brief Выводит справку по использованию программы (help)
 * 
 * Функция выводит информацию о параметрах командной строки и
 * описании программы на текущем языке системы.
 */
void print_help(void) {
    printf(_("Usage: binsearch [OPTIONS]\n"));
    printf(_("Number guessing game using binary search algorithm\n\n"));
    printf(_("Options:\n"));
    printf(_("  -r, --roman        Use Roman numerals instead of Arabic\n"));
    printf(_("  -h, --help         Display this help message\n"));
    printf(_("Description:\n"));
    printf(_("  The program guesses a number between 1 (I) and %d (%s) that you think of. It uses binary search algorithm.\n"), MAX_NUMBER, MAX_ROMAN_NUMBER);
    printf(_("  Answer with 'Yes' or 'No' to the questions asked by the program.\n\n"));
    printf(_("Examples:\n"));
    printf(_("  binsearch           Run with Arabic numerals (default)\n"));
    printf(_("  binsearch -r        Run with Roman numerals\n"));
}

/**
 * @brief Основная функция программы
 * 
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return 0 при успешном завершении, 1 при ошибке
 * 
 * Функция обрабатывает аргументы командной строки, настраивает локализацию
 * и запускает основной игровой цикл.
 */
int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain("binsearch", LOCALEDIR);
    textdomain("binsearch");
    
    static struct option long_options[] = {
        {"roman", no_argument, 0, 'r'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "rh", long_options, NULL)) != -1) {
        switch (opt) {
            case 'r': use_roman = 1; break;
            case 'h': print_help(); return 0;
            default:
                fprintf(stderr, _("Try '%s --help' for more information.\n"), argv[0]);
                return 1;
        }
    }

    if (use_roman) {
        printf(_("Think of a number between I and %s\n"), MAX_ROMAN_NUMBER);
    } else {
        printf(_("Think of a number between 1 and %d\n"), MAX_NUMBER);
    }

    const char *yes_str = _("Yes");
    const char *no_str = _("No");

    int L = 0, R = MAX_NUMBER;
    char ans[MAX_ANSWER_LEN + 1];
    while (R - L > 1) {
        int M = (L + R) / 2;

        if (use_roman) {
            char roman_buf[MAX_ROMAN_LEN + 1];
            arabic_to_roman(M, roman_buf);
            printf(_("Is the number greater than %s?\n"), roman_buf);
        } else {
            printf(_("Is the number greater than %d?\n"), M);
        }
        if (scanf("%10s", ans) != 1) {
			if (feof(stdin)) {
				fprintf(stderr, _("Error: standard input closed\n"));
			} else {
				fprintf(stderr, _("Error reading input\n"));
			}
			return 1;
		}
        if (!strcmp(ans, yes_str)) {
            L = M;
        } else if (!strcmp(ans, no_str)) {
            R = M;
        } else {
            fprintf(stderr, _("Incorrect answer. Two possible answers: Yes or No\n"));
        }
    }
    if (use_roman) {
        char roman_buf[MAX_ROMAN_LEN + 1];
        arabic_to_roman(R, roman_buf);
        printf(_("Your number is %s!\n"), roman_buf);
    } else {
        printf(_("Your number is %d!\n"), R);
    }
    return 0;
}