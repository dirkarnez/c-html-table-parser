#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

// Function to parse an HTML tag
int parse_tag(char* buffer, char** tag_name, char** attributes) {
    char* start = buffer;
    char* end = strchr(buffer, '>');
    if (!end) return 0;
    *end = '\0';

    // Extract the tag name
    char* space = strchr(start, ' ');
    if (space) {
        *space = '\0';
        *tag_name = start + 1;
        *attributes = space + 1;
    } else {
        *tag_name = start + 1;
        *attributes = NULL;
    }

    return end - start + 1;
}

// Function to parse an HTML table
void parse_table(char* html) {
    char buffer[MAX_BUFFER_SIZE];
    char* tag_name;
    char* attributes = NULL;
    int i = 0;

    while (html[i]) {
        if (html[i] == '<') {
            int len = parse_tag(html + i, &tag_name, &attributes);
            if (len > 0) {
                if (strcmp(tag_name, "table") == 0) {
                    printf("Found table tag %d %s\n", len, attributes);
                    // Parse table content here
                } else if (strcmp(tag_name, "tr") == 0) {
                    printf("Found tr tag %d %s\n", len, attributes);
                    // Parse tr content here
                } else if (strcmp(tag_name, "th") == 0) {
                    printf("Found th tag %d %s\n", len, attributes);
                    // Parse th/td content here
                } else if (strcmp(tag_name, "td") == 0) {
                    printf("Found td tag %d %s\n", len, attributes);
                }
                i += len;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
}

int main() {
    char html[] = "<table style=\"width: 100%\">"
                  "  <tr>"
                  "    <th>Name</th>"
                  "    <th>Age</th>"
                  "  </tr>"
                  "  <tr>"
                  "    <td>John</td>"
                  "    <td>25</td>"
                  "  </tr>"
                  "  <tr>"
                  "    <td>Jane</td>"
                  "    <td>30</td>"
                  "  </tr>"
                  "</table>";

    parse_table(html);
    return 0;
}
