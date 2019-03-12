# pragma once

#include <string>

#include "sqlite3.h"

using namespace std;


#define ADD_PERSON_STMT_ID                  0
#define UPDATE_PERSON_STMT_ID               1
#define DELETE_PERSON_STMT_ID               2
#define SELECT_PERSON_STMT_ID               3
#define SELECT_ALL_PERSONS_STMT_ID          4

#define ADD_RECRUITER_STMT_ID               5
#define UPDATE_RECRUITER_STMT_ID            6
#define DELETE_RECRUITER_STMT_ID            7
#define SELECT_RECRUITER_STMT_ID            8
#define SELECT_ALL_RECRUITERS_STMT_ID       9
#define ADD_RECRUITER_PERSON_STMT_ID        10
#define DELETE_RECRUITER_PERSON_STMT_ID     11
#define SELECT_RECRUITER_PERSON_STMT_ID     12

#define ADD_POSITION_STMT_ID                13
#define UPDATE_POSITION_STMT_ID             14
#define DELETE_POSITION_STMT_ID             15
#define SELECT_POSITION_STMT_ID             16
#define SELECT_ALL_POSITIONS_STMT_ID        17
#define SELECT_RECRUITER_POSITIONS_STMT_ID  18
#define ADD_POSITION_PERSON_STMT_ID         19
#define DELETE_POSITION_PERSON_STMT_ID      20
#define SELECT_POSITION_PERSON_STMT_ID      21

#define STATEMENT_COUNT                     22



typedef sqlite3_stmt *Sqlite3StmtPtr;

class Sqlite3StatementFactory
{
    public:
        Sqlite3StatementFactory(sqlite3 *db);
        virtual ~Sqlite3StatementFactory();

        Sqlite3StatementFactory(const Sqlite3StatementFactory& sf) = delete;
        Sqlite3StatementFactory& operator=(const Sqlite3StatementFactory& sf) = delete;

        Sqlite3StmtPtr GetPreparedStatement(const int id);
    protected:

    private:
        sqlite3 *m_db;
        Sqlite3StmtPtr *m_statements;
};

extern const string CreateRecruitersTableSQL;
extern const string CreatePositionsTableSQL;
extern const string CreatePersonsTableSQL;
extern const string CreateRecruitersPersonsTableSQL;
extern const string CreatePositionPersonsTableSQL;

