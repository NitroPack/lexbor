/*
 * Copyright (C) 2019-2020 Alexander Borisov
 *
 * Author: Alexander Borisov <borisov@lexbor.com>
 */

#include "base.h"

lxb_char_t* readStdIn()
{
    int c;
    size_t p4kB = 4096, i = 0;
    void *newPtr = NULL;
    lxb_char_t *ptrString = malloc(p4kB * sizeof (lxb_char_t));

    while (ptrString != NULL && (c = getchar()) != EOF)
    {
        if (i == p4kB * sizeof (lxb_char_t))
        {
            p4kB += 4096;
            if ((newPtr = realloc(ptrString, p4kB * sizeof (lxb_char_t))) != NULL)
                ptrString = (lxb_char_t*) newPtr;
            else
            {
                free(ptrString);
                return NULL;
            }
        }
        ptrString[i++] = c;
    }

    if (ptrString != NULL)
    {
        ptrString[i] = '\0';
        ptrString = realloc(ptrString, strlen(ptrString) + 1);
    } 
    else return NULL;

    return ptrString;
}

int main(int argc, const char *argv[])
{
    lxb_status_t status;
    lxb_html_parser_t *parser;
    lxb_html_document_t *doc;

    lxb_char_t *html = readStdIn();
    size_t html_len = strlen(html);

    /* Initialization */
    parser = lxb_html_parser_create();
    status = lxb_html_parser_init(parser);

    if (status != LXB_STATUS_OK) {
        FAILED("Failed to create HTML parser");
    }

    /* Parse */
    doc = lxb_html_parse(parser, html, html_len);
    if (doc == NULL) {
        FAILED("Failed to create Document object");
    }

    /* Destroy parser */
    lxb_html_parser_destroy(parser);

    status = lxb_html_serialize_tree_cb(lxb_dom_interface_node(doc),
                                               serializer_callback, NULL);
    if (status != LXB_STATUS_OK) {
        FAILED("Failed to serialize HTML tree");
    }

    /* Destroy */
    lxb_html_document_destroy(doc);

    return 0;
}
