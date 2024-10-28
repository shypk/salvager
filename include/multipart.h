#ifdef __cplusplus
extern "C"{
#endif

#define MULTIPART_KEY_MAX   1024

bool parse_multipart_offset(const char* data, int offset, const char* delim, int limit, char* key, char* value, int* from, int* to);
bool parse_multipart(const char* data, const char* delim, int limit, char* key, char* value, int* from, int* to);

#ifdef __cplusplus
}
#endif


