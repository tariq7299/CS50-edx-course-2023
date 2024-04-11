-- 4.sql, write a SQL query to determine the number of movies with an IMDb rating of 10.0.
SELECT COUNT(movie_id) AS "Number of movies with a rate of 10" FROM ratings WHERE rating = "10.0";
