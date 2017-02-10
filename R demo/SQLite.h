#pragma once
#include"Database.h"

struct sqlite3;
typedef struct sqlite3 sqlite3;

struct sqlite3_stmt;
typedef struct sqlite3_stmt sqlite3_stmt;

class SQLite:
	public Database
{
private:
	sqlite3* db;
	std::string statement;
	std::string statement1;//for additional use
	std::vector<std::string> record;
	char* error;
	char buffer[100];
	sqlite3_stmt* stmt;
	Field field;
	Fields fields;
	TableList created_table_list;
protected:

public:
	SQLite();
	virtual ~SQLite();
	std::string Connect(const char* address = NULL, const char* user_id = NULL, const char* password = NULL);
	bool IsConnected();
	bool IsTableExist(const char* table_name);
	std::string CreateTable(const char* table_name);
	void AddFieldNotNull(const char* col_name, DataType data_type);
	void AddFieldNull(const char* col_name, DataType data_type);
	void AddFieldForInsert(const char* col_name, const char* value);
	void AddFieldForInsert(const char* col_name, double value);
	void AddFieldForInsert(const char* col_name, int value);
	std::string Insert(const char* table_name);
	std::string ExecuteSelect(const char* full_sql, std::vector<std::vector<std::string>>* result_set, int column_count);
	std::string Delete(const char* sql);
	std::string Update(const char* sql);
};
