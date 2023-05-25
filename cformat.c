#include <stdio.h>
#include <string.h>

#define MAX_TOK_SIZE 100
#define TAB_SIZE 8
#define MAX_LINE_LEN -1

void new_line_with_indent(FILE* f, int n)
{
        puts("");
        for (int i = 1; i <= n; ++i) putchar('\t');
        fputs("\n", f);
        for (int i = 1; i <= n; ++i) fprintf(f, "\t");
}

int is_control_keyword(char token[])
{
        return !strcmp(token, "if") || !strcmp(token, "while") || !strcmp(token, "switch") || !strcmp(token, "do");
}

int main(int argc, char* argv[])
{
        FILE* input_file = fopen(argv[1], "r");
        FILE* output_file = fopen("out.c", "w");
        char c;
        while (strchr(" \t", c = fgetc(input_file)));
        int indent = 0, new_line = 0, is_control = 0;
        char token[MAX_TOK_SIZE];
        char* tok = NULL;
        int len = 0;
        while (c != EOF) {
                if (tok && (c < 'a' || c > 'z')) {
                        *tok = '\0';
                        is_control = is_control_keyword(token);
                        tok = NULL;
                }
                switch (c) {
                case '{':
                        if (!is_control) {
                                printf("  // len = %d", len);
                                new_line_with_indent(output_file, indent);
                                len = TAB_SIZE * indent;
                        }
                        ++indent;
                        break;
                case '}':
                        --indent;
                        printf("  // len = %d", len);
                        new_line_with_indent(output_file, indent);
                        len = TAB_SIZE * indent;
                        break;
                case ';':
                        break;
                case '\n':
                        c = fgetc(input_file);
                        continue;
                default:
                        if (new_line) {
                                printf("  // len = %d", len);
				 new_line_with_indent(output_file, indent);
                                len = TAB_SIZE * indent;
                                new_line = 0;
                                tok = token;
                        }
                        if (len == MAX_LINE_LEN) {
                                new_line_with_indent(output_file, indent + 1);
                                len = TAB_SIZE * (indent + 1);
                        }
                        putchar(c);
                        fprintf(output_file, "%c", c);
                        ++len;
                        if (tok) *tok++ = c;
                        c = fgetc(input_file);
                        continue;
                }
                if (len == MAX_LINE_LEN) {
                        new_line_with_indent(output_file, indent + 1);
                        len = TAB_SIZE * (indent + 1);
                }
                putchar(c);
                fprintf(output_file, "%c", c);
                ++len;
                new_line = 1;
                while (strchr(" \t", c = fgetc(input_file)));
        }
        puts("");
        fputs("\n", output_file);
        fclose(input_file);
        fclose(output_file);
}
