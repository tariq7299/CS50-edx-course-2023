

# Solution
UPDATE shows SET title = 'Adventure Time' WHERE title LIKE 'Adventure Time';
UPDATE shows SET title = 'Arrow' WHERE title LIKE 'Arrow';
UPDATE shows SET title = 'Avatar' WHERE title LIKE 'Avatar';
UPDATE shows SET title = 'Brooklyn Nine-Nine' WHERE title LIKE 'B99' OR title LIKE 'Brooklyn%';
UPDATE shows SET title = 'Community' WHERE title LIKE 'Community';
UPDATE shows SET title = 'Family Guy' WHERE title LIKE 'Family Guy';
UPDATE shows SET title = 'Friends' WHERE title LIKE 'Friends';
UPDATE shows SET title = 'Game of Thrones' WHERE title LIKE 'Game of Thrones' OR title LIKE 'GoT';
UPDATE shows SET title = 'Gilmore Girls' WHERE title LIKE 'Gilmore Girls';
UPDATE shows SET title = "Grey’s Anatomy" WHERE title LIKE "Grey%";
UPDATE shows SET title = 'How I Met Your Mother' WHERE title LIKE 'How I Met Your Mother';
UPDATE shows SET title = "It’s Always Sunny in Philadelphia" WHERE title LIKE "%Philadelphia";
UPDATE shows SET title = 'Parks and Recreation' WHERE title LIKE 'Parks and Re%';
UPDATE shows SET title = 'Sherlock' WHERE title LIKE 'Sherlock';
UPDATE shows SET title = 'Squid Game' WHERE title LIKE 'Squid Game';
UPDATE shows SET title = 'The Bachelorette' WHERE title LIKE 'The Bachelorette';
UPDATE shows SET title = 'The Crown' WHERE title LIKE 'The Crown';
UPDATE shows SET title = 'The Office' WHERE title LIKE 'The Office' OR title LIKE 'office';
UPDATE shows SET title = "The Queen’s Gambit" WHERE title LIKE "The Queen’s Gambit";
UPDATE shows SET title = 'The Untamed' WHERE title LIKE 'The Untamed';

# Tricky ones!!
UPDATE shows SET title = 'Brooklyn Nine-Nine' WHERE title LIKE 'B99' OR title LIKE 'Brooklyn%';
UPDATE shows SET title = 'Game of Thrones' WHERE title LIKE 'Game of Thrones' OR title LIKE 'GoT';
UPDATE shows SET title = "Grey’s Anatomy" WHERE title LIKE "Grey%";
UPDATE shows SET title = 'Parks and Recreation' WHERE title LIKE 'Parks and Re%';
UPDATE shows SET title = "It’s Always Sunny in Philadelphia" WHERE title LIKE "%Always Sunny%";
UPDATE shows SET title = 'The Office' WHERE title LIKE 'The Office' OR title LIKE 'office';






# Another solution for to-do number 2
# We can use this method also
UPDATE shows
SET title = CASE
    WHEN title LIKE "Show's title" THEN "Show's title"
    ELSE title
END;



