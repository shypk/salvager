#include "json.h"
#include "multipart.h"

#include <stdio.h>
#include <string.h>

void getToken( const char* data )
{
    int limit = strlen(data);
    int from = 0;
    int to = 0;

    char ret[limit+JSON_MAX_DEPTH_6];

    printf( "orginal : %s\n", data);
    parse_json( data, limit, (char*)ret, &from, &to);
    printf( "done : %s, [%d:%d]\n", ret, from, to );
}


void getMultipart( const char* data, const char* delim )
{
    int limit = strlen(data);
    int from = 0;
    int to = 0;
    int acc = 0;

    char key[MULTIPART_KEY_MAX];
    char value[limit];

    printf( "orginal : %s\n", data);
    for (int i = 0; i < 10; i++)
    {
        key[0] = '\0';
        value[0] = '\0';
        parse_multipart_offset( data, acc, delim, limit, (char*)key,(char*)value, &from, &to);
        printf( "done : %s, :, %s, [%d:%d]\n", key, value, from, to );
        if (to < 0) {
            break;
        }
        else {
            acc = to;
        }
    }
}

int main() {
    //parseJSON(jsonString);
    //
    int in_size = 8192;

    getToken( " e\":\"John\",\"age\":30,\"city\": {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}}" );	 
    getToken( " \"name\":\"John\",\"age\":30,\"city\":\"New York\"}}" );	 
    getToken( " \"name\":\"John\",\"age\":30,\"city\":\"New Y" );	   

    getToken( " {\"name\":\"John\",\"age\":30,\"city\": {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}}" );	 
    getToken( " {\"name\":\"John\",\"age\":30,\"city\": {\"name\":\"John\",\"age\":30,\"city\":\"New Yo" ); 
    getToken( " {\"name\":\"John\",\"age\":30,\"city\": {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}" );
    
    getToken( " ame\":\"John\"},\"age\":30,\"city\": {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}}" );	 
    getToken( " ame\":\"John\", \"nick\":\"green\"},\"age\":30,\"city\": {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}}" );	 
    getToken( " ame\":\"John\", \"nick\":\"green\"},\"age\":30,\"city\": {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}" );	 
    getToken( " ame\":\"John\", \"nick\":\"green\"},\"age\":30,\"city\": {\"name\":\"John\",\"age\":30,\"city\":\"New Y" );	

    getToken( " [\"John\", {\"nick\":\"green\"},\"Mother\", {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}]" );	 
    getToken( " ick\":\"green\"},\"Mother\", {\"name\":\"John\",\"age\":30,\"city\":\"New " );	 
    getToken( " ick\":\"green\"},\"Mother\", {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}]" );	 
    getToken( " ick\":\"green\",\"nick2\":\"green\"},\"Mother\", {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}]" );	 
    getToken( " ick\":\"green\"},{\"Role\":\"Mother\"}, {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}]" );	 
    getToken( " me\":\"John\",\"age\":30,\"city\":\"New York\"}]" );	 

    getToken( " [\"John\", {\"nick\":\"green\"},\"Mother\", {\"name\":\"John\",\"age\":30,\"city\":\"New York\"}" );	 
    getToken( " [\"John\", {\"nick\":\"green\"},\"Mother\", {\"name\":\"John\",\"age\":30,\"city\":\"New " );	 
    getToken( " nick\":\"green\", \"nick2\":\"green\"},\"Mother\", {\"name\":\"John\",\"age\":30,\"city\":\"New" );	 
    getToken( " \"asdf\": \"zxc\", \"wwf\": \"efdf\"}}}}}}}}}}}}}}}, \"asdf\", \"eeed\" " );
    
    getToken( "{\"part1\": \"Bax24kKsvGzml8AnUrsL8IrK387u6\", \"part2\": \"sdfiwaejgnlasdfnwe\", \"part3\": \"sdkfhaueg" );

    getMultipart( "some body\r\nother body\r\n"
    "--somedel\r\n\r\nContent-Disposition: form-data; name=\"user_name\"\r\nContent-Type: text/plain\r\n\r\ntest\r\n"
    "--somedel\r\nContent-Disposition: form-da", "--somedel" );
    
    getMultipart( "some body\r\nother body\r\n"
    "--somedel\r\nContent-Disposition: form-data; name=\"user_name\"\r\nContent-Type: text/plain\r\n\r\ntest\r\n"
    "--somedel\r\nContent-Disposition: form-data; filename=\"image.jpeg\"; name=\"user_profile\"\r\nContent-Type: image\r\n\r\nbodybody\r\n"
    "--somedel\r\nContent-Disposition: form-data; name=\"gender\"\n\nmale", "--somedel" );
    
    getMultipart( "some body\r\nother body\r\n"
    "--somedel\r\nContent-Disposition: form-data; name=\"user_name\"\r\nContent-Type: text/plain\r\n\r\ntest\r\n"
    "--somedel\r\nContent-Disposition: form-data; filename=\"image.jpeg\"; name=\"user_profile\"\r\nContent-Type: image\r\n\r\nbodybody\r\n"
    "--somedel\r\nContent-Disposition: form-data; name=\"gender\"\n\nmale\n"
    "--somedel--", "--somedel" );
    return 0;
}



