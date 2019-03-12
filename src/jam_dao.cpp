
#include "jam_dao.h"
#include "jam_sql.h"

#include "utils.h"

JobApplicationManagerDao::JobApplicationManagerDao(string dbName) :
    m_dbName(dbName),
    m_connected(false)
{
}

JobApplicationManagerDao::~JobApplicationManagerDao()
{
    // cout << "Deleting Dao object" << endl;
    if (m_connected)
    {
        Close();
    }
}

bool JobApplicationManagerDao::Open()
{
    if (m_connected) {
        Close();
    }
    bool mustCreateDb = (m_dbName == ":memory:") || !FileExists(m_dbName);

    int rc = sqlite3_open(m_dbName.c_str(), &m_db);
    m_connected = (rc == SQLITE_OK);
    if (m_connected && mustCreateDb)
    {
        CreateTables();
    }
    else if (!m_connected)
    {
        LogSqlite3Error(m_db, "An error occurred when trying to open database " + m_dbName);
        sqlite3_close(m_db);
    }

    m_statements = new Sqlite3StatementFactory(m_db);

    return m_connected;
}

void JobApplicationManagerDao::Close()
{
    if (m_connected) {
        cout << "Closing database" << endl;

        delete m_statements;
        int rc = sqlite3_close(m_db);
        if (rc == SQLITE_BUSY)
        {
            LogSqlite3Error(m_db, "An error occurred when trying to close database " + m_dbName, true);
        }
        m_connected = false;
    }
}

void JobApplicationManagerDao::ExecSql(const string& sql)
{
    char *error_msg = nullptr;

    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &error_msg);
    if (rc != SQLITE_OK)
    {

    }
}

void JobApplicationManagerDao::CreateTables()
{
    cout << "Creating tables" <<endl;

    ExecSql(CreateRecruitersTableSQL);
    ExecSql(CreatePositionsTableSQL);
    ExecSql(CreatePersonsTableSQL);
    ExecSql(CreateRecruitersPersonsTableSQL);
    ExecSql(CreatePositionPersonsTableSQL);
}

Sqlite3StmtPtr JobApplicationManagerDao::GetPreparedStatement(const int id)
{
    return m_statements->GetPreparedStatement(id);
}

int JobApplicationManagerDao::GetLastInsertedId()
{
    return sqlite3_last_insert_rowid(m_db);
}

void JobApplicationManagerDao::AddPerson(Person& person)
{
    // cout << "Add person: " << person.Name() << endl;

    Sqlite3StmtPtr stmt = GetPreparedStatement(ADD_PERSON_STMT_ID);

    sqlite3_bind_text(stmt, 1, person.Name().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, person.Email().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 3, person.Phone().c_str(), -1, nullptr);
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    int id = sqlite3_last_insert_rowid(m_db);

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    person.SetId(id);
}

void JobApplicationManagerDao::UpdatePerson(Person& person)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(UPDATE_PERSON_STMT_ID);

    sqlite3_bind_text(stmt, 1, person.Name().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, person.Email().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 3, person.Phone().c_str(), -1, nullptr);
    sqlite3_bind_int(stmt, 4, person.Id());

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

void JobApplicationManagerDao::DeletePerson(Person& person)
{
    DeletePerson(person.Id());
}

void JobApplicationManagerDao::DeletePerson(int id)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(DELETE_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

vector<Person *> JobApplicationManagerDao::FindAllPersons()
{
    vector<Person *> persons;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_ALL_PERSONS_STMT_ID);

    int rc;
    while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Person *p = new Person();
        p->SetId(sqlite3_column_int(stmt, 0));
        p->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        p->SetEmail(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        p->SetPhone(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        persons.push_back(p);
    }

    sqlite3_reset(stmt);
    // sqlite3_clear_bindings(stmt);

    return persons;
}

Person* JobApplicationManagerDao::FindPerson(int id)
{
    Person *p = nullptr;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, id);

   int rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        p = new Person();
        p->SetId(id);
        p->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        p->SetEmail(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        p->SetPhone(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
            LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
        }
    }
    else if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    return p;
}

void JobApplicationManagerDao::AddRecruiter(Recruiter& recruiter)
{
    // cout << "Add recruiter: " << recruiter.Name() << endl;

    Sqlite3StmtPtr stmt = GetPreparedStatement(ADD_RECRUITER_STMT_ID);

    sqlite3_bind_text(stmt, 1, recruiter.Name().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, recruiter.Description().c_str(), -1, nullptr);
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    int id = sqlite3_last_insert_rowid(m_db);

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    recruiter.SetId(id);

    for (Person *p : recruiter.Persons())
    {
        AddPerson(*p);
        AddRecruiterPerson(recruiter, *p);
    }
    for (Position *p : recruiter.Positions())
    {
        p->SetRecruiterId(id);
        AddPosition(*p);
    }
}

void JobApplicationManagerDao::UpdateRecruiter(Recruiter& recruiter)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(UPDATE_RECRUITER_STMT_ID);

    sqlite3_bind_text(stmt, 1, recruiter.Name().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, recruiter.Description().c_str(), -1, nullptr);
    sqlite3_bind_int(stmt, 3, recruiter.Id());

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

}

void JobApplicationManagerDao::DeleteRecruiter(Recruiter& recruiter)
{
    DeleteRecruiter(recruiter.Id());
}

void JobApplicationManagerDao::DeleteRecruiter(int id)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(DELETE_RECRUITER_STMT_ID);

    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

vector<Recruiter *> JobApplicationManagerDao::FindAllRecruiters()
{
    vector<Recruiter *> recruiters;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_ALL_RECRUITERS_STMT_ID);

    int rc;
    while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Recruiter *r = new Recruiter();
        r->SetId(sqlite3_column_int(stmt, 0));
        r->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        r->SetDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        r->SetPositions(FindPositionsForRecruiter(*r));
        r->SetPersons(FindPersonsForRecruiter(*r));
        recruiters.push_back(r);
    }

    sqlite3_reset(stmt);
    // sqlite3_clear_bindings(stmt);


    return recruiters;
}

Recruiter* JobApplicationManagerDao::FindRecruiter(int id)
{
    Recruiter *r = nullptr;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_RECRUITER_STMT_ID);

    sqlite3_bind_int(stmt, 1, id);

   int rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        r = new Recruiter();
        r->SetId(id);
        r->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        r->SetDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        r->SetPositions(FindPositionsForRecruiter(*r));
        r->SetPersons(FindPersonsForRecruiter(*r));
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
            LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
        }
    }
    else if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    return r;
}

void JobApplicationManagerDao::AddPosition(Position& position)
{
    // cout << "Add position: " << position.Company() << " - " << position.Name() << endl;

    Sqlite3StmtPtr stmt = GetPreparedStatement(ADD_POSITION_STMT_ID);

    sqlite3_bind_text(stmt, 1, position.Company().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, position.Name().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 3, position.Description().c_str(), -1, nullptr);
    sqlite3_bind_int(stmt, 4, position.Status());
    sqlite3_bind_int(stmt, 5, position.Result());
    sqlite3_bind_int(stmt, 6, position.RecruiterId());

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    int id = sqlite3_last_insert_rowid(m_db);

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    position.SetId(id);

    for (Person *p : position.Persons())
    {
        AddPerson(*p);
        AddPositionPerson(position, *p);
    }

}

void JobApplicationManagerDao::UpdatePosition(Position& position)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(UPDATE_POSITION_STMT_ID);

    sqlite3_bind_text(stmt, 1, position.Company().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, position.Name().c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 3, position.Description().c_str(), -1, nullptr);
    sqlite3_bind_int(stmt, 4, position.Status());
    sqlite3_bind_int(stmt, 5, position.Result());
    sqlite3_bind_int(stmt, 6, position.Id());

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

}

void JobApplicationManagerDao::DeletePosition(Position& position)
{
    DeletePosition(position.Id());
}

void JobApplicationManagerDao::DeletePosition(int id)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(DELETE_POSITION_STMT_ID);

    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

}

vector<Position *> JobApplicationManagerDao::FindAllPositions()
{
    vector<Position *> positions;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_ALL_POSITIONS_STMT_ID);

    int rc;
    while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Position *p = new Position();
        p->SetId(sqlite3_column_int(stmt, 0));
        p->SetCompany(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        p->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        p->SetDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        p->SetStatus(ApplicationStatus(sqlite3_column_int(stmt, 4)));
        p->SetResult(ApplicationResult(sqlite3_column_int(stmt, 5)));
        p->SetRecruiterId(sqlite3_column_int(stmt, 6));

        p->SetPersons(FindPersonsForPosition(*p));

        positions.push_back(p);
    }

    sqlite3_reset(stmt);

    return positions;
}

Position* JobApplicationManagerDao::FindPosition(int id)
{
    Position *p = nullptr;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, id);

   int rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        p = new Position();
        p->SetId(id);
        p->SetCompany(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        p->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        p->SetDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        p->SetStatus(ApplicationStatus(sqlite3_column_int(stmt, 3)));
        p->SetResult(ApplicationResult(sqlite3_column_int(stmt, 4)));
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
            LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
        }
    }
    else if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    if (p)
    {
        p->SetPersons(FindPersonsForPosition(*p));
    }
    return p;
}

void JobApplicationManagerDao::AddRecruiterPerson(Recruiter& recruiter, Person& person)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(ADD_RECRUITER_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, recruiter.Id());
    sqlite3_bind_int(stmt, 2, person.Id());
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

void JobApplicationManagerDao::DeleteRecruiterPerson(Recruiter& recruiter, Person& person)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(DELETE_RECRUITER_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, recruiter.Id());
    sqlite3_bind_int(stmt, 2, person.Id());
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

void JobApplicationManagerDao::AddPositionPerson(Position& position, Person& person)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(ADD_POSITION_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, position.Id());
    sqlite3_bind_int(stmt, 2, person.Id());
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

void JobApplicationManagerDao::DeletePositionPerson(Position& position, Person& person)
{
    Sqlite3StmtPtr stmt = GetPreparedStatement(DELETE_POSITION_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, position.Id());
    sqlite3_bind_int(stmt, 2, person.Id());
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        LogSqlite3Error(m_db, "Unexpected result from database: " + to_string(rc));
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}

vector<Person *> JobApplicationManagerDao::FindPersonsForRecruiter(Recruiter& recruiter)
{
    vector<Person *> persons;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_RECRUITER_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, recruiter.Id());

    int rc;
    while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Person *p = new Person();
        p->SetId(sqlite3_column_int(stmt, 0));
        p->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        p->SetEmail(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        p->SetPhone(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        persons.push_back(p);
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    return persons;
}

vector<Person *> JobApplicationManagerDao::FindPersonsForPosition(Position& position)
{
    vector<Person *> persons;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_POSITION_PERSON_STMT_ID);

    sqlite3_bind_int(stmt, 1, position.Id());

    int rc;
    while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Person *p = new Person();
        p->SetId(sqlite3_column_int(stmt, 0));
        p->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        p->SetEmail(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        p->SetPhone(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        persons.push_back(p);
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    return persons;
}

vector <Position *> JobApplicationManagerDao::FindPositionsForRecruiter(Recruiter& recruiter)
{
    vector<Position*> positions;

    Sqlite3StmtPtr stmt = GetPreparedStatement(SELECT_RECRUITER_POSITIONS_STMT_ID);

    sqlite3_bind_int(stmt, 1, recruiter.Id());

    int rc;
    while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Position *p = new Position();
        p->SetId(sqlite3_column_int(stmt, 0));
        p->SetCompany(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        p->SetName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        p->SetDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        p->SetStatus(ApplicationStatus(sqlite3_column_int(stmt, 4)));
        p->SetResult(ApplicationResult(sqlite3_column_int(stmt, 5)));
        p->SetRecruiterId(sqlite3_column_int(stmt, 6));

        p->SetPersons(FindPersonsForPosition(*p));

        positions.push_back(p);
    }

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    return positions;
}


