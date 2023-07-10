#include <stdio.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 8080

static int send_response(struct MHD_Connection *connection, const char *content)
{
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(content), (void *)content, MHD_RESPMEM_PERSISTENT);
    if (response == NULL)
        return MHD_NO;

    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

static int handle_request(void *data, struct MHD_Connection *connection, const char *url, const char *method,
                          const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls)
{
    if (strcmp(method, "GET") == 0 && strcmp(url, "/") == 0)
    {
        const char *content = "<html><body>"
                              "<h1>Directory Listing</h1>"
                              "<ul>"
                              "<li><a href=\"/folder1/\">Folder 1</a></li>"
                              "<li><a href=\"/folder2/\">Folder 2</a></li>"
                              "<li><a href=\"/file1.txt\">File 1</a></li>"
                              "<li><a href=\"/file2.jpg\">File 2</a></li>"
                              "</ul>"
                              "</body></html>";

        return send_response(connection, content);
    }
    else if (strcmp(method, "GET") == 0 && strcmp(url, "/folder1/") == 0)
    {
        const char *content = "<html><body>"
                              "<h1>Folder 1</h1>"
                              "<ul>"
                              "<li><a href=\"/folder1/subfolder/\">Subfolder</a></li>"
                              "<li><a href=\"/folder1/file1.txt\">File 1</a></li>"
                              "</ul>"
                              "</body></html>";

        return send_response(connection, content);
    }
    else if (strcmp(method, "GET") == 0 && strcmp(url, "/folder1/subfolder/") == 0)
    {
        const char *content = "<html><body>"
                              "<h1>Subfolder</h1>"
                              "<ul>"
                              "<li><a href=\"/folder1/subfolder/file1.txt\">File 1</a></li>"
                              "</ul>"
                              "</body></html>";

        return send_response(connection, content);
    }
    else if (strcmp(method, "GET") == 0 && strcmp(url, "/folder1/file1.txt") == 0)
    {
        const char *content = "This is the content of file 1.";

        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(content), (void *)content, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Content-Type", "text/plain");
        int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }
    else if (strcmp(method, "GET") == 0 && strcmp(url, "/folder2/") == 0)
    {
        const char *content = "<html><body>"
                              "<h1>Folder 2</h1>"
                              "<ul>"
                              "<li><a href=\"/folder2/file2.jpg\">File 2</a></li>"
                              "</ul>"
                              "</body></html>";

        return send_response(connection, content);
    }
    else if (strcmp(method, "GET") == 0 && strcmp(url, "/folder2/file2.jpg") == 0)
    {
        // TODO: Serve the content of file2.jpg
        // You can use appropriate libraries to read and send image data

        // Placeholder content for demonstration purposes
        const char *content = "This is the content of file 2.";

        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(content), (void *)content, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Content-Type", "text/plain");
        int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }
    else
    {
        const char *content = "<html><body><h1>404 Not Found</h1></body></html>";
        return send_response(connection, content);
    }
}

int main()
{
    struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &handle_request, NULL, MHD_OPTION_END);
    if (daemon == NULL)
    {
        printf("Failed to start the server\n");
        return 1;
    }

    printf("Server running on port %d\n", PORT);

    getchar();

    MHD_stop_daemon(daemon);

    return 0;
}

