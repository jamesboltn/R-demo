#pragma once
#include"SQLite.h"
#include"sqlite3.h"

SQLite::SQLite()
{
	db = NULL;
}

SQLite::~SQLite()
{
	if (db != NULL)
	{
		sqlite3_close(db);
		db = NULL;
	}
}

std::string SQLite::Connect(const char* address, const char* user_id, const char* password)
{
	if (sqlite3_open("fastshoot.db", &db) != SQLITE_OK)
	{
		sqlite3_close(db);
		db = NULL;
		return "failed to open SQLite";
	}
	else
	{
		return "ok";
	}
}

bool SQLite::IsConnected()
{
	if (db != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SQLite::AddFieldNull(const char* col_name, DataType data_type)
{
	field.col_name = col_name;
	field.data_type = data_type;
	field.null_flag = _NULL;
	fields.push_back(field);
}

void SQLite::AddFieldNotNull(const char* col_name, DataType data_type)
{
	field.col_name = col_name;
	field.data_type = data_type;
	field.null_flag = NOT_NULL;
	switch (data_type)
	{
	case TEXT:
		field.default = "--";
		break;
	case INTEGER:
		field.default = "0";
		break;
	case _DOUBLE:
		field.default = "0.0";
		break;
	case DATETIME:
		break;
	}
	fields.push_back(field);
}

std::string SQLite::CreateTable(const char* table_name)
{
	if (IsConnected())
	{
		if (strcmp(table_name, "") == 0)
		{
			return "table name not supplied";
		}
		else if (fields.empty())
		{
			return "fields not supplied";
		}
		else
		{
			statement = "CREATE TABLE IF NOT EXISTS '" + std::string(table_name) + "' ('";
			for (auto it = fields.begin(); it != fields.end(); ++it)
			{
				statement += it->col_name + "'	";
				switch (it->data_type)
				{
				case TEXT:
					statement += "TEXT ";
					break;
				case INTEGER:
					statement += "INTEGER ";
					break;
				case _DOUBLE:
					statement += "NUMERIC ";
					break;
				case DATETIME:
					break;
				default:
					break;
				}

				if (it->null_flag == NOT_NULL)
				{
					statement += "NOT NULL DEFAULT ";
					if (it->data_type == TEXT)
					{
						statement += "'" + it->default;
						if (it != --fields.end())
						{
							statement += "','";
						}
						else
						{
							statement += "'";
						}
					}
					else
					{
						statement += it->default;
						if (it != --fields.end())
						{
							statement += ",'";
						}
					}
				}
				else
				{
					if (it != --fields.end())
					{
						statement += ",'";
					}
				}
			}
			statement += ");";
			if (sqlite3_exec(db, statement.c_str(), NULL, NULL, &error) != SQLITE_OK)
			{
				statement = sqlite3_errmsg(db) + std::string(" sql:") + statement;
				sqlite3_free(error);
				fields.clear();
				return statement;
			}
			else
			{
				created_table_list.insert(std::make_pair(table_name, fields));
				fields.clear();
				return "ok";
			}
		}
	}
	else
	{
		return "SQLite not connected";
	}
}

bool SQLite::IsTableExist(const char* table_name)
{
	if (IsConnected())
	{
		statement = "SELECT * FROM sqlite_master WHERE name = '" + std::string(table_name) + "' and type = 'table';";

		if (sqlite3_prepare_v2(db, statement.c_str(), -1, &stmt, NULL) != SQLITE_OK)
		{
			return false;
		}
		else if (sqlite3_step(stmt) != SQLITE_ROW)
		{
			sqlite3_finalize(stmt);
			return false;
		}
		else
		{
			sqlite3_finalize(stmt);
			return true;
		}
	}
	else
	{
		return false;
	}
}

void SQLite::AddFieldForInsert(const char* col_name, const char* value)
{
	field.col_name = col_name;
	field.default = value;
	fields.push_back(field);
}

void SQLite::AddFieldForInsert(const char* col_name, int value)
{
	field.col_name = col_name;
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%d", value);
	field.default = buffer;
	fields.push_back(field);
}

void SQLite::AddFieldForInsert(const char* col_name, double value)
{
	field.col_name = col_name;
	sprintf_s(buffer, ARRAYCOUNT(buffer), "%f", value);
	field.default = buffer;
	fields.push_back(field);
}

std::string SQLite::Insert(const char* table_name)
{
	if (IsConnected())
	{
		if (strcmp(table_name, "") == 0)
		{
			return "table name not supplied";
		}
		else if (fields.empty())
		{
			return "fields not supplied";
		}
		else
		{
			statement = "INSERT INTO " + std::string(table_name) + " (";
			for (auto it = fields.begin(); it != fields.end(); ++it)
			{
				statement += it->col_name;

				auto it1 = created_table_list.find(table_name);
				if (it1 != created_table_list.end())
				{
					for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
					{
						if (it2->col_name == it->col_name)
						{
							switch (it2->data_type)
							{
							case TEXT:
							{
								if (it == fields.begin())
								{
									statement1 = "'" + it->default + "'";
								}
								else
								{
									statement1 += "'" + it->default + "'";
								}
								
							}
								break;
							case INTEGER:
							{
								if (it == fields.begin())
								{
									statement1 = it->default;
								}
								else
								{
									statement1 += it->default;
								}
								
							}
								break;
							case _DOUBLE:
							{
								if (it == fields.begin())
								{
									statement1 = it->default;
								}
								else
								{
									statement1 += it->default;
								}
								
							}
								break;
							case DATETIME:
							{

							}
								break;
							default:
								break;
							}
						}
					}
				}

				if (it != --fields.end())
				{
					statement += ",";
					statement1 += ",";
				}
				else
				{
					statement += ") VALUES(";
					statement1 += ");";
					statement += statement1;
				}
			}
			if (sqlite3_exec(db, statement.c_str(), NULL, NULL, &error) != SQLITE_OK)
			{
				statement = sqlite3_errmsg(db) + std::string(" sql:") + statement;
				sqlite3_free(error);
				fields.clear();
				return statement;
			}
			else
			{
				fields.clear();
				return "ok";
			}
		}
	}
	else
	{
		return "SQLite not connected";
	}
}

std::string SQLite::ExecuteSelect(const char* full_sql, std::vector<std::vector<std::string>>* result_set, int column_count)
{
	if (IsConnected())
	{
		if (sqlite3_prepare_v2(db, full_sql, -1, &stmt, NULL) != SQLITE_OK)
		{
			return std::string("error select sql:") + full_sql;
		}
		else
		{
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				for (int i = 0; i < column_count; ++i)
				{
					record.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
				}
				result_set->push_back(record);
				record.clear();
			}
			sqlite3_finalize(stmt);
			return "ok";
		}
	}
	else
	{
		return "SQLite not connected";
	}
}

std::string SQLite::Delete(const char* sql)
{
	if (IsConnected())
	{
		if (sqlite3_exec(db, sql, NULL, NULL, &error) != SQLITE_OK)
		{
			sqlite3_free(error);
			statement = sqlite3_errmsg(db) + std::string(" sql:") + statement;
			return statement;
		}
		else
		{
			return "ok";
		}
	}
	else
	{
		return "SQLite not connected";
	}
	
}

std::string SQLite::Update(const char* sql)
{
	if (IsConnected())
	{
		if (sqlite3_exec(db, sql, NULL, NULL, &error) != SQLITE_OK)
		{
			sqlite3_free(error);
			statement = sqlite3_errmsg(db) + std::string(" sql:") + statement;
			return statement;
		}
		else
		{
			return "ok";
		}
	}
	else
	{
		return "SQLite not connected";
	}
}