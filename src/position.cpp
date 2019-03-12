#include "position.h"

Position::Position()
{
}

Position::Position(const string &company, const string &name) :
    m_company(company),
    m_name(name),
    m_status(ApplicationStatus::JOB_REGISTERED),
    m_result(ApplicationResult::IN_PROGRESS)
{

}

Position::Position(const string &company, const string &name, const string &description) :
    m_company(company),
    m_name(name),
    m_description(description),
    m_status(ApplicationStatus::JOB_REGISTERED),
    m_result(ApplicationResult::IN_PROGRESS)
{

}

const string &Position::Company()
{
    return m_company;
}

const string &Position::Name()
{
    return m_name;
}

const string &Position::Description()
{
    return m_description;
}

void Position::AddPerson(Person* person)
{
    m_persons.push_back(person);
}

ApplicationStatus Position::Status()
{
    return m_status;
}

ApplicationResult Position::Result()
{
    return m_result;
}

const vector<Person *> Position::Persons()
{
    return m_persons;
}

void Position::SetName(const string& value)
{
    m_name = value;
}

void Position::SetCompany(const string& value)
{
    m_company = value;
}

void Position::SetDescription(const string& value)
{
    m_description = value;
}

void Position::SetStatus(ApplicationStatus status)
{
    m_status = status;
}

void Position::SetResult(ApplicationResult result)
{
    m_result = result;
}

int Position::RecruiterId()
{
    return m_recruiterId;
}

void Position::SetRecruiterId(const int value)
{
    m_recruiterId = value;
}

void Position::SetPersons(vector<Person*>value)
{
    DeletePersons();
    m_persons = value;
}

void Position::DeletePersons()
{
    while (m_persons.size() > 0) {
        Person * p = m_persons.back();
        m_persons.pop_back();
        delete p;
    }
}

void Position::Print()
{
    cout << m_company << ": " << m_name << endl;
    if (m_description.size() > 0)
    {
        cout << "  " << m_description << endl;
    }
    if (m_persons.size() > 0) {
        for (Person * p : m_persons)
        {
            cout << "    ";
            p->Print();
        }
    }
    cout << "Status: " << StatusToString(m_status) << endl;
    cout << "Result: " << ResultToString(m_result) << endl;
 }

Position::~Position()
{
    // cout << "Deleting position " << m_name << " at " << m_company << endl;

    DeletePersons();
}

const string StatusToString(ApplicationStatus status)
{
    switch (status)
    {
        case JOB_REGISTERED:
            return "Job registered";
        case CONSIDERING_APPLICATION:
            return "Considering application";
        case APPLICATION_SENT:
            return "Application sent";
        case APPLICATION_CONFIRMED:
            return "Application Confirmed";
        case AT_INTERVIEW_STAGE:
            return "At interview stage";
        case AWAITING_CONCLUSION:
            return "Awaiting conclusion";
        case APPLICATION_FINISHED:
            return "Application process finished";
        default:
            return "Status code " + to_string(status);
    }

    return "undefined";
}

const string ResultToString(ApplicationResult result)
{
    switch (result)
    {
    case NEW_APPLICATION:
        return "New Application";
    case IN_PROGRESS:
        return "In Progress";
    case NO_REPLY:
        return "No Reply";
    case NOT_QUALIFIED:
        return "Not Qualified";
    case OVER_QUALIFIED:
        return "Overqualified";
    case WRONG_PROFILE:
        return "Wrong profile";
    case JOB_OFFERED:
        return "Job offered";
    case JOB_ACCEPTED:
        return "Job accepted";
    default:
        return "Result code " + to_string(result);
    }
}
