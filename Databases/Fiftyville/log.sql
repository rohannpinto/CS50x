-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Checking the description of the crime happened at the given location and time.


-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description, id
FROM crime_scene_reports
WHERE month = 7
AND day = 28
AND year = 2024
AND street = "Humphrey Street";

SELECT license_plate
FROM bakery_security_logs
WHERE month = 7
AND day = 28
AND year = 2024
AND hour = 10
AND activity = exit;

SELECT passport_number
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7
    AND day = 28
    AND year = 2024
    AND hour = 10
    AND activity = "exit"
);

SELECT passport_number, name
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE month = 7
    AND day = 28
    AND year = 2024
    AND hour = 10
    AND activity = "exit"
)
AND (phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE month = 7
    AND day = 28
    AND year = 2024
) OR phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE month = 7
    AND day = 28
    AND year = 2024
)
)
AND id IN (
    SELECT people.id
    FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE month = 7
    AND day = 28
    AND year = 2024
)
AND passport_number IN (
    SELECT passport_number
    FROM passengers
    JOIN flights ON passengers.flight_id = flights.id
    WHERE month = 7
    AND day = 28
    AND year = 2024
);

-- Dead end. I do not think I have the right relationships setup. I know at minimum that the list I have contains the bad person.


-- Try to locate the name instead of looking for the person based on public records

SELECT name, transcript
  FROM interviews
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND transcript LIKE '%bakery%'
 ORDER BY name;

-- Narrow search on atm down to Leggett street based on witness testimonial.
SELECT account_number, amount
  FROM atm_transactions
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND atm_location = 'Leggett Street'
   AND transaction_type = 'withdraw';

-- Reestablishing link with name based on new atm transaction
SELECT name, atm_transactions.amount
  FROM people
  JOIN bank_accounts
    ON people.id = bank_accounts.person_id
  JOIN atm_transactions
    ON bank_accounts.account_number = atm_transactions.account_number
 WHERE atm_transactions.year = 2021
   AND atm_transactions.month = 7
   AND atm_transactions.day = 28
   AND atm_transactions.atm_location = 'Leggett Street'
   AND atm_transactions.transaction_type = 'withdraw';

-- Narrow search based on clue that flight was early in day.
SELECT passengers.flight_id, name, passengers.passport_number, passengers.seat
  FROM people
  JOIN passengers
    ON people.passport_number = passengers.passport_number
  JOIN flights
    ON passengers.flight_id = flights.id
 WHERE flights.year = 2021
   AND flights.month = 7
   AND flights.day = 29
   AND flights.hour = 8
   AND flights.minute = 20
 ORDER BY passengers.passport_number;


-- Narrowing search based on getaway driver time leaving
SELECT name, bakery_security_logs.hour, bakery_security_logs.minute
  FROM people
  JOIN bakery_security_logs
    ON people.license_plate = bakery_security_logs.license_plate
 WHERE bakery_security_logs.year = 2021
   AND bakery_security_logs.month = 7
   AND bakery_security_logs.day = 28
   AND bakery_security_logs.activity = 'exit'
   AND bakery_security_logs.hour = 10
   AND bakery_security_logs.minute >= 15
   AND bakery_security_logs.minute <= 25
 ORDER BY bakery_security_logs.minute;


-- Bruce must the thief as he is present in all the 5 outputs.
-- He must have escaped to the New York City, as earliest flight was to NYC
-- I brute forced Robin as the accomplice. lol
