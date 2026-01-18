/**
 * @file test_in_output.c
 * @brief Input/output tests для Mastermind
 * 
 * Тесты про функций проверки догадок, истории, подсказок с перенаправление стандартного ввода-вывода.
 * Tests: game_check_guess output, print_history, get_hint.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define _(STRING) (STRING)
#define N_(STRING) (STRING)

#include "../src/lib/mastermind.h"

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

typedef struct {
    FILE* original_stdout;
    FILE* captured_file;
} OutputCapture;

OutputCapture start_capture(void) {
    OutputCapture cap;
    cap.original_stdout = stdout;
    cap.captured_file = tmpfile();
    if (cap.captured_file == NULL) {
        fprintf(stderr, "Error: Failed to create temporary file\n");
        exit(1);
    }
    stdout = cap.captured_file;
    return cap;
}

char* end_capture(OutputCapture cap, char* buffer, size_t size) {
    stdout = cap.original_stdout;
    rewind(cap.captured_file);
    
    size_t i = 0;
    int c;
    while ((c = fgetc(cap.captured_file)) != EOF && i < size - 1) {
        buffer[i++] = (char)c;
    }
    buffer[i] = '\0';
    
    fclose(cap.captured_file);
    return buffer;
}

// для тестирований идейной части без привязки к языку
void extract_digits(const char* input, char* digits) {
    int j = 0;
    for (int i = 0; input[i] != '\0' && j < 19; i++) {
        if (isdigit((unsigned char)input[i])) {
            digits[j++] = input[i];
        }
    }
    digits[j] = '\0';
}

int contains_digits(const char* str, const char* expected_digits) {
    char extracted[20];
    extract_digits(str, extracted);
    return strstr(extracted, expected_digits) != NULL;
}

void test_game_check_guess_output(void) {
    printf("Test 1: game_check_guess output for three attempts... ");
    
    Game* game = game_create();
    TEST_ASSERT(game != NULL, "Failed to create game");
    
    strncpy(game->secret_code, "1234", CODE_LENGTH);
    game->secret_code[CODE_LENGTH] = '\0';
        
    Feedback feedback1 = game_check_guess(game, "1256");
    TEST_ASSERT(feedback1.bulls == 2, "Feedback should have 2 bulls for '1256'");
    TEST_ASSERT(feedback1.cows == 0, "Feedback should have 0 cows for '1256'");
    
    Feedback feedback2 = game_check_guess(game, "4321");
    TEST_ASSERT(feedback2.bulls == 0, "Feedback should have 0 bulls for '4321'");
    TEST_ASSERT(feedback2.cows == 4, "Feedback should have 4 cows for '4321'");
    
    Feedback feedback3 = game_check_guess(game, "1234");
    TEST_ASSERT(feedback3.bulls == 4, "Feedback should have 4 bulls for '1234'");
    TEST_ASSERT(feedback3.cows == 0, "Feedback should have 0 cows for '1234'");

    TEST_ASSERT(game->attempts == 3, "Should have 3 attempts");
    TEST_ASSERT(game->history->size == 3, "History should have 3 entries");
    TEST_ASSERT(game_is_over(game) == 1, "Game should be over after correct guess");
    
    game_clean(game);
    printf("PASS\n");
}

void test_print_history_output(void) {
    printf("Test 2: print_history output (digits only)... ");
    
    Game* game = game_create();
    TEST_ASSERT(game != NULL, "Failed to create game");
    
    strncpy(game->secret_code, "1234", CODE_LENGTH);
    game->secret_code[CODE_LENGTH] = '\0';
    
    game_check_guess(game, "1256"); // (2, 0)
    game_check_guess(game, "4321"); // (0, 4)
    game_check_guess(game, "1234"); // (4, 0) - correct
    
    OutputCapture cap = start_capture();
    print_history(game);
    
    char output[1024];
    end_capture(cap, output, sizeof(output));
    
    char digits[50];
    extract_digits(output, digits);
    
    // Should contain: 1256, 2, 0, 4321, 0, 4, 1234, 4, 0
    const char* expected_sequence = "125620432104123440";
    fprintf(stderr, "\tDEBUG: %s\n", digits);
    fprintf(stderr, "\tDEBUG: %s\n", output);
    TEST_ASSERT(contains_digits(digits, expected_sequence), "Should match 1256, 2, 0, 4321, 0, 4, 1234, 4, 0");
    
    game_clean(game);
}

void test_get_hint_output(void) {
    printf("Test 3: get_hint output (digits only)... ");
    
    Game* game = game_create();
    TEST_ASSERT(game != NULL, "Failed to create game");
    
    strncpy(game->secret_code, "1234", CODE_LENGTH);
    game->secret_code[CODE_LENGTH] = '\0';
    
    char output[256];
    char digits[20];
    
    OutputCapture cap1 = start_capture();
    get_hint(game);
    end_capture(cap1, output, sizeof(output));
    extract_digits(output, digits);
    TEST_ASSERT(strstr(digits, "1") != NULL, "First hint should reveal digit '1'");
    TEST_ASSERT(game->has_hints == 1, "Hint counter should be 1");
    
    OutputCapture cap2 = start_capture();
    get_hint(game);
    end_capture(cap2, output, sizeof(output));
    extract_digits(output, digits);
    TEST_ASSERT(strstr(digits, "2") != NULL, "Second hint should reveal digit '2'");
    TEST_ASSERT(game->has_hints == 2, "Hint counter should be 2");
    
    game_clean(game);
}

int main(void) {
    printf("=== Running I/O tests for Mastermind ===\n\n");
    
    test_game_check_guess_output();
    test_print_history_output();
    test_get_hint_output();
    
    printf("\n=== Test Results ===\n");
    printf("Total assertions: %d\n", stats.total);
    printf("Passed: %d\n", stats.passed);
    printf("Failed: %d\n", stats.failed);
    
    if (stats.failed > 0) {
        fprintf(stderr, "\nSome tests failed!\n");
        return 1;
    }
    
    printf("\nAll I/O tests passed!\n");
    return 0;
}