#ifdef __cplusplus
extern "C"{
#endif

#define JSON_MAX_DEPTH   16
#define JSON_MAX_DEPTH_5 80
#define JSON_MAX_DEPTH_6 96

bool parse_json(const char* data, int limit, char* ret, int* from, int* to);

#ifdef __cplusplus
}
#endif


