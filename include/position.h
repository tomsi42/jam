#pragma once

#include <iostream>
#include <vector>

#include "dbobject.h"
#include "person.h"

using namespace std;

enum ApplicationStatus
{
    JOB_REGISTERED,
    CONSIDERING_APPLICATION,
    APPLICATION_SENT,
    APPLICATION_CONFIRMED,
    AT_INTERVIEW_STAGE,
    AWAITING_CONCLUSION,
    APPLICATION_FINISHED
};

enum ApplicationResult
{
    NEW_APPLICATION,
    IN_PROGRESS,
    NO_REPLY,
    NOT_QUALIFIED,
    OVER_QUALIFIED,
    WRONG_PROFILE,
    JOB_OFFERED,
    JOB_ACCEPTED
};

class Position : public DbObject
{
    public:
        Position();
        Position(const string &company, const string &name);
        Position(const string &company, const string &name, const string &description);
        virtual ~Position();

        Position(const Position& position) = delete;
        Position& operator=(const Position& position) = delete;

        const string &Company();
        const string &Name();
        const string &Description();
        ApplicationStatus Status();
        ApplicationResult Result();
        int RecruiterId();
        const vector<Person *> Persons();

        void AddPerson(Person *person);

        void SetName(const string& value);
        void SetCompany(const string& value);
        void SetDescription(const string& value);
        void SetStatus(ApplicationStatus value);
        void SetResult(ApplicationResult value);
        void SetRecruiterId(const int value);
        void SetPersons(vector<Person *> value);

        void Print();

    protected:

    private:
        string m_company;
        string m_name;
        string m_description;
        ApplicationStatus m_status;
        ApplicationResult m_result;
        int m_recruiterId;
        vector<Person *> m_persons;

        void DeletePersons();
};

const string StatusToString(ApplicationStatus status);
const string ResultToString(ApplicationResult result);


