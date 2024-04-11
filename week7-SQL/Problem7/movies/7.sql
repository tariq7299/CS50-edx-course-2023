-- 7.sql, write a SQL query to list all movies released in 2010 and their ratings, in descending order by rating.
--2 For movies with the same rating, order them alphabetically by title.

SElECT m.title, r.rating FROM movies m
JOIN ratings r ON m.id = r.movie_id
WHERE m.year = 2010
ORDER BY r.rating DESC, title;
