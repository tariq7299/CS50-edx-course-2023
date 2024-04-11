import csv
from cs50 import SQL




def main():

    db = SQL("sqlite:///roster.db")

    # First method to read data into 'rows' (But the source (students.sql) as a 'sql')
    rows = db.execute("SELECT * FROM students")

    # Second method to read data into 'rows' (but the source (students.csv) as 'csv')
    # with open('students.csv', 'r') as students_file:
    #     rows = csv.DictReader(students_file)

    for row in rows:
        Student_id = db.execute("SELECT Student_id FROM new_students where Student_name = ?", row["student_name"])
        House_id = db.execute("SELECT House_id FROM houses WHERE House_name = ?", row["house"])

        db.execute("INSERT INTO assignments(Student_id, House_id) VALUES(?, ?)", Student_id[0]["Student_id"], House_id[0]["House_id"])


main()

