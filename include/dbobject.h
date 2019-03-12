#pragma once

class DbObject
{
    public:
        DbObject();
        virtual ~DbObject();

        int Id();
        void SetId(int id);
    protected:

    private:
        int m_id;
};
