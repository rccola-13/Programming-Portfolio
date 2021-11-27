/*
 * CS3425 Assignment 5
 * Author: Ryan Cole
 * Email: rccole@mtu.edu
 */

select "1.";
#1. List all departments with more than 100 students.
select dept_name, count(dept_name) as total_students
from student
group by dept_name
having total_students > 100
order by total_students desc;

select "2.";
#2. List the 5 departments with the smallest number of students.
select dept_name, count(ID) as total_students
from department natural left outer join student
group by dept_name
order by total_students asc
limit 5;

select "3.";
#3. List the name and number of instructors, courses, and students for each department.
select dept_name, ifnull(totS, 0) as total_students, ifnull(totI, 0) as total_instructors, total_courses
from department
natural left outer join
(select dept_name, count(ID) as totS from student group by dept_name) as studentD
natural left outer join
(select dept_name, count(ID) as totI from instructor group by dept_name) as instructorD
natural left outer join
(select dept_name, count(course_id) as total_courses from course group by dept_name) as courseD
order by total_students desc, total_instructors desc, total_courses desc;

select "4.";
#4. List the ID, title, and total number of sections offered for each course in history.
select course_id, title, count(sec_id) as times
from course natural left outer join section
group by course_id
order by times desc, title;

select "5.";
#5. List the total number of sections that were offered in Spring 2008 by each department.
select dept_name, ifnull(totSec, 0)as total_sections
from department natural left outer join
(select dept_name, count(sec_id) as totSec
 from course natural left outer join section
 where year = 2008 and semester = 'Spring'
 group by dept_name
 order by dept_name) as sectionS8;

select "6.";
#6. Generate a chronological transcript for the student whose ID is 1000.
select course_id, title, credits, grade, semester, year
from course natural join takes
where ID = 1000
order by year, semester desc;

select "7.";
#7. List the total teaching credit hours for instructors who taught in Spring 2008.
select dept_name, ID, name, sum(ifnull(hours, 0)) as total_credit_hours
from instructor natural left outer join teaches natural left outer join
(select course_id, sec_id, (count(ID) * credits) as hours
from course natural left outer join section natural left outer join takes
where semester = 'Spring' and year = 2008
group by course_id, sec_id
order by course_id) as credit_hours
group by ID, name, dept_name
order by dept_name, total_credit_hours desc, name;

select "8.";
#8. List the total number of students taught by each instructor.
select dept_name, ID, name, ifnull(stuNum, 0) as stu_number
from instructor natural left outer join
(select teaches.ID as ID, count(distinct takes.id) as stuNum
 from teaches, takes
 where teaches.course_id = takes.course_id and teaches.sec_id = takes.sec_id
 group by teaches.ID) as teachesStu
order by dept_name, stu_number desc, name;

select "9.";
#9. List the ID, name, sum of credits, and GPA of the Math department's top 20 students based on GPA.
select ID, name, total_credits, GPA
from student natural join
(select ID, sum(credits) as total_credits, round(sum(credits * point)/sum(credits), 2) as GPA
 from takes natural join gradepoint natural join course
 group by ID) as GPA
where dept_name = 'Math'
order by GPA desc
limit 20;

select "10.";
#10. List section information for calsses offered in Spring 2008.
select dept_name as Dept, course_id as Crse, sec_id as Sec, credits as Cred, title as Title, capacity as Cap, ifnull(Act, 0) as Act, ifnull((capacity - Act), capacity) as Rem, ifnull(name, '--TBD--') as Instructor, building as Building, room_number as Room_Number
from course natural join section natural left outer join teaches natural left outer join instructor natural left outer join
(select course_id, count(ID) as Act
 from section natural join takes
 where semester = 'Spring' and year = 2008
 group by course_id) as took
where semester = 'Spring' and year = 2008
order by Dept, Crse, Sec;