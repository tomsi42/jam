#pragma once

#include <iostream>
#include <vector>

#include "dbobject.h"

#include "person.h"
#include "position.h"

using namespace std;

class Recruiter : public DbObject
{
    public:
        Recruiter();
        Recruiter(const string &name);
        Recruiter(const string &name, const string &description);
        virtual ~Recruiter();

        Recruiter(const Recruiter& recruiter) = delete;
        Recruiter& operator=(const Recruiter& recruiter) = delete;

        const string &Name();
        const string &Description();
        const vector<Person *> &Persons();
        const vector<Position *> &Positions();

        void SetName(const string& value);
        void SetDescription(const string& value);
        void SetPositions(vector<Position *> value);
        void SetPersons(vector<Person *> value);

        void AddPerson(Person *person);
        void AddPosition(Position *postion);
        void Print();

    protected:

    private:
        string m_name;
        string m_description;
        vector<Person *> m_persons;
        vector<Position *> m_positions;

        void DeletePositions();
        void DeletePersons();
};
