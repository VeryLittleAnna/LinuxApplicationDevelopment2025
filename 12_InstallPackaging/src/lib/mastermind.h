/**
 * @file mastermind.h
 * @brief Заголовочный файл библиотеки Mastermind
 * 
 * Объявления функций и структур для игры "Быки и коровы".
 * Поддерживает историю попыток, подсказки и локализацию.
 * 
 * @author VeryLittleAnna
 * @date 2026
 */
#define CODE_LENGTH 4
#define ALPHABET "123456"
#define MAX_ATTEMPTS 10
#define BUFFER_SIZE 100

typedef struct Game Game;
typedef struct History History;

// Другое название игры - "Быки и коровы". Бык - корректный символ на своей позиции, корова - существующий символ, но на другой позиции
typedef struct {
    int bulls;
    int cows;
} Feedback;

Game* game_create(void);

void game_clean(Game* game);

void add_step_to_history(Game* game, const char* guess, Feedback feedback);

Feedback game_check_guess(Game* game, const char* guess);

void print_history(const Game* game);

char* get_verdict(const Game *game);

void get_hint(Game* game);

int game_is_over(const Game* game);
void print_feedback(Feedback* feedback);


struct Game {
    char secret_code[CODE_LENGTH + 1];
    int attempts;
    History* history;
    int has_hints;
    int game_over;
};

struct History {
    char guesses[MAX_ATTEMPTS][CODE_LENGTH + 1];
    Feedback results[MAX_ATTEMPTS];
    int size;
};

int _is_valid_guess(const char* guess);
void _generate_secret_code(Game* game);
