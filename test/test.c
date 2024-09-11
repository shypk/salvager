#include "json.h"

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

    return 0;
}



