CREATE TABLE Recruiters (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    name            TEXT,
    description     TEXT
);

-- insert into Recruiters (name, description) values ( "Tom Simonsen", "My applications" );

CREATE TABLE Positions (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    company         TEXT,
    name            TEXT,
    description     TEXT,
    app_status      INTEGER,
    app_result      INTEGER,
    recruiter_id    INTEGER
);

CREATE TABLE Persons (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    name            TEXT,
    email           TEXT,
    phone           TEXT
);

CREATE Table RecruiterPersons (
    recruiter_id    INTEGER,
    person_id       INTEGER
);

CREATE Table PositionPersons (
    position_id     INTEGER,
    person_id       INTEGER
);


