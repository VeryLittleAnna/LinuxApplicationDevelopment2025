#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <locale.h>
#include <libgen.h>
#include <libintl.h>

#define _(STRING) gettext(STRING)
#define N_(STRING) (STRING)


int main() {
    setlocale(LC_ALL, "");
    bindtextdomain("binsearch", LOCALEDIR);
    textdomain("binsearch");

    printf(_("Think of a number between 1 and 100\n"));

    const char *yes_str = _("Yes");
    const char *no_str = _("No");

    int L = 0, R = 100;
    char ans[11];
    while (R - L > 1) {
        int M = (L + R) / 2;
        printf(_("Is the number greater than %d?\n"), M);
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
    printf(_("Your number is %d!\n"), R);
    return 0;
}