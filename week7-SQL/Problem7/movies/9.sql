-- 9.sql, write a SQL query to list the names of all people who starred in a movie released in 2004, ordered by birth year.
SELECT p.name FROM people p
WHERE p.id IN (
    SELECT person_id FROM stars WHERE movie_id IN (
        SELECT id FROM movies WHERE year = 2004
    )
)
ORDER BY birth;

--or

-- SELECT p.name FROM people p
-- JOIN stars s on p.id = s.person_id
-- JOIN movies m ON s.movie_id = m.id
-- WHERE m.year = 2004;
