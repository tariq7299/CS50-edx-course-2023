-- 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.

-- SELECT p.name
-- FROM people p
-- JOIN stars s1 ON p.id = s1.person_id
-- JOIN movies m1 ON s1.movie_id = m1.id
-- JOIN stars s2 ON m1.id = s2.movie_id
-- JOIN people kb ON s2.person_id = kb.id
-- WHERE kb.name = 'Kevin Bacon' and kb.birth = 1958;

-- or

SELECT DISTINCT(p.name)
FROM people p
JOIN stars s ON p.id = s.person_id
JOIN movies m ON s.movie_id = m.id
WHERE m.id IN (
    SELECT movie_id FROM stars WHERE person_id IN (
        SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958
    )
)
AND p.name != "Kevin Bacon";


-- SELECT DISTINCT(p.name)
-- FROM people p
-- JOIN stars s1 ON p.id = s1.person_id
-- JOIN movies m1 ON s1.movie_id = m1.id
-- WHERE m1.title IN (
--     SELECT DISTINCT(movies.title) FROM movies
--     JOIN stars s2 ON movies.id = s2.movie_id
--     JOIN people p ON s2.person_id = p.id
--     WHERE p.name = "Kevin Bacon" AND p.birth = 1958
-- )
-- AND p.name != "Kevin Bacon";