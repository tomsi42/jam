#include <iostream>

#include "recruiter.h"
#include "person.h"
#include "jam_dao.h"

using namespace std;


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
        // int id = BuildObjectModel(dao);
		int id = 1;
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
