#include "recruiter.h"

Recruiter::Recruiter()
{
}

Recruiter::Recruiter(const string &name) :
    m_name(name)
{
}

Recruiter::Recruiter(const string &name, const string &description) :
    m_name(name),
    m_description(description)
{
}

const string& Recruiter::Name()
{
    return m_name;
}

const string& Recruiter::Description()
{
    return m_description;
}

const vector<Person *>& Recruiter::Persons()
{
    return m_persons;
}

const vector<Position *>& Recruiter::Positions()
{
    return m_positions;
}

void Recruiter::SetName(const string& value)
{
    m_name = value;
}

void Recruiter::SetDescription(const string& value)
{
    m_description = value;
}

void Recruiter::AddPerson(Person* person)
{
    m_persons.push_back(person);
}

void Recruiter::AddPosition(Position* position)
{
    m_positions.push_back(position);
}

void Recruiter::SetPositions(vector<Position*> value)
{
    DeletePositions();
    m_positions = value;
}

void Recruiter::SetPersons(vector<Person*> value)
{
    DeletePersons();
    m_persons = value;
}

void Recruiter::DeletePositions()
{
    if (m_positions.size() > 0) {
        Position * p = m_positions.back();
        m_positions.pop_back();
        delete p;
    }
}

void Recruiter::DeletePersons()
{
    while (m_persons.size() > 0) {
        Person * p = m_persons.back();
        m_persons.pop_back();
        delete p;
    }
}

void Recruiter::Print()
{
    cout << "Recruiter:   " << m_name << endl;
    cout << "Description: " << m_description << endl;
    cout << "--------------------------------------------------------------" << endl;
    if (m_persons.size() > 0) {
        cout << "Contact persons: " << endl;
        for (Person * p : m_persons)
        {
            p->Print();
        }
    }
    cout << endl;
    if (m_positions.size() > 0) {
        cout << "Positions: " << endl;
        for (Position * p : m_positions)
        {
            p->Print();
        }
    }
    cout << "--------------------------------------------------------------" << endl << endl;
}

Recruiter::~Recruiter()
{
    // cout << "Deleting recruiter " << m_name << endl;

    DeletePositions();
    DeletePersons();
}

