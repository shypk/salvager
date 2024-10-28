#include "multipart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* buffer;
    int pos;
} MultipartParser;


int find_position(const char* str, int from, const char *delim) {
    const char* found = strstr(str + from, delim);
    if( found == NULL ){
        return -1;
    }
    return strstr(str + from, delim) - str;
}

int next_line(const char* str, int from, int limit ) {
    int i = from;
    while (str[i] != '\0' || i < limit) {
        if (str[i] == '\n' ) {
            return i+1;
        }
        i++;
    }
    return -1;
}

int next_doubleline(const char* str, int from, int limit ) {
    int i = from;
    while (str[i] != '\0' || i < limit) {

        if (str[i] == '\r' && str[i+1] == '\n' && str[i+2] == '\r' && str[i+3] == '\n') {
            return i+4;
        }

        if (str[i] == '\n' && str[i+1] == '\n') {
            return i+2;
        }
        i++;
    }
    return -1;
}

bool out_of_bound(int pos, int limit) {
	return pos < 0 || pos >= limit;
}

char* extract_name_from_headers(const char* v, char* name) {
    const char* v_ptr = v;

    // Parse each line in the headers string
    while (v_ptr) {
        // Find the end of the current line
        const char* next_line = strchr(v_ptr, '\n');

        // find Content-Disposition header
        if (strncmp(v_ptr, "Content-Disposition:", 20) == 0) {

            // Look for the "name=" attribute within the line
            const char* name_start = strstr(v_ptr, " name=\"");
            if (name_start) {
                name_start += 7;
                const char* name_end = strchr(name_start, '"');
                if (name_end) {
                    size_t name_length = name_end - name_start;
					memcpy(name, name_start, name_length);
					name[name_length] = '\0';
                }
            }
			break;
        }

        // Move to the next line
        v_ptr = next_line ? next_line + 1 : NULL;
    }

    return name;
}

bool parse_multipart_offset(const char* data, int offset, const char* delim, int limit, char* key, char* value, int* from, int* to) {
    bool ret = parse_multipart(data + offset, delim, limit - offset, key, value, from, to);
    
    if( !out_of_bound( *from, limit ) ){
        *from += offset;
    }
    if( !out_of_bound( *to, limit ) ){
        *to += offset;
    }
    return ret;
}

bool parse_multipart(const char* data, const char* delim, int limit, char* key, char* value, int* from, int* to) {
    key[0] = '\0';
    value[0] = '\0';

	*from = -1;
	*to = -1;
    int part_start = find_position(data, 0, delim);    
	if( out_of_bound(part_start, limit) ){
        return false;
	}

	int header_start = next_line(data, part_start, limit); 
	if( out_of_bound(header_start, limit) ){
        return false;
	}
	
	extract_name_from_headers(data + header_start, key);

	int body_start = next_doubleline(data, header_start, limit);
	if( out_of_bound(body_start, limit) ){
        return false;
	}

	int body_end = find_position(data, body_start, delim); 
	if( out_of_bound(body_end, limit) ){
		memcpy(value, data + body_start, limit - body_start);
	    value[limit - body_start] = '\0';
		return false;
	}

	memcpy(value, data + body_start, body_end - body_start);
	value[body_end - body_start] = '\0';
	*from = part_start;
	*to = body_end;

	return true;
}


