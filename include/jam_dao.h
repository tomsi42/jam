# pragma once

#include <iostream>
#include <vector>

#include "recruiter.h"
#include "position.h"
#include "person.h"

#include "jam_sql.h"

#include "sqlite3.h"


using namespace std;

class JobApplicationManagerDao
{
    public:
        JobApplicationManagerDao(string dbName);
        virtual ~JobApplicationManagerDao();

        JobApplicationManagerDao(const JobApplicationManagerDao& dao) = delete;
        JobApplicationManagerDao& operator=(const JobApplicationManagerDao& dao) = delete;

        bool Open();
        void Close();

        void ExecSql(const string& sql);

        void AddPerson(Person& person);
        void UpdatePerson(Person& person);
        void DeletePerson(Person& person);
        void DeletePerson(int id);
        vector<Person *> FindAllPersons();
        Person *FindPerson(int id);

        void AddRecruiter(Recruiter& recruiter);
        void UpdateRecruiter(Recruiter& recruiter);
        void DeleteRecruiter(Recruiter& recruiter);
        void DeleteRecruiter(int id);
        vector<Recruiter *> FindAllRecruiters();
        Recruiter *FindRecruiter(int id);

        void AddRecruiterPerson(Recruiter& recruiter, Person& person);
        void DeleteRecruiterPerson(Recruiter& recruiter, Person& person);
        vector<Person *> FindPersonsForRecruiter(Recruiter& recruiter);

        void AddPosition(Position& position);
        void UpdatePosition(Position& position);
        void DeletePosition(Position& position);
        void DeletePosition(int id);
        Position *FindPosition(int id);
        vector<Position *> FindAllPositions();

        void AddPositionPerson(Position& position, Person& person);
        void DeletePositionPerson(Position& position, Person& person);
        vector<Person *> FindPersonsForPosition(Position& position);
        vector<Position *> FindPositionsForRecruiter(Recruiter& recruiter);

    protected:

    private:
        sqlite3 * m_db;
        string m_dbName;
        bool m_connected;
        Sqlite3StatementFactory *m_statements;

        void CreateTables();
        Sqlite3StmtPtr GetPreparedStatement(const int id);
        int GetLastInsertedId();
};
