-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Query entire crime_scene_reports table to establish baseline
SELECT * FROM crime_scene_reports;

-- Query the year, month, day, and street from crime_scene_reports to isolate the incident.
-- Theft occured at 10:15am at Humphrey Street bakery, three witnesses were interviewed on the same day and each transcript mentions the bakery
SELECT * FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = 'Humphrey Street';

-- Query the year, month, day, and transcript from interviews to find the names of the witnesses
-- Ruth: the theif got in a car in the parking lot within 10 min of the theft. Look through security cam footage
-- Eugene: saw the thief withdrawing money from the atm on Leggett Street earlier the same day
-- Raymond: thief called someone for less than a minute while leaving the bakery. Heard the thief say they were planning to take the earliest flight out tomorrow and asked the other person to purchase the ticket
SELECT * FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28
AND transcript LIKE ('%bakery%');

-- Query the bakery_security_logs table to find the license plates of cars leaving the bakery parking lot within a 10 minute window of the theft
-- Plates of exiting cars are: 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55
SELECT * FROM bakery_security_logs
WHERE year = 2021
AND month = 7
AND day = 28
AND hour = 10
AND minute BETWEEN 15 AND 25;

-- Query the atm_transactions table joined to the bank_accounts table joined to the people table to find names associated with bank accounts for withdraws from the Leggett street atm on the day of the theft and their license plates
-- Potential suspects; Bruce: phone (367) 555-5533, passport 5773159633; Diana: phone (770) 555-1861, passport 3592750733; Iman: phone (829) 555-5269, passport 7049073643; Luca: phone (389) 555-5198, passport 8496433585
SELECT * FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON bank_accounts.person_id = people.id
WHERE atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND atm_transactions.atm_location = 'Leggett Street'
AND atm_transactions.transaction_type = 'withdraw'
AND people.license_plate IN ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55');

-- Query the first row from the phone_calls table to get an idea of how the data is formatted
SELECT * FROM phone_calls LIMIT 1;

-- Query phone_calls table for calls on the date of the theft from the suspects that had a duration of less than 60 seconds
-- Suspects narrowed to Bruce, who called (375) 555-8161, and Diana who called (725) 555-3243
SELECT * FROM phone_calls
WHERE caller IN ('(367) 555-5533', '(770) 555-1861', '(829) 555-5269', '(389) 555-5198')
AND year = 2021
AND month = 7
AND day = 28
AND duration < 60;

-- Query people associated with phone numbers to get an idea of accomplices
-- Bruce called Robin and Diana called Philip
SELECT name, phone_number FROM people
WHERE phone_number IN ('(375) 555-8161', '(725) 555-3243');

-- Query flights joined with airports to determine the earliest flight out of Fiftyville on the following day
-- Earliest flight is at 0820
SELECT * FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE flights.year = 2021
AND flights.month = 7
AND flights.day = 29
AND airports.city = 'Fiftyville';

-- Query flights joined with airports joined with people to determine the thief
-- Turns out Bruce is supposed to be on the 0820 out of Fiftyville the following day
SELECT * FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
JOIN passengers ON passengers.flight_id = flights.id
WHERE flights.year = 2021
AND flights.month = 7
AND flights.day = 29
AND airports.city = 'Fiftyville'
AND passengers.passport_number IN (5773159633, 3592750733);

-- Bruce is the thief and Robin is his accomplice. He's taking the 0820 flight riding in seat 4A going to the LaGuardia Airport in New York City
SELECT full_name, city FROM airports
WHERE id = 4;