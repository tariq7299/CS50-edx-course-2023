-- 8.SQL, write a SQL query to list the names of all people who starred in Toy Story.
SELECT p.name FROM people p
JOIN stars s ON p.id = s.person_id
JOIN movies m ON s.movie_id = m.id
WHERE m.title = "Toy Story";

-- or

-- SELECT p.name FROM people p
-- WHERE p.id IN (
--     SELECT person_id FROM stars WHERE movie_id = (
--         SELECT id FROM movies WHERE title = "Toy Story"
--     )
-- );
