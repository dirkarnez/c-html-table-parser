HtmlTable* parse_html_table(char* html) {
    HtmlTable* table = malloc(sizeof(HtmlTable));
    table->thead = NULL;
    table->tbody = NULL;
    table->tfoot = NULL;

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
                    table->thead = current_element = malloc(sizeof(HtmlElement));
                    current_element->tag_name = strdup(tag_name);
                    current_element->attributes = parse_attributes(attributes);
                    current_element->children = NULL;
                    current_element->next = NULL;
                } else if (strcmp(tag_name, "tbody") == 0) {
                    table->tbody = current_element = malloc(sizeof(HtmlElement));
                    current_element->tag_name = strdup(tag_name);
                    current_element->attributes = parse_attributes(attributes);
                    current_element->children = NULL;
                    current_element->next = NULL;
                } else if (strcmp(tag_name, "tfoot") == 0) {
                    table->tfoot = current_element = malloc(sizeof(HtmlElement));
                    current_element->tag_name = strdup(tag_name);
                    current_element->attributes = parse_attributes(attributes);
                    current_element->children = NULL;
                    current_element->next = NULL;
                } else if (strcmp(tag_name, "tr") == 0) {
                    current_row = current_element = malloc(sizeof(HtmlElement));
                    current_element->tag_name = strdup(tag_name);
                    current_element->attributes = parse_attributes(attributes);
                    current_element->children = NULL;
                    current_element->next = NULL;
                    if (table->thead) {
                        table->thead->children = current_element;
                    } else if (table->tbody) {
                        table->tbody->children = current_element;
                    } else if (table->tfoot) {
                        table->tfoot->children = current_element;
                    }
                } else if (strcmp(tag_name, "th") == 0 || strcmp(tag_name, "td") == 0) {
                    HtmlElement* cell = malloc(sizeof(HtmlElement));
                    cell->tag_name = strdup(tag_name);
                    cell->attributes = parse_attributes(attributes);
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

    return table;
}
