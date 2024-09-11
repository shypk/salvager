#include "json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strrev(char* str) {
    int length = strlen(str);
    int i, j;

    for (i = 0, j = length - 1; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int prev(const char* data, int needle, int limit) {
	char* next = (char*)memrchr( data, needle,  limit);
	if (next != NULL) {
		return next - data;
	}

	return -1;

}



int next(const char* data, int needle, int limit) {	
	char* next = (char*)memchr( (void*)data, needle,  limit);
	if (next != NULL) {
		return next - data;
	}

	return -1;

}


bool popStack( char* stack, int* pos, char* stack_h, int* pos_h, const char& target )
{
	if (*pos > 0){
		stack[*pos] = target;
		*pos = *pos-1;
	}
	else
	{
		if( JSON_MAX_DEPTH < *pos_h )
			return false;
		stack_h[*pos_h] = target;
		*pos_h = *pos_h+1;
	}
	return true;
}

bool pushStack( char* stack, int* pos, const char& target )
{
	if( JSON_MAX_DEPTH < *pos )
		return false;
	stack[*pos] = target;
	*pos = *pos+1;
	return true;
}
	
int findFirst( const char* data, int limit ){
	int token_n = 3;
	// state =    0   1   2
	// v     =    {   ,   [ 
	int v[3] = {123, 44, 91 };
	int found = -1;
	int min = limit;
	for( int i = 0; i < token_n; i++) {
		found = next(data, v[i], min);
		if (found > -1){
			min = found;
		}
	}
	return min == limit ? -1 : min;
}

int findLast( const char* data, int limit ){
	int token_n = 3;
	// state =    0   1   2  
	// v     =    }   ,   ] 
	int v[3] = {125, 44, 93 };
	int found = -1;
	int offset = 0;
	for( int i = 0; i < token_n; i++) {
		found = prev(data + offset, v[i], limit - offset);
		if (found > -1){
			offset = found + offset;
		}
	}
	return offset == 0 ? -1 : offset;
}

bool parse_json(const char* data, int limit, char* ret, int* from, int* to) {
	bool success = true;

	int first = findFirst( data, limit);
	int last = findLast( data, limit);

	if( first == last )
		return false;

	*from = first;
	*to = last;

	if( data[first] == ',' )
	{	
		first++;
	}

	if( data[last] == ',' ){
		last = last;
	}
	else {
		last = last+1;
	}

	limit = last;


	// state =    0   1   2   3   4   5   6
	// v     =    "   :   {   }   ,   [   ]
	//  v[7] = { 34, 58,123,125, 44, 91, 93 };

	char stack[JSON_MAX_DEPTH];
	int stack_pos = 0;
	char stack_h[JSON_MAX_DEPTH];
	int stack_pos_h = 0;
	bool done = false;

	bool outter_dict = false;
	int deeper = -1;

	for( int  i = first; i < limit; i++){
		if (done) break;
		switch ((int)(data[i])){
			case '{':
				//TODO stack overflow!
				success = pushStack( stack, &stack_pos, '}' );
				if( !success ) return success;
				break;
			case '}':
				success = popStack( stack, &stack_pos, stack_h, &stack_pos_h, '{');
				if( !success ) return success;
				break;
			case '[':
				success = pushStack( stack, &stack_pos, ']' );
				if( !success ) return success;
				break;
			case ']':
				success = popStack( stack, &stack_pos, stack_h, &stack_pos_h, '[');
				if( !success ) return success;
				break;
			case ':':
				if( stack_pos == 0 && deeper <= stack_pos_h ){
			       		outter_dict = true;
					deeper = stack_pos_h;
				}
				break;
			case ',':
				if( stack_pos == 0 && deeper <= stack_pos_h ){
			       		outter_dict = false;
					deeper = stack_pos_h;
				}
				break;
			case '\0':
				done = true;
				break;
		}
	}

	bool outter = false;
	if (deeper == stack_pos_h) outter = true;

	if( outter )
	{
		if( outter_dict )
		{
			success = pushStack( stack_h, &stack_pos_h, '{' );
			if( !success ) return success;
		}
		else
		{
			success = pushStack( stack_h, &stack_pos_h, '[' );
			if( !success ) return success;
		}
	}


	char header[JSON_MAX_DEPTH_5];
	char trail[JSON_MAX_DEPTH];
	header[0] = '\0';
	trail[0] = '\0';

	if ( stack_pos_h > 0 ){
		for( int i = stack_pos_h - 1; i >= 0; i--)
		{
			if( i < stack_pos_h - 1){
				if( stack_h[i+1] == '{' ){
					strcat(header, "\"k\":");
				}
			}

			if( stack_h[i] == '{' )
				strcat(header, "{");
			else
				strcat(header, "[");
		}

	}

	if (stack_pos > 0 ){
		stack[stack_pos] = '\0';
		strrev(stack);
		strcat(trail, stack);
	}

	if( outter )
	{
		if( outter_dict )
		{
			strcat(trail, "}" );
		}
		else
		{
			strcat(trail, "]" );
		}
	}

	int trim_len = last - first;
	sprintf(ret, "%s%.*s%s", header, trim_len, (data + first) , trail);

	return true;
}


