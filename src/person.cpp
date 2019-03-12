#include <iostream>

#include "person.h"

using namespace std;

Person::Person() :
    DbObject()
{
}

Person::Person(const Person& person) :
    Person()
{
    m_name = person.m_name;
    m_email = person.m_email;
    m_phone = person.m_phone;
}

Person::Person(const string &name) :
    Person()
{
    m_name = name;
}

Person::Person(const string &name, const string &email) :
    Person()
{
    m_name = name;
    m_email = email;
}

Person::Person(const string &name, const string &email, const string &phone) :
    Person()
{
    m_name = name;
    m_email = email;
    m_phone = phone;
}

Person& Person::operator=(const Person& person)
{
    m_name = person.m_name;
    m_email = person.m_email;
    m_phone = person.m_phone;

    return *this;
}

const string &Person::Name()
{
    return m_name;
}

const string& Person::Email()
{
    return m_email;
}

const string& Person::Phone()
{
    return m_phone;
}

void Person::SetName(const string& value)
{
    m_name = value;
}

void Person::SetEmail(const string& value)
{
    m_email = value;
}

void Person::SetPhone(const string& value)
{
    m_phone = value;
}

void Person::Print()
{
    cout << m_name;
    if (m_email.size() > 0)
    {
        cout << " <" << m_email << ">";
    }
    if (m_phone.size() > 0)
    {
        cout << ", phone: " << m_phone;
    }
    cout << endl;
}


Person::~Person()
{
    // cout << "Deleting person " << m_name << endl;
}
