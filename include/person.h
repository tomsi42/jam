#pragma once

#include <iostream>

#include "dbobject.h"

using namespace std;

class Person : public DbObject
{
    public:
        Person();
        Person(const Person& person);
        Person& operator=(const Person& person);
        Person(const string& name);
        Person(const string& name, const string& email);
        Person(const string& name, const string& email, const string& phone);
        virtual ~Person();

        const string& Name();
        const string& Email();
        const string& Phone();
        void SetName(const string& value);
        void SetEmail(const string& value);
        void SetPhone(const string& value);

        void Print();

    protected:

    private:
        string m_name;
        string m_email;
        string m_phone;
};
