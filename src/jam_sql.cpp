#include <iostream>

#include "jam_sql.h"

#include "utils.h"

using namespace std;


const string preparedSqlStatements[STATEMENT_COUNT] = {
    // Persons
    "INSERT INTO Persons(name, email, phone) VALUES (?, ?, ?)",
    "UPDATE Persons SET name=?, email=?, phone=? WHERE (id = ?)",
    "DELETE FROM Persons WHERE (id = ?)",
    "SELECT name, email, phone FROM Persons WHERE (id = ?)",
    "SELECT id, name, email, phone FROM Persons",
    // Recruiters
    "INSERT INTO Recruiters(name, description) VALUES (?, ?)",
    "UPDATE Recruiters SET name=?, description=? WHERE (id = ?)",
    "DELETE FROM Recruiters WHERE (id = ?)",
    "SELECT name, description FROM Recruiters WHERE (id = ?)",
    "SELECT id, name, description FROM Recruiters",
    "INSERT INTO RecruiterPersons(recruiter_id, person_id) VALUES (?, ?)",
    "DELETE FROM RecruiterPersons WHERE recruiter_id = ? AND person_id = ?",
    "SELECT p.id, p.name, p.email, p.phone FROM Persons p, RecruiterPersons rp WHERE rp.recruiter_id = ? AND p.id = rp.person_id",
    // Positions
    "INSERT INTO Positions(company, name, description, app_status, app_result, recruiter_id) VALUES (?, ?, ?, ?, ?, ?)",
    "UPDATE Positions SET company=?, name=?, description=?, app_status=?, app_result=? WHERE (id = ?)",
    "DELETE FROM Positions WHERE (id = ?)",
    "SELECT company, name, description, app_status, app_result, recruiter_id FROM Positions WHERE (id = ?)",
    "SELECT id, company, name, description, app_status, app_result, recruiter_id FROM Positions",
    "SELECT id, company, name, description, app_status, app_result FROM Positions WHERE (recruiter_id = ?)",
    "INSERT INTO PositionPersons(position_id, person_id) VALUES (?, ?)",
    "DELETE FROM PositionPersons WHERE position_id = ? AND person_id = ?",
    "SELECT p.id, p.name, p.email, p.phone FROM Persons p, PositionPersons pp WHERE pp.position_id = ? AND p.id = pp.person_id",
};

Sqlite3StatementFactory::Sqlite3StatementFactory(sqlite3 *db) :
    m_db(db)
{
    m_statements = new Sqlite3StmtPtr[STATEMENT_COUNT];
    for (int i = 0; i < STATEMENT_COUNT; i++)
    {
        m_statements[i] = nullptr;
    }
}

Sqlite3StatementFactory::~Sqlite3StatementFactory()
{
    for (int i = 0; i < STATEMENT_COUNT; i++)
    {
        if (m_statements[i] != nullptr)
        {
            sqlite3_finalize(m_statements[i]);
        }
    }
    delete m_statements;
    m_statements = nullptr;
}

Sqlite3StmtPtr Sqlite3StatementFactory::GetPreparedStatement(const int id)
{
    if (m_statements[id] == nullptr)
    {
        // cout << "Preparing statement " << id << ": " << preparedSqlStatements[id] << endl;

        int rc = sqlite3_prepare_v2(m_db, preparedSqlStatements[id].c_str(), preparedSqlStatements[id].size(), &m_statements[id], nullptr);

        if (rc != SQLITE_OK)
        {
            LogSqlite3Error(m_db, "Could not prepare SQL statement: " + preparedSqlStatements[id], true);
        }
    }
    return m_statements[id];
}


const string CreateRecruitersTableSQL =
"CREATE TABLE Recruiters (\
    id              INTEGER PRIMARY KEY AUTOINCREMENT,\
    name            TEXT,\
    description     TEXT\
)";

const string CreatePositionsTableSQL =
"CREATE TABLE Positions (\
    id              INTEGER PRIMARY KEY AUTOINCREMENT,\
    company         TEXT,\
    name            TEXT,\
    description     TEXT,\
    app_status      INTEGER,\
    app_result      INTEGER,\
    recruiter_id    INTEGER\
)";

const string CreatePersonsTableSQL =
"CREATE TABLE Persons (\
    id              INTEGER PRIMARY KEY AUTOINCREMENT,\
    name            TEXT,\
    email           TEXT,\
    phone           TEXT\
)";


const string CreateRecruitersPersonsTableSQL =
"CREATE Table RecruiterPersons (\
    recruiter_id    INTEGER,\
    person_id       INTEGER\
)";

const string CreatePositionPersonsTableSQL =
"CREATE Table PositionPersons (\
    position_id     INTEGER,\
    person_id       INTEGER\
)";



