#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bson.h"
using namespace std;

// Some helper functions
static inline int32_t _read_int32(const char *data);
static inline int64_t _read_int64(const char *data);
static inline double _read_double(const char *data);
static inline const char *_type_to_string(BsonElementType type);

static const char *_type_names[] = {
	"end of list", "double", "string", "document", "array", "binary",
	"undefined", "objectid", "boolean", "UTC datetime", "null",
	"regular expression", "DBPointer", "JavaScript code", "deprecated",
	"JavaScript code w/ scope", "32-bit integer", "timestamp", 
	"64-bit integer"
};

static inline const char *_type_to_string(BsonElementType type)
{
	if (1 <= type && type <= 0x12)
		return _type_names[type];
	else if (type == 0xFF)
		return "min key";
	else if (type == 0x7F)
		return "max key";
	else
		return "unknown type";
}

static inline uint16_t _u16_swap(uint16_t x)
{
	return ((x & 0x00ffU) << 8) | ((x & 0xff00U) >> 8);
}

static inline uint32_t _u32_swap(uint32_t x)
{
	uint16_t a = x;
	uint16_t b = x >> 16;
	return ((uint32_t)_u16_swap(a) << 16) | ((uint32_t)_u16_swap(b));
}

static inline uint64_t _u64_swap(uint64_t x)
{
	uint32_t a = x;
	uint32_t b = x >> 32;
	return ((uint64_t)_u32_swap(a) << 32) | ((uint64_t)_u32_swap(b));
}

#ifdef LITTLE_ENDIAN
#  define _u32_to_local(x) (x)
#  define _u64_to_local(x) (x)
#  define _u32_to_le(x) (x)
#  define _u64_to_le(x) (x)
#elif BIG_ENDIAN
#  define _u32_to_local(x) _u32_swap(x)
#  define _u64_to_local(x) _u64_swap(x)
#  define _u32_to_local(x) _u32_swap(x)
#  define _u64_to_local(x) _u64_swap(x)
#else
#  error "Please define LITTLE_ENDIAN or BIG_ENDIAN!"
#endif

#ifdef DEBUG
#  define LOG printf
#else
#  define LOG(...)
#endif

static inline int32_t _read_int32(const char *data)
{
	int32_t value;
	memcpy(&value, data, sizeof(value));
	return (int32_t)_u32_to_local(value);
}

static inline int64_t _read_int64(const char *data)
{
	int64_t value;
	memcpy(&value, data, sizeof(value));
	return (int64_t)_u64_to_local(value);
}

static inline double _read_double(const char *data)
{
	double value;
	memcpy(&value, data, sizeof(value));
	return value;
}

bool BsonParser::LoadFromData(
	const char *data, 
	BsonError *error)
{
	return ParseDocument(data, &root, error);
}

BsonBuffer::BsonBuffer(const char *src, uint32_t len)
{
	buf = (char *)malloc(len);
	memcpy(buf, src, len);
}

BsonBuffer::~BsonBuffer()
{
	free(buf);
}

bool BsonParser::ParseDocument(
	const char *data, 
	BsonDocument *doc, // output
	BsonError *error) // output
{
	const char *dataptr = data + sizeof(uint32_t); // skip size
	BsonElementType type;
	const char *key;
	while (true)
	{
		type = (BsonElementType)*dataptr;
		dataptr++; // skip type
		key = dataptr; // NUL-terminated
		while (*dataptr++); // jump to data
		switch (type)
		{
		case BSON_TYPE_EOL:
			return true;
		case BSON_TYPE_DOUBLE:
			{
				double val = _read_double(dataptr);
				LOG("%s double %lf\n", key, val);
				BsonElement *element = new BsonDouble(val);
				doc->AddElement(BsonPair(key, element));
				dataptr += sizeof(double);
			}
			break;
		case BSON_TYPE_INT32:
			{
				int32_t val = _read_int32(dataptr);
				LOG("%s int32 %lf\n", key, val);
				BsonElement *element = new BsonInt32(val);
				doc->AddElement(BsonPair(key, element));
				dataptr += sizeof(int32_t);
			}
			break;
		case BSON_TYPE_INT64:
			{
				int32_t val = _read_int64(dataptr);
				LOG("%s int64 %lf\n", key, val);
				BsonElement *element = new BsonInt64(val);
				doc->AddElement(BsonPair(key, element));
				dataptr += sizeof(int64_t);
			}
			break;
		case BSON_TYPE_STRING:
			{
				uint32_t strlength = _read_int32(dataptr);
				dataptr += sizeof(strlength); // skip length
				LOG("%s string %s\n", key, val);
				BsonElement *element = new BsonString(BsonBuffer(dataptr, strlength));
				doc->AddElement(BsonPair(key, element));
				dataptr += strlength;
			}
			break;
		case BSON_TYPE_BOOLEAN:
			{
				bool val;
				switch (*dataptr)
				{
				case 1:
					val = true;
					break;
				case 0:
					val = false;
					break;
				default:
					if (error)
					{
						error->code = BSON_INVALID_VALUE;
						error->message = "Bad value for boolean";
					}
					return false;
				}
				LOG("%s boolean %s\n", key, val?"true":"false");
				BsonElement *element = new BsonBoolean(val);
				doc->AddElement(BsonPair(key, element));
				dataptr += 1;
			}
			break;
		case BSON_TYPE_DOCUMENT:
			{
				LOG("%s document\n", key);
				BsonDocument *element = new BsonDocument();
				uint32_t length = (uint32_t)_read_int32(dataptr);
				bool success = ParseDocument(dataptr, element, error);
				if (!success)
				{
					return false;
				}
				doc->AddElement(BsonPair(key, element));
				dataptr += length; // length comprising the doc
			}
			break;
		case BSON_TYPE_NULL:
			break;
		case BSON_TYPE_ARRAY:
		case BSON_TYPE_OBJECTID:
		case BSON_TYPE_DATETIME:
		case BSON_TYPE_REGEXP:
		case BSON_TYPE_JSCODE:
		case BSON_TYPE_JSCODEWS:
		case BSON_TYPE_TIMESTAMP:
		case BSON_TYPE_MINKEY:
		case BSON_TYPE_MAXKEY:
			if (error)
			{
				error->code = BSON_NOT_IMPLEMENTED;
				error->message = string("not implemented type: ") + _type_to_string(type);
			}
			return false;
		default:
			if (error)
			{
				error->code = BSON_UNKNOWN_TYPE;
				error->message = "Unknown or deprecated element type";
			}
			return false;
		}
	}
	// should never reach here
	if (error)
	{
		error->code = BSON_BUG;
		error->message = "You found a bug in ParseDocument";
	}
	return false;
}

void BsonDocument::AddElement(BsonPair pair)
{
	elements.push_back(pair);
}
