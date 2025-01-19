#include "esp_wifi.h"
#include "esp_log.h"

#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>
#include "webserver.h"

static void http_server_netconn_serve(struct netconn *conn) {
	struct netbuf *inbuf;
	char *buf;
	u16_t buflen;
	err_t err;

	/* Read the data from the port, blocking if nothing yet there.
	 We assume the request (the part we care about) is in one netbuf */
	err = netconn_recv(conn, &inbuf);

	if (err == ERR_OK) {
		netbuf_data(inbuf, (void**) &buf, &buflen);

		/* Is this an HTTP GET command? (only check the first 5 chars, since
		 there are other formats for GET, and we're keeping it very simple )*/
		if (buflen >= 5 && strncmp("GET ",buf,4)==0) {

			//Parse URL
			char* path = NULL;
			char* line_end = strchr(buf, '\n');
			if( line_end != NULL )
			{
				//Extract the path from HTTP GET request
				path = (char*)malloc(sizeof(char)*(line_end-buf+1));
				int path_length = line_end - buf - strlen("GET ")-strlen("HTTP/1.1")-2;
				strncpy(path, &buf[4], path_length );
				path[path_length] = '\0';
				//Get remote IP address
				ip_addr_t remote_ip;
				u16_t remote_port;
				netconn_getaddr(conn, &remote_ip, &remote_port, 0);
				printf("[ "IPSTR" ] GET %s\n", IP2STR(&(remote_ip.u_addr.ip4)),path);
			}

			/* Send the HTML header
			 * subtract 1 from the size, since we dont send the \0 in the string
			 * NETCONN_NOCOPY: our data is const static, so no need to copy it
			 */
			bool bNotFound = false;
			if(path != NULL)
			{

				if (strcmp("/api/v1/rainbow",path)==0) {
					
				}
				else if (strcmp("/api/v1/hsv",path)==0) {
					
				}
				else if (strcmp("/",path)==0)
				{
				
				}
				else
				{
					bNotFound = true;	// 404 Not found
				}
				
				free(path);
				path=NULL;
			}

			// Send HTTP response header
			if(bNotFound)
			{
				netconn_write(conn, http_404_hdr, sizeof(http_404_hdr) - 1,
					NETCONN_NOCOPY);
			}
			else
			{
				netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1,
					NETCONN_NOCOPY);
			}

			// Send HTML content
			netconn_write(conn, http_index_hml, sizeof(http_index_hml) - 1,
					NETCONN_NOCOPY);
		}

	}
	// Close the connection (server closes in HTTP)
	netconn_close(conn);

	// Delete the buffer (netconn_recv gives us ownership,
	// so we have to make sure to deallocate the buffer)
	netbuf_delete(inbuf);
}

static void http_server(void *pvParameters) {
	struct netconn *conn, *newconn;	//conn is listening thread, newconn is new thread for each client
	err_t err;
	conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn, NULL, 80);
	netconn_listen(conn);
	do {
		err = netconn_accept(conn, &newconn);
		if (err == ERR_OK) {
			http_server_netconn_serve(newconn);
			netconn_delete(newconn);
		}
	} while (err == ERR_OK);
	netconn_close(conn);
	netconn_delete(conn);
}

void start_server(void) {
    xTaskCreate(&http_server, "http_server", 2048, NULL, 5, NULL);
}
