#pragma once

#include <vector>
#include <string>
#include <stdint.h>

enum BsonErrorCode 
{
	BSON_BUG, // something that should never happen happened
	BSON_NOT_IMPLEMENTED, 
	BSON_INVALID_VALUE, // invalid value
	BSON_UNKNOWN_TYPE
};

enum BsonElementType
{
	BSON_TYPE_EOL = 0x00, // End Of List
	BSON_TYPE_DOUBLE = 0x01, // 64-bit binary floating point
	BSON_TYPE_STRING = 0x02, // UTF-8 string
	BSON_TYPE_DOCUMENT = 0x03, // Embedded document
	BSON_TYPE_BOOLEAN = 0x08, // Boolean
	BSON_TYPE_NULL = 0x0A, // Null value
	BSON_TYPE_INT32 = 0x10, // 32-bit integer
	BSON_TYPE_INT64 = 0x12, // 64-bit integer
	
	// The following types are not implemented
	BSON_TYPE_ARRAY = 0x04, // Array
	BSON_TYPE_BINARY = 0x05, // Binary data
	BSON_TYPE_OBJECTID = 0x07, // ObjectId
	BSON_TYPE_DATETIME = 0x09, // UTC datetime
	BSON_TYPE_REGEXP = 0x0B, // Regular expression
	BSON_TYPE_JSCODE = 0x0D, // JavaScript code
	BSON_TYPE_JSCODEWS = 0x0F, // JavaScript code w/ scope
	BSON_TYPE_TIMESTAMP = 0x11, // Timestamp
	BSON_TYPE_MINKEY = 0xFF, // Min key
	BSON_TYPE_MAXKEY = 0x7F // Max key
};

struct BsonError
{
	BsonErrorCode code;
	std::string message;
};

struct BsonElement
{
	virtual ~BsonElement() = default;
	virtual std::string ToData() const = 0;
	virtual uint32_t GetLength() const = 0;
	virtual BsonElementType GetType() const = 0;
};

struct BsonPair
{
	BsonPair(std::string key, BsonElement *value):key(key),value(value){}
	~BsonPair () { delete value; }
	std::string key;
	BsonElement *value;
};

struct BsonDouble : public BsonElement
{
public:
	BsonDouble(double value):value(value){}
	virtual std::string ToData() const { return ""; }
	virtual uint32_t GetLength() const { return sizeof(double); }
	virtual BsonElementType GetType() const { return BSON_TYPE_DOUBLE; }
private:
	double value;
};

struct BsonInt32 : public BsonElement
{
public:
	BsonInt32(int32_t value):value(value){}
	virtual std::string ToData() const { return ""; }
	virtual uint32_t GetLength() const { return sizeof(int32_t); }
	virtual BsonElementType GetType() const { return BSON_TYPE_INT32; }
private:
	int32_t value;
};

struct BsonInt64 : public BsonElement
{
public:
	BsonInt64(int64_t value):value(value){}
	virtual std::string ToData() const { return ""; }
	virtual uint32_t GetLength() const { return sizeof(int64_t); }
	virtual BsonElementType GetType() const { return BSON_TYPE_INT64; }
private:
	int64_t value;
};

struct BsonBuffer
{
	BsonBuffer() = default;
	BsonBuffer(const char *src, uint32_t len);
	~BsonBuffer();
	char *buf = nullptr;
	uint32_t len = 0;
};

struct BsonString : public BsonElement
{
public:
	BsonString(BsonBuffer buf):buffer(buf){}
	virtual std::string ToData() const { return buffer.buf; }
	virtual uint32_t GetLength() const { return buffer.len; }
	virtual BsonElementType GetType() const { return BSON_TYPE_INT64; }
private:
	BsonBuffer buffer;
};

struct BsonBoolean : public BsonElement
{
public:
	BsonBoolean(bool value):value(value){}
	virtual std::string ToData() const { return ""; }
	virtual uint32_t GetLength() const { return sizeof(bool); }
	virtual BsonElementType GetType() const { return BSON_TYPE_INT64; }
private:
	bool value;
};

class BsonDocument : public BsonElement
{
public:
	BsonDocument() = default;
	~BsonDocument() = default;
	virtual std::string ToData() const { return ""; }
	virtual uint32_t GetLength() const { return 0; }
	virtual BsonElementType GetType() const { return BSON_TYPE_DOCUMENT; }
	void AddElement(BsonPair pair);
private:
	std::vector<BsonPair> elements;
};

class BsonParser
{
public:
	BsonParser();
	~BsonParser();
	bool LoadFromData(const char *data, BsonError *error);
private:
	bool ParseDocument(const char *data, BsonDocument *doc, BsonError *error);
	
	BsonDocument root;
};

class BsonDocumentBuilder
{
public:
	BsonDocumentBuilder(BsonDocument *doc) : doc(doc) {}
	~BsonDocumentBuilder() = default;
private:
	BsonDocument *doc; // not taking the ownership
};
