#ifndef WEBSERVER_H
#define WEBSERVER_H

// Build http header
const static char http_html_hdr[] =
		"HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";

// Build 404 header
const static char http_404_hdr[] =
"HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\n\r\n";

// Build http body
const static char http_index_hml[] =
		"<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
		<title>Control</title><style>body{background-color:lightblue;font-size:24px;}</style></head>\
		<body><h1>Wifithing</h1></body></html>";


void start_server(void);

#endif // WEBSERVER_H


