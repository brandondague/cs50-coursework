SELECT name FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON movies.id = stars.movie_id
WHERE movies.id IN (
SELECT movies.id FROM movies
JOIN people ON people.id = stars.person_id
JOIN stars ON stars.movie_id = movies.id
WHERE people.name LIKE ('%kevin bacon%')
AND people.birth = 1958)
AND NOT people.name LIKE ('%kevin bacon%');