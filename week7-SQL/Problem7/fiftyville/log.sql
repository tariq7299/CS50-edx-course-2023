-- Keep a log of any SQL queries you execute as you solve the mystery.
/*

-- Having a look ag the schema of the Data base
.schema

CREATE TABLE crime_scene_reports (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    street TEXT,
    description TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE interviews (
    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE atm_transactions (
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE bank_accounts (
    account_number INTEGER,
    person_id INTEGER,
    creation_year INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);
CREATE TABLE airports (
    id INTEGER,
    abbreviation TEXT,
    full_name TEXT,
    city TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE flights (
    id INTEGER,
    origin_airport_id INTEGER,
    destination_airport_id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
    FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
);
CREATE TABLE passengers (
    flight_id INTEGER,
    passport_number INTEGER,
    seat TEXT,
    FOREIGN KEY(flight_id) REFERENCES flights(id)
);
CREATE TABLE phone_calls (
    id INTEGER,
    caller TEXT,
    receiver TEXT,
    year INTEGER,
    month INTcheck50 cs50/problems/2023/x/fiftyville
    EGER,
    day INTEGER,
    duration INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE people (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE bakery_security_logs (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
);
*/

+--------+---------+----------------+-----------------+---------------+
-- Havinh a look at all the tables inside of the data base

.tables

airports              crime_scene_reports   people
atm_transactions      flights               phone_calls
bakery_security_logs  interviews
bank_accounts         passengers


+--------+---------+----------------+-----------------+---------------+
-- Details about the day and place where the crime took place

SELECT *
FROM crime_scene_reports
WHERE day = 28
  AND street = "Humphrey Street";

--  295 | 2021 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
-- | 297 | 2021 | 7     | 28  | Humphrey Street | Littering took place at 16:36. No known witnesses.

+--------+---------+----------------+-----------------+---------------+
-- Accessing each interview record of the witnesses

SELECT *
FROM interviews
WHERE year = 2021
  AND month = 7
  AND day = 28;

       /*
                                                                                                    |
| 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
*/
+--------+---------+----------------+-----------------+---------------+
-- Accessing the bakery security logs, because the bakery has been mentioned in each interview

SELECT *
FROM bakery_security_logs
WHERE year = 2021
  AND month = 7
  AND day = 28
  AND hour = 10
  AND minute BETWEEN 15 AND 40;
  /*

  +-----+------+-------+-----+------+--------+----------+---------------+
| id  | year | month | day | hour | minute | activity | license_plate |
+-----+------+-------+-----+------+--------+----------+---------------+
| 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
| 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
| 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
| 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
| 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
| 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
| 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
| 268 | 2021 | 7     | 28  | 10   | 35     | exit     | 1106N58       |

+--------+---------+----------------+-----------------+---------------+
*/

-- Then Here iam trying to know all the names of the people with those license plates
SELECT *
FROM people
WHERE license_plate IN(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
      AND month = 7
      AND day = 28
      AND hour = 10
      AND minute BETWEEN 15 AND 40
);
/*


   id   |  name   |  phone_number  | passport_number | license_plate |
+--------+---------+----------------+-----------------+---------------+
| 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
| 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
| 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
| 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
| 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
| 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
| 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
| 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X

+--------+---------+----------------+-----------------+---------------+
*/
-- Accessing all the transactions that occured the day of the theft, as that what has been said by one of the witnesses that he saw the theive withdrawing money from an atm in Leggett Street

SELECT *
FROM atm_transactions
WHERE atm_location = "Leggett Street"
  AND year = 2021
  AND month = 7
  AND day = 28;

/*

+-----+----------------+------+-------+-----+----------------+------------------+--------+
| id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
+-----+----------------+------+-------+-----+----------------+------------------+--------+
| 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw        | 48     |
| 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
| 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
| 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
| 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
| 275 | 86363979       | 2021 | 7     | 28  | Leggett Street | deposit          | 10     |
| 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
| 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
| 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
+-----+----------------+------+-------+-----+----------------+------------------+--------+

+--------+---------+----------------+-----------------+---------------+
*/


-- Accessing the bank_accounts of the transactions that happend
SELECT *
FROM bank_accounts
WHERE account_number IN (
    SELECT account_number
FROM atm_transactions
WHERE atm_location = "Leggett Street"
  AND year = 2021
  AND month = 7
  AND day = 28
);

-- +--------+---------+----------------+-----------------+---------------+
*/
--Viewing all the ondividuals data of those bank accounts from people table
SELECT *
FROM people
WHERE id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE atm_location = "Leggett Street"
          AND year = 2021
          AND month = 7
          AND day = 28
    )
);

/*
+--------+---------+----------------+-----------------+---------------+
|   id   |  name   |  phone_number  | passport_number | license_plate |
+--------+---------+----------------+-----------------+---------------+
| 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
| 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
| 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
| 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
| 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
| 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
| 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
| 948985 | Kaelyn  | (098) 555-1164 | 8304650265      | I449449       |
+--------+---------+----------------+-----------------+---------------+


+--------+---------+----------------+-----------------+---------------+
*/


 -- Trying to know intersections between table of individuals info who left the parkung lot within 10 minutes from the theft and individuals info who made transactions in the same day at the morning
SELECT Orders.OrderID, Customers.CustomerName
FROM Orders
INNER JOIN Customers ON Orders.CustomerID = Customers.CustomerID;


SELECT p.name
FROM people AS p
INNER JOIN bank_accounts AS bk ON p.id = bk.id
INNER JOIN






SELECT *
FROM people
WHERE license_plate IN(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
      AND month = 7
      AND day = 28
      AND hour = 10
      AND minute BETWEEN 15 AND 40
)

INTERSECT

SELECT *
FROM people
WHERE id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE atm_location = "Leggett Street"
          AND year = 2021
          AND month = 7
          AND day = 28
          AND transaction_type = "withdraw"
    )
);

/*

-- List of suspects
+--------+--------+----------------+-----------------+---------------+
|   id   |  name  |  phone_number  | passport_number | license_plate |
+--------+--------+----------------+-----------------+---------------+
| 396669 | Iman   | (829) 555-5269 | 7049073643      | L93JTIZ       |
| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
| 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
| 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+---------+----------------+-----------------+---------------+
+--------+---------+----------------+-----------------+---------------+
*/
-- I'am here creating a table of suspects using CREATE VIEW, to just make things easier for me
CREATE VIEW list_of_suspects AS

SELECT *
FROM people
WHERE license_plate IN(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
      AND month = 7
      AND day = 28
      AND hour = 10
      AND minute BETWEEN 00 AND 40
)

INTERSECT

SELECT *
FROM people
WHERE id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE atm_location = "Leggett Street"
          AND year = 2021
          AND month = 7
          AND day = 28
    )
);



-- +--------+---------+----------------+-----------------+---------------+
-- --

SELECT *
FROM phone_calls
WHERE caller IN (
    SELECT phone_number
    FROM list_of_suspects
    WHERE year = 2021
      AND month = 7
      AND day = 28
);


/*

+-----+----------------+----------------+------+-------+-----+----------+
| id  |     caller     |    receiver    | year | month | day | duration |
+-----+----------------+----------------+------+-------+-----+----------+
| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
| 236 | (367) 555-5533 | (344) 555-9601 | 2021 | 7     | 28  | 120      |
| 245 | (367) 555-5533 | (022) 555-4052 | 2021 | 7     | 28  | 241      |
| 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
| 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       |
| 284 | (286) 555-6063 | (310) 555-8568 | 2021 | 7     | 28  | 235      |
| 285 | (367) 555-5533 | (704) 555-5790 | 2021 | 7     | 28  | 75       |
+-----+----------------+----------------+------+-------+-----+----------+
+-----+----------------+----------------+------+-------+-----+----------+
*/
-- Creating a new list of suspects, (an update)

CREATE VIEW list_of_suspects2 AS
SELECT *
FROM list_of_suspects
WHERE phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2021
      AND month = 7
      AND day = 28
);


SELECT *
FROM list_of_suspects2;
/*

+--------+--------+----------------+-----------------+---------------+
|   id   |  name  |  phone_number  | passport_number | license_plate |
+--------+--------+----------------+-----------------+---------------+
| 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
| 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+--------+----------------+-----------------+---------------+


-- +-----+----------------+----------------+------+-------+-----+----------+
*/
-- Creating a table which contains a list of suspected accomplices which i got from the p
    CREATE VIEW list_of_suspected_accomplices AS
    SELECT *
    FROM people
    WHERE phone_number IN (
        SELECT receiver
        FROM phone_calls
        WHERE year = 2021
          AND month = 7
          AND day = 28
          AND caller IN (
            SELECT phone_number
            FROM list_of_suspects2
        )
    );


SELECT *
FROM list_of_suspected_accomplices;

/*

+--------+---------+----------------+-----------------+---------------+
|   id   |  name   |  phone_number  | passport_number | license_plate |
+--------+---------+----------------+-----------------+---------------+
| 250277 | James   | (676) 555-6554 | 2438825627      | Q13SVG6       |
| 315221 | Gregory | (022) 555-4052 | 3355598951      | V4C670D       |
| 652398 | Carl    | (704) 555-5790 | 7771405611      | 81MZ921       |
| 847116 | Philip  | (725) 555-3243 | 3391710505      | GW362R6       |
| 985497 | Deborah | (344) 555-9601 | 8714200946      | 10I5658       |
| 994229 | Angela  | (310) 555-8568 | 9920757687      | SS458D7       |


+--------+--------+----------------+-----------------+---------------+
*/
-- Trying to figure out which airport and which flight also



SELECT *
FROM flights
WHERE origin_airport_id =
(
    SELECT id
    FROM airports
    WHERE city = "Fiftyville"
)
  AND year = 2021
  AND month = 7
  AND day = 29
  AND hour =
  (
    SELECT MIN(hour)
    FROM flights
     WHERE year = 2021
       AND month = 7
       AND day = 29
);


/*

+----+-------------------+------------------------+------+-------+-----+------+--------+
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     |
+----+-------------------+------------------------+------+-------+-----+------+--------+
*/

-- Figuring out the thieve, as he/she was the one on the first fligh of the next day of the crime 07/28)

SELECT *
FROM list_of_suspects2
WHERE passport_number = (
SELECT passport_number
FROM passengers
WHERE passport_number IN (
    SELECT passport_number
    FROM list_of_suspects2
    )
  AND flight_id = 36
);

/*

+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
+--------+-------+----------------+-----------------+---------------+
-- The thive is .....Bruce


+----+-------------------+------------------------+------+-------+-----+------+--------+
*/
--Figuring out his accomplice

SELECT *
FROM list_of_suspects2
WHERE passport_number = (
SELECT passport_number
FROM passengers
WHERE passport_number IN (
    SELECT passport_number
    FROM list_of_suspected_accomplices
    )
  AND flight_id = 18
);

No results, so it seems that her accomplices didnot got in the same flight of her

-- +----+-------------------+------------------------+------+-------+-----+------+--------+

-- There is another way to fiuer out her accomplice, by seeing who has she called in the day after when she was running away from the bakery !

SELECT *
FROM list_of_suspected_accomplices
WHERE phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE caller = (
        SELECT phone_number
        FROM list_of_suspects2
        WHERE name = "Bruce"
    )
      AND year = 2021
      AND month = 7
      AND day = 28
      AND duration < 60
);

/*

+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 864400 | Robin | (375) 555-8161 | NULL            | 4V16VO0       |
+--------+-------+----------------+-----------------+---------------+
so his accomplice is .... Robin

*/


