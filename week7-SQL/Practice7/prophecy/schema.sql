
CREATE TABLE new_students (
    Student_id INTEGER PRIMARY KEY,
    Student_name TEXT NOT NULL
);

CREATE TABLE houses (
    House_id INTEGER PRIMARY KEY,
    House_name TEXT NOT NULL UNIQUE,
    Head_name TEXT NOT NULL UNIQUE
);

CREATE TABLE assignments (
    Student_id INTEGER,
    House_id INTEGER NOT NULL,
    FOREIGN KEY(Student_id) REFERENCES new_students(Student_id),
    FOREIGN KEY(House_id) REFERENCES houses(House_id),
    PRIMARY KEY(Student_id, House_id)
);


INSERT OR IGNORE INTO houses(House_id, House_id, Head_name)
SELECT house, head FROM students;

INSERT INTO new_students (Student_id, Student_name)
SELECT id, student_name FROM students;