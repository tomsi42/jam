#include <iostream>

#include "recruiter.h"
#include "person.h"
#include "jam_dao.h"

using namespace std;

int BuildObjectModel(JobApplicationManagerDao& dao)
{
    Recruiter *myself = new Recruiter("Tom Simonsen", "My own job applications");
    Person *me = new Person("Tom Simonsen", "toms@pinglebo.net");

    myself->AddPerson(me);

    Position *job = new Position("Cool Tech AS", "Senior SW Dev. position");
    Person *contact = new Person("Albert Einstein", "albert@ctech.org");
    job->AddPerson(contact);
    job->AddPerson(new Person("Ernie"));
    job->AddPerson(new Person("Donald D."));
    job->SetDescription("We are looking for an innovative software developer that is a bit cool.");
    job->SetStatus(ApplicationStatus::AT_INTERVIEW_STAGE);
    job->SetResult(ApplicationResult::IN_PROGRESS);
    myself->AddPosition(job);

    job = new Position("Money Inc", "Senior Software developer");
    job->AddPerson(new Person("Some Guy", "someguy@money.nu"));
    job->SetDescription("We are looking for a coder.");
    job->SetStatus(ApplicationStatus::APPLICATION_FINISHED);
    job->SetResult(ApplicationResult::WRONG_PROFILE);
    myself->AddPosition(job);

    job = new Position("Fast Tech Inc", "Senior Software developer");
    job->AddPerson(new Person("Fredde Krueger", "fk@fti.net"));
    job->SetDescription("We are looking for a intelligent coder.");
    job->SetStatus(ApplicationStatus::APPLICATION_SENT);
    job->SetResult(ApplicationResult::NEW_APPLICATION);
    myself->AddPosition(job);

    dao.AddRecruiter(*myself);
    int id = myself->Id();

    delete myself;

    return id;
}



void LoadObjectModel(JobApplicationManagerDao& dao, int id)
{
        Recruiter *recruiter = dao.FindRecruiter(id);
        if (recruiter != nullptr)
        {
            recruiter->Print();
            delete recruiter;
        }
        else
        {
            cerr << "Could not find recruiter with ID " << id << endl;
        }
}

void AddPersons(JobApplicationManagerDao& dao)
{
    Person tom("Tom Simonsen", "tom-si2@online.no");
    dao.AddPerson(tom);
    cout << "saved " << tom.Name() << " with id " << tom.Id() << endl;
    tom.SetEmail("toms@pinglebo.net");
    dao.UpdatePerson(tom);
}

void ListPersons(JobApplicationManagerDao& dao)
{
    vector<Person *> persons = dao.FindAllPersons();
    for (Person *p : persons)
    {
        p->Print();
    }
    while (persons.size() > 0)
    {
        Person *p = persons.back();
        persons.pop_back();
        delete p;
    }
}

void FindPerson(JobApplicationManagerDao& dao, int id)
{
    Person *p = dao.FindPerson(id);
    if (p)
    {
        p->Print();
        delete p;
    }
    else
    {
        cout << "No person with ID = " << id << endl;
    }
}

void TestDao()
{
    // JobApplicationManagerDao dao("jam.db");
    JobApplicationManagerDao dao(":memory:");
    if (dao.Open())
    {
        int id = BuildObjectModel(dao);
        if (id > 0)
            LoadObjectModel(dao, id);
        dao.Close();
    }
}

int main()
{
    TestDao();

    return 0;
}
