#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define CONNECTION_FAIL -1

int OpenConnection(const char *hostname, int port)
{
    int socket_descriptor;
    struct hostent *host_entity;
    struct sockaddr_in socket_address;
    
    if ((host_entity = gethostbyname(hostname)) == NULL)
    {
        perror(hostname);
        abort();
    }
    
    socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&socket_address, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);
    socket_address.sin_addr.s_addr = *(long *)(host_entity->h_addr);
    
    if (connect(socket_descriptor, (struct sockaddr *)&socket_address, sizeof(socket_address)) != 0)
    {
        close(socket_descriptor);
        perror(hostname);
        abort();
    }
    
    return socket_descriptor;
}

SSL_CTX *InitializeSSLContext(void)
{
    SSL_METHOD *ssl_method;
    SSL_CTX *ssl_context;
    
    OpenSSL_add_all_algorithms(); 
    SSL_load_error_strings();    
    ssl_method = TLSv1_2_client_method(); 
    ssl_context = SSL_CTX_new(ssl_method); 
    
    if (ssl_context == NULL)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    
    return ssl_context;
}

void ShowCertificates(SSL *ssl_connection)
{
    X509 *certificate;
    char *line;
    
    certificate = SSL_get_peer_certificate(ssl_connection); 
    
    if (certificate != NULL)
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(certificate), 0, 0);
        printf("Subject: %s\n", line);
        free(line); 
        
        line = X509_NAME_oneline(X509_get_issuer_name(certificate), 0, 0);
        printf("Issuer: %s\n", line);
        free(line); 
        
        X509_free(certificate); 
    }
    else
    {
        printf("Info: No server certificates available.\n");
    }
}

int main(int argument_count, char *argument_values[])
{
    SSL_CTX *ssl_context;
    int server_socket;
    SSL *ssl_connection;
    char buffer[1024];
    char client_request[1024] = {0};
    int bytes;
    char *hostname, *portnum;
    
    if (argument_count != 3)
    {
        printf("usage: %s <hostname> <portnum>\n", argument_values[0]);
        exit(0);
    }
    
    SSL_library_init();
    hostname = argument_values[1];
    portnum = argument_values[2];
    
    ssl_context = InitializeSSLContext();
    server_socket = OpenConnection(hostname, atoi(portnum));
    
    ssl_connection = SSL_new(ssl_context); 
    SSL_set_fd(ssl_connection, server_socket); 
    
    if (SSL_connect(ssl_connection) == CONNECTION_FAIL) 
    {
        ERR_print_errors_fp(stderr);
    }
    else
    {
        char username[16] = {0};
        char password[16] = {0};
        const char *request_message_format = "<Body>\
                               <UserName>%s<UserName>\
                 <Password>%s<Password>\
                 <\Body>";
        
        printf("Enter the User Name : ");
        scanf("%s", username);
        
        printf("\n\nEnter the Password : ");
        scanf("%s", password);
        
        sprintf(client_request, request_message_format, username, password);
        
        printf("\n\nConnected with %s encryption\n", SSL_get_cipher(ssl_connection));
        ShowCertificates(ssl_connection); 
        
        SSL_write(ssl_connection, client_request, strlen(client_request)); 
        
        bytes = SSL_read(ssl_connection, buffer, sizeof(buffer)); 
        buffer[bytes] = '\0';
        printf("Received: \"%s\"\n", buffer);
        
        SSL_free(ssl_connection); 
    }
    
    close(server_socket); 
    SSL_CTX_free(ssl_context); 
    
    return 0;
}
