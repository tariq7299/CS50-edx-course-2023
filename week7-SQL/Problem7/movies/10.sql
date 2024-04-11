-- 10.sql wrinte a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
SELECT name FROM people WHERE id IN (
    SELECT person_id FROM directors
    WHERE movie_id IN (
    SELECT movie_id FROM ratings WHERE rating = 9.0 OR rating > 9.0
    )
);
-- or

-- SELECT p.name FROM people p
-- JOIN stars s ON p.id = s.person_id
-- JOIN ratings r ON s.movie_id = r.movie_id
-- WHERE r.rating > 9.0 ORDER BY p.name ASC;
