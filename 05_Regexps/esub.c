#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>

#define MAX_GROUPS 10

const char* COLORS[] = {
    "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m",
    "\033[36m", "\033[91m", "\033[92m", "\033[93m", "\033[0m" 
};

char* process_match(const char* substitution, const regmatch_t* matches, const char* original, const int use_color) {
    size_t sub_len = strlen(substitution);
    char* result = malloc(sub_len * 5 + 1); // for colors
    if (!result) {
        fprintf(stderr, "Error in malloc\n");
        return NULL;
    }
    size_t result_len = 0, i = 0;
    while (i < sub_len) {
        if (substitution[i] == '\\') {
            if (i + 1 < sub_len) {
                if (substitution[i + 1] == '\\') {
                    result[result_len++] = '\\';
                    i += 2;
                } else if (substitution[i + 1] >= '0' && substitution[i + 1] <= '9') {
                    int group_num = substitution[i + 1] - '0';
                    
                    if (group_num < MAX_GROUPS && matches[group_num].rm_so != -1) {
                        int start = matches[group_num].rm_so;
                        int end = matches[group_num].rm_eo;
                        int length = end - start;
                        
                        if (use_color && group_num > 0) {
                            const char* color = COLORS[group_num - 1];
                            size_t color_len = strlen(color);
                            memcpy(result + result_len, color, color_len);
                            result_len += color_len;
                        }
                        
                        memcpy(result + result_len, original + start, length);
                        result_len += length;
                        
                        if (use_color && group_num > 0) {
                            const char* reset_color = COLORS[9];
                            size_t reset_len = strlen(reset_color);
                            memcpy(result + result_len, reset_color, reset_len);
                            result_len += reset_len;
                        }
                    } else {
                        fprintf(stderr, "Regex error: reference to non-existent group \\%d\n", group_num);
                        free(result);
                        return NULL;
                    }
                    i += 2;
                } else {
                    result[result_len++] = substitution[i++];
                    result[result_len++] = substitution[i++];
                }
            } else {
                result[result_len++] = substitution[i++];
            }
        } else {
            result[result_len++] = substitution[i++];
        }
    }
    result[result_len] = '\0';
    return result;
}

void print_regexp_error(int errcode, const regex_t *preg) {
    char error_msg[1024];
    regerror(errcode, preg, error_msg, sizeof(error_msg));
    fprintf(stderr, "Regex error: %s\n", error_msg);
}

int replace(const char* input, const char* regexp_str, const char* substitution, const int use_color) {
    regex_t regex;
    regmatch_t matches[MAX_GROUPS];    
    int regex_result = regcomp(&regex, regexp_str, REG_EXTENDED);
    if (regex_result != 0) {
        print_regexp_error(regex_result, &regex);
        return 1;
    }
    
    regex_result = regexec(&regex, input, MAX_GROUPS, matches, 0);
    if (regex_result == REG_NOMATCH) {
        printf("%s\n", input);
        
        regfree(&regex);
        return 0;
    } else if (regex_result != 0) {
        print_regexp_error(regex_result, &regex);
        
        regfree(&regex);
        return 1;
    }
    char* result_sub = process_match(substitution, matches, input, use_color);
    if (!result_sub) {
        
        regfree(&regex);
        return 1;
    }
    int last_end = 0;
    
    if (matches[0].rm_so > 0) {
        fprintf(stdout, "%.*s", (int)matches[0].rm_so, input);
    }
    printf("%s", result_sub);
    
    free(result_sub);
    last_end = matches[0].rm_eo;
    if (input[last_end] != '\0') {
        printf("%s", input + last_end);
    }
    printf("\n");
    
    regfree(&regex);
    return 0;
}

int parse_arguments(int argc, char* argv[], char** regexp, char** substitution, char** input_string, int* use_color) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--color") == 0) {
            *use_color = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            return -1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            return 1;
        }
    }
    if (argc < 4 + *use_color) {
        fprintf(stderr, "Error: need 3 arguments, see help\n");
        return 1;
    }
    *regexp = argv[1 + *use_color];
    *substitution = argv[2 + *use_color];
    *input_string = argv[3 + *use_color];
    return 0;
}

int main(int argc, char* argv[]) {
    char* regexp;
    char* substitution;
    char* input_string;
    int use_color = 0;
    
    int parse_args_res = parse_arguments(argc, argv, &regexp, &substitution, &input_string, &use_color);
    if (parse_args_res == -1) {
        printf("Usage: esub [OPTIONS] regexp substitution [string]\n");
        printf("Options:\n\
        -c, --color    Colorize capture groups in output\n\
        -h, --help     Show this help message\n\n");
        return 1;
    } else if (parse_args_res != 0 || !substitution || !input_string || !regexp) {
        fprintf(stderr, "Error in parsing");
        return 1;
    }

    int result = replace(input_string, regexp, substitution, use_color);
    
    return result == 0 ? 0 : 1;
}