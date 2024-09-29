#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

typedef struct HtmlAttribute {
    char* name;
    char* value;
    struct HtmlAttribute* next;
} HtmlAttribute;

typedef struct HtmlElement {
    char* tag_name;
    HtmlAttribute* attributes;
    char* text_content;
    struct HtmlElement* children;
    struct HtmlElement* next;
} HtmlElement;



typedef struct HtmlTableFormal {
    HtmlElement* thead;
    HtmlElement* tbody;
    HtmlElement* tfoot;
} HtmlTableFormal;

typedef struct HtmlTableBasic {
    HtmlElement* tr;
    HtmlElement* tr_next;
} HtmlTableBasic;



typedef struct HtmlTable {
    union
    {
        HtmlTableFormal formal;
        HtmlTableBasic basic;
    } styledTable;
    int style;
} HtmlTable;

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

HtmlAttribute* parse_attributes(char* attribute_str) {
    HtmlAttribute* head = NULL;
    HtmlAttribute* current = NULL;

    char buffer[MAX_BUFFER_SIZE];
    char* name = buffer;
    char* value = NULL;
    int i = 0, j = 0;

    while (attribute_str[i]) {
        if (attribute_str[i] == '=') {
            buffer[j] = '\0';
            name = buffer;
            value = attribute_str + i + 1;
            j = 0;
            i++;
            while (attribute_str[i] != ' ' && attribute_str[i] != '\0' && attribute_str[i] != '"') {
                i++;
            }
            if (attribute_str[i] == '"') {
                i++;
                value = attribute_str + i;
                while (attribute_str[i] != '"' && attribute_str[i] != '\0') {
                    i++;
                }
            }
            attribute_str[i] = '\0';
            i++;

            HtmlAttribute* attr = malloc(sizeof(HtmlAttribute));
            attr->name = strdup(name);
            attr->value = strdup(value);
            attr->next = NULL;

            if (head == NULL) {
                head = current = attr;
            } else {
                current->next = attr;
                current = attr;
            }

            j = 0;
        } else if (attribute_str[i] == ' ') {
            buffer[j] = '\0';
            name = buffer;
            value = NULL;
            j = 0;
            i++;

            HtmlAttribute* attr = malloc(sizeof(HtmlAttribute));
            attr->name = strdup(name);
            attr->value = NULL;
            attr->next = NULL;

            if (head == NULL) {
                head = current = attr;
            } else {
                current->next = attr;
                current = attr;
            }
        } else {
            buffer[j++] = attribute_str[i++];
        }
    }

    if (j > 0) {
        buffer[j] = '\0';
        name = buffer;
        value = NULL;

        HtmlAttribute* attr = malloc(sizeof(HtmlAttribute));
        attr->name = strdup(name);
        attr->value = NULL;
        attr->next = NULL;

        if (head == NULL) {
            head = current = attr;
        } else {
            current->next = attr;
            current = attr;
        }
    }

    return head;
}

void parse_html_table(char* html, HtmlTable* table) {
    // table->thead = NULL;
    // table->tbody = NULL;
    // table->tfoot = NULL;

    HtmlElement* current_element = NULL;
    HtmlElement* current_row = NULL;

    char buffer[MAX_BUFFER_SIZE];
    char* tag_name;
    char* attributes;
    int i = 0;

    while (html[i]) {
        if (html[i] == '<') {
            int len = parse_tag(html + i, &tag_name, &attributes);
            if (len > 0) {
                if (strcmp(tag_name, "table") == 0) {
                    // Parse table attributes
                } else if (strcmp(tag_name, "thead") == 0) {
                    table->style = 1;
                    table->styledTable.formal = current_element = malloc(sizeof(HtmlElement));
                    current_element->tag_name = strdup(tag_name);
                    // current_element->attributes = parse_attributes(attributes);
                    current_element->children = NULL;
                    current_element->next = NULL;
                } else if (strcmp(tag_name, "tbody") == 0) {
                    table->tbody = current_element = malloc(sizeof(HtmlElement));
                    current_element->tag_name = strdup(tag_name);
                    //current_element->attributes = parse_attributes(attributes);
                    current_element->children = NULL;
                    current_element->next = NULL;
                } else if (strcmp(tag_name, "tfoot") == 0) {
                    table->tfoot = current_element = malloc(sizeof(HtmlElement));
                    current_element->tag_name = strdup(tag_name);
                    //current_element->attributes = parse_attributes(attributes);
                    current_element->children = NULL;
                    current_element->next = NULL;
                } else if (strcmp(tag_name, "tr") == 0) {
                    current_row = current_element = malloc(sizeof(HtmlElement));
                    current_element->tag_name = strdup(tag_name);
                    //current_element->attributes = parse_attributes(attributes);
                    current_element->children = NULL;
                    current_element->next = NULL;
                    if (table->thead) {
                        table->thead->children = current_element;
                    } else if (table->tbody) {
                        table->tbody->children = current_element;
                    } else if (table->tfoot) {
                        table->tfoot->children = current_element;
                    } else {
                        if (table->tr) {
                            table->tr->children = current_element;
                        } else {
                            table->tr = current_element;
                        }
                    }
                } else if (strcmp(tag_name, "th") == 0 || strcmp(tag_name, "td") == 0) {
                    HtmlElement* cell = malloc(sizeof(HtmlElement));
                    cell->tag_name = strdup(tag_name);
                    //cell->attributes = parse_attributes(attributes);
                    cell->text_content = NULL;
                    cell->children = NULL;
                    cell->next = NULL;
                    if (current_row) {
                        current_row->children = cell;
                        current_row = current_row->next;
                    }
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

    // parse_table(html);

    HtmlTable table;

    parse_html_table(html, &table);

    // const char* a = table.tr->tag_name;

    // printf("%s", a);
    return 0;
}
