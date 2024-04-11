-- 11.sql, write a SQL query to list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.
SELECT title FROM movies m
JOIN ratings r ON m.id = r.movie_id
WHERE id IN (
    SELECT movie_id FROM stars WHERE person_id IN (
        SELECT id FROM people WHERE name = "Chadwick Boseman"
    )
)
ORDER BY r.rating DESC LIMIT 5;


-- OR


-- SELECT title FROM movies m
-- JOIN stars s ON m.id = s.movie_id
-- JOIN ratings r ON s.movie_id = r.movie_id
-- JOIN people p ON s.person_id = p.id
-- WHERE p.name = "Chadwick Boseman" ORDER BY rating DESC LIMIT 5;
