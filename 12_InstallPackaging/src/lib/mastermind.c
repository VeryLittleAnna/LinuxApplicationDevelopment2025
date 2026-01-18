
/**
 * @file mastermind.c
 * @brief Реализация логики игры Mastermind (Быки и коровы)
 *
 * Этот файл содержит реализацию логики игры:
 * - Создание и управление игровой сессией
 * - Проверка догадок с алгоритмом "быки и коровы"
 * - Управление историей попыток
 * - Работа с подсказками
 * 
 * @author VeryLittleAnna
 * @date 2026
 *
 * @note Для локализации используется gettext
 * @see mastermind.h для объявлений функций
 */


#include "mastermind.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <locale.h>
#include <libintl.h>
#include <stdlib.h>
#include <time.h>

#define _(STRING) gettext(STRING)
#define N_(STRING) (STRING)

/**
 * Создает и инициализирует новую игровую сессию
 * 
 * Выделяет память для структур Game и History, инициализирует все поля
 * начальными значениями и генерирует случайный секретный код.
 * 
 * @return указатель на созданную игру или NULL при ошибке выделения памяти
 * 
 * @note Память должна быть освобождена с помощью game_clean()
 * @warning Не проверяет результат malloc - может привести к падению
 */
Game* game_create() {
    Game* game = (Game*)malloc(sizeof(Game));
    
    game->attempts = 0;
    game->game_over = 0;
    game->history = (History*)malloc(sizeof(History));
    game->history->size = 0;
    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        game->history->guesses[i][0] = '\0';
    }
    game->has_hints = 0;
    _generate_secret_code(game);
    
    return game;
}

/**
 * Добавляет результат попытки в историю игры
 * 
 * Сохраняет догадку игрока и соответствующий результат Feedback
 * в массивах истории для последующего отображения.
 * 
 * @param game указатель на текущую игровую сессию
 * @param guess строка с догадкой игрока (должна быть валидной)
 * @param feedback результат проверки догадки
 * 
 * @note Пропускает добавление если история заполнена (MAX_ATTEMPTS достигнут)
 */
void add_step_to_history(Game* game, const char* guess, Feedback feedback) {
    if (game->history->size >= MAX_ATTEMPTS) {
        return;
    }
    int current_size = game->history->size;
    strncpy(game->history->guesses[current_size], guess, CODE_LENGTH);
    game->history->guesses[current_size][CODE_LENGTH] = '\0';
    
    game->history->results[current_size] = feedback;
    game->history->size++;
}


/**
 * Проверяет догадку игрока и обновляет состояние игры
 * 
 * Вычисляет количество быков (правильные цифры на правильных позициях)
 * и коров (правильные цифры на неправильных позициях), сохраняет результат
 * в истории и обновляет счетчик попыток.
 * 
 * @param game указатель на текущую игровую сессию
 * @param guess строка с догадкой игрока
 * 
 * @return Возвращает результат в формате "(быки, коровы)"
 * @note Завершает игру при достижении MAX_ATTEMPTS или правильной догадке
 */
Feedback game_check_guess(Game* game, const char* guess) {
    Feedback feedback = {0, 0};
    
    if (game == NULL || guess == NULL || !_is_valid_guess(guess)) {
        return feedback;
    }
    int code_used[CODE_LENGTH] = {0};
    int guess_used[CODE_LENGTH] = {0};
    
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == game->secret_code[i]) {
            feedback.bulls++;
            code_used[i] = 1;
            guess_used[i] = 1;
        }
    }
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess_used[i]) continue;
        for (int j = 0; j < CODE_LENGTH; j++) {
            if (!code_used[j] && guess[i] == game->secret_code[j]) {
                feedback.cows++;
                code_used[j] = 1;
                guess_used[i] = 1;
                break;
            }
        }
    }
    
    add_step_to_history(game, guess, feedback);
    game->attempts++;
    if (game->attempts >= MAX_ATTEMPTS || feedback.bulls == CODE_LENGTH) game->game_over = 1;
    return feedback;
}

/**
 * Выводит результат в формате "(быки, коровы)"
 * 
 * @param feedback указатель на результат

*/
void print_feedback(Feedback* feedback) {
    printf(_("(%d, %d)\n"), feedback->bulls, feedback->cows);
}

/**
 * Освобождает ресурсы, занятые игровой сессией
 * 
 * Корректно освобождает всю динамически выделенную память
 * для структур Game и History.
 * 
 * @param game указатель на игру для очистки
 * 
 * @warning Передача NULL может привести к неопределенному поведению
 *          из-за отсутствия проверки указателя game
 */
void game_clean(Game* game) {
    if (game->history != NULL) {
        free(game->history);
    }
    free(game);
}

/**
 * Выводит на экран историю всех попыток текущей игры
 * 
 * Отображает все сохраненные догадки с соответствующими результатами
 * в формате "догадка - (быки, коровы)".
 * 
 * @param game указатель на текущую игровую сессию
 */
void print_history(const Game* game) {
    printf(_("История в формате: догадка - (быки, коровы)\n"));
    for (int i = 0; i < game->history->size; ++i) {
        printf("%s - (%d, %d)\n", \
            game->history->guesses[i], \
            game->history->results[i].bulls, \
            game->history->results[i].cows \
        );
    }
    printf("\n");
}

/**
 * Проверяет корректность формата догадки
 * 
 * Валидирует догадку по двум критериям:
 * 1. Длина строки должна равняться CODE_LENGTH
 * 2. Все символы должны принадлежать алфавиту ALPHABET
 * 
 * @param guess строка догадки для проверки
 * @return 1 если догадка валидна, 0 в противном случае
 * 
 * @internal
 * Эта функция используется только внутри библиотеки
 */
int _is_valid_guess(const char* guess) {
    if (strlen(guess) != CODE_LENGTH) {
        return 0;
    }
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (strchr(ALPHABET, guess[i]) == NULL) {
            return 0;
        }
    }
    return 1;
}

/**
 * Генерирует случайный секретный код для новой игры
 * 
 * Создает строку из CODE_LENGTH символов, каждый из которых
 * случайно выбирается из алфавита ALPHABET.
 * 
 * @param game указатель на игру для установки секретного кода
 * 
 * @note Использует текущее время как seed для генератора случайных чисел
 */
void _generate_secret_code(Game* game) {
    if (game == NULL) return;
    srand(time(NULL));
    for (int i = 0; i < CODE_LENGTH; i++) {
        int index = rand() % (strlen(ALPHABET));
        game->secret_code[i] = ALPHABET[index];
    }
    game->secret_code[CODE_LENGTH] = '\0';
}

/**
 * Определяет и возвращает итоговый вердикт игры
 * 
 * Анализирует состояние игры и возвращает соответствующее
 * текстовое сообщение о результате (победа, поражение, прерывание).
 * 
 * @param game указатель на завершенную игровую сессию
 * @return строка с вердиктом или NULL если состояние не определено
 * 
 * @note Использует статический буфер для форматированных строк
 */
char* get_verdict(const Game *game) {
    static char buffer[BUFFER_SIZE];
    if (!game_is_over(game)) {
        snprintf(buffer, sizeof(buffer), _("Игра прервана. Правильный ответ был: %s\n"), game->secret_code);
        return buffer;
    }
    Feedback feedback = game->history->results[game->history->size - 1];
    if (feedback.bulls == CODE_LENGTH && game->attempts <= MAX_ATTEMPTS) {
        if (game->has_hints == 0) {
            return _("Поздравляю! Вы победили!\n");
        } else {
            snprintf(buffer, sizeof(buffer), _("Вы победили с %d подсказками!\n"), game->has_hints);
            return buffer;
        }
    } else if (game->attempts >= MAX_ATTEMPTS) {
        snprintf(buffer, sizeof(buffer), _("Игра окончена. Правильный ответ был: %s\n"), game->secret_code);
    return buffer;
    }
    return NULL;
}

/**
 * Предоставляет игроку подсказку о секретном коде
 * 
 * Показывает один правильный символ из секретного кода
 * на позиции, соответствующей количеству уже данных подсказок.
 * 
 * @param game указатель на текущую игровую сессию
 * 
 * @note Подсказки даются последовательно от первой позиции к последней
 * @note После CODE_LENGTH подсказок показывает последнюю позицию
 */
void get_hint(Game* game) {
    int position = game->has_hints >= CODE_LENGTH ? CODE_LENGTH - 1 : game->has_hints; 
    printf(_("Верная цифра на позиции %d - %c\n"), position + 1, game->secret_code[position]);
    game->has_hints++;
}

/**
 * Определяет, завершена ли игра
 * 
 * Проверяет условия завершения игры:
 * 1. Флаг game_over установлен вручную
 * 2. Игрок угадал код (bulls == CODE_LENGTH)
 * 3. Исчерпаны все попытки (attempts >= MAX_ATTEMPTS)
 * 
 * @param game указатель на проверяемую игровую сессию
 * @return 1 если игра завершена, 0 если продолжается
 */
int game_is_over(const Game* game) {
    if (game->game_over) {
        return 1;
    }
    if (game->history->size == 0) {
        return 0;
    }
    Feedback feedback = game->history->results[game->history->size - 1];
    return (feedback.bulls == CODE_LENGTH || game->attempts >= MAX_ATTEMPTS) ? 1 : 0;
}

