-- 6.sql, write a SQL query to determine the average rating of all movies released in 2012.
SELECT AVG(rating) AS average_rating
FROM ratings
WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);

-- or

-- WITH movies_2012 AS (
--   SELECT id
--   FROM movies
--   WHERE year = 2012
-- )
-- SELECT AVG(rating) AS average_rating
-- FROM ratings
-- WHERE movie_id IN movies_2012;