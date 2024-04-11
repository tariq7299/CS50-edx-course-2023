
-- The schema of songs.db
-- CREATE TABLE songs (
--     id INTEGER,
--     name TEXT,
--     artist_id INTEGER,
--     danceability REAL,
--     energy REAL,
--     key INTEGER,
--     loudness REAL,
--     speechiness REAL,
--     valence REAL,
--     tempo REAL,
--     duration_ms INTEGER
-- );
-- CREATE TABLE artists (
--     id INTEGER,
--     name TEXT
-- );

-- A command to execute what written in a specific sql file
-- cat filename.sql | sqlite3 songs.db

-- 1- write a SQL query to list the names of all songs in the database.
SELECT name FROM songs;











