#include <iostream>
#include <sys/stat.h>

#include "sqlite3.h"

using namespace std;

bool FileExists(const string& name) {
  struct stat buffer;

  return (stat (name.c_str(), &buffer) == 0);
}

void LogSqlite3Error(sqlite3 *db, const string& message, bool quit)
{
    cerr << message << endl;
    cerr << "Error message: " << sqlite3_errmsg(db) << endl;
    if (quit)
    {
        cerr << "Aborting..." << endl;
        abort();
    }
}

void LogSqlite3Error(sqlite3 *db, const string& message)
{
    LogSqlite3Error(db, message, false);
}
