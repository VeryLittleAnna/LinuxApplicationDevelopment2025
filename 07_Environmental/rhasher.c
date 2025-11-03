#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <rhash.h>

// #define USE_READLINE

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#define MAX_LINE_LENGTH 1024
#define MAX_HASH_LENGTH 130

void compute_hash(const char *algorithm, const char *input, int is_file, int uppercase) {
    unsigned char output[MAX_HASH_LENGTH];
    char formatted_output[MAX_HASH_LENGTH];
    int output_format = (uppercase ? RHPR_HEX: RHPR_BASE64);
    
    int hash_id;
    if (strcasecmp(algorithm, "MD5") == 0) {
        hash_id = RHASH_MD5;
    } else if (strcasecmp(algorithm, "SHA1") == 0) {
        hash_id = RHASH_SHA1;
    } else if (strcasecmp(algorithm, "TTH") == 0) {
        hash_id = RHASH_TTH;
    } else {
        fprintf(stderr, "Error: Unknown algorithm '%s'\n", algorithm);
        return;
    }
    
    
    int result;
    if (is_file) {
        result = rhash_file(hash_id, input, output);
    } else {
        result = rhash_msg(hash_id, input, strlen(input), output);
    }
    
    if (result < 0) {
        if (is_file) {
            fprintf(stderr, "Error: Cannot open or read file '%s'\n", input);
        } else {
            fprintf(stderr, "Error: Failed to compute hash for string\n");
        }
        return;
    }
    
    if (rhash_print_bytes(formatted_output, output, rhash_get_digest_size(hash_id), output_format) < 0) {
        fprintf(stderr, "Error: Failed to format hash output\n");
        return;
    }
    
    printf("%s\n", formatted_output);
}

void process_command(const char *line) {
    char * token;
    char * line_copy = strdup(line);
    if (!line_copy) return;
    token = strtok(line_copy, " \t\n");
    if (!token) {
        free(line_copy);
        return;
    }
    
    char *algorithm = token;
    int uppercase = isupper((unsigned char)algorithm[0]);
    token = strtok(NULL, " \t\n");
    if (!token) {
        fprintf(stderr, "Error: Missing input\n");
        free(line_copy);
        return;
    }
    
    char *input = token;
    int is_file = 1;
    
    if (input[0] == '"') {
        is_file = 0;
        input++;
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '"') {
            input[len-1] = '\0';
        }
    }
    
    compute_hash(algorithm, input, is_file, uppercase);
    free(line_copy);
}

int main() {
    #ifdef USE_READLINE
        char *line = NULL;
    #else
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
    #endif
    
    rhash_library_init();
    
    printf("\tRhasher REPL. Ctrl+D to exit.\n");
    printf("\tSupported algorithms: MD5, SHA1, TTH\n");
    printf("\tUsage: <Algorithm> <file> or <Algorithm> \"string\"\n");
    
    while (1) {
#ifdef USE_READLINE
        line = readline("> ");
        if (!line) break;
        if (*line) add_history(line);
#else
        printf("> ");
        fflush(stdout);
        read = getline(&line, &len, stdin);
        if (read == -1) break;
        if (read > 0 && line[read-1] == '\n') {
            line[read-1] = '\0';
        }
#endif
        
        if (line && strlen(line) > 0) {
            process_command(line);
        }
        
#ifndef USE_READLINE
        free(line);
        line = NULL;
        len = 0;
#endif
    }
    
#ifdef USE_READLINE
    if (line) free(line);
#endif
    
    // printf("\nGoodbye!\n");
    return 0;
}