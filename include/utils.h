#pragma once

#include <string>

#include "sqlite3.h"

using namespace std;

extern bool FileExists(const string& name);
extern void LogSqlite3Error(sqlite3 *db, const string& message, bool quit);
extern void LogSqlite3Error(sqlite3 *db, const string& message);
