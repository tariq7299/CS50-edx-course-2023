-- 12.sql write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.

SELECT title FROM movies
WHERE id IN (
    SELECT movie_id FROM stars WHERE person_id IN (
        SELECT id FROM people WHERE name = "Johnny Depp" OR name = "Helena Bonham Carter"
    )
    GROUP BY movie_id
    HAVING COUNT(DISTINCT person_id) = 2
);


-- -- or

-- SELECT title FROM movies m
-- JOIN stars s1 ON m.id = s1.movie_id
-- JOIN stars s2 ON m.id = s2.movie_id
-- JOIN people p1 ON s1.person_id = p1.id
-- JOIN people p2 ON s2.person_id = p2.id
-- WHERE p1.name = "Johnny Depp" AND p2.name = "Helena Bonham Carter";
