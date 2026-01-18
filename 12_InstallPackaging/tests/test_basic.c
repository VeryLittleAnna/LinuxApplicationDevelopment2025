/**
 * @file test_basic.c
 * @brief Базовые unit tests для Mastermind
 * 
 * Простые последовательные тесты без внешних зависимостей.
 * Тесты: создание и очистка игры, генерация кода, проверка, управление состоянием игры и историей.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define _(STRING) (STRING)
#define N_(STRING) (STRING)


#include "../src/lib/mastermind.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int passed;
    int failed;
    int total;
} TestStats;

static TestStats stats = {0, 0, 0};

#define TEST_ASSERT(cond, msg) \
    do { \
        stats.total++; \
        if (!(cond)) { \
            fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, msg); \
            stats.failed++; \
        } else { \
            stats.passed++; \
        } \
    } while(0)

void test_game_create_clean() {
    printf("Test 1: Game creation and cleanup... \n");
    
    Game* game = game_create();
    TEST_ASSERT(game != NULL, "game_create failed");
    TEST_ASSERT(game->attempts == 0, "Initial attempts should be 0");
    TEST_ASSERT(game->history != NULL, "History should be initialized");
    TEST_ASSERT(game->history->size == 0, "Initial history size should be 0");
    
    game_clean(game);
}

void test_generate_secret_code() {
    printf("Test 2: Secret code generation... ");
    Game game;
    for (int i = 0; i < 10; i++) {
        _generate_secret_code(&game);
        
        TEST_ASSERT(strlen(game.secret_code) == CODE_LENGTH,
               "Code length should be CODE_LENGTH");
        
        for (int j = 0; j < CODE_LENGTH; j++) {
            char* found = strchr(ALPHABET, game.secret_code[j]);
            TEST_ASSERT(found != NULL,
                   "Code characters must be from ALPHABET");
        }
        TEST_ASSERT(game.secret_code[CODE_LENGTH] == '\0', 
               "Code should be null-terminated");
    }
}

void test_is_valid_guess() {
    printf("Test 3: Guess validation... \n");
    TEST_ASSERT(_is_valid_guess("1234") == 1, "Valid guess rejected");
    TEST_ASSERT(_is_valid_guess("6666") == 1, "Valid guess rejected");
    TEST_ASSERT(_is_valid_guess("123") == 0, "Invalid guess accepted");
    TEST_ASSERT(_is_valid_guess("12345") == 0, "Invalid guess accepted");
    TEST_ASSERT(_is_valid_guess("12a3") == 0, "Invalid guess accepted");
}

void test_game_is_over() {
    printf("Test 4: Game over conditions... ");
    
    Game* game = game_create();
    TEST_ASSERT(game_is_over(game) == 0, "New game should not be over");
    strncpy(game->secret_code, "1234", CODE_LENGTH);
    game->secret_code[CODE_LENGTH + 1] = '\0';
    
    Feedback feedback = {CODE_LENGTH, 0};
    
    strcpy(game->history->guesses[0], "1234");
    game->history->results[0] = feedback;
    game->history->size = 1;
    game->attempts = 1;
    game->game_over = 0;
    
    Feedback test_feedback = game_check_guess(game, "1234");
    TEST_ASSERT(test_feedback.bulls == CODE_LENGTH, "4 bulls should by for correct answer");
    TEST_ASSERT(game_is_over(game) == 1, "Game should be over after win");
    

    game_clean(game);
    game = game_create();
    
    game->attempts = MAX_ATTEMPTS;
    game->game_over = 0;
    
    Feedback dummy = {0, 0};
    strcpy(game->history->guesses[0], "1111");
    game->history->results[0] = dummy;
    game->history->size = 1;
    
    TEST_ASSERT(game_is_over(game) == 1, "Game should be over after max attempts");
    
    game_clean(game);
    printf("PASS\n");
}

void test_add_step_to_history() {
    printf("Test 5: History management... ");
    
    Game* game = game_create();
    
    Feedback feedback1 = {2, 1};
    Feedback feedback2 = {3, 0};
    Feedback feedback3 = {0, 4};
    
    add_step_to_history(game, "1234", feedback1);
    TEST_ASSERT(game->history->size == 1, "History size should be 1");
    TEST_ASSERT(strcmp(game->history->guesses[0], "1234") == 0, 
           "First guess should be '1234'");
    TEST_ASSERT(game->history->results[0].bulls == 2, 
           "First result bulls should be 2");
    TEST_ASSERT(game->history->results[0].cows == 1, 
           "First result cows should be 1");
    
    add_step_to_history(game, "5678", feedback2);
    TEST_ASSERT(game->history->size == 2, "History size should be 2");
    
    add_step_to_history(game, "4321", feedback3);
    TEST_ASSERT(game->history->size == 3, "History size should be 3");
    
    TEST_ASSERT(game->history->guesses[0][CODE_LENGTH] == '\0', 
           "Guess should be null-terminated");
    TEST_ASSERT(game->history->guesses[1][CODE_LENGTH] == '\0', 
           "Guess should be null-terminated");
    TEST_ASSERT(game->history->guesses[2][CODE_LENGTH] == '\0', 
           "Guess should be null-terminated");
    
    for (int i = 3; i < MAX_ATTEMPTS + 5; i++) {
        char guess[CODE_LENGTH + 1];
        snprintf(guess, sizeof(guess), "111%d", i % 10);
        add_step_to_history(game, guess, feedback1);
    }
    
    TEST_ASSERT(game->history->size <= MAX_ATTEMPTS, 
           "History should not exceed MAX_ATTEMPTS");
    
    game_clean(game);
}

int main(void) {
    printf("=== Running basic Mastermind tests ===\n\n");
    
    test_game_create_clean();
    test_generate_secret_code();
    test_is_valid_guess();
    test_game_is_over();
    test_add_step_to_history();
    
    printf("\n=== Test Results ===\n");
    printf("Total tests: %d\n", stats.total);
    printf("Passed: %d\n", stats.passed);
    printf("Failed: %d\n", stats.failed);
    
    if (stats.failed > 0) {
        return 1;
    }
    
    printf("\nAll tests passed!\n");
    return 0;
}