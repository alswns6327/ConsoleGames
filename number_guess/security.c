#define FNV_OFFSET_64 14695981039346656037ULL
#define FNV_PRIME_64 1099511628211ULL

unsigned long long basic_hash(char* password) {
	unsigned long long hash = FNV_OFFSET_64;
	while (*password) {
		hash ^= (unsigned char)*password++;
		hash *= FNV_PRIME_64;
	}
	return hash;
}